#include "dma.h"

void DMA1_Channel1_IRQHandler(void){
	if(DMA1->ISR & DMA_ISR_TCIF1){
		ADC1->CR |= ADC_CR_ADSTP; //stop adc1 (and ADC2 bcs dual mode)
		DMA1_Channel1->CCR = 0x00002aaa;// disable dma1
		DMA1->IFCR = DMA_IFCR_CGIF1 | DMA_IFCR_CTCIF1; // clear full transfer interrupt flag and global interrupt flag
	}
	else{
		if(DMA1->ISR & DMA_ISR_TEIF1){
			DMA1->IFCR = DMA_IFCR_CGIF1 | DMA_IFCR_CTEIF1; // clear transfer error interrupt flag, and global interrupt flag
			assertError(12);
		}
		else{
			DMA1->IFCR = DMA_IFCR_CGIF1; // clear global interrupt flag
		}
	}
}

void DMA2_Channel1_IRQHandler(void){
	if(DMA2->ISR & DMA_ISR_TCIF1){
		ADC2->CR |= ADC_CR_ADSTP; //stop adc1 (and ADC2 bcs dual mode)
		DMA2_Channel1->CCR = 0x00002aaa;// disable dma1
		DMA2->IFCR = DMA_IFCR_CGIF1 | DMA_IFCR_CTCIF1; // clear full transfer interrupt flag and global interrupt flag
	}
	else{
		if(DMA2->ISR & DMA_ISR_TEIF1){
			DMA2->IFCR = DMA_IFCR_CGIF1 | DMA_IFCR_CTEIF1; // clear transfer error interrupt flag, and global interrupt flag
			assertError(12);
		}
		else{
			DMA2->IFCR = DMA_IFCR_CGIF1; // clear global interrupt flag
		}
	}
}

void DMA2_Channel5_IRQHandler(void){
	if(DMA2->ISR & DMA_ISR_TCIF5){ // if transfer complete interrupt flag for ch5
		ADC3->CR |= ADC_CR_ADSTP; //stop adc3
		DMA2_Channel5->CCR = 0x00002aaa;// disable dma2
//		if(dso1.quad){
//			ADC1->CR |= ADC_CR_ADSTP; //stop adc1 (and ADC2 bcs dual mode)
//			DMA1_Channel1->CCR = 0x00002aaa;// disable dma1
//		}
		DMA2->IFCR = DMA_IFCR_CGIF5 | DMA_IFCR_CTCIF5; // clear full transfer interrupt flag and global interrupt flag
	}
	else{
		if(DMA2->ISR & DMA_ISR_TEIF5){
			DMA2->IFCR = DMA_IFCR_CGIF5 | DMA_IFCR_CTEIF5; // clear transfer error interrupt flag, and global interrupt flag
			assertError(12);
		}
		else{
			DMA2->IFCR = DMA_IFCR_CGIF5;; // clear global interrupt flag
		}
	}
}

void DMA2_Channel2_IRQHandler(void){
	if(DMA2->ISR & DMA_ISR_TCIF2){
		ADC4->CR |= ADC_CR_ADSTP; //stop adc1 (and ADC2 bcs dual mode)
		DMA2_Channel2->CCR = 0x00002aaa;// disable dma1
		DMA2->IFCR = DMA_IFCR_CGIF2 | DMA_IFCR_CTCIF2; // clear full transfer interrupt flag and global interrupt flag
	}
	else{
		if(DMA2->ISR & DMA_ISR_TEIF2){
			DMA2->IFCR = DMA_IFCR_CGIF2 | DMA_IFCR_CTEIF2; // clear transfer error interrupt flag, and global interrupt flag
			assertError(12);
		}
		else{
			DMA2->IFCR = DMA_IFCR_CGIF2; // clear global interrupt flag
		}
	}
	interrupts[0] = 1;
}

void setupDMA(channel *chx){
//	setDMACntr(chx);
	switch(chx->id){
		case 1:{
			DMA1_Channel1->CPAR = (uint32_t)(&ADC1->DR); // address of peripheral
			DMA1_Channel1->CMAR = buffer1; // base address of memory buffer
			break;
		}
		case 2:{
			DMA2_Channel1->CPAR = (uint32_t)(&ADC2->DR);
			DMA2_Channel1->CMAR = buffer2;
			break;
		}
		case 3:{
			DMA2_Channel5->CPAR = (uint32_t)(&ADC3->DR); // address of peripheral
			DMA2_Channel5->CMAR = buffer3; // base address of memory buffer
			break;
		}
		case 4:{
			DMA2_Channel2->CPAR = (uint32_t)(&ADC4->DR);
			DMA2_Channel2->CMAR = buffer4;
			break;
		}
	}
}

void setupDMADual(uint32_t num){//num = 0 -> adc1_2 num = 1 ->adc3_4
	if(num){
		DMA2_Channel5->CPAR = &ADC34_COMMON->CDR; // address of peripheral
		DMA2_Channel5->CMAR = buffer3; // base address of memory buffer
	}
	else{
		DMA1_Channel1->CPAR = &ADC12_COMMON->CDR; // address of peripheral
		DMA1_Channel1->CMAR = buffer1; // base address of memory buffer
	}
}

void setupDMAParallel(uint32_t num){
	if(num){
		setupDMA(&dso1.ch3);
		setupDMA(&dso1.ch4);
	}
	else{
		setupDMA(&dso1.ch1);
		setupDMA(&dso1.ch2);
	}
}

void setupDMAQuad(void){
	DMA1_Channel1->CPAR = &ADC12_COMMON->CDR; // address of peripheral
	DMA1_Channel1->CMAR = buffer1; // base address of memory buffer
	DMA2_Channel5->CPAR = &ADC34_COMMON->CDR; // address of peripheral
	DMA2_Channel5->CMAR = buffer3; // base address of memory buffer

}

void setDMACntr(channel *chx){
	switch(chx->id){
		case 1:
				DMA1_Channel1->CNDTR = 2*BUFF_SIZE; // size of memory target
		break;
		case 2:
				DMA2_Channel1->CNDTR = 2*BUFF_SIZE; // size of memory target
		break;
		case 3:
				DMA2_Channel5->CNDTR = 2*BUFF_SIZE;
		break;
		case 4:
				DMA2_Channel2->CNDTR = 2*BUFF_SIZE;
		break;
	}
}

//void setDMACntr(channel *chx){
//	switch(chx->id){
//		case 1:{
//			switch(dso1.timeDiv){
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
//			switch(dso1.timeDiv){
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
//			switch(dso1.timeDiv){
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
//			switch(dso1.timeDiv){
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

