# EventLinkInterrupt

This code is to allow use of event link interrupts on the UNO-R4 boards.  

It works by hijacking the mechanism in IRQManager class that attaches the RTC Carry interrupt. 
This code includes RTC.h from the core in order to get the RTC configuration struct.  I'm not sure what that will break in your code. 

It includes two functions:
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


