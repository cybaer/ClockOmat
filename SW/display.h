/*
 *  Created on: 21.08.2021
 *      Author: cybaer
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef DISPLAY_H_
#define DISPLAY_H_

#include "stdio.h"
#include "avrlib/base.h"
#include "7_SegmentCharacter.h"

typedef void (*setFct)(uint8_t);

static const int8_t BrightnessLevels = 4;

extern const uint8_t* LUT_7Seg_Character;

template<typename GPIO, int8_t Digits, setFct setD1, setFct setD2, setFct setD3>
class Display_7_Seg
{
 public:
  static inline void Init()
  {
    
    m_ActivePosition = 0;
  }

  static inline void Refresh()
  {
    //if (brightness_pwm_cycle_ <= m_Brightness) 
    //{
      DigitIO[m_ActivePosition](false);
      m_ActivePosition = (m_ActivePosition + 1) % Digits;
      //writeDigit(/*chars[static_cast<uint8_t>*/(m_Buffer[m_ActivePosition])]);
      writeDigit(LUT_7Seg_Character[m_Buffer[m_ActivePosition]]);

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

  static inline void Print(const uint16_t v)
  {
    if(v < 1000)
    {
      char buf[4];
      sprintf(buf, "%3u", v);
      Print(buf);
    }
    else Print("Err");
  }

  static inline void Print(const char* s, const uint8_t v)
  {
    Print(v);
    strncpy(m_Buffer, s, Digits-1);
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
