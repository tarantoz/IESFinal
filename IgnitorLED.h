#include <msp430>

void startIgnitor();

void stopIgnitor();

void ignitorInit();

volatile unsigned int seconds = 0;
char fiveMinDelay = 0;
char stabalize = 0;



// Configure clock: SMCLK = 1 MHz / 8 = 125 kHz
CSCTL0_H = CSKEY_H;
CSCTL1 = DCOFSEL_0;         // DCO = 1 MHz
CSCTL2 = SELS__DCOCLK;
CSCTL3 = DIVS__8;           // SMCLK = DCO / 8 = 125 kHz
CSCTL0_H = 0;

// Timer_A0 setup for 1s interrupt:
// 125000 counts @ 125kHz = 1 second
TA0CCR0 = 125000 - 1;       
TA0CCTL0 = CCIE;            // Enable CCR0 interrupt
TA0CTL = TASSEL_2 | ID_0 | MC_1 | TACLR; // SMCLK, /1 divider, up mode

