#ifndef ADC_H
#define ADC_H

#include "main.h"

void setupADCs(void);
void calibrateADCs(void);
void initADC(channel *chx);
void setupADCSingle(channel *chx);
void setupADCDual(uint32_t num); // num = 0 -> adc1_2, num = 1 -> adc3_4
void setupADCParallel(uint32_t num); // ch1/ch2 in regular simultaneous or ch3/ch4 in regular simultaneous
void setupADCQuad(void);
void ADC_Clocks(void);
void calibrateADC(channel *chx);

#endif
