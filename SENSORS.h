#include <msp430.h>

char ADCFinished = 0;
unsigned int ADCResult = 0;
char ADCChannel = 3;
unsigned int potTemp, thermistorTemp, thermoTemp;
unsigned int thermmistorResistance;
const unsigned int THRESHOLD = 250;
char flag = 0;
char flameLosses = 0;


void ADCInit();
unsigned int readADC(char channel);
unsigned int readThermistor();
unsigned int readThermocouple();
unsigned int readPot();