/*
*     CAC_Example.ino
*      Example code for EventLinkInterrupt.h
*      
*      This code sets the Clock Acuracy Circuit (CAC) to
*      measure the number of ticks of the system clock for
*      for one tick of the LOCO clock.  
*      There are three possible interrupts that this can generate
*      and the code intentionally causes all three to fire and 
*      attaches interrupt handlers for each. 
*
*
*/
#include "EventLinkInterrupt.h"

int mendiEventLinkIndex;
volatile bool mendiFired = false;
int ferriEventLinkIndex;
volatile bool ferriFired = false;
int ovfiEventLinkIndex;
volatile bool ovfiFired = false;

void setup() {
  Serial.begin(115200);
  while (!Serial)
    ;
  Serial.println("Starting ILC Experiment!");
  //setup registers for CAC operation
  // and set to fire the interrupts
  setupCAC();
  
  // link interrupts to event codes in the ILCS
  //  See table 13.4 in the User's Manual for Event Link Codes (Event Number)
  // For the CAC interrupts it is 
  //0x47 for CAC_FERRI 
  //0x48 for CAC_MENDI
  //0x49 CAC_OVFI
  mendiEventLinkIndex = attachEventLinkInterrupt(0x48, cacMendiHandler);
  ferriEventLinkIndex = attachEventLinkInterrupt(0x47, cacFerriHandler);
  ovfiEventLinkIndex = attachEventLinkInterrupt(0x49, cacOvfiHandler);  

  // Start the CAC running.
  startCAC();

  Serial.println("End Setup!");
}

void loop() {
  if (mendiFired) {
    Serial.println("MENDI Fired.");
    mendiFired = false;
  }
  if (ferriFired) {
    Serial.println("FERRI Fired.");
    ferriFired = false;
  }
  if (ovfiFired) {
    Serial.println("OVFI Fired.");
    ovfiFired = false;
  }
}

void cacMendiHandler() {
  // clear interrupt flag in peripheral hardware
  R_CAC->CAICR |= (R_CAC_CAICR_MENDFCL_Msk);
  // clear event link interrupt flag
  resetEventLink(mendiEventLinkIndex);
  mendiFired = true;
}

void cacFerriHandler() {
  // clear interrupt flag in peripheral hardware
  R_CAC->CAICR |= (R_CAC_CAICR_FERRFCL_Msk);
  // clear event link interrupt flag
  resetEventLink(ferriEventLinkIndex);
  ferriFired = true;
}

void cacOvfiHandler() {
  // clear interrupt flag in peripheral hardware
  R_CAC->CAICR |= (R_CAC_CAICR_OVFFCL_Msk);
  // clear event link interrupt flag
  resetEventLink(ovfiEventLinkIndex);
  ovfiFired = true;
}


void setupCAC() {

  // Enable the CAC in the Module Stop Control Register
  R_MSTP->MSTPCRC &= ~((uint32_t)R_MSTP_MSTPCRC_MSTPC0_Msk);
  /*
I'm caculating 48MHz on MOSC / 32.767kHz on LOCO should
give a ratio of around 1:1465 when they both have the same pre-scaler
with ref clock on LOCO set to 1/8192 and the measure clock on HOCO set to 1/32
we should have 1:375000 and that will give us plenty of overflows.  
it will also take the LOCO clock about 250ms to generate MENDI and FERRI. 
*/
  //  Set limits loose so we get a MENDI interrupt.
  R_CAC->CALLVR = 1464;
  R_CAC->CAULVR = 1466;

  // Set source clock
  //  (EDGES[1:0]=00 Rising Edge) (TCSS[1:0]=11 1/32 divider) (FMCS[2:0]=010 HOCO) (CACREFE = 0 Disable CACREF pin) 
  R_CAC->CACR1 = 0x34;

  // set reference clock                          = 00 for 1/32 divider
  // (DFS[1:0] = 00 No Digital Filter) (RCDS[1:0] = 11 1/8192 divider) (RSCS[2:0] = 100 LOCO) (RPS = 1 Internal Clock Reference)
  R_CAC->CACR2 = 0x39;

  //  Enable interrupts in the peripheral hardware and clear flags there.
  R_CAC->CAICR |= (R_CAC_CAICR_MENDIE_Msk | R_CAC_CAICR_MENDFCL_Msk);
  R_CAC->CAICR |= (R_CAC_CAICR_FERRIE_Msk | R_CAC_CAICR_FERRFCL_Msk);
  R_CAC->CAICR |= (R_CAC_CAICR_OVFIE_Msk | R_CAC_CAICR_OVFFCL_Msk);
}

void startCAC(){
  // Set CFME in CACR0 to turn on the CAC unit
  R_CAC->CACR0 = (R_CAC_CACR0_CFME_Msk);
}