/*    
    Copyright (C) 2019 Elia Ritterbusch 

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/
#ifndef F_CPU
//#define F_CPU 16000000UL
#define F_CPU 1000000UL
#endif

#include <avr/io.h>
#include <util/twi.h>
#include <avr/interrupt.h>

#include "i2c_master.h"

#define F_SCL 100000UL // SCL frequency
#define Prescaler 1
#define TWBR_val ((((F_CPU / F_SCL) / Prescaler) - 16) / 2)

static void (*I2C_recv)(uint8_t);
static void (*I2C_req)();

void i2c_init(void)
{
	TWBR = (uint8_t)TWBR_val;

}

void i2c_slave_init(uint8_t address)
{
	cli();
	i2c_init();
	// DDRC  &= ~(1 << 4);
    // PORTC |=  (1 << 4);
	// DDRC  &= ~(1 << 5);
    // PORTC |=  (1 << 5);
	// load address into TWI address register
	TWAR = address << 1;
	// set the TWCR to enable address matching and enable TWI, clear TWINT, enable TWI interrupt
	TWCR = (1 << TWIE) | (1 << TWEA) | (1 << TWINT) | (1 << TWEN);
	sei();
}

void i2c_slave_set_callbacks(void (*recv)(uint8_t), void (*req)())
{
  I2C_recv = recv;
  I2C_req = req;
}

uint8_t i2c_start(uint8_t address)
{
	// reset TWI control register
	TWCR = 0;
	// transmit START condition
	TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
	// wait for end of transmission
	while (!(TWCR & (1 << TWINT)))
		;

	// check if the start condition was successfully transmitted
	if ((TWSR & 0xF8) != TW_START)
	{
		return 1;
	}

	// load slave address into data register
	TWDR = address;
	// start transmission of address
	TWCR = (1 << TWINT) | (1 << TWEN);
	// wait for end of transmission
	while (!(TWCR & (1 << TWINT)))
		;

	// check if the device has acknowledged the READ / WRITE mode
	uint8_t twst = TW_STATUS & 0xF8;
	if ((twst != TW_MT_SLA_ACK) && (twst != TW_MR_SLA_ACK))
		return 1;

	return 0;
}

uint8_t i2c_write(uint8_t data)
{
	// load data into data register
	TWDR = data;
	// start transmission of data
	TWCR = (1 << TWINT) | (1 << TWEN);
	// wait for end of transmission
	while (!(TWCR & (1 << TWINT)))
		;

	if ((TWSR & 0xF8) != TW_MT_DATA_ACK)
	{
		return 1;
	}

	return 0;
}

uint8_t i2c_read_ack(void)
{

	// start TWI module and acknowledge data after reception
	TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA);
	// wait for end of transmission
	while (!(TWCR & (1 << TWINT)))
		;
	// return received data from TWDR
	return TWDR;
}

uint8_t i2c_read_nack(void)
{

	// start receiving without acknowledging reception
	TWCR = (1 << TWINT) | (1 << TWEN);
	// wait for end of transmission
	while (!(TWCR & (1 << TWINT)))
		;
	// return received data from TWDR
	return TWDR;
}

uint8_t i2c_transmit(uint8_t address, uint8_t *data, uint16_t length)
{
	if (i2c_start(address | I2C_WRITE))
		return 1;

	for (uint16_t i = 0; i < length; i++)
	{
		if (i2c_write(data[i]))
			return 1;
	}

	i2c_stop();

	return 0;
}

uint8_t i2c_receive(uint8_t address, uint8_t *data, uint16_t length)
{
	if (i2c_start(address | I2C_READ))
		return 1;

	for (uint16_t i = 0; i < (length - 1); i++)
	{
		data[i] = i2c_read_ack();
	}
	data[(length - 1)] = i2c_read_nack();

	i2c_stop();

	return 0;
}

uint8_t i2c_write_reg(uint8_t devaddr, uint8_t regaddr, uint8_t *data, uint16_t length)
{
	if (i2c_start(devaddr | 0x00))
		return 1;

	i2c_write(regaddr);

	for (uint16_t i = 0; i < length; i++)
	{
		if (i2c_write(data[i]))
			return 1;
	}

	i2c_stop();

	return 0;
}

uint8_t i2c_read_reg(uint8_t devaddr, uint8_t regaddr, uint8_t *data, uint16_t length)
{
	if (i2c_start(devaddr))
		return 1;

	i2c_write(regaddr);

	if (i2c_start(devaddr | 0x01))
		return 1;

	for (uint16_t i = 0; i < (length - 1); i++)
	{
		data[i] = i2c_read_ack();
	}
	data[(length - 1)] = i2c_read_nack();

	i2c_stop();

	return 0;
}

void i2c_stop(void)
{
	cli();
	TWAR = 0;
	// transmit STOP condition
	TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);
	sei();
}


ISR(TWI_vect)
{
  switch(TW_STATUS)
  {
    case TW_SR_DATA_ACK:
      // received data from master, call the receive callback
      I2C_recv(TWDR); 
      TWCR = (1<<TWIE) | (1<<TWINT) | (1<<TWEA) | (1<<TWEN);
      break;
    case TW_ST_SLA_ACK:
      // master is requesting data, call the request callback
      I2C_req();
      TWCR = (1<<TWIE) | (1<<TWINT) | (1<<TWEA) | (1<<TWEN);
      break;
    case TW_ST_DATA_ACK:
      // master is requesting data, call the request callback
      I2C_req();
      TWCR = (1<<TWIE) | (1<<TWINT) | (1<<TWEA) | (1<<TWEN);
      break;
    case TW_BUS_ERROR:
      // some sort of erroneous state, prepare TWI to be readdressed
      TWCR = 0;
      TWCR = (1<<TWIE) | (1<<TWINT) | (1<<TWEA) | (1<<TWEN); 
      break;
    default:
      TWCR = (1<<TWIE) | (1<<TWINT) | (1<<TWEA) | (1<<TWEN);
      break;
  }
} 