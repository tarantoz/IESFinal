#include "SYSTEM.h"

void systemInit()
{
    pilotValveInit();
    ignitorInit();
    callForHeatInit();
    ADCInit();
    timerB_init();

}