#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include <string.h>

#include "main.h"
#include <i2c_master.h>

using namespace hpasteur;

HPLetterFont hpSegment;
char currentChar = 'X';
mode currentMode = RUNNING_MODE;
bool scrolling = false;
bool display_flag = false;
char display_buffer[256];
uint8_t string_size = 0;
uint8_t i2c_slaves_connected = 0;

//////////////////////////////////////////////////////////
// TIMER
//////////////////////////////////////////////////////////

void setup_timer()
{
  cli();

  //set timer1 interrupt at 5Hz
  TCCR1A = 0; // set entire TCCR1A register to 0
  TCCR1B = 0; // same for TCCR1B
  TCNT1 = 0;  //initialize counter value to 0
  // set compare match register for 1hz increments
  OCR1A = 3125; // = (16*10^6) / (5*1024) - 1 (must be <65536)
  // turn on CTC mode
  TCCR1B |= (1 << WGM12);
  // Set CS12 and CS10 bits for 1024 prescaler
  TCCR1B |= (1 << CS12) | (1 << CS10);
  // enable timer compare interrupt
  TIMSK1 |= (1 << OCIE1A);

  // //set timer2 interrupt at 150Hz
  // TCCR2A = 0; // set entire TCCR2A register to 0
  // TCCR2B = 0; // same for TCCR2B
  // TCNT2 = 0;  //initialize counter value to 0
  // // set compare match register for 8khz increments
  // OCR2A = 50; //249; // = (16*10^6) / (65*1024) - 1 (must be <256)
  // // turn on CTC mode
  // TCCR2A |= (1 << WGM21);
  // // 1024 prescaler
  // TCCR2B |= (1 << CS22) | (1 << CS20);
  // // enable timer compare interrupt
  // TIMSK2 |= (1 << OCIE2A);

  sei();
}

ISR(TIMER1_COMPA_vect)
{
  display_flag = true;
}

// ISR(TIMER2_COMPA_vect)
// {
//   hpSegment.write(currentChar);
// }

//////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////
// SPI
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

ISR(SPI_STC_vect)
{
  uint8_t spi_data = SPDR;

  if (currentMode != MASTER_MODE)
  {
    currentMode = MASTER_SETUP_MODE;
  }

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
// I2C
//////////////////////////////////////////////////////////

void i2c_receive_event(uint8_t data)
{
  if (currentMode == RUNNING_MODE && currentChar != (char)data && hpSegment.write((char)data))
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
    if (ret != 0)
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
  spi_init_slave();
  uint8_t currentAddr = 0;

  while (true)
  {
    switch (currentMode)
    {
    case SETUP_MODE:
      currentChar = 'S';
      if (hpSegment.getButton(0))
      {
        currentAddr = get_first_slave_available_addr();
        if (currentAddr == -1)
        {
          currentChar = 'E';
        }
        else
        {
          i2c_slave_init(currentAddr);
          i2c_slave_set_callbacks(i2c_receive_event, i2C_request_event);
          currentChar = ' ';
          currentMode = RUNNING_MODE;
        }
        _delay_ms(20);
      }
      break;
    case RUNNING_MODE:
      if (hpSegment.getButton(0))
      {
        currentMode = SETUP_MODE;
        _delay_ms(20);
      }
      if (hpSegment.getButton(1))
      {
        // Nothing for the moment
      }
      break;
    case MASTER_SETUP_MODE:
      currentChar = 'M';
      if (hpSegment.getButton(0))
      {
        i2c_stop();
        i2c_init();
        i2c_slaves_connected = 0;
        for (uint8_t addr = 8; addr < 0x77; addr++)
        {
          uint8_t ret = i2c_start(addr + I2C_WRITE);
          if (ret == 0)
          {
            i2c_slaves_connected++;
          }
          i2c_stop();
        }
        currentChar = ' ';
        currentMode = MASTER_MODE;
        setup_timer();
        _delay_ms(20);
      }
      break;
    case MASTER_MODE:
      if (hpSegment.getButton(0))
      {
        currentMode = MASTER_SETUP_MODE;
        _delay_ms(20);
        break;
      }

      if (hpSegment.getButton(1))
      {
        scrolling = !scrolling;
        _delay_ms(20);
      }

      if (spi_received_flag)
      {
        memcpy(&display_buffer, &spi_buffer, 256);
        string_size = spi_pos - 1;
        spi_pos = 0;
        spi_received_flag = false;
      }

      if (display_flag && string_size > 0)
      {
        if (scrolling)
        {
          while (string_size < i2c_slaves_connected + 2)
          {
            display_buffer[string_size] = ' ';
            string_size++;
          }

          char first = display_buffer[0];
          for (int i = 0; i < string_size; i++)
          {
            display_buffer[i] = display_buffer[i + 1];
          }
          display_buffer[string_size] = first;
        }

        uint8_t positionMessage = 0;
        i2c_stop();
        i2c_init();
        for (uint8_t addr = 8; addr < 8 + i2c_slaves_connected; addr++)
        {
          uint8_t ret = i2c_start(addr + I2C_WRITE);
          if (ret == 0)
          {
            uint8_t data[1];
            if (positionMessage < string_size)
              data[0] = display_buffer[positionMessage];
            else
              data[0] = ' ';
            i2c_transmit(addr, data, 1);
            positionMessage++;
          }
          i2c_stop();
          // hpSegment.write(currentChar);
        }

        if (positionMessage < string_size && currentChar != display_buffer[positionMessage])
          currentChar = display_buffer[positionMessage];
        else if (positionMessage >= string_size)
          currentChar = ' ';

        display_flag = false;
      }
      break;
    }

    hpSegment.write(currentChar);
  }
}
