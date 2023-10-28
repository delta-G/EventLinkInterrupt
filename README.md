# EventLinkInterrupt

> This code is to allow use of event link interrupts on the UNO-R4 boards.
> It works by hijacking the mechanism in IRQManager class that attaches the RTC Carry interrupt. 
> This code includes RTC.h from the core in order to get the RTC configuration struct.  I'm not sure what that will break in your code.

<br><br><br>


## **It includes four functions:**

## int attachEventLinkInterrupt(uint8_t eventCode, Irq_f func);
 
> attaches an interrupt handler in the Interrupt Link Controller
>
>> **eventCode** - The 8 bit code for the event link (See User's Manual Table 13.4)
>
>> **func** - void () function to attach as a handler. 
>
>> returns int - the index of the event link that was used. You need to save this to call resetEventLink in the interrupt handler.

## void detachEventLinkInterrupt(int eventLinkInterrupt);

> detaches an interrupt by clearing its entry in the IELSR registers
> This does not remove the entry from the vector table, so you can
> reattach an interrupt to the same handler with reattachEventLinkInterrupt
> This is necessary since the IRQManager will not re-use slots
>
>> **eventLinkIndex** - the index of the event link to dettach

## int reattachEventLinkInterrupt(uint8_t eventCode, int eventLinkIndex);

>  reattach an interrupt handler that was prevously detached with detachEventLinkInterrupt
> 
>>  **eventCode** - The 8 bit code for the event link (See User's Manual Table 13.4)
>
>>  **eventLinkIndex** - the index of the event link to reattach
>
>>  returns int - the index of the event link that was used. 

## void resetEventLink(int eventLinkIndex);
 
> clears the IR flag in the event link register to clear the interrupt
> This must be called from the interrupt handler or the board will hang
> There is probably also another interrupt flag for the event you linked
> in the registers for the peripheral you are linking
> 
>> **eventLinkIndex** - The index of the event
 
                 



