#ifndef DMA_H
#define DMA_H

#define DMA1_CHANNEL1_IRQ_PRIO		6u
#define DMA2_CHANNEL1_IRQ_PRIO		6u
#define DMA2_CHANNEL2_IRQ_PRIO		6u
#define DMA2_CHANNEL5_IRQ_PRIO		6u

void setupDMA(channel_t *chx)  __attribute__((section(".ccmram")));
void setupDMADual(uint32_t num)  __attribute__((section(".ccmram")));//num = 0 -> adc1_2 num = 1 ->adc3_4
void setupDMAParallel(uint32_t num)  __attribute__((section(".ccmram")));
void setupDMAQuad(void)  __attribute__((section(".ccmram")));
void setDMACntr(channel_t *chx)  __attribute__((section(".ccmram")));

#endif
