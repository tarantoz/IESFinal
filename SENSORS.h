#include <msp430.h>

void ADCInit();
unsigned int readADC(char channel);
float readThermistor();
unsigned int readThermocouple();
unsigned int readPot();