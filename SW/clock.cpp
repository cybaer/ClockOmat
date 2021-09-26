/*
 * clock.cpp
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

#include "clock.h"

Clock clock;
static const int32_t BPM2HZ = 60L * static_cast<uint32_t>(Resolution) / 4L;
bool Clock::m_Running = true;
volatile uint16_t Clock::m_TickCount = 0;
uint16_t Clock::m_OldTick = 0;
int8_t Clock::m_DeltaTick = 0;

uint16_t Clock::m_Interval = (F_CPU / PRESCALER_VALUE / static_cast<uint32_t>(INTERVALL_TICKS) / static_cast<uint32_t>(60)) * BPM2HZ;

void Clock::update(uint16_t bpm, uint8_t multiplier, uint8_t divider)
{
  //                                                      |-- 1 period --> 2 toggle
  int32_t base_tick_duration = (F_CPU / PRESCALER_VALUE / 2 /static_cast<uint32_t>(INTERVALL_TICKS)) * BPM2HZ / static_cast<uint32_t>(bpm) ;
  m_Interval = base_tick_duration;
}
