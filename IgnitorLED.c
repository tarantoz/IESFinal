#include "IgnitorLED.h"
#include <msp430.h>


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
    P5DIR |= BIT4;
    P5OUT &= ~ BIT4;
}


// Timer0_A0 interrupt service routine
#pragma vector = TIMER0_A0_VECTOR
__interrupt void Timer_A(void)
{
    //If there is a five min delay, it will count the seconds until 300 is reached, then set variable to 0
    if(fiveMinDelay)
    {
        seconds++;          // Increments seconds by one

        if (seconds >= 300) // 5 minutes reached
        {
            seconds = 0;   // Reset if you want it to repeat
            fiveMinDelay = 0;
        }
    }
    //If stabalize is called, it will hold until 5 seconds is reached, then set variable to 0
    if(stabalize)
    {
        seconds++;

        if (seconds >= 5) // 5 minutes reached
        {
            seconds = 0;   // Reset if you want it to repeat
            stabalize = 0;
        }
    }
}