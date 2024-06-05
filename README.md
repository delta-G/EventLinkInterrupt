# EventLinkInterrupt

> This library is deprecated. <br> The Arduino Renesas Core now includes code that makes this library redundant. <br> Here is an example that uses the new core functions:

```
#include "IRQManager.h"

volatile bool interruptFired = false;

GenericIrqCfg_t cfg;

void setup() {

  Serial.begin(115200);
  while (!Serial)
    ;
  Serial.println("\n\n *** " __FILE__ " ***\n\n");

  R_ICU->IRQCR[0] = 0xB0;  // Configure some peripheral for an interrupt

  // Create a generic interrupt configuration and attach
  cfg.irq = FSP_INVALID_VECTOR;
  cfg.ipl = 12;
  cfg.event = ELC_EVENT_ICU_IRQ0;  
  IRQManager::getInstance().addGenericInterrupt(cfg, Myirq0_callback);

  // Enable the interrupt at the peripheral. 
  R_PFS->PORT[1].PIN[5].PmnPFS = (1 << R_PFS_PORT_PIN_PmnPFS_PCR_Pos) | (1 << R_PFS_PORT_PIN_PmnPFS_ISEL_Pos);

}

void loop() {
  Serial.println("Loop Running");
  delay(500);

  if (interruptFired) {
    interruptFired = false;
    Serial.println("Interrupt Fired");
  }
}

void Myirq0_callback() {
  R_ICU->IELSR[cfg.irq] &= ~(R_ICU_IELSR_IR_Msk);
  interruptFired = true;
}
```

 
                 



