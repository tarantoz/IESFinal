#include "SERVO.h"
#include <msp430.h>

// Initializes servo/main valve pins
void initServo() 
{
        // Configure P2.0 as output and select Timer function
        P2DIR |= BIT0;
        P2SEL0 |= BIT0;
        P2SEL1 &= ~BIT0;

        // Disable GPIO high-impedance mode
        PM5CTL0 &= ~LOCKLPM5;

        // Configure Timer1_B
        TB1CCR0 = 20000 - 1;                       // PWM period (~20ms)
        TB1CCTL1 = OUTMOD_7;                       // CCR1 reset/set mode
        TB1CCR1 = 1500;                            // Default pulse width ~1.5ms (neutral)
        TB1CTL = TBSSEL__SMCLK | MC__UP | TBCLR;   // SMCLK, Up mode, clear TBR
}

// Sets servo/main gas valve position, 1500 = closed, 4000 = open
// open and close functions were not used because it is supposed to be a variable gas valve
// If in the future it was going to be made to open to an angle depending on temp, this fucntion would still work
void setServoPosition(unsigned int position)
{
    TB1CCR1 = position; 
} 