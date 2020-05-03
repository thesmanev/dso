/*
 * acquisition.c
 *
 *  Created on: Apr 26, 2020
 *      Author: Stanimir Manev
 */
#include "main.h"


/////////////////////////////////////////
// quad channel aquisiton ch1/ch2 interleaved
// ch3/ch4 interleaved and start after 7 TADC (
////////////////////////////////////////
void aquireDataQuad(void){
	//ADC1 -> CR |= 0x1 << 2; //Start ADC1 Software //Conversion
	// length of line : expected to be 7 cycles
	setupDMAQuad();
	setDMACntr(&dso1.ch1);//master1
	setDMACntr(&dso1.ch3);//master3
	asm("mov.w r2, #1342177280"); // 0x50000000
//	asm("mov.w r3, #1342177280"); // 0x50000000
	asm("ldr r3, [r2, #8]");
	asm("orr.w r3, r3, #4");
	asm("str r3, [r2, #8]"); // ADC1_2 started
	asm("ldr r3, [r2, #1032]"); // 2 cycles
	asm("orr.w r3, r3, #4"); // 1 cycle
	asm("nop"); // 1 cycle
	asm("nop"); // 1 cycle
	asm("str r3, [r2, #1032]"); // ADC3_4 started // 2 cycles
	//asm("nop");
	DMA1_Channel1->CCR = 0x00002a8b;// Configure and start DMA1 channel 1
	// expected duration 5 cycles
	DMA2_Channel5->CCR = 0x00002a8b;// Configure and start DMA2 channel 5
	// length of line : expected to be 7 cycles
	// first DMA transfer expected 18 cycles after ADC1 start
	asm("wfi");
	//ADC3->CR |= 0x1 << 2; //Start ADC3 Software conversion
	// length of line : expected to be 7 cycles
	//__WFI();

}
/////////////////////////////////////////
// ch1 and ch2 interleaved, ch3 and ch4 interleaved
// in parallel
////////////////////////////////////////
void aquireDataDualParallel(void){
	//ADC1 -> CR |= 0x1 << 2; //Start ADC1 Software //Conversion
	// length of line : expected to be 7 cycles
//	setupDMAQuad();
	setDMACntr(&dso1.ch1);//master1
	setDMACntr(&dso1.ch3);//master3
	asm("mov.w r2, #1342177280"); // 0x50000000
//	asm("mov.w r3, #1342177280"); // 0x50000000
	asm("ldr r3, [r2, #8]");
	asm("orr.w r3, r3, #4");
	asm("str r3, [r2, #8]"); // ADC1_2 started
	asm("str r3, [r2, #1032]"); // ADC3_4 started // 2 cycles
//	asm("orr.w r3, r3, #4"); // 1 cycle
//	asm("nop"); // 1 cycle
//	asm("nop"); // 1 cycle
//	asm("str r3, [r2, #1032]"); // ADC3_4 started // 2 cycles
	//asm("nop");
	DMA1_Channel1->CCR = 0x00002a8b;// Configure and start DMA1 channel 1
	// expected duration 5 cycles
	DMA2_Channel5->CCR = 0x00002a8b;// Configure and start DMA2 channel 5
	// length of line : expected to be 7 cycles
	// first DMA transfer expected 18 cycles after ADC1 start
	asm("wfi");
	//ADC3->CR |= 0x1 << 2; //Start ADC3 Software conversion
	// length of line : expected to be 7 cycles
	//__WFI();

}

/////////////////////////////////////////
// ch1/ch2 interleaved only or ch4/ch4 interleaved only
////////////////////////////////////////
void aquireDataDual(uint32_t num){
	if(num){
		setupDMADual(1);
		setDMACntr(&dso1.ch3);//master1
		asm("mov.w r2, #1342177280"); // 0x50000000
//		asm("mov.w r3, #1342177280"); // 0x50000000
		asm("ldr r3, [r2, #1032]");
		asm("orr.w r3, r3, #4");
		asm("str r3, [r2, #1032]"); // ADC3_4 started // 2 cycles
		DMA2_Channel5->CCR = 0x00002a8b;// Configure and start DMA1 channel 1
		asm("wfi");
	}
	else{
		setupDMADual(0);
		setDMACntr(&dso1.ch1);//master1
		asm("mov.w r2, #1342177280"); // 0x50000000
//		asm("mov.w r3, #1342177280"); // 0x50000000
		asm("ldr r3, [r2, #8]");
		asm("orr.w r3, r3, #4");
		asm("str r3, [r2, #8]"); // ADC1_2 started
		DMA1_Channel1->CCR = 0x00002a8b;// Configure and start DMA1 channel 1
		asm("wfi");
	}
}
/////////////////////////////////////////
// ch1 and ch2 regular simultaneous or ch3 and ch4 regular simultaneous
// in parallel
////////////////////////////////////////
void aquireDataParallel(uint32_t num){
	if(num){
		setDMACntr(&dso1.ch3);//master1
		setDMACntr(&dso1.ch4);//master1
		asm("mov.w r2, #1342177280"); // 0x50000000
//		asm("mov.w r3, #1342177280"); // 0x50000000
		asm("ldr r3, [r2, #1032]");
		asm("orr.w r3, r3, #4");
		asm("str r3, [r2, #1032]"); // ADC3_4 started // 2 cycles
		DMA2_Channel5->CCR = 0x0000268b;// Configure and start DMA2 channel 5, mem 16bit, periph 32bit
		DMA2_Channel2->CCR = 0x0000268b;// Configure and start DMA2 channel 2, mem 16bit, periph 32bit
		asm("wfi");
	}
	else{
		setDMACntr(&dso1.ch1);//master1
		setDMACntr(&dso1.ch2);//master1
		asm("mov.w r2, #1342177280"); // 0x50000000
	//	asm("mov.w r3, #1342177280"); // 0x50000000
		asm("ldr r3, [r2, #8]");
		asm("orr.w r3, r3, #4");
		asm("str r3, [r2, #8]"); // ADC1_2 started
		DMA1_Channel1->CCR = 0x0000268b;// Configure and start DMA1 channel 1, mem 16bit, periph 32bit
		DMA2_Channel1->CCR = 0x0000268b;// Configure and start DMA2 channel 1, mem 16bit, periph 32bit
		asm("wfi");
	}
}


/////////////////////////////////////////
// ch1 and ch2 regular simultaneous AND ch3 and ch4 regular simultaneous
// in parallel
////////////////////////////////////////
void aquireDataAllParallel(void){
	setDMACntr(&dso1.ch1);//master1
	setDMACntr(&dso1.ch2);//slave2
	setDMACntr(&dso1.ch3);//master3
	setDMACntr(&dso1.ch4);//slave4
	asm("mov.w r2, #1342177280"); // 0x50000000
//	asm("mov.w r3, #1342177280"); // 0x50000000
	asm("ldr r3, [r2, #8]");
	asm("orr.w r3, r3, #4");
	asm("str r3, [r2, #8]"); // ADC1_2 started
	asm("str r3, [r2, #1032]"); // ADC3_4 started // 2 cycles
//	asm("orr.w r3, r3, #4"); // 1 cycle
//	asm("nop"); // 1 cycle
//	asm("nop"); // 1 cycle
//	asm("str r3, [r2, #1032]"); // ADC3_4 started // 2 cycles
	//asm("nop");
	DMA1_Channel1->CCR = 0x0000268b;// Configure and start DMA1 channel 1, mem 16bit, periph 32bit
	DMA2_Channel1->CCR = 0x0000268b;// Configure and start DMA1 channel 1, mem 16bit, periph 32bit
	// expected duration 5 cycles
	DMA2_Channel5->CCR = 0x0000268b;// Configure and start DMA2 channel 5, mem 16bit, periph 32bit
	DMA2_Channel2->CCR = 0x0000268b;// Configure and start DMA1 channel 1, mem 16bit, periph 32bit
	// length of line : expected to be 7 cycles
	// first DMA transfer expected 18 cycles after ADC1 start
	asm("wfi");
	//ADC3->CR |= 0x1 << 2; //Start ADC3 Software conversion
	// length of line : expected to be 7 cycles
	//__WFI();
}

void aquireDataSingle(channel *chx){
	switch(chx->id){
		case 1:{
			setDMACntr(&dso1.ch1);//master1
			asm("mov.w r2, #1342177280"); // 0x50000000
		//	asm("mov.w r3, #1342177280"); // 0x50000000
			asm("ldr r3, [r2, #8]");
			asm("orr.w r3, r3, #4");
			asm("str r3, [r2, #8]"); // ADC1 started
			DMA1_Channel1->CCR = 0x0000268b;// Configure and start DMA1 channel 1, mem 16bit, periph 32bit
			asm("wfi");
			break;
		}
		case 2:{
			setDMACntr(&dso1.ch2);//master1
			asm("mov.w r2, #1342177280"); // 0x50000000
		//	asm("mov.w r3, #1342177280"); // 0x50000000
							// ADC1 base + ADC slave offset + CR register offset
			asm("ldr r3, [r2, #264]"); // 0x50000000 + 0x0100 + 0x08
			asm("orr.w r3, r3, #4");
			asm("str r3, [r2, #264]"); // ADC2 started
			DMA2_Channel1->CCR = 0x0000268b;// Configure and start DMA1 channel 1, mem 16bit, periph 32bit
			asm("wfi");
			break;
		}
		case 3:{
			setDMACntr(&dso1.ch3);//master1
			asm("mov.w r2, #1342177280"); // 0x50000000
		//	asm("mov.w r3, #1342177280"); // 0x50000000
			// ADC1 base + 1k (adc3 offset) + CR register offset
			asm("ldr r3, [r2, #1032]"); // 0x50000000 + 0x0400 + 0x08
			asm("orr.w r3, r3, #4");
			asm("str r3, [r2, #1032]"); // ADC3 started
			DMA2_Channel5->CCR = 0x0000268b;// Configure and start DMA1 channel 1, mem 16bit, periph 32bit
			asm("wfi");
			break;
		}
		case 4:{
			setDMACntr(&dso1.ch4);//master1
			asm("mov.w r2, #1342177280"); // 0x50000000
		//	asm("mov.w r3, #1342177280"); // 0x50000000
			// ADC1 base + 1k (adc3 offset) + ADC slave offset + CR register offset
			asm("ldr r3, [r2, #1288]"); // 0x50000000 + 0x0400 + 0x0100 + 0x08
			asm("orr.w r3, r3, #4");
			asm("str r3, [r2, #1288]"); // ADC4 started
			DMA2_Channel2->CCR = 0x0000268b;// Configure and start DMA1 channel 1, mem 16bit, periph 32bit
			asm("wfi");
			break;
		}
	}
}

void aquireDataParallel_23(void){
	setDMACntr(&dso1.ch2);
	setDMACntr(&dso1.ch3);
	asm("mov.w r2, #1342177280"); // 0x50000000
//	asm("mov.w r3, #1342177280"); // 0x50000000
					// ADC1 base + ADC slave offset + CR register offset
	asm("ldr r3, [r2, #264]"); // 0x50000000 + 0x0100 + 0x08
	asm("orr.w r3, r3, #4");
	asm("str r3, [r2, #264]"); // ADC2 started
	asm("str r3, [r2, #1032]"); // ADC3 started // 2 cycles
	DMA2_Channel1->CCR = 0x0000268b;// Configure and start DMA2 channel 1, mem 16bit, periph 32bit
	DMA2_Channel5->CCR = 0x0000268b;// Configure and start DMA2 channel 5, mem 16bit, periph 32bit
	asm("wfi");
}

void aquireDataParallel_234(void){
	setDMACntr(&dso1.ch2);
	setDMACntr(&dso1.ch3);
	setDMACntr(&dso1.ch4);
	asm("mov.w r2, #1342177280"); // 0x50000000
//	asm("mov.w r3, #1342177280"); // 0x50000000
					// ADC1 base + ADC slave offset + CR register offset
	asm("ldr r3, [r2, #264]"); // 0x50000000 + 0x0100 + 0x08
	asm("orr.w r3, r3, #4");
	asm("str r3, [r2, #264]"); // ADC2 started
	asm("str r3, [r2, #1032]"); // ADC3 started // 2 cycles
	DMA2_Channel1->CCR = 0x0000268b;// Configure and start DMA2 channel 1, mem 16bit, periph 32bit
	DMA2_Channel5->CCR = 0x0000268b;// Configure and start DMA2 channel 5, mem 16bit, periph 32bit
	DMA2_Channel1->CCR = 0x0000268b;//, mem 16bit, periph 32bit
	asm("wfi");
}

void aquireDataParallel_24(void){
	setDMACntr(&dso1.ch2);
	setDMACntr(&dso1.ch4);
	asm("mov.w r2, #1342177280"); // 0x50000000
//	asm("mov.w r3, #1342177280"); // 0x50000000
					// ADC1 base + ADC slave offset + CR register offset
	asm("ldr r3, [r2, #264]"); // 0x50000000 + 0x0100 + 0x08
	asm("orr.w r3, r3, #4");
	asm("str r3, [r2, #264]"); // ADC2 started
	asm("str r3, [r2, #1288]"); // ADC4 started
	DMA2_Channel1->CCR = 0x0000268b;// Configure and start DMA2 channel 1, mem 16bit, periph 32bit
	DMA2_Channel2->CCR = 0x0000268b;// Configure and start DMA2 channel 2, mem 16bit, periph 32bit
	asm("wfi");
}

void aquireDataParallel_14(void){
	setDMACntr(&dso1.ch1);
	setDMACntr(&dso1.ch4);
	asm("mov.w r2, #1342177280"); // 0x50000000
//	asm("mov.w r3, #1342177280"); // 0x50000000
					// ADC1 base + ADC slave offset + CR register offset
	asm("ldr r3, [r2, #8]"); // 0x50000000 + 0x08
	asm("orr.w r3, r3, #4");
	asm("str r3, [r2, #8]"); // ADC1 started
	asm("str r3, [r2, #1288]"); // ADC4 started
	DMA1_Channel1->CCR = 0x0000268b;// Configure and start DMA1 channel 1, mem 16bit, periph 32bit
	DMA2_Channel2->CCR = 0x0000268b;// Configure and start DMA2 channel 2, mem 16bit, periph 32bit
	asm("wfi");
}

void aquireDataParallel_124(void){
	setDMACntr(&dso1.ch1);
	setDMACntr(&dso1.ch2);
	setDMACntr(&dso1.ch4);
	asm("mov.w r2, #1342177280"); // 0x50000000
//	asm("mov.w r3, #1342177280"); // 0x50000000
					// ADC1 base + ADC slave offset + CR register offset
	asm("ldr r3, [r2, #8]"); // 0x50000000 + 0x08
	asm("orr.w r3, r3, #4");
	asm("str r3, [r2, #8]"); // ADC1 started
	asm("str r3, [r2, #1288]"); // ADC4 started
	DMA1_Channel1->CCR = 0x0000268b;// Configure and start DMA1 channel 1, mem 16bit, periph 32bit
	DMA2_Channel1->CCR = 0x0000268b;//, mem 16bit, periph 32bit
	DMA2_Channel2->CCR = 0x0000268b;// Configure and start DMA2 channel 2, mem 16bit, periph 32bit
	asm("wfi");
}


void aquireDataParallel_13(void){
	//ADC1 -> CR |= 0x1 << 2; //Start ADC1 Software //Conversion
	// length of line : expected to be 7 cycles
//	setupDMAQuad();
	setDMACntr(&dso1.ch1);//master1
	setDMACntr(&dso1.ch3);//master3
	asm("mov.w r2, #1342177280"); // 0x50000000
//	asm("mov.w r3, #1342177280"); // 0x50000000
	asm("ldr r3, [r2, #8]");
	asm("orr.w r3, r3, #4");
	asm("str r3, [r2, #8]"); // ADC1_2 started
	asm("str r3, [r2, #1032]"); // ADC3_4 started // 2 cycles
//	asm("orr.w r3, r3, #4"); // 1 cycle
//	asm("nop"); // 1 cycle
//	asm("nop"); // 1 cycle
//	asm("str r3, [r2, #1032]"); // ADC3_4 started // 2 cycles
	//asm("nop");
	DMA1_Channel1->CCR = 0x0000268b;// Configure and start DMA1 channel 1
	// expected duration 5 cycles
	DMA2_Channel5->CCR = 0x0000268b;// Configure and start DMA2 channel 5
	// length of line : expected to be 7 cycles
	// first DMA transfer expected 18 cycles after ADC1 start
	asm("wfi");
	//ADC3->CR |= 0x1 << 2; //Start ADC3 Software conversion
	// length of line : expected to be 7 cycles
	//__WFI();
}


void aquireData(uint32_t mode){
	switch(x){
		case 0:
			break;
		case 1:{
			if(dso1.timeDiv < timeDiv_10us){
				if(mode)
					displayWaveformTrig(1, mode);
				else{
					aquireDataSingle(&dso1.ch1);
					findTrigger();
					displayWaveformTrig(1, mode);
				}
			}
			else if(dso1.timeDiv < timeDiv_2_5us){
				if(mode)
					displayWaveformTrig(0, mode);
				else{
					aquireDataDual(0);
					findTrigger();
					displayWaveformTrig(0, mode);
				}
			}
			else{
				if(mode)
					displayWaveformTrig(0, mode);
				else{
					aquireDataQuad();
					findTrigger();
					displayWaveformTrig(0, mode);
				}
			}
		break;
		}
		case 2:{
			if(dso1.timeDiv < timeDiv_10us){
				if(mode)
					displayWaveformTrig(2, mode);
				else{
					aquireDataSingle(&dso1.ch2);
					findTrigger();
					displayWaveformTrig(2, mode);
				}
			}
			//unavailable
			else if(dso1.timeDiv < timeDiv_2_5us){
//				aquireDataDual(0);
//				//find trigger
				//display waveform
			}
			else{
//     			aquireDataQuad();
//				//find trigger
//				//display waveform
			}
		break;
		}
		case 3:{
			if(dso1.timeDiv < timeDiv_10us){
				if(mode){
					displayWaveformTrig(1, mode);
					displayWaveformTrig(2, mode);
				}
				else{
					aquireDataParallel(0);
					findTrigger();
					displayWaveformTrig(1, mode);
					displayWaveformTrig(2, mode);
				}
			}
			//unavailable
			else if(dso1.timeDiv < timeDiv_2_5us){
//				aquireDataDual(0);
//				//find trigger
//				//display waveform
			}
			else{
//				aquireDataQuad();
//				//find trigger
//				//display waveform
			}
		break;
		}
		case 4:{
			if(dso1.timeDiv < timeDiv_10us){
				if(mode)
					displayWaveformTrig(3, mode);
				else{
					aquireDataSingle(&dso1.ch3);
					findTrigger();
					displayWaveformTrig(3, mode);
				}
			}
			else if(dso1.timeDiv < timeDiv_2_5us){
				if(mode)
					displayWaveformTrig(0, mode);
				else{
					aquireDataDual(1);
					findTrigger();
					displayWaveformTrig(0, mode);
				}
			}
			else{
			//unavailable
//				aquireDataQuad();
//				//find trigger
//				//display waveform
			}
		break;
		}
		case 5:{
			if(dso1.timeDiv < timeDiv_10us){
				if(mode){
					displayWaveformTrig(1, mode);
					displayWaveformTrig(3, mode);
				}
				else{
					aquireDataParallel_13();
					findTrigger();
					displayWaveformTrig(1, mode);
					displayWaveformTrig(3, mode);
				}
			}
			else if(dso1.timeDiv < timeDiv_2_5us){
				if(mode)
					displayWaveformTrig(0, mode);
				else{
					aquireDataDualParallel();
					findTrigger();
					displayWaveformTrig(0, mode);
				}
			}
			else{
//				aquireDataQuad();
//				//find trigger
//				//display waveform
			}
		break;
		}
		case 6:{
			if(dso1.timeDiv < timeDiv_10us){
				if(mode){
					displayWaveformTrig(2, mode);
					displayWaveformTrig(3, mode);
				}
				else{
					aquireDataParallel_23();
					findTrigger();
					displayWaveformTrig(2, mode);
					displayWaveformTrig(3, mode);
				}
			}
			else if(dso1.timeDiv < timeDiv_2_5us){
//				aquireDataDualParallel();
//				//find trigger
//				//display waveform
			}
			else{
//				aquireDataQuad();
//				//find trigger
//				//display waveform
			}
		break;
		}
		case 7:{
			if(dso1.timeDiv < timeDiv_10us){
				if(mode){
					displayWaveformTrig(1, mode);
					displayWaveformTrig(2, mode);
					displayWaveformTrig(3, mode);
				}
				else{
					aquireDataAllParallel();
					findTrigger();
					displayWaveformTrig(1, mode);
					displayWaveformTrig(2, mode);
					displayWaveformTrig(3, mode);
				}
			}
			else if(dso1.timeDiv < timeDiv_2_5us){
			}
			else{
			}
		break;
		}
		case 8:{
			if(dso1.timeDiv < timeDiv_10us){
				if(mode)
					displayWaveformTrig(4, mode);
				else{
					aquireDataSingle(&dso1.ch4);
					findTrigger();
					displayWaveformTrig(4, mode);
				}
			}
			else if(dso1.timeDiv < timeDiv_2_5us){
//				aquireDataDualParallel();
//				//find trigger
//				//display waveform
			}
			else{
//				aquireDataQuad();
//				//find trigger
//				//display waveform
			}
		break;
		}
		case 9:{
			if(dso1.timeDiv < timeDiv_10us){
				if(mode){
					displayWaveformTrig(1, mode);
					displayWaveformTrig(4, mode);
				}
				else{
					aquireDataParallel_14();
					findTrigger();
					displayWaveformTrig(1, mode);
					displayWaveformTrig(4, mode);
				}
			}
			else if(dso1.timeDiv < timeDiv_2_5us){
//				aquireDataDualParallel();
//				//find trigger
//				//display waveform
			}
			else{
//				aquireDataQuad();
//				//find trigger
//				//display waveform
			}
		break;
		}
		case 10:{
			if(dso1.timeDiv < timeDiv_10us){
				if(mode){
					displayWaveformTrig(2, mode);
					displayWaveformTrig(4, mode);
				}
				else{
					aquireDataParallel_24();
					findTrigger();
					displayWaveformTrig(2, mode);
					displayWaveformTrig(4, mode);
				}
			}
			else if(dso1.timeDiv < timeDiv_2_5us){
//				aquireDataDualParallel();
//				//find trigger
//				//display waveform
			}
			else{
//				aquireDataQuad();
//				//find trigger
//				//display waveform
			}
		break;
		}
		case 11:{
			if(dso1.timeDiv < timeDiv_10us){
				if(mode){
					displayWaveformTrig(1, mode);
					displayWaveformTrig(2, mode);
					displayWaveformTrig(4, mode);
				}
				else{
					aquireDataParallel_124();
					findTrigger();
					displayWaveformTrig(1, mode);
					displayWaveformTrig(2, mode);
					displayWaveformTrig(4, mode);
				}
			}
			else if(dso1.timeDiv < timeDiv_2_5us){
//				aquireDataDualParallel();
//				//find trigger
//				//display waveform
			}
			else{
//				aquireDataQuad();
//				//find trigger
//				//display waveform
			}
		break;
		}
		case 12:{
			if(dso1.timeDiv < timeDiv_10us){
				if(mode){
					displayWaveformTrig(3, mode);
					displayWaveformTrig(4, mode);
				}
				else{
					aquireDataParallel(1);
					findTrigger();
					displayWaveformTrig(3, mode);
					displayWaveformTrig(4, mode);
				}
				//display waveform
			}
			else if(dso1.timeDiv < timeDiv_2_5us){
//				aquireDataParallel(1);
//				//find trigger
//				//display waveform
			}
			else{
//				aquireDataQuad();
//				//find trigger
//				//display waveform
			}
		break;
		}
		case 13:{
			if(dso1.timeDiv < timeDiv_10us){
				if(mode){
					displayWaveformTrig(1, mode);
					displayWaveformTrig(3, mode);
					displayWaveformTrig(4, mode);
				}
				else{
					aquireDataAllParallel();
					findTrigger();
					displayWaveformTrig(1, mode);
					displayWaveformTrig(3, mode);
					displayWaveformTrig(4, mode);
				}
				//display waveform
			}
			else if(dso1.timeDiv < timeDiv_2_5us){
//				aquireDataDualParallel();
//				//find trigger
//				//display waveform
			}
			else{
//				aquireDataQuad();
//				//find trigger
//				//display waveform
			}
		break;
		}
		case 14:{
			if(dso1.timeDiv < timeDiv_10us){
				if(mode){
					displayWaveformTrig(2, mode);
					displayWaveformTrig(3, mode);
					displayWaveformTrig(4, mode);
				}
				else{
				aquireDataParallel_234();
				findTrigger();
				displayWaveformTrig(2, mode);
				displayWaveformTrig(3, mode);
				displayWaveformTrig(4, mode);
				}
			}
			else if(dso1.timeDiv < timeDiv_2_5us){
//				aquireDataDualParallel();
//				//find trigger
//				//display waveform
			}
			else{
//				aquireDataQuad();
//				//find trigger
//				//display waveform
			}
		break;
		}
		case 15:{
			if(dso1.timeDiv < timeDiv_10us){
				if(mode){
					displayWaveformTrig(1, mode);
					displayWaveformTrig(2, mode);
					displayWaveformTrig(3, mode);
					displayWaveformTrig(4, mode);
				}
				else{
					aquireDataAllParallel();
					findTrigger();
					displayWaveformTrig(1, mode);
					displayWaveformTrig(2, mode);
					displayWaveformTrig(3, mode);
					displayWaveformTrig(4, mode);
				}
			}
			else if(dso1.timeDiv < timeDiv_2_5us){
//				aquireDataDualParallel();
//				//find trigger
//				//display waveform
			}
			else{
//				aquireDataQuad();
//				//find trigger
//				//display waveform
			}
		break;
		}
		default:
			break;
	}
}
