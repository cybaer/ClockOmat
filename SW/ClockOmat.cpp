/*
 * ClockOmat.cpp
 *
 *  Created on: 21.05.2020
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

#include <avr/io.h>
#include <avr/interrupt.h>
#include "avrlib/time.h"
#include "avrlib/gpio.h"
#include "avrlib/adc.h"
#include "HardwareConfig.h"
#include "clock.h"
#include "ui.h"
#include "Divider.h"

// __Compiler Bug__
int __cxa_guard_acquire(__guard *g) {return !*(char *)(g);};
void __cxa_guard_release (__guard *g) {*(char *)g = 1;};
void __cxa_guard_abort (__guard *) {};
void __cxa_pure_virtual() {};


volatile uint8_t num_clock_ticks = 0;
volatile bool poll = false;
Ui ui;

DividerFarm dividerFarm;
Divider Div1(Output_1::set_value);
Divider Div2(Output_2::set_value, 2);
Divider Div3(Output_3::set_value, 3);
Divider Div4(Output_4::set_value, 4);
Divider Div5(Output_5::set_value, 5);
Divider Div6(Output_6::set_value, 6);
//Divider Div7(Output_7::set_value);

ISR(TIMER1_COMPA_vect)
{
  PwmChannel1A::set_frequency(clock.Tick());  // N Ticks per Clock from ClockInput
  if(clock.running())
  {
    ++num_clock_ticks;
  }
}

ISR(TIMER2_OVF_vect, ISR_NOBLOCK)
{
  //ca 4kHz
  while (num_clock_ticks)
  {
    --num_clock_ticks;
    ui.onClock();
  }

  static int8_t subClock = 0;
  subClock = (subClock + 1) & 1;

  if (subClock == 0)
  { // 2kHz
    poll = true;
  }
}


int main(void)
{
  // Configure the timers.
    
    //PWM with CTC
    Timer<1>::set_mode(0, _BV(WGM12), PRESCALER);
    int16_t startFreq = 2000;
    int16_t counterForStartFreq = (F_CPU / PRESCALER_VALUE) / startFreq -1;
    PwmChannel1A::set_frequency(counterForStartFreq);
    Timer<1>::StartCompare();
    //TIMSK |= (1<<OCIE1A);

    Timer<2>::set_prescaler(2);
    Timer<2>::set_mode(TIMER_PWM_PHASE_CORRECT);
    Timer<2>::Start();

    sei();

  Adc::StartConversion(AdcChannelButtons);

  ui.init();
  // after initialization of all port extender IOs
  initHW();
  _delay_ms(50);

  
  while(1)
  {
    if(poll)
    {
      poll = false;
      ui.poll();
      ui.doEvents();
    }
    ui.update();
  }
}
