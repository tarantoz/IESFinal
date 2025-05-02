#include "CallForHeat.h"

// States of call for heat signal


enum states{IDLE, HEATING}  state;

//Returns of one if call of heat signal is detected, 0 otherwise
char callForHeat()
{
    if(state == IDLE)
        return 0;
    else
        return 1;
}

// Pin initalization for P3.1 (input and interrupt enabled)
void callForHeatInit()
{
    _bis_SR_register(GIE);
    P3DIR &= ~BIT1;
    P3IES &= ~BIT1;
    P3IE |= BIT1;
    P3REN |= BIT1;
    P3IFG &= ~BIT1;
}

// Port 3 interrupt service routine
// Change LED State
#pragma vector=PORT3_VECTOR
__interrupt void Port_3(void)
{
    P3IFG &= ~BIT1;                         // Clear P3.1 IFG
    if((P3IES & BIT1) == 0)  //Rising edge
    {
        state = HEATING;
        P3IES |= BIT1; //Set to falling edge
    }
    else //Falling edge
    {
        state = IDLE;
        P3IES &= ~BIT1; //Set to rising edge
    }
            
}   
