/*
 * HardwareConfig.h
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
 *
 */

#ifndef HARDWARECONFIG_H_
#define HARDWARECONFIG_H_

#include "avrlib/base.h"
#include "avrlib/gpio.h"
#include "avrlib/devices/switch.h"
#include "avrlib/adc.h"
#include "lib/ui/EdgeTrigger.h"
#include "lib/ui/analogSwitch.h"
#include "avrlib/devices/mcp23s08.h"
#include "display.h"

// __Compiler Bug__
__extension__ typedef int __guard __attribute__((mode (__DI__)));

extern "C" int __cxa_guard_acquire(__guard *);
extern "C" void __cxa_guard_release (__guard *);
extern "C" void __cxa_guard_abort (__guard *);
extern "C" void __cxa_pure_virtual();


using namespace avrlib;

// HW SPI
static const uint8_t SPI_Speed = 4;
typedef SpiMasterBase<MSB_FIRST, SPI_Speed> spi_master;

// Port Extender
typedef Gpio<PortC, 0> ExtenderSlaveSelect;
typedef MCP23S08<spi_master, ExtenderSlaveSelect, 0> portExtender;

typedef Inverter<Gpio<PortD, 7> > Digit_1;
typedef Inverter<Gpio<PortB, 0> > Digit_2;
typedef Inverter<Gpio<PortB, 1> > Digit_3;

typedef Display_7_Seg<portExtender, 3, Digit_1::set_value, Digit_2::set_value, Digit_3::set_value> Display;



typedef EdgeTrigger<Gpio<PortD, 2>, 0> ClockIn;
typedef EdgeTrigger<Gpio<PortC, 4>, 1> ResetIn;


typedef Inverter<Gpio<PortC, 1> > Output_7;
typedef Inverter<Gpio<PortC, 3> > Output_6;
typedef Inverter<Gpio<PortC, 5> > Output_5;
typedef Inverter<Gpio<PortB, 5> > Output_4;
typedef Inverter<Gpio<PortC, 2> > Output_3;
typedef Inverter<Gpio<PortD, 0> > Output_2;
typedef Inverter<Gpio<PortD, 1> > Output_1;

extern Adc adc;
static const uint8_t AdcChannelButtons = 7;

typedef AnalogSwitch<Adc, AdcChannelButtons, 3> Buttons;

inline void initInputs(void)
{
  ClockIn::init();
  ResetIn::init();
}
inline void initOutputs(void)
{
  Output_1::set_mode(DIGITAL_OUTPUT);
  Output_1::set_value(false);
  Output_2::set_mode(DIGITAL_OUTPUT);
  Output_2::set_value(false);
  Output_3::set_mode(DIGITAL_OUTPUT);
  Output_3::set_value(false);
  Output_4::set_mode(DIGITAL_OUTPUT);
  Output_4::set_value(false);
  Output_5::set_mode(DIGITAL_OUTPUT);
  Output_5::set_value(false);
  Output_6::set_mode(DIGITAL_OUTPUT);
  Output_6::set_value(false);
  Output_7::set_mode(DIGITAL_OUTPUT);
  Output_7::set_value(false);
  
  Digit_1::set_mode(DIGITAL_OUTPUT);
  Digit_1::set_value(false);
  Digit_2::set_mode(DIGITAL_OUTPUT);
  Digit_2::set_value(false);
  Digit_3::set_mode(DIGITAL_OUTPUT);
  Digit_3::set_value(false);
 
}

inline void initAnalogIn(void)
{
  adc.Init();
  Buttons::init();
}

inline void initHW(void)
{
  initInputs();
  initOutputs();
  initAnalogIn();

  spi_master::Init();
  portExtender::Init();

  Display::Init();
  Display::Refresh();
}

#endif
