#define HPASTEUR_DISPLAY_TESTS
#include <HpasteurDisplay.h>
#include <font.h>

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

    // TODO : AWAITING BUTTON FIX
    GpioPin::State_t  getButton(size_t index)
    {
        return GpioPin::G_LOW;
    }
  };

} // namespace hpasteur

enum mode
{
  SETUP_MODE,
  RUNNING_MODE,
  MASTER_MODE
};