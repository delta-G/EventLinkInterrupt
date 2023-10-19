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

/*
*
*  attachEventLinkInterrupt(uint8_t eventCode, Irq_f func);
*  attaches an interrupt handler in the event link controller
*
*  eventCode - The 8 bit code for the event link (See User's Manual Table 13.4)
*  func - void () function to attach as a handler. 
*
*  returns int - the index of the event link that was used. 
*                you will need this in the ISR to reset the interrupt
*/
int attachEventLinkInterrupt(uint8_t eventCode, Irq_f func = nullptr);

/*
*
*  resetEventLink(int eventLinkIndex);
*  clears the IR flag in the event link register to clear the interrupt
*  This must be called from the interrupt handler or the board will hang
*  There is probably also another interrupt flag for the event you linked
*
*  eventLinkIndex - The index of the event in the ELC.
*                   returned from attachEventLinkInterrupt
*
*/
void resetEventLink(int eventLinkIndex);



#endif  // defined EVENT_LINK_INTERRUPT_H
