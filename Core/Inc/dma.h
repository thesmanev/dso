#ifndef DMA_H
#define DMA_H

#define DMA1_CHANNEL1_IRQ_PRIO		6u
#define DMA2_CHANNEL1_IRQ_PRIO		6u
#define DMA2_CHANNEL2_IRQ_PRIO		6u
#define DMA2_CHANNEL5_IRQ_PRIO		6u

void setupDMA(channel_t *chx);
void setupDMADual(uint32_t num);//num = 0 -> adc1_2 num = 1 ->adc3_4
void setupDMAParallel(uint32_t num);
void setupDMAQuad(void);
void setDMACntr(channel_t *chx);

#endif
