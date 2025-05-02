#include "SYSTEM.h"

// Initalizes all pins of system
void systemInit()
{
    pilotValveInit();
    ignitorInit();
    callForHeatInit();
    ADCInit();
    initServo();
    RGBLEDInit();

}