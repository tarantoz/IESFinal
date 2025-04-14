#include "SENSORS.h"
#include <math.h>
// Thermistor, thermocouple, potentiometer
// Thermocouple -> P1.3 (A3)
// Thermistor -> P1.5 (A5)
// Pot -> 1.4  (A4)
// Datasheet pg. 96



void ADCInit()
{
    // Configure ADC Pins
    P1SEL0 |= BIT3 | BIT4 | BIT5;
    P1SEL1 |= BIT3 | BIT4 | BIT5;

    // Configure ADC
    ADCCTL0 |= ADCSHT_2 | ADCON;                             // ADCON, S&H=16 ADC clks
    ADCCTL1 |= ADCSHP;                                       // ADCCLK = MODOSC; sampling timer
    ADCCTL2 &= ~ADCRES;                                      // clear ADCRES in ADCCTL
    ADCCTL2 |= ADCRES_2;                                     // 12-bit conversion results
    ADCIE |= ADCIE0;  
}

unsigned int readADC(char channel)
{
    switch(channel)
    {
        case 3: //Thermo
            ADCMCTL0 |= ADCINCH_3;
            break;
        case 4: //Pot
            ADCMCTL0 |= ADCINCH_4;
            break;
        case 5: //Thermistor
            ADCMCTL0 |= ADCINCH_5; 
            break;
        default: //Thermistor
            ADCMCTL0 |= ADCINCH_5; 
            break;
    }

    ADCFinished = 0;
    ADCCTL0 |= ADCENC | ADCSC;
    while(ADCFinished != 1); 
    return ADCResult;
}

unsigned int readThermistor()
{
    ADCChannel = 5;
    thermmistorResistance = 41250000/readADC(5);
    thermistorTemp = (0.003 + +0.0003 * log(thermmistorResistance/10000) + 0.000002 * log(thermmistorResistance/10000)^^2 + 0.0000004 * log(thermmistorResistance/10000)^^3)^^-1;
}

unsigned int readThermocouple()
{
    ADCChannel = 3;
    thermoTemp = readADC(3) / 0.0008;
    if(thermoTemp > 10)
        return 1;
    else
        return 0;
}

unsigned int readPot()
{
    ADCChannel = 4;
    potTemp = 40 * (readADC(4) * 0.0008)/3.3 + 60;
}

// ADC interupt routine
#pragma vector=ADC_VECTOR
__interrupt void ADC_ISR(void)
{
    switch(__even_in_range(ADCIV,ADCIV_ADCIFG))
    {
        case ADCIV_NONE:
            break;
        case ADCIV_ADCOVIFG:
            break;
        case ADCIV_ADCTOVIFG:
            break;
        case ADCIV_ADCHIIFG:
            break;
        case ADCIV_ADCLOIFG:
            break;
        case ADCIV_ADCINIFG:
            break;
        case ADCIV_ADCIFG:
            ADCResult = ADCMEM0;
            ADCFinished = 1;
            break;
        default:
            break;
    }
            
}   

