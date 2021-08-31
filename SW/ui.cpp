/*
 * ui.cpp
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

#include "avrlib/op.h"
#include "ui.h"
#include "clock.h"

//#include "med5.h"

void Ui::init()
{
  Adc::StartConversion(m_AdcChannel);
}
void Ui::poll()
{
  Display::Refresh();
}
void Ui::doEvents()
{
  if(ResetIn::isTriggered())
  {
    DividerFarm::reset();
  }
  if(ClockIn::isTriggered())
  {
    m_Stop = false;
    (void)clock.ClockInEdge();
  }

  if (Adc::ready())
  {
    m_204 = Buttons::getValue();
    Adc::StartConversion(AdcChannelButtons);
  }
}

void Ui::onClock()
{
  bool clk = m_Stop ? false : clock.getClock();
  bool out1 = false;
  DividerFarm::clock(clk);
  out1 = clk;
  
  Output_1::set_value(out1);
  
 // Output_5::set_value(Div_2.getValue());
 // Output_6::set_value(Div_4.getValue());
 // Output_7::set_value(Div_8.getValue());
  
}

Divider::Divider(uint8_t divider)
: m_Divider(divider)
, m_Counter(0)
, m_OldValue(false)
, m_Output(false)
{
  DividerFarm::registerDivider(this);
}

Divider* DividerFarm::m_Divider[MaxDivider];
int8_t DividerFarm::m_Counter=0;
