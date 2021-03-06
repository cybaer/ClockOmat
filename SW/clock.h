/*
 * clock.h
 *
 *  Created on: 21.05.2021
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
 */

#ifndef CLOCK_H_
#define CLOCK_H_

#include "avrlib/base.h"
#include "avrlib/gpio.h"
#include "HardwareConfig.h"

//Settings for timer1 (16bit timer)
static const uint8_t PRESCALER = 1;
static const uint16_t PRESCALER_VALUE = 1L;


using namespace avrlib;
static const uint16_t INTERVALL_TICKS = 256;
static const uint8_t Resolution = 2;

class Clock
{
public:
  static inline void init(void)
  {
    m_TickCount = 0;
    m_DeltaTick = 0;
    m_Running = true;
  }

  static inline void start(void)
  {
    m_TickCount = 0;
    m_Running = true;
  }
  static inline void stop(void) { m_Running = false; }
  static inline void reset(void);
  static inline bool running(void) { return m_Running; }
  
  // runs in Interrupt context
  static inline uint16_t Tick()
  {
    m_TickCount++;
    m_DeltaTick = m_TickCount - m_OldTick;
    return m_Interval;
  }
  
  static inline bool ClockInEdge(void)
  {
    uint16_t newTick;
    // safe actual m_TickCount (Attention! 16bit copy not thread safe)
    do newTick = m_TickCount;
    while(newTick != m_TickCount);
    // INTERVALL_TICKS from Clock to Clock
    uint16_t deltaTick = newTick - m_OldTick;
    uint32_t numerator = static_cast<uint32_t>(m_Interval) * deltaTick;
    volatile uint16_t interval = (numerator + INTERVALL_TICKS / 2) / INTERVALL_TICKS;
    // safety copy of 16 bit
    cli();
    m_Interval = interval;
    sei();

    m_OldTick = newTick;
    return false;
  }

  static inline bool getClock(void)
  {
    static uint8_t resBit = INTERVALL_TICKS / Resolution;
    return (m_DeltaTick & resBit) == 0;
  }
  void update(uint16_t bpm, uint8_t multiplier = 1, uint8_t divider = 1);

private:
  static bool m_Running;
  static volatile uint16_t m_TickCount;
  static uint16_t m_OldTick;
  static int8_t m_DeltaTick;
  static uint16_t m_Interval;
};

extern Clock clock;

#endif /* CLOCK_H_ */
