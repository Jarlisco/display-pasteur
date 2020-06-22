#define F_CPU 1000000UL
//#define F_CPU 16000000UL
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>

#include "main.h"
#include <i2c_master.h>

#define MODE_ADDR 0
#define CURRENT_CHAR_ADDR 1

using namespace hpasteur;

HPLetterFont hpSegment;
char currentChar = 'X';
mode currentMode = SETUP_MODE;

void testAllLetters(HPLetterFont *hpSegment)
{
  for (char letter = 'A'; letter <= 'Z'; letter++)
  {
    for (size_t index = 0; index < 100; index++)
    {
      hpSegment->write(letter);
    }
  }
  for (char digit = '0'; digit <= '9'; digit++)
  {
    for (size_t index = 0; index < 100; index++)
    {
      hpSegment->write(digit);
    }
  }
}

//////////////////////////////////////////////////////////
//-- SPI
//////////////////////////////////////////////////////////
char spi_buffer[256];
volatile uint8_t spi_pos;
volatile bool spi_received_flag = false;

void spi_init_slave(void)
{
  cli();
  DDRB = (1 << PINB4);             // MISO output - other input
  SPCR = (1 << SPE) | (1 << SPIE); // Enable SPI with interrupt - MODE 0 - MSBFIRST - Serial line 400khz
  SPDR = 0;                        // Set SPI data register to 0
  sei();
}

// Interruption routine
ISR(SPI_STC_vect)
{
  uint8_t spi_data = SPDR;
  currentMode = MASTER_MODE;

  if (spi_pos < 256)
  {
    spi_buffer[spi_pos] = spi_data;
    spi_pos++;
    SPDR = spi_data;
  }

  if (spi_data == '\r')
  {
    spi_received_flag = true;
  }
}

//////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////
//-- I2C
//////////////////////////////////////////////////////////

void i2c_receive_event(uint8_t data)
{
  if (currentMode == RUNNING_MODE && hpSegment.write((char)data))
  {
    currentChar = data;
  }
}

void i2C_request_event() {}

uint8_t get_first_slave_available_addr()
{
  i2c_stop();
  i2c_init();
  uint8_t i, ret;
  for (i = 0x08; i < 0x77; i++)
  {
    ret = i2c_start(i + I2C_WRITE);
    if (ret)
    {
      i2c_stop();
      return i;
    }
    i2c_stop();
  }
  return -1;
}

uint8_t find_first_found_slave_addr()
{
  i2c_stop();
  i2c_init();
  uint8_t i, ret;
  for (i = 0x08; i < 0x77; i++)
  {
    ret = i2c_start(i + I2C_WRITE);
    if (ret == 0)
    {
      i2c_stop();
      return i;
    }
    i2c_stop();
  }
  return -1;
}

//////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////


int main(void)
{
  uint8_t currentAddr = 0;
  uint8_t lastAddr = 0;

  spi_init_slave();
  testAllLetters(&hpSegment);

  // need to test if empty first
  //currentChar = eeprom_read_word((uint16_t*) CURRENT_CHAR_ADDR);
  //currentMode = eeprom_read_word((uint16_t*) MODE_ADDR);

  while (1)
  {

    switch (currentMode)
    {
    case SETUP_MODE:
      currentChar = 'X';
      if (hpSegment.getButton(0))
      {
        currentAddr = get_first_slave_available_addr();
        i2c_stop();
        i2c_slave_init(currentAddr);
        i2c_slave_set_callbacks(i2c_receive_event, i2C_request_event);
        currentChar = 'V';
        currentMode = RUNNING_MODE;
      }
      break;
    case RUNNING_MODE:
      if (hpSegment.getButton(0))
      {
        currentMode = SETUP_MODE;
      }
      break;
    case MASTER_MODE:
      if (spi_received_flag)
      {
        //lastAddr = get_first_slave_available_addr() - 1;
        lastAddr = find_first_found_slave_addr();
        i2c_init();
        uint8_t ret = i2c_start(lastAddr + I2C_WRITE);
        if (ret == 0)
        {
          // uint8_t data[] = {lastAddr, spi_pos};
          // i2c_transmit(lastAddr, data, 2);
          i2c_transmit(lastAddr, (uint8_t *)spi_buffer, spi_pos);
        }
        i2c_stop();
        spi_pos = 0;
        spi_received_flag = false;

        // send data to connected i2c devices
        // uint8_t i;
        // for (i = 0x08; i < lastAddr; i++)
        // {
        // }
      }
      break;
    }

    hpSegment.write(currentChar);
  }
}
