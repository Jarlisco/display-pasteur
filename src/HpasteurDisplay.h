#ifndef HPASTEUR_DISPLAY_H
#define HPASTEUR_DISPLAY_H

#include <AvrGPIO.h>

/*
  Hotel Pasteur Display Programming interfaces based on Atmel AVR

  \author L.Morin (UnivRennes1).
*/

namespace hpasteur
{

#include "netlist.h"

  /*
    Control all AVR GPIO segments connected to the Display Board.
    
    Associate all AVR GPIO to its specific controling line (a resistor).
    The Segment C and D are used, and a part of the segment A is used for buttons.
  */
  class AVRDisplaySegment : public GpioSegment
  {
  protected:
    typedef AvrOuputPin<((uint8_t)&DDRC), ((uint8_t)&PORTC)> AvrOuputPinC;
    typedef AvrOuputPin<((uint8_t)&DDRD), ((uint8_t)&PORTD)> AvrOuputPinD;
    typedef AvrInputPin<((uint8_t)&DDRB), ((uint8_t)&PORTB), ((uint8_t)&PINB)> AvrInputPinB;

    static const uint8_t GpioSegment_C_Count = 3;
    static const uint8_t GpioSegment_D_Count = 8;

  public:
    /* Constructor */ AVRDisplaySegment(void) : GpioSegment()
    {
      // unset disable pull-up
      MCUCR &= ~(1 << PUD);

      m_count = 0;
      ((AvrOuputPinD *)&m_gpioTable[m_count])->reset(2);
      m_count++;
      ((AvrOuputPinD *)&m_gpioTable[m_count])->reset(3);
      m_count++;
      ((AvrOuputPinD *)&m_gpioTable[m_count])->reset(0);
      m_count++;
      ((AvrOuputPinD *)&m_gpioTable[m_count])->reset(1);
      m_count++;
      ((AvrOuputPinC *)&m_gpioTable[m_count])->reset(1);
      m_count++;
      ((AvrOuputPinC *)&m_gpioTable[m_count])->reset(2);
      m_count++;
      ((AvrOuputPinC *)&m_gpioTable[m_count])->reset(0);
      m_count++;
      ((AvrOuputPinD *)&m_gpioTable[m_count])->reset(6);
      m_count++;
      ((AvrOuputPinD *)&m_gpioTable[m_count])->reset(7);
      m_count++;
      ((AvrOuputPinD *)&m_gpioTable[m_count])->reset(5);
      m_count++;
      ((AvrOuputPinD *)&m_gpioTable[m_count])->reset(4);
      m_count++;

      ((AvrInputPinB *)&m_buttonTable[0])->reset(1);
      ((AvrInputPinB *)&m_buttonTable[1])->reset(0);
    }
    OutputPin *getGPIO(size_t index) { return &m_gpioTable[index]; }
    void setLevel(size_t index, GpioPin::State_t state) { m_gpioTable[index].setLevel(state); }
    bool getButton(size_t index) { return !m_buttonTable[index & 0x03].getLevel(); }

  protected:
    OutputPin m_gpioTable[GpioSegment_D_Count + GpioSegment_C_Count];
    InputPin m_buttonTable[/*AvrI*/ (2)];
  };

  /*
    Adapt the control the Display Board to the location of LEDs on the board.
    
    The association table is based on a file generated automatically from the Kicad Project.
  */
  class HPLetter : public AVRDisplaySegment
  {
  public:
    /* Constructor */ HPLetter(void) : AVRDisplaySegment(), m_activeLed(0) {}

    uint8_t getLedID(uint8_t led) { return g_ledTable[led][0]; }
    uint8_t getLedXPos(uint8_t led) { return g_ledTable[led][1]; }
    uint8_t getLedYPos(uint8_t led) { return g_ledTable[led][2]; }

    uint8_t ledCount(void) { return g_ledTableSize; }

    void ledOn(uint8_t led)
    {
      if (m_activeLed == led)
        return;
      uint8_t panode = g_ledTable[m_activeLed][3];
      uint8_t pcathode = g_ledTable[m_activeLed][4];
      uint8_t anode = g_ledTable[led][3];
      uint8_t cathode = g_ledTable[led][4];
      if (panode != anode && panode != cathode)
        setLevel(panode - 1, GpioPin::HIGH_IMPEDANCE);
      if (pcathode != anode && pcathode != cathode)
        setLevel(pcathode - 1, GpioPin::HIGH_IMPEDANCE);

      setLevel(anode - 1, GpioPin::G_HIGH);
      setLevel(cathode - 1, GpioPin::G_LOW);
      m_activeLed = led;
    }

    void ledAllOff()
    {
      for (int i = 0; i < GpioSegment_D_Count + GpioSegment_C_Count; i++)
      {
        m_gpioTable[i].setLevel(GpioPin::HIGH_IMPEDANCE);
      }
    }

  protected:
    uint8_t m_activeLed;
  };

#ifdef HPASTEUR_DISPLAY_TESTS
  extern void exploresubCombinations(AVRDisplaySegment &gpioSegment, size_t index);
  extern void swapLevels(AVRDisplaySegment &gpioSegment);
  extern void swapLeds(HPLetter &segment);
#endif

} // namespace hpasteur
#endif
