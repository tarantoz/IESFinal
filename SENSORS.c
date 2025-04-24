#include "SENSORS.h"
#include <math.h>
#include <msp430.h>
// Thermistor, thermocouple, potentiometer
// Thermocouple -> P1.3 (A3)
// Thermistor -> P1.5 (A5)
// Pot -> 1.4  (A4)
// Datasheet pg. 96

//All temps are in Celsius

char ADCFinished = 0;
unsigned int ADCResult = 0;
char ADCChannel = 3;
unsigned int thermistorTemp, thermoTemp;
unsigned int potTemp;
unsigned int thermistorResistance;
float Vout;
//unsigned int;
char flag = 0;
char flameLosses = 0;


// Initalizes Pins for ADC
void ADCInit()
{
    // Configure ADC Pins
    P1SEL0 |= BIT3 | BIT4 | BIT5;                           //P0 Sel for desired ADC setup is 1
    P1SEL1 |= BIT3 | BIT4 | BIT5;                           //P1 Sel for desired ADC setup is 1

    // Configure ADC
    ADCCTL0 |= ADCSHT_2 | ADCON;                             // ADCON, S&H=16 ADC clks
    ADCCTL1 |= ADCSHP;                                       // ADCCLK = MODOSC; sampling timer
    ADCCTL2 &= ~ADCRES;                                      // clear ADCRES in ADCCTL
    ADCCTL2 |= ADCRES_2;                                     // 12-bit conversion results
    ADCIE |= ADCIE0;  

    // Configure OA0 in PGA mode
    SAC0OA = NMUXEN | PMUXEN | PSEL_0 | NSEL_1 | OAPM;
    // PMUXEN: enable positive mux (OA0+ = P1.3)
    // NMUXEN: enable internal PGA feedback (OA0– fed back)
    // PSEL_1: OA0+ = P1.3
    // NSEL_1: OA0– = internal node for PGA
    // OAPM: high-speed mode

    SAC0PGA |= MSEL_2 | GAIN;
    // MSEL_0 = Gain of 1
    // MSEL_1 = 2, MSEL_2 = 4, ..., MSEL_5 = 32
    SAC0OA |= SACEN | OAEN; // Enable SAC and OA
}

// Reads ADC value from the channel chosen, channel 3 = Thermocouple, channel 4 = Pot, channel 5 = Thermistor
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

// Sets ADC channel to thermistor, converts ADC value to temperature
float readThermistor()
{
    ADCChannel = 5;
    float result = readADC(5);
    float Beta = 3977; //Found in Datasheet
    float T0 = 298.15;
    Vout = (result/4095)*3.3;
    thermistorResistance = 10000*((3.3/Vout)-1);
    //thermistorResistance = 10000*(Vout/(3.3 - Vout));
    thermistorTemp = 1/((1/T0)+(1/Beta)*log(thermistorResistance/10000.0));
    //thermistorTemp = 1/(2251.74 *log(thermistorResistance));
    thermistorTemp = thermistorTemp - 273.15;
    // thermistorResistance = 41250000/readADC(5);
    // thermistorTemp = (0.003 + 0.0003 * log(thermistorResistance/10000));
    return thermistorTemp;
}

// Sets ADC channel to thermocouple, converts ADC value to temperature
unsigned int readThermocouple()
{
    ADCChannel = 3;
    thermoTemp = readADC(3);
    //return thermoTemp;
    //Compare against threshold value for ~1000°C flame (e.g., Type K + gain 32) -> used chatgpt
    if (thermoTemp >= 3)  // corresponds to ~1000°C
        return 1;         // Flame detected
    else
        return 0;         // No flame
}

// Sets ADC value to potentiometer, converts ADC value to temperature
unsigned int readPot()
{
    ADCChannel = 4;
    potTemp = (((readADC(4))>>6) - 4) + 40;
    return potTemp;
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
            ADCResult = ADCMEM0;    // Stored ADC value
            ADCFinished = 1;        // Marks when ADC is finished reading
            break;
        default:
            break;
    }
            
} 