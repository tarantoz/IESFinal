#include "pilotValve.h"

// P5.0
// Output GPIO

// Initalizes pilot vale pins, P5.0 (output GPIO)
void pilotValveInit()
{
    P5DIR |= BIT0;
    P5OUT &= ~BIT0;
}

// Opens pilot valve
void openPilotValve()
{
    P5OUT |= BIT0;
}

// Close Pilot Valve
void closePilotValve()
{
    P5OUT &= ~BIT0;
}

