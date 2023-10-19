/*

EventLinkInterrupt.cpp 

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



#include "EventLinkInterrupt.h"
extern rtc_instance_ctrl_t rtc_ctrl;
extern rtc_cfg_t rtc_cfg;


int attachEventLinkInterrupt(uint8_t eventCode, Irq_f func /*= nullptr*/) {
  RTCIrqCfg_t rtc_irq_cfg;

  rtc_irq_cfg.req = RTC_CARRY;
  rtc_irq_cfg.cfg = &rtc_cfg;
  rtc_irq_cfg.ctrl = &rtc_ctrl;

  int eventLinkIndex = -1;
  if (!(rtc_cfg.carry_irq == FSP_INVALID_VECTOR)) {
    // carry irq is already set.
    //  Our trick won't work.
  } else {

    if (IRQManager::getInstance().addPeripheral(IRQ_RTC, &rtc_irq_cfg)) {
      // Find the event link we just got
      for (uint8_t i = 0; i < 32; i++) {
        volatile uint32_t val = R_ICU->IELSR[i];
        // RTC_CUP event code is 0x28
        if ((val & 0xFF) == 0x28) {
          eventLinkIndex = i;
          break;
        }
      }
      //  If we found something.
      if (eventLinkIndex >= 0) {
        /* getting the address of the current location of the irq vector table */
        volatile uint32_t *irq_ptr = (volatile uint32_t *)SCB->VTOR;
        /* set the displacement to the "programmable" part of the table */
        irq_ptr += 16;
        __disable_irq();
        // replace the rtc_carry_isr handler with our own
        *(irq_ptr + eventLinkIndex) = (uint32_t)func;
        // replace the event link code (0x47 is CAC_FERRI)
        R_ICU->IELSR[eventLinkIndex] = eventCode;
        // clear any pending flags
        R_CAC->CAICR |= (R_CAC_CAICR_FERRFCL_Msk);
        R_ICU->IELSR[eventLinkIndex] &= ~(R_ICU_IELSR_IR_Msk);
        __enable_irq();
        // Take the carry irq back out of the rt configuration
        // in case it matters elsewhere or in case you want to attach it again.
        rtc_cfg.carry_irq = FSP_INVALID_VECTOR;
      } 
    } 
  }
  return eventLinkIndex;
}

void resetEventLink(int eventLinkIndex) {
  R_ICU->IELSR[eventLinkIndex] &= ~(R_ICU_IELSR_IR_Msk);
}

