#include "SYSTEM.h"
#include "intrinsics.h"
#inlcude <msp430>

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;               // Stop watchdog timer
    
    systemInit();

    PM5CTL0 &= ~LOCKLPM5;                   // Disable the GPIO power-on default high-impedance mode
                                            // to activate previously configured port settings

    while(1)
    {
        // Need to add RGB stuff once Ethan is done
        if(callForHeat())
        {
            start:                      //Jump location for the start of the heating cycle
            setIgnitor();               // turns on the ignitor
            turnOnPilotValve();
            if(!readThermocouple())
            {
                closePilotValve();
                stopIgnitor();
                delay:
                fiveMinDelay = 1;
                while(fiveMinDelay);
                goto start;
            }
           
            stopIgnitor();
            openMainValve();  //PWM
            stabalize = 1;
            while(stabalize);
            while(1)
            {
                cfh = callForHeat();
                cf = readThermocouple();
                bt = (readThermistor() < THRESHOLD);
                if(callForHeat() || readThermocouple() || (readThermistor() < THRESHOLD))
                {
                    flag = 0;
                }

                if(!callForHeat())
                {
                    flag |= BIT0;
                    break;
                    
                }
                if(!readThermocouple())
                {
                    flag |= BIT1;
                    break;
                }
                if(readThermistor() > Threshold)
                {
                    flag |= BIT2;
                    break;
                }
            }

            if(flag & BIT1)
            {
                closePilotValve();
                closeMainValve();
                flameLosses += 1;
                if(flameLosses <= 5)
                {
                    goto start;
                }
                else
                {
                    goto delay;
                }
            }
            if(flag & BIT0)
            {
                closePilotValve();
                closeMainValve();
            }

        }
        
        //Loop
        _bis_SR_register(CPUOFF)
    }
}
