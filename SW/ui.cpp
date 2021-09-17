/*
 * ui.cpp
 *
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
 */

#include "avrlib/op.h"
#include "ui.h"
#include "clock.h"

Ui::Ui()
: m_NewClock(false)
, m_Xcrement(0)
, m_AdcChannel()
, m_Button(0)
, m_Stop(false)
, m_BPM(120)
, m_State(&CInitState::getInstance())
{}

void Ui::init()
{
  m_Stop = false;
  Adc::StartConversion(m_AdcChannel);
  setState(CClockState::getInstance());
}

void Ui::poll()
{
  m_Xcrement = Encoder::Read(); 
  if (Adc::ready())
  {
    auto button = Buttons::getButton();
    if(m_Button == 0 && button == 1) m_Stop = !m_Stop;
    if(m_Button == 0 && button == 2) 
    {
      m_Stop = true;
      dividerFarm.reset();
    }
    
    m_Button = button;
    Adc::StartConversion(AdcChannelButtons);
  }
}

void Ui::doEvents()
{
  if(m_Xcrement != 0) m_State->onXcrement(*this, m_Xcrement);
  if(Encoder::clicked()) m_State->onClick(*this);

  clock.update(m_BPM); 
  Display::Refresh();
}

void Ui::update()
{
  if(ResetIn::isTriggered())
  {
    dividerFarm.reset();
  }
  if(ClockIn::isTriggered())
  {
    m_Stop = false;
    (void)clock.ClockInEdge();
  }
  if(m_NewClock) 
  {
    m_NewClock = false;
    if(!m_Stop)
    {
      bool clk = clock.getClock();
      Output_7::set_value(clk);
      dividerFarm.clock(clk);
    }
  }
}

void Ui::onClock()
{
  m_NewClock = true;
}

int16_t Ui::setBPM(int8_t xcr)
{
  m_BPM += xcr;
  if(m_BPM < 30) m_BPM = 30;
  if(m_BPM > 300) m_BPM = 300;
  return m_BPM;
}


// ***** State Machine *****

void Ui::CClockState::onEntry(Ui& context) const
{
  Display::Print("Clc");
}

void Ui::CClockState::onExit(Ui& context) const
{

}

void Ui::CClockState::onClick(Ui& context) const
{
  context.setState(CClockEditState::getInstance());
}

void Ui::CClockState::onXcrement(Ui& context, int8_t xcrement) const
{
  if(xcrement > 0)
  {
    (void)dividerFarm.setFirstDivider();
    context.setState(CDividerState::getInstance());
  }
}

//-------------------------

void Ui::CClockEditState::onEntry(Ui& context) const
{
  Display::Print(context.getBPM());
}

void Ui::CClockEditState::onExit(Ui& context) const
{

}
void Ui::CClockEditState::onClick(Ui& context) const
{
  context.setState(CClockState::getInstance());
}

void Ui::CClockEditState::onXcrement(Ui& context, int8_t xcrement) const
{
  int16_t bpm = context.setBPM(xcrement);
  Display::Print(bpm);
}

//-------------------------

void Ui::CDividerState::onEntry(Ui& context) const
{
  Display::Print("Ch", dividerFarm.getDividerNo());
}

void Ui::CDividerState::onExit(Ui& context) const
{

}

void Ui::CDividerState::onClick(Ui& context) const
{
  context.setState(CDividerEditState::getInstance());
}

void Ui::CDividerState::onXcrement(Ui& context, int8_t xcrement) const
{
  if(xcrement > 0)
  {
    dividerFarm.setNextDivider();
    Display::Print("Ch", dividerFarm.getDividerNo());
  }
  if(xcrement < 0)
  {
    if(dividerFarm.getDividerNo() > 1)
    {
      dividerFarm.setPrevDivider();
      Display::Print("Ch", dividerFarm.getDividerNo());
    }
    else
    {
      context.setState(CClockState::getInstance());
    }
  }
}

//-------------------------

void Ui::CDividerEditState::onEntry(Ui& context) const
{
  Display::Print(dividerFarm.getActualDivider().getDivider());
}

void Ui::CDividerEditState::onExit(Ui& context) const
{
  
}

void Ui::CDividerEditState::onClick(Ui& context) const
{
  context.setState(CDividerState::getInstance());
}

void Ui::CDividerEditState::onXcrement(Ui& context, int8_t xcrement) const
{
  Divider& div = dividerFarm.getActualDivider();
  auto idx = div.getDividerIndex();
  /// ToDo: Begrenzung einbauen!
  idx += xcrement;
  
  auto val = div.setDividerIndex(idx);
  Display::Print(val);
}