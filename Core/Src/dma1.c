#include "main.h"

/**
 * @brief DMA1 Channel 1 Interrupt handler
 */
void DMA1_Channel1_IRQHandler(void)
{
	if(DMA1->ISR & DMA_ISR_TCIF1)
	{
		ADC1->CR |= ADC_CR_ADSTP; //stop adc1 (and ADC2 bcs dual mode)
		DMA1_Channel1->CCR &= ~DMA_CCR_EN; // disable dma1
		DMA1->IFCR = DMA_IFCR_CGIF1 | DMA_IFCR_CTCIF1; // clear full transfer interrupt flag and global interrupt flag
	}
	else
	{
		if(DMA1->ISR & DMA_ISR_TEIF1)
		{
			DMA1->IFCR = DMA_IFCR_CGIF1 | DMA_IFCR_CTEIF1; // clear transfer error interrupt flag, and global interrupt flag
			assertError(12);
		}
		else
		{
			DMA1->IFCR = DMA_IFCR_CGIF1; // clear global interrupt flag
		}
	}
}

/**
 * @brief DMA2 Channel 1 Interrupt handler
 */
void DMA2_Channel1_IRQHandler(void)
{
	if(DMA2->ISR & DMA_ISR_TCIF1)
	{
		ADC2->CR |= ADC_CR_ADSTP; //stop adc1 (and ADC2 bcs dual mode)
		DMA2_Channel1->CCR &= ~DMA_CCR_EN; // disable dma1
		DMA2->IFCR = DMA_IFCR_CGIF1 | DMA_IFCR_CTCIF1; // clear full transfer interrupt flag and global interrupt flag
	}
	else
	{
		if(DMA2->ISR & DMA_ISR_TEIF1)
		{
			DMA2->IFCR = DMA_IFCR_CGIF1 | DMA_IFCR_CTEIF1; // clear transfer error interrupt flag, and global interrupt flag
			assertError(12);
		}
		else
		{
			DMA2->IFCR = DMA_IFCR_CGIF1; // clear global interrupt flag
		}
	}
}

/**
 * @brief DMA2 Channel 5 Interrupt handler
 */
void DMA2_Channel5_IRQHandler(void)
{
	if(DMA2->ISR & DMA_ISR_TCIF5)
	{ // if transfer complete interrupt flag for ch5
		ADC3->CR |= ADC_CR_ADSTP; //stop adc3
		DMA2_Channel5->CCR &= ~DMA_CCR_EN; // disable dma2
		DMA2->IFCR = DMA_IFCR_CGIF5 | DMA_IFCR_CTCIF5; // clear full transfer interrupt flag and global interrupt flag
	}
	else
	{
		if(DMA2->ISR & DMA_ISR_TEIF5)
		{
			DMA2->IFCR = DMA_IFCR_CGIF5 | DMA_IFCR_CTEIF5; // clear transfer error interrupt flag, and global interrupt flag
			assertError(12);
		}
		else
		{
			DMA2->IFCR = DMA_IFCR_CGIF5;
			; // clear global interrupt flag
		}
	}
}

/**
 * @brief DMA2 Channel 2 Interrupt handler
 */
void DMA2_Channel2_IRQHandler(void)
{
	if(DMA2->ISR & DMA_ISR_TCIF2)
	{
		ADC4->CR |= ADC_CR_ADSTP; //stop adc1 (and ADC2 bcs dual mode)
		DMA2_Channel2->CCR &= ~DMA_CCR_EN; // disable dma1
		DMA2->IFCR = DMA_IFCR_CGIF2 | DMA_IFCR_CTCIF2; // clear full transfer interrupt flag and global interrupt flag
	}
	else
	{
		if(DMA2->ISR & DMA_ISR_TEIF2)
		{
			DMA2->IFCR = DMA_IFCR_CGIF2 | DMA_IFCR_CTEIF2; // clear transfer error interrupt flag, and global interrupt flag
			assertError(12);
		}
		else
		{
			DMA2->IFCR = DMA_IFCR_CGIF2; // clear global interrupt flag
		}
	}
}

/**
 * @brief Setup DMA for the given channel
 * @param chx Pointer to a channel structure.
 */
void setupDMA(channel_t *chx)
{
	switch (chx->id)
	{
		default:
		case CH1_ID:
		{
			DMA1_Channel1->CPAR = (uint32_t) (&ADC1->DR); // address of peripheral
			DMA1_Channel1->CMAR = (uint32_t) buffer1; 	  // base address of memory buffer
			break;
		}
		case CH2_ID:
		{
			DMA2_Channel1->CPAR = (uint32_t) (&ADC2->DR);
			DMA2_Channel1->CMAR = (uint32_t) buffer2;
			break;
		}
		case CH3_ID:
		{
			DMA2_Channel5->CPAR = (uint32_t) (&ADC3->DR); // address of peripheral
			DMA2_Channel5->CMAR = (uint32_t) buffer3;      // base address of memory buffer
			break;
		}
		case CH4_ID:
		{
			DMA2_Channel2->CPAR = (uint32_t) (&ADC4->DR);
			DMA2_Channel2->CMAR = (uint32_t) buffer4;
			break;
		}
	}
}

/**
 * @brief Setup DMA for ADCs running in dual mode
 * @param num 0 for ADCs 1 & 2, 1 for ADCs 3 & 4
 */
void setupDMADual(uint32_t num)
{
	if(num == ACQ_DUAL_34)
	{
		DMA2_Channel5->CPAR = (uint32_t) (&ADC34_COMMON->CDR); // address of peripheral
		DMA2_Channel5->CMAR = (uint32_t) buffer3;  // base address of memory buffer
	}
	else if(num == ACQ_DUAL_12)
	{
		DMA1_Channel1->CPAR = (uint32_t) (&ADC12_COMMON->CDR); // address of peripheral
		DMA1_Channel1->CMAR = (uint32_t) buffer1;  // base address of memory buffer
	}
}

/**
 * @brief Setup DMA for two channels running in parallel
 * @param num 0 for ADCs 1 & 2, 1 for ADCs 3 & 4
 */
void setupDMAParallel(uint32_t num)
{
	if(num == ACQ_PARALLEL_34)
	{
		setupDMA(&dso.ch3);
		setupDMA(&dso.ch4);
	}
	else if(num == ACQ_PARALLEL_12)
	{
		setupDMA(&dso.ch1);
		setupDMA(&dso.ch2);
	}
}

/**
 * @brief Setup DMA for QUAD channel acquisition
 */
void setupDMAQuad(void)
{
	DMA1_Channel1->CPAR = (uint32_t) (&ADC12_COMMON->CDR);  // address of peripheral
	DMA1_Channel1->CMAR = (uint32_t) buffer1;               // base address of memory buffer
	DMA2_Channel5->CPAR = (uint32_t) (&ADC34_COMMON->CDR); // address of peripheral
	DMA2_Channel5->CMAR = (uint32_t) buffer3;               // base address of memory buffer

}

/**
 * @brief Set the DMA counter for each channel
 * @param chx Pointer to a channel structure
 */
void setDMACntr(channel_t *chx)
{
	switch (chx->id)
	{
		default:
		case CH1_ID:
			DMA1_Channel1->CNDTR = 2 * BUFF_SIZE; // size of memory target
			break;
		case CH2_ID:
			DMA2_Channel1->CNDTR = 2 * BUFF_SIZE; // size of memory target
			break;
		case CH3_ID:
			DMA2_Channel5->CNDTR = 2 * BUFF_SIZE;
			break;
		case CH4_ID:
			DMA2_Channel2->CNDTR = 2 * BUFF_SIZE;
			break;
	}
}

//void setDMACntr(channel *chx){
//	switch(chx->id){
//		case 1:{
//			switch(dso.timeDiv){
//				case 0:{ // 1 ms
//					DMA1_Channel1->CNDTR = 2048; // size of memory target
//					break;
//				}
//				case 1:{// 500 us
//					DMA1_Channel1->CNDTR = 2048; // size of memory target
//					break;
//				}
//				case 2:{//250 us
//					DMA1_Channel1->CNDTR = 2048; // size of memory target
//					break;
//				}
//				case 3:{// 100 us
//					DMA1_Channel1->CNDTR = 2048; // size of memory target
//					break;
//				}
//				case 4:{//50 us
//					DMA1_Channel1->CNDTR = 4096; // size of memory target
//					break;
//				}
//				case 5:{//25 us
//					DMA1_Channel1->CNDTR = 2048; // size of memory target
//					break;
//				}
//				case 6:{//10 us
//					DMA1_Channel1->CNDTR = 1024; // size of memory target
//					break;
//				}
//				case 7:{//5 us
//					DMA1_Channel1->CNDTR = 1024; // size of memory target
//					break;
//				}
//				case 8:{//2.5 us
//					DMA1_Channel1->CNDTR = 1024; // size of memory target
//					break;
//				}
//				case 9:{//1 us
//					DMA1_Channel1->CNDTR = 512; // size of memory target
//					break;
//				}
//			}
//		}
//		case 2:{
//			switch(dso.timeDiv){
//				case 0:{ // 1 ms
//					ADC2->SMPR1 = 0x00000030; // => sampling time 194 cycles (371,13 KS/s)
//					break;
//				}
//				case 1:{// 500 us
//					ADC2->SMPR1 = 0x00000028; // => sampling time 74 cycles (973 KS/s)
//					break;
//				}
//				case 2:{//250 us
//					ADC2->SMPR1 = 0x00000028; // => sampling time 74 cycles (973 KS/s)
//					break;
//				}
//				case 3:{// 100 us
//					ADC2->SMPR1 = 0x00000020; // => sampling time 32 cycles (2.25 MS/s)
//					break;
//				}
//				case 4:{//50 us
//					ADC2->SMPR1 = 0x00000000; // => sampling time 14 cycles (5.14 MS/s)
//					break;
//				}
//				case 5:{//25 us
//					ADC2->SMPR1 = 0x00000000; // => sampling time 14 cycles (5.14 MS/s)
//					break;
//				}
//				case 6:{//10 us
//					ADC2->SMPR1 = 0x00000000; // => sampling time 14 cycles (5.14 MS/s)
//					break;
//				}
//				case 7:{//5 us
//					ADC2->SMPR1 = 0x00000000; // => sampling time 14 cycles (5.14 MS/s)
//					break;
//				}
//				case 8:{//2.5 us
//					ADC2->SMPR1 = 0x00000000; // => sampling time 14 cycles (5.14 MS/s)
//					break;
//				}
//				case 9:{//1 us
//					ADC2->SMPR1 = 0x00000000; // => sampling time 14 cycles (5.14 MS/s)
//					break;
//				}
//			}
//		}
//		case 3:{
//			switch(dso.timeDiv){
//				case 0:{ // 1 ms
//					ADC3->SMPR1 = 0x00000030; // => sampling time 194 cycles (371,13 KS/s)
//					break;
//				}
//				case 1:{// 500 us
//					ADC3->SMPR1 = 0x00000028; // => sampling time 74 cycles (973 KS/s)
//					break;
//				}
//				case 2:{//250 us
//					ADC3->SMPR1 = 0x00000028; // => sampling time 74 cycles (973 KS/s)
//					break;
//				}
//				case 3:{// 100 us
//					ADC3->SMPR1 = 0x00000020; // => sampling time 32 cycles (2.25 MS/s)
//					break;
//				}
//				case 4:{//50 us
//					ADC3->SMPR1 = 0x00000000; // => sampling time 14 cycles (5.14 MS/s)
//					break;
//				}
//				case 5:{//25 us
//					ADC3->SMPR1 = 0x00000000; // => sampling time 14 cycles (5.14 MS/s)
//					break;
//				}
//				case 6:{//10 us
//					ADC3->SMPR1 = 0x00000000; // => sampling time 14 cycles (5.14 MS/s)
//					break;
//				}
//				case 7:{//5 us
//					ADC3->SMPR1 = 0x00000000; // => sampling time 14 cycles (5.14 MS/s)
//					break;
//				}
//				case 8:{//2.5 us
//					ADC3->SMPR1 = 0x00000000; // => sampling time 14 cycles (5.14 MS/s)
//					break;
//				}
//				case 9:{//1 us
//					ADC3->SMPR1 = 0x00000000; // => sampling time 14 cycles (5.14 MS/s)
//					break;
//				}
//			}
//		}
//		case 4:{
//			switch(dso.timeDiv){
//				case 0:{ // 1 ms
//					ADC4->SMPR1 = 0x00000030; // => sampling time 194 cycles (371,13 KS/s)
//					break;
//				}
//				case 1:{// 500 us
//					ADC4->SMPR1 = 0x00000028; // => sampling time 74 cycles (973 KS/s)
//					break;
//				}
//				case 2:{//250 us
//					ADC4->SMPR1 = 0x00000028; // => sampling time 74 cycles (973 KS/s)
//					break;
//				}
//				case 3:{// 100 us
//					ADC4->SMPR1 = 0x00000020; // => sampling time 32 cycles (2.25 MS/s)
//					break;
//				}
//				case 4:{//50 us
//					ADC4->SMPR1 = 0x00000000; // => sampling time 14 cycles (5.14 MS/s)
//					break;
//				}
//				case 5:{//25 us
//					ADC4->SMPR1 = 0x00000000; // => sampling time 14 cycles (5.14 MS/s)
//					break;
//				}
//				case 6:{//10 us
//					ADC4->SMPR1 = 0x00000000; // => sampling time 14 cycles (5.14 MS/s)
//					break;
//				}
//				case 7:{//5 us
//					ADC4->SMPR1 = 0x00000000; // => sampling time 14 cycles (5.14 MS/s)
//					break;
//				}
//				case 8:{//2.5 us
//					ADC4->SMPR1 = 0x00000000; // => sampling time 14 cycles (5.14 MS/s)
//					break;
//				}
//				case 9:{//1 us
//					ADC4->SMPR1 = 0x00000000; // => sampling time 14 cycles (5.14 MS/s)
//					break;
//				}
//			}
//		}
//	}
//}

