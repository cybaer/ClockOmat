/*
 * ui.h
 *
 *  Created on: 21.08.2020
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

#ifndef UI_H_
#define UI_H_

#include"Divider.h"
#include "HardwareConfig.h"

using namespace avrlib;
class Ui
{
public:
  Ui(void);
  void init();
  void poll();
  void doEvents();
  void update();
  void onClock();

private:
  int16_t setBPM(int8_t xcr);
  int16_t getBPM() { return m_BPM; }

  
  bool m_NewClock;
  int8_t m_Xcrement;
  int8_t m_AdcChannel;
  int8_t m_Button;
  bool m_Stop;
  int16_t m_BPM;

  Divider* m_ActualDivider;
  
  class IUiState
  {
  public:
    //** Empty Constructor */
    IUiState(void) {}
    virtual void onEntry(Ui&) const {}
    virtual void onExit(Ui&) const {}
    virtual void onClick(Ui&) const {}
    virtual void onLongClick(Ui&) const {}
    virtual void onXcrement(Ui&, int8_t xcrement) const {}
    virtual void onTimeout(Ui&) const {}

  private:
    DISALLOW_COPY_AND_ASSIGN (IUiState);
  };
  
  /**
   * State machine
   */
  class CInitState: public IUiState
  {
  public:
    static CInitState& getInstance(void)
    {
      static CInitState s_instance;
      return s_instance;
    }
  };

  class CClockState: public IUiState
  {
  public:
    static CClockState& getInstance(void)
    {
      static CClockState s_instance;
      return s_instance;
    }
    virtual void onEntry(Ui&) const;
    virtual void onExit(Ui&) const;
    virtual void onClick(Ui&) const;
    virtual void onXcrement(Ui&, int8_t xcrement) const;
  };
  class CClockEditState: public IUiState
  {
  public:
    static CClockEditState& getInstance(void)
    {
      static CClockEditState s_instance;
      return s_instance;
    }
    virtual void onEntry(Ui&) const;
    virtual void onExit(Ui&) const;
    virtual void onClick(Ui&) const;
    virtual void onXcrement(Ui&, int8_t xcrement) const;
  };

  class CDividerState: public IUiState
  {
  public:
    static CDividerState& getInstance(void)
    {
      static CDividerState s_instance;
      return s_instance;
    }
    virtual void onEntry(Ui&) const;
    virtual void onExit(Ui&) const;
    virtual void onClick(Ui&) const;
    virtual void onXcrement(Ui&, int8_t xcrement) const;
  };
  class CDividerEditState: public IUiState
  {
  public:
    static CDividerEditState& getInstance(void)
    {
      static CDividerEditState s_instance;
      return s_instance;
    }
    virtual void onEntry(Ui&) const;
    virtual void onExit(Ui&) const;
    virtual void onClick(Ui&) const;
    virtual void onXcrement(Ui&, int8_t xcrement) const;
  };
  
  void setState(IUiState& state)
  {
    m_State->onExit(*this);
    m_State = &state;
    m_State->onEntry(*this);
  }
  
  IUiState* m_State;
};

extern Ui ui;

#endif /* UI_H_ */
