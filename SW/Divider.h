/*
 * Divider.h
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

#ifndef DIVIDER_H_
#define DIVIDER_H_

#include "lib/static_list.h"
#include "avrlib/base.h"
#include "avrlib/gpio.h"

constexpr int32_t gcd(int32_t m, int32_t n)
{
    return n == 0 ? m : gcd(n, m % n);
}

constexpr int32_t lcm(int32_t m, int32_t n)
{ 
    return (m / gcd(m,n) * n); 
}

static const auto DIV_COUNT = 21;
static constexpr int32_t DIVIDER[DIV_COUNT] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,20,24,32,64,128};

constexpr auto DividerLCM()
{
    auto a = DIVIDER[0] * 2;
    
    for(auto i=1; i<DIV_COUNT; i++)
    {
        auto b = DIVIDER[i] * 2;
        a = lcm(a, b);
    }
    return a;
}

static const int32_t COUNT_MAX = DividerLCM();
typedef void (*setIOFct)(uint8_t);

class Divider : public Node
{
public:
  Divider(setIOFct output, uint8_t div=1);
  void reset();
  void clock(bool in, const int32_t& count);
  uint8_t setDividerIndex(int8_t idx);
  int8_t getDividerIndex() const { return m_DividerIndex; }
  uint8_t getDivider() const { return m_Divider; }
  
private:
  setIOFct setOutput;
  int8_t m_Divider;
  int8_t m_DividerIndex;
  int32_t m_NextCounter;
  bool m_OldValue;
  bool m_Output;
};

class DividerFarm
{
public:
  DividerFarm();
  void Register(Divider& div);
  Divider& setFirstDivider();
  Divider& setNextDivider();
  Divider& setPrevDivider();
  Divider& getActualDivider() const { return *m_ActualDivider; }
  uint8_t getDividerNo() const { return m_DividerNo; }
  int32_t getMasterCount() const { return m_MasterCount; }

  void reset(void);
  void clock(bool in);
  
private:
  StaticList<Divider> m_Dividers;
  StaticList<Divider>::iterator m_ActualDivider;
  uint8_t m_DividerNo;
  int32_t m_MasterCount;
  bool m_OldValue;
};

extern DividerFarm dividerFarm;

#endif
