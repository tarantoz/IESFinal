#include "RGBLED.h"
#include <msp430.h>

// Initializes rgb LED pins and timerB3
void RGBLEDInit(){
  
  P6DIR |= BIT0 | BIT1 | BIT2;              // P6.0 - P6.2 output
  P6SEL1 &= ~(BIT0 | BIT1 | BIT2);                    // P1.6 and P1.7 options select
  P6SEL0 |= BIT0 | BIT1 | BIT2;

  // Disable the GPIO power-on default high-impedance mode to activate
  // previously configured port settings
  PM5CTL0 &= ~LOCKLPM5;

  TB3CCR0 = 1024-1;                         // PWM Period
  TB3CCTL1 = OUTMOD_3;                      // CCR1 set/reset
  TB3CCR1 = 750;                            // CCR1 PWM duty cycle
  TB3CCTL2 = OUTMOD_3;                      // CCR2 set/reset
  TB3CCR2 = 250;                            // CCR2 PWM duty cycle
  TB3CCTL3 = OUTMOD_3;                      // CCR3 set/reset
  TB3CCR3 = 500;                            // CCR3 PWM Duty Cycle
  TB3CTL = TBSSEL__SMCLK | MC__UP | TBCLR;  // SMCLK, up mode, clear TBR
 setRGBLED(0, 0, 255); 
}

// Sets rgb color through 0-255 arguments
void setRGBLED(char Red, char Green, char Blue){
  TB3CCR1 = Red << 2;     // Red * 4 to scale to range of 1024
  TB3CCR2 = Green << 2;
  TB3CCR3 = Blue << 2;
}


