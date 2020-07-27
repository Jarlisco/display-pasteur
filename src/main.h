#define HPASTEUR_DISPLAY_TESTS
#define AVR_TICK_COUNTER_USE_32BIT
#include <HpasteurDisplay.h>
#include <font.h>

namespace hpasteur
{
  /*
   */
  class HPLetterFont : public HPLetter, public Font
  {
    private:
    bool blank = false;
  public:
    /* Constructor */ HPLetterFont(void) : HPLetter(), Font() {}

    bool write(char chr)
    {
      if (chr == ' ' && !blank)
      {
        ledAllOff();
        blank = true;
        _delay_ms(5);
        return true;
      }

      const uint8_t *charTable;
      size_t char_size;
      if (getCharacterByChar(chr, charTable, char_size))
      {
        for (size_t idx = 0; idx < char_size; idx++)
        {
          ledOn(pgm_read_word_near(charTable + idx));
        }
        blank = false;
        return true;
      }
      return false;
    }
  };

} // namespace hpasteur

enum mode
{
  SETUP_MODE,
  RUNNING_MODE,
  MASTER_MODE,
  MASTER_SETUP_MODE
};

// ticks_t bench(void (*func)()) {
//   const uint8_t limit = 100;
//   ticks_t rec = 0;
//   for(uint8_t i = 0; i < limit; i++) {
//     Tick::ResetCounter();
//     Tick::StartCounter();
//     func();
//     Tick::StopCounter();
//     rec += (Tick::Get() / limit);
//   }
//   //rec = rec / limit;
//   return rec;
// }

/*void testAllLetters(HPLetterFont *hpSegment)
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
}*/