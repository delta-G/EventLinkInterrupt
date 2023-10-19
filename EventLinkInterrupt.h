/*

EventLinkInterrupt.h 

     Copyright (C) 2023  David C. deltagrobotics@gmail.com

     This program is free software: you can redistribute it and/or modify
     it under the terms of the GNU General Public License as published by
     the Free Software Foundation, either version 3 of the License, or
     (at your option) any later version.

     This program is distributed in the hope that it will be useful,
     but WITHOUT ANY WARRANTY; without even the implied warranty of
     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
     GNU General Public License for more details.

     You should have received a copy of the GNU General Public License
     along with this program.  If not, see <http://www.gnu.org/licenses/>.

     */

#ifndef EVENT_LINK_INTERRUPT_H
#define EVENT_LINK_INTERRUPT_H

#include "Arduino.h"
#include "IRQManager.h"
#include "RTC.h"


int attachEventLinkInterrupt(uint8_t eventCode, Irq_f func = nullptr);
void resetEventLink(int eventLinkIndex);



#endif  // defined EVENT_LINK_INTERRUPT_H