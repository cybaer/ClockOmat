// Copyright 2012 Emilie Gillet.
//
// Author: Emilie Gillet (emilie.o.gillet@gmail.com)
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//
// See http://creativecommons.org/licenses/MIT/ for more information.
//
// -----------------------------------------------------------------------------
//
// Driver for 4x14-segments display.

#ifndef DRIVERS_DISPLAY_H_
#define DRIVERS_DISPLAY_H_


#include "avrlib/base.h"
#include "7_SegmentCharacter.h"


typedef void (*setFct)(uint8_t);

static const int8_t BrightnessLevels = 4;


static const auto a = A<0x20, 0x80, 0x01, 0x08, 0x10, 0x40, 0x02>(); 

template<typename GPIO, int8_t Digits, setFct setD1, setFct setD2, setFct setD3>
class Display_7_Seg
{
 public:
  static inline void Init()
  {
    m_Buffer[0] = '7';
    m_Buffer[1] = '6';
    m_Buffer[2] = 'E';
    m_ActivePosition = 0;
  }

  static inline void Refresh()
  {
    //if (brightness_pwm_cycle_ <= m_Brightness) 
    //{
      DigitIO[m_ActivePosition](false);
      m_ActivePosition = (m_ActivePosition + 1) % Digits;
      //writeDigit(/*chars[static_cast<uint8_t>*/(m_Buffer[m_ActivePosition])]);
      writeDigit(a.arr[m_Buffer[m_ActivePosition]]);

      DigitIO[m_ActivePosition](true);
    /*} else 
    {
    GPIOB->BRR = kCharacterEnablePins[active_position_];
    }*/
    //brightness_pwm_cycle_ = (brightness_pwm_cycle_ + 1) % kBrightnessLevels;
  }


  static inline void Print(const char* s)
  {
    strncpy(m_Buffer, s, Digits);
  }

  static inline char* mutable_buffer() { return m_Buffer; }
  static inline void setBrightness(int8_t brightness) { m_Brightness = brightness; }

 private:
  static inline void writeDigit(uint8_t data)
  {
    GPIO::Ports.Gpio = data;
    GPIO::WriteIO();
  }

  static setFct DigitIO[Digits];
  static char m_Buffer[Digits];
  static int8_t m_ActivePosition;
  //static int8_t brightness_pwm_cycle_;
  static int8_t m_Brightness;

};

template<typename GPIO, int8_t Digits, setFct setD1, setFct setD2, setFct setD3>
setFct Display_7_Seg<GPIO, Digits, setD1, setD2, setD3>::DigitIO[Digits] = {setD1, setD2, setD3};

template<typename GPIO, int8_t Digits, setFct setD1, setFct setD2, setFct setD3>
char Display_7_Seg<GPIO, Digits, setD1, setD2, setD3>::m_Buffer[Digits] = {0};

template<typename GPIO, int8_t Digits, setFct setD1, setFct setD2, setFct setD3>
int8_t Display_7_Seg<GPIO, Digits, setD1, setD2, setD3>::m_ActivePosition = {0};

template<typename GPIO, int8_t Digits, setFct setD1, setFct setD2, setFct setD3>
int8_t Display_7_Seg<GPIO, Digits, setD1, setD2, setD3>::m_Brightness = {BrightnessLevels - 1};

#endif
