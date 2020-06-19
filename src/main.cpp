#define F_CPU 1000000UL
//#define F_CPU 16000000UL
#include <util/delay.h>
#include <avr/interrupt.h>
#include "Wire.h"

#define HPASTEUR_DISPLAY_TESTS
#include <HpasteurDisplay.h>
#include <font.h>
#include <i2c_master.h>

namespace hpasteur
{
  /*
   */
  class HPLetterFont : public HPLetter, public Font
  {
  public:
    /* Constructor */ HPLetterFont(void) : HPLetter(), Font() {}

    bool write(char chr)
    {
      const uint8_t *charTable;
      size_t char_size;
      if (getCharacterByChar(chr, charTable, char_size))
      {
        for (size_t idx = 0; idx < char_size; idx++)
        {
          ledOn(pgm_read_word_near(charTable + idx));
        }
        return true;
      }
      return false;
    }
  };

} // namespace hpasteur

//////////////////////////////////////////////////////////
//-- SPI
//////////////////////////////////////////////////////////
uint8_t spi_data = 0;
char message_buffer[256];
volatile byte pos;
volatile boolean message_flag = false;

void spi_init_slave(void)
{
  DDRB = (1 << PINB4);             // MISO output - other input
  SPCR = (1 << SPE) | (1 << SPIE); // Enable SPI with interrupt - MODE 0 - MSBFIRST - Serial line 400khz
  SPDR = 0;                        // Set SPI data register to 0
}

unsigned char spi_tranceiver(void)
{
  while (!(SPSR & (1 << SPIF)))
    ;            // Wait until data transfer complete
  return (SPDR); // Return data
}

// Interruption routine
ISR(SPI_STC_vect)
{
  uint8_t spi_data = SPDR;

  if (pos < 256)
  {
    message_buffer[pos++] = spi_data;
    SPDR = spi_data;
  }

  if (spi_data == '\r')
  {
    message_flag = true;
  }
}
//////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////

// using namespace hpasteur;

// int main(void)
// {

//   HPLetterFont hpSegment;
//   spi_init_slave();

//   while(1)
//   {
//     if (message_flag)
//     {
//       pos = 0;
//       message_flag = false;
//     }

//   }
// }

/*
  using namespace hpasteur;

  char buffer [50];
  volatile byte index;
  volatile boolean flag;

  ISR (SPI_STC_vect) // SPI interrupt routine 
  { 
    byte c = SPDR; // read byte from SPI Data Register
    if (index < sizeof buffer) {
      buffer[index++] = c; // save data in the next index in the array buff
      if (c == '\r') //check for the end of the word
      flag = true;
    }
  }



  int main(void) {

    HPLetterFont hpSegment;

    // Init SPI as Slave
    SPCR |= _BV(SPE);
    index = 0;
    flag = false;
    SPI.attachInterrupt();

    int char_cursor = 0;
    int msg_size = 0;
    sei();



    while(1){

      if (flag) {
        hpSegment.ledOn(0);
        _delay_ms(1000);

        flag = false;
        msg_size = index;
        char_cursor = 0;
        index = 0;
      }

      if(msg_size == 0){
        hpSegment.write('X');
      }else{
        for (size_t index = 0; index < 100; index++) {
          hpSegment.write(buffer[char_cursor]);
        }

        char_cursor++;
        if(char_cursor >= msg_size)
          char_cursor = 0;
      }
      
    }

  }
*/

using namespace hpasteur;

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

HPLetterFont hpSegment;
char currentChar = ' ';
uint8_t currentAddr = 0;
uint8_t lastAddr = 0;

void receiveEvent(uint8_t data)
{
  currentChar = data; // receive byte as a character
}

void requestEvent() {}

int main(void)
{

  testAllLetters(&hpSegment);

  unsigned char ret;
  i2c_slave_init(0x2c); // initialize I2C library
  i2c_slave_set_callbacks(receiveEvent, requestEvent);
  // Wire.begin();
  // Wire.onReceive(receiveEvent);

  // int address = 80;
  byte address = 0x58;

  while (1)
  {
    // ret = i2c_start(ADDR + I2C_WRITE);


    // for (address = 1; address < 127; address++)
    // {
      

    //   _delay_ms(200);
    // }

    // ret = i2c_start(address + I2C_WRITE);
    //   if (ret)
    //   {
    //     i2c_stop();
    //     hpSegment.write('N');
    //   }
    //   else
    //   {
    //     //char c = ((uint8_t)i2c_read_ack()) << 8;
    //     //c |= i2c_read_nack();
    //     byte data[] = {address, 'Y', 'A', 'O'};
    //     i2c_transmit(address, data, 4);
    //     i2c_stop();
    //     //hpSegment.write(c);
    //     hpSegment.write('Y');
    //   }


    // ret = i2c_start(address + I2C_READ);
    // if (ret)
    // {
    //   i2c_stop();
    // }
    // else
    // {
    //   char c = ((uint8_t)i2c_read_ack()) << 8;
    //   c |= i2c_read_nack();
    //   i2c_stop();
    //   hpSegment.write(c);
    // }

    // i2c_stop();

    
    hpSegment.write(currentChar);
  }
}
