#ifndef DMA_H
#define DMA_H

void setupDMA(channel *chx);
void setupDMADual(uint32_t num);//num = 0 -> adc1_2 num = 1 ->adc3_4
void setupDMAParallel(uint32_t num);
void setupDMAQuad(void);
void setDMACntr(channel *chx);

#endif
