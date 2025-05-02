#include "SYSTEM.h"
#include "intrinsics.h"
#include <msp430.h>

extern char flag;
extern char flameLosses;
unsigned int threshold, cf, bt;
char cfh;
extern char fiveMinDelay, stabalize;
extern unsigned int thermoTemp;




int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;               // Stop watchdog timer
    
    //Initalizes all of the pins in the system, timers, and adc
    systemInit();

    PM5CTL0 &= ~LOCKLPM5;                   // Disable the GPIO power-on default high-impedance mode
                                            // to activate previously configured port settings

    while(1)
    {
        cfh = callForHeat();
        if(cfh)
        {
            setRGBLED(0, 255, 0);
            start:                      //Jump location for the start of the heating cycle
            flameLosses = 0;
            startIgnitor();               // turns on the ignitor
            openPilotValve();

            //Flame lost suring pilot stage, start 5 min delay
            readThermocouple();
            if(thermoTemp < 100)  
            {
                closePilotValve();
                stopIgnitor();
                delay:
                fiveMinDelay = 1;
                while(fiveMinDelay);
                goto start;
            }
           
           //Pilot flame proven, opens main valve
            stopIgnitor();
            setServoPosition(4000);  //PWM
            //__delay_cycles(5000000);    //Stabalize 5 seconds
            while(1)
            {
                threshold = readPot();
                cfh = callForHeat();
                cf = readThermocouple();
                bt = (readThermistor(readPot()));

                // System working as intended
                if(callForHeat() && readThermocouple() && (readThermistor() < threshold))
                {
                    //Green
                    setRGBLED(0, 255, 0);
                    flag = 0;
                }

                // Call for heat signal lost, reached desired temp
                if(!callForHeat())
                {
                    //Blue
                    setRGBLED(0, 0, 255);
                    flag |= BIT0;
                    break;
                    
                }

                //Flame lost
                if(!readThermocouple())
                {
                    //Yellow
                    setRGBLED(255, 255, 0);
                    flag |= BIT1;
                    flameLosses++;
                    break;
                }

                // DANGER - Boiler temp exceeds set temp
                if(readThermistor() >= threshold)
                {
                    //Red
                    setRGBLED(255, 0, 0);
                    flag |= BIT2;
                    break;
                }
            }

            //Shuts system down when boilerTemp > potTemp
            if(flag & BIT1)
            {
                closePilotValve();
                setServoPosition(1500);
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

            //Shuts valves after flame is lost
            if(flag & BIT0 )
            {
                closePilotValve();
                setServoPosition(1500);
            }

        }
        
        //Loop
        //_bis_SR_register(CPUOFF);
    }
}
