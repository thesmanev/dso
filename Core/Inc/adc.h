#ifndef ADC_H
#define ADC_H

void setupADCs(void);
void calibrateADCs(void);
void initADC(channel_t *chx);
void setupADCSingle(channel_t *chx);
void setupADCDual(uint32_t num);
void setupADCParallel(uint32_t num);
void setupADCQuad(void);
void ADC_Clocks(void);
void calibrateADC(channel_t *chx);

#endif
