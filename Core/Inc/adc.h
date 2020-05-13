#ifndef ADC_H
#define ADC_H

#define ADC_SMPL_RATE_5_14_MSPS		0U
#define ADC_SMPL_RATE_3_6_MSPS		0x00000018U
#define ADC_SMPL_RATE_973_KSPS		0x00000028U
#define ADC_SMPL_RATE_371_31_KSPS	0x00000030U
#define ADC_SMPL_RATE_117_264_KSPS	0x00000038U

void setupADCs(void);
void calibrateADCs(void);
void initADC(channel_t *chx);
void setupADCSingle(channel_t *chx);
void setupADCDual(uint32_t num);
void setupADCParallel(uint32_t num);
void setupADCQuad(void);
void ADC_Clocks(void);

#endif
