/*
 * Divider.cpp
 *
 *  Created on: 07.09.2021
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

#include "HardwareConfig.h"
#include "Divider.h"

Divider::Divider(setIOFct output, uint8_t div)
: Node()
, setOutput(output)
, m_Divider(div)
, m_DividerIndex(0)
, m_NextCounter(1)
, m_OldValue(false)
, m_Output(false)
{
  dividerFarm.Register(*this);
}

void Divider::reset()
{
  m_NextCounter = 1;
  m_OldValue = false;
  m_Output = false;
  setOutput(m_Output);
}

void Divider::clock(bool in, const int32_t& count)
{
  if(in != m_OldValue)
  {
    m_OldValue = in;
    if(m_NextCounter == count)
    {
      if((m_NextCounter += m_Divider) > COUNT_MAX) m_NextCounter = m_Divider;
      m_Output = !m_Output;
    }
  }
  setOutput(m_Output);
}

uint8_t Divider::setDividerIndex(uint8_t idx)
{
  m_DividerIndex = idx;
  m_Divider = DIVIDER[idx];

  auto actualCnt = dividerFarm.getMasterCount();
  int32_t quot = actualCnt / (m_Divider * 2);
  int32_t lastPeriodEnd = m_Divider * 2 * quot;

  if(!m_Output) 
  {
    m_NextCounter = lastPeriodEnd + 2 * m_Divider + 1;
  }
  else
  {
    if(actualCnt < lastPeriodEnd + m_Divider + 1)
    {
      m_NextCounter = lastPeriodEnd + m_Divider + 1;
    }
    else
    {
      m_NextCounter = lastPeriodEnd + 3 * m_Divider + 1;
    }
  }
    
  return m_Divider;
}

DividerFarm::DividerFarm()
: m_Dividers()
, m_ActualDivider(DividerFarm::m_Dividers.begin())
, m_DividerNo(0)
, m_MasterCount(-1)
, m_OldValue(false)
{}

void DividerFarm::Register(Divider& div)
{
  m_Dividers.push_back(div);
}

Divider& DividerFarm::setFirstDivider()
{
  m_ActualDivider = DividerFarm::m_Dividers.begin();
  m_DividerNo = 1;
  return *m_ActualDivider;  
}

Divider& DividerFarm::setNextDivider()
{
  if(m_ActualDivider != m_Dividers.last())
  {
    ++m_ActualDivider;
    ++m_DividerNo;
  }
  return *m_ActualDivider;
}

Divider& DividerFarm::setPrevDivider()
{
  if(m_ActualDivider != m_Dividers.begin())
  {
    --m_ActualDivider;
    --m_DividerNo;
  }
  return *m_ActualDivider;
}

void DividerFarm::reset(void)
{
  m_MasterCount = 0;
  m_OldValue = false;
  for(auto& div : m_Dividers)
  {
    div.reset();
  }
}

void DividerFarm::clock(bool in)
{
  if(in != m_OldValue)
  {
    m_OldValue = in;
    if(++m_MasterCount > COUNT_MAX) m_MasterCount = 0;
    for(auto& div : m_Dividers)
    {
      div.clock(in, m_MasterCount);
    }
  }
}