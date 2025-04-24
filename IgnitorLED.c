#include "IgnitorLED.h"
#include <msp430.h>

volatile unsigned int seconds = 0;
char fiveMinDelay = 0;
char stabalize = 0;

// Turns the ignitor on
void startIgnitor()
{
    P5OUT |= BIT4;
}

// Turns the ignitor off
void stopIgnitor()
{
    P5OUT &= ~BIT4;
}

// Initializes the ingitor pin, P5.4 (output GPIO)
void ignitorInit()
{
    P5DIR |= BIT4;     // Set P5.4 as output
    P5OUT &= ~BIT4;    // Ensure ignitor is off
}

void timerB_init()
{
    // Configure Timer_B0 to count in seconds (1 second interrupt)
    TB0CCR0 = 1956 - 1;  // Set CCR0 for 1 second (15.625kHz * 1s = 1956)
    TB0CCTL0 |= CCIE;    // Enable CCR0 interrupt (interrupt on overflow)
    TB0CTL = TBSSEL_2 |  // Clock source: SMCLK (1 MHz / 8 = 125kHz)
             ID_3 |      // Timer input divider: /8 (for 15.625kHz clock)
             MC_1 |      // Up mode (counts up to CCR0)
             TBCLR;      // Clear Timer_B0 (start from 0)
} 

// Timer_B0 interrupt service routine
#pragma vector = TIMER0_B0_VECTOR
__interrupt void Timer_B(void)
{
    // If there is a five min delay, it will count the seconds until 300 is reached, then set variable to 0
    if(fiveMinDelay)
    {
        seconds++;          // Increments seconds by one

        if (seconds >= 300) // 5 minutes reached
        {
            seconds = 0;   // Reset if you want it to repeat
            fiveMinDelay = 0;
        }
    }

    // If stabilize is called, it will hold until 5 seconds is reached, then set variable to 0
    if(stabalize)
    {
        seconds++;

        if (seconds >= 5) // 5 seconds reached
        {
            seconds = 0;   // Reset if you want it to repeat
            stabalize = 0;
        }
    }
}