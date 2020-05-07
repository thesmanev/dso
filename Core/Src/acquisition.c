/*
 * acquisition.c
 *
 *  Created on: Apr 26, 2020
 *      Author: Stanimir Manev
 */
#include "main.h"

/**
 * @brief Acquire/sample data on channel one using all four ADCs in interleaved mode
 */
void aquireDataQuad(void)
{
	setupDMAQuad();
	setDMACntr(&dso.ch1); //master1
	setDMACntr(&dso.ch3); //master3
	asm("mov.w r2, #1342177280");
	// 0x50000000
	asm("ldr r3, [r2, #8]");
	asm("orr.w r3, r3, #4");
	asm("str r3, [r2, #8]");
	// ADC1_2 started
	asm("ldr r3, [r2, #1032]");
	// 2 cycles
	asm("orr.w r3, r3, #4");
	// 1 cycle
	asm("nop");
	// 1 cycle
	asm("nop");
	// 1 cycle
	asm("str r3, [r2, #1032]");
	// ADC3_4 started // 2 cycles
	DMA1_Channel1->CCR = 0x00002a8b; // Configure and start DMA1 channel 1
	// expected duration 5 cycles
	DMA2_Channel5->CCR = 0x00002a8b;	// Configure and start DMA2 channel 5
	// length of line : expected to be 7 cycles
	// first DMA transfer expected 18 cycles after ADC1 start
	asm("wfi");
}

/**
 * @brief Acquire data on channel 1 and channel 3 in parallel. Channels 1 & 2 in interlaved,
 * 			channels 3 & 4 in interlaved mode, 1 & 2 are in parallel to 3 & 4
 */
void aquireDataDualParallel(void)
{
	setDMACntr(&dso.ch1);	//master1
	setDMACntr(&dso.ch3);	//master3
	asm("mov.w r2, #1342177280");
	// 0x50000000
	asm("ldr r3, [r2, #8]");
	asm("orr.w r3, r3, #4");
	asm("str r3, [r2, #8]");
	// ADC1_2 started
	asm("str r3, [r2, #1032]");
	// ADC3_4 started // 2 cycles
	DMA1_Channel1->CCR = 0x00002a8b; // Configure and start DMA1 channel 1
	// expected duration 5 cycles
	DMA2_Channel5->CCR = 0x00002a8b;	// Configure and start DMA2 channel 5
	// length of line : expected to be 7 cycles
	// first DMA transfer expected 18 cycles after ADC1 start
	asm("wfi");
}

/**
 * Acquire data in dual mode but only using channels 1 & 2 or only channels 3 & 4 in interleaved mode
 * @param num 0 for using channels 1 & 2, 1 for using channels 3 & 4
 */
void aquireDataDual(uint32_t num)
{
	if(num)
	{
		setupDMADual(1);
		setDMACntr(&dso.ch3);	//master1
		asm("mov.w r2, #1342177280");
		// 0x50000000
		asm("ldr r3, [r2, #1032]");
		asm("orr.w r3, r3, #4");
		asm("str r3, [r2, #1032]");
		// ADC3_4 started // 2 cycles
		DMA2_Channel5->CCR = 0x00002a8b; // Configure and start DMA1 channel 1
		asm("wfi");
	}
	else
	{
		setupDMADual(0);
		setDMACntr(&dso.ch1); //master1
		asm("mov.w r2, #1342177280");
		// 0x50000000
		asm("ldr r3, [r2, #8]");
		asm("orr.w r3, r3, #4");
		asm("str r3, [r2, #8]");
		// ADC1_2 started
		DMA1_Channel1->CCR = 0x00002a8b; // Configure and start DMA1 channel 1
		asm("wfi");
	}
}

/**
 * @brief Acquire data from two channels in parallel
 * @param num 0 for channels 1 & 2 in parallel, 1 for channels 3 & 4
 */
void aquireDataParallel(uint32_t num)
{
	if(num)
	{
		setDMACntr(&dso.ch3); //master1
		setDMACntr(&dso.ch4); //master1
		asm("mov.w r2, #1342177280");
		// 0x50000000
		asm("ldr r3, [r2, #1032]");
		asm("orr.w r3, r3, #4");
		asm("str r3, [r2, #1032]");
		// ADC3_4 started // 2 cycles
		DMA2_Channel5->CCR = 0x0000268b; // Configure and start DMA2 channel 5, mem 16bit, periph 32bit
		DMA2_Channel2->CCR = 0x0000268b; // Configure and start DMA2 channel 2, mem 16bit, periph 32bit
		asm("wfi");
	}
	else
	{
		setDMACntr(&dso.ch1); //master1
		setDMACntr(&dso.ch2); //master1
		asm("mov.w r2, #1342177280");
		// 0x50000000
		asm("ldr r3, [r2, #8]");
		asm("orr.w r3, r3, #4");
		asm("str r3, [r2, #8]");
		// ADC1_2 started
		DMA1_Channel1->CCR = 0x0000268b; // Configure and start DMA1 channel 1, mem 16bit, periph 32bit
		DMA2_Channel1->CCR = 0x0000268b; // Configure and start DMA2 channel 1, mem 16bit, periph 32bit
		asm("wfi");
	}
}

/**
 * @brief Acquire data with all four channels in parallel
 */
void aquireDataAllParallel(void)
{
	setDMACntr(&dso.ch1); //master1
	setDMACntr(&dso.ch2); //slave2
	setDMACntr(&dso.ch3); //master3
	setDMACntr(&dso.ch4); //slave4
	asm("mov.w r2, #1342177280");
	// 0x50000000
	asm("ldr r3, [r2, #8]");
	asm("orr.w r3, r3, #4");
	asm("str r3, [r2, #8]");
	// ADC1_2 started
	asm("str r3, [r2, #1032]");
	// ADC3_4 started // 2 cycles
	DMA1_Channel1->CCR = 0x0000268b; // Configure and start DMA1 channel 1, mem 16bit, periph 32bit
	DMA2_Channel1->CCR = 0x0000268b; // Configure and start DMA1 channel 1, mem 16bit, periph 32bit
	// expected duration 5 cycles
	DMA2_Channel5->CCR = 0x0000268b;	// Configure and start DMA2 channel 5, mem 16bit, periph 32bit
	DMA2_Channel2->CCR = 0x0000268b;	// Configure and start DMA1 channel 1, mem 16bit, periph 32bit
	// length of line : expected to be 7 cycles
	asm("wfi");
}

/**
 * @brief Acquire data on a single channel
 * @param chx Pointer to a channel structure
 */
void aquireDataSingle(channel_t *chx)
{
	switch (chx->id)
	{
		default:
		case 1:
		{
			setDMACntr(&dso.ch1);	//master1
			asm("mov.w r2, #1342177280");
			// 0x50000000
			asm("ldr r3, [r2, #8]");
			asm("orr.w r3, r3, #4");
			asm("str r3, [r2, #8]");
			// ADC1 started
			DMA1_Channel1->CCR = 0x0000268b; // Configure and start DMA1 channel 1, mem 16bit, periph 32bit
			//asm("wfi");
			break;
		}
		case 2:
		{
			setDMACntr(&dso.ch2);			//master1
			asm("mov.w r2, #1342177280");
			// 0x50000000
			// ADC1 base + ADC slave offset + CR register offset
			asm("ldr r3, [r2, #264]");
			// 0x50000000 + 0x0100 + 0x08
			asm("orr.w r3, r3, #4");
			asm("str r3, [r2, #264]");
			// ADC2 started
			DMA2_Channel1->CCR = 0x0000268b; // Configure and start DMA1 channel 1, mem 16bit, periph 32bit
			asm("wfi");
			break;
		}
		case 3:
		{
			setDMACntr(&dso.ch3); //master1
			asm("mov.w r2, #1342177280");
			// 0x50000000
			// ADC1 base + 1k (adc3 offset) + CR register offset
			asm("ldr r3, [r2, #1032]");
			// 0x50000000 + 0x0400 + 0x08
			asm("orr.w r3, r3, #4");
			asm("str r3, [r2, #1032]");
			// ADC3 started
			DMA2_Channel5->CCR = 0x0000268b; // Configure and start DMA1 channel 1, mem 16bit, periph 32bit
			asm("wfi");
			break;
		}
		case 4:
		{
			setDMACntr(&dso.ch4); //master1
			asm("mov.w r2, #1342177280");
			// 0x50000000
			// ADC1 base + 1k (adc3 offset) + ADC slave offset + CR register offset
			asm("ldr r3, [r2, #1288]");
			// 0x50000000 + 0x0400 + 0x0100 + 0x08
			asm("orr.w r3, r3, #4");
			asm("str r3, [r2, #1288]");
			// ADC4 started
			DMA2_Channel2->CCR = 0x0000268b; // Configure and start DMA1 channel 1, mem 16bit, periph 32bit
			asm("wfi");
			break;
		}
	}
}

/**
 * @brief Acquire data on channels 2 and 3 in parallel
 */
void aquireDataParallel_23(void)
{
	setDMACntr(&dso.ch2);
	setDMACntr(&dso.ch3);
	asm("mov.w r2, #1342177280");
	// 0x50000000
	// ADC1 base + ADC slave offset + CR register offset
	asm("ldr r3, [r2, #264]");
	// 0x50000000 + 0x0100 + 0x08
	asm("orr.w r3, r3, #4");
	asm("str r3, [r2, #264]");
	// ADC2 started
	asm("str r3, [r2, #1032]");
	// ADC3 started // 2 cycles
	DMA2_Channel1->CCR = 0x0000268b; // Configure and start DMA2 channel 1, mem 16bit, periph 32bit
	DMA2_Channel5->CCR = 0x0000268b; // Configure and start DMA2 channel 5, mem 16bit, periph 32bit
	asm("wfi");
}

/**
 * @brief Acquire data in parallel on channels 2, 3 and 4
 */
void aquireDataParallel_234(void)
{
	setDMACntr(&dso.ch2);
	setDMACntr(&dso.ch3);
	setDMACntr(&dso.ch4);
	asm("mov.w r2, #1342177280");
	// 0x50000000
//	asm("mov.w r3, #1342177280"); // 0x50000000
	// ADC1 base + ADC slave offset + CR register offset
	asm("ldr r3, [r2, #264]");
	// 0x50000000 + 0x0100 + 0x08
	asm("orr.w r3, r3, #4");
	asm("str r3, [r2, #264]");
	// ADC2 started
	asm("str r3, [r2, #1032]");
	// ADC3 started // 2 cycles
	DMA2_Channel1->CCR = 0x0000268b; // Configure and start DMA2 channel 1, mem 16bit, periph 32bit
	DMA2_Channel5->CCR = 0x0000268b; // Configure and start DMA2 channel 5, mem 16bit, periph 32bit
	DMA2_Channel1->CCR = 0x0000268b; //, mem 16bit, periph 32bit
	//asm("wfi");
//	ulTaskNotifyTake(pdTRUE, 100);
}

void aquireDataParallel_24(void)
{
	setDMACntr(&dso.ch2);
	setDMACntr(&dso.ch4);
	asm("mov.w r2, #1342177280");
	// 0x50000000
	// ADC1 base + ADC slave offset + CR register offset
	asm("ldr r3, [r2, #264]");
	// 0x50000000 + 0x0100 + 0x08
	asm("orr.w r3, r3, #4");
	asm("str r3, [r2, #264]");
	// ADC2 started
	asm("str r3, [r2, #1288]");
	// ADC4 started
	DMA2_Channel1->CCR = 0x0000268b; // Configure and start DMA2 channel 1, mem 16bit, periph 32bit
	DMA2_Channel2->CCR = 0x0000268b; // Configure and start DMA2 channel 2, mem 16bit, periph 32bit
	asm("wfi");
}

/**
 * @brief Acqure data in parallel on channels 1 and 4
 */
void aquireDataParallel_14(void)
{
	setDMACntr(&dso.ch1);
	setDMACntr(&dso.ch4);
	asm("mov.w r2, #1342177280");
	// 0x50000000
	// ADC1 base + ADC slave offset + CR register offset
	asm("ldr r3, [r2, #8]");
	// 0x50000000 + 0x08
	asm("orr.w r3, r3, #4");
	asm("str r3, [r2, #8]");
	// ADC1 started
	asm("str r3, [r2, #1288]");
	// ADC4 started
	DMA1_Channel1->CCR = 0x0000268b; // Configure and start DMA1 channel 1, mem 16bit, periph 32bit
	DMA2_Channel2->CCR = 0x0000268b; // Configure and start DMA2 channel 2, mem 16bit, periph 32bit
	asm("wfi");
}

/**
 * @brief Acquire data in parallel on channels 1, 2 and 4
 */
void aquireDataParallel_124(void)
{
	setDMACntr(&dso.ch1);
	setDMACntr(&dso.ch2);
	setDMACntr(&dso.ch4);
	asm("mov.w r2, #1342177280");
	// 0x50000000
	// ADC1 base + ADC slave offset + CR register offset
	asm("ldr r3, [r2, #8]");
	// 0x50000000 + 0x08
	asm("orr.w r3, r3, #4");
	asm("str r3, [r2, #8]");
	// ADC1 started
	asm("str r3, [r2, #1288]");
	// ADC4 started
	DMA1_Channel1->CCR = 0x0000268b; // Configure and start DMA1 channel 1, mem 16bit, periph 32bit
	DMA2_Channel1->CCR = 0x0000268b; //, mem 16bit, periph 32bit
	DMA2_Channel2->CCR = 0x0000268b; // Configure and start DMA2 channel 2, mem 16bit, periph 32bit
	asm("wfi");
}

/**
 * @brief Acquire data in parallel on channels 1 and 3
 */
void aquireDataParallel_13(void)
{
	setDMACntr(&dso.ch1); //master1
	setDMACntr(&dso.ch3); //master3
	asm("mov.w r2, #1342177280");
	// 0x50000000
	asm("ldr r3, [r2, #8]");
	asm("orr.w r3, r3, #4");
	asm("str r3, [r2, #8]");
	// ADC1_2 started
	asm("str r3, [r2, #1032]");
	// ADC3_4 started // 2 cycles
	DMA1_Channel1->CCR = 0x0000268b; // Configure and start DMA1 channel 1
	// expected duration 5 cycles
	DMA2_Channel5->CCR = 0x0000268b;	// Configure and start DMA2 channel 5
	asm("wfi");
}

/**
 * @Brief Depending on the enabled channels and the timebase, this function determines how to acquire/sample data.
 * @param mode
 */
void aquireData(uint32_t mode)
{
	switch (channel_state)
	{
		case 0:
			break;
		case 1:
		{
			if(dso.timeDiv < timeDiv_10us)
			{
				if(mode)
					displayWaveformTrig(1, mode);
				else
				{
					aquireDataSingle(&dso.ch1);
					findTrigger();
					displayWaveformTrig(1, mode);
				}
			}
			else if(dso.timeDiv < timeDiv_2_5us)
			{
				if(mode)
					displayWaveformTrig(0, mode);
				else
				{
					aquireDataDual(0);
					findTrigger();
					displayWaveformTrig(0, mode);
				}
			}
			else
			{
				if(mode)
					displayWaveformTrig(0, mode);
				else
				{
					aquireDataQuad();
					findTrigger();
					displayWaveformTrig(0, mode);
				}
			}
			break;
		}
		case 2:
		{
			if(dso.timeDiv < timeDiv_10us)
			{
				if(mode)
					displayWaveformTrig(2, mode);
				else
				{
					aquireDataSingle(&dso.ch2);
					findTrigger();
					displayWaveformTrig(2, mode);
				}
			}
			//unavailable
			else if(dso.timeDiv < timeDiv_2_5us)
			{
//				aquireDataDual(0);
//				//find trigger
				//display waveform
			}
			else
			{
//     			aquireDataQuad();
//				//find trigger
//				//display waveform
			}
			break;
		}
		case 3:
		{
			if(dso.timeDiv < timeDiv_10us)
			{
				if(mode)
				{
					displayWaveformTrig(1, mode);
					displayWaveformTrig(2, mode);
				}
				else
				{
					aquireDataParallel(0);
					findTrigger();
					displayWaveformTrig(1, mode);
					displayWaveformTrig(2, mode);
				}
			}
			//unavailable
			else if(dso.timeDiv < timeDiv_2_5us)
			{
//				aquireDataDual(0);
//				//find trigger
//				//display waveform
			}
			else
			{
//				aquireDataQuad();
//				//find trigger
//				//display waveform
			}
			break;
		}
		case 4:
		{
			if(dso.timeDiv < timeDiv_10us)
			{
				if(mode)
					displayWaveformTrig(3, mode);
				else
				{
					aquireDataSingle(&dso.ch3);
					findTrigger();
					displayWaveformTrig(3, mode);
				}
			}
			else if(dso.timeDiv < timeDiv_2_5us)
			{
				if(mode)
					displayWaveformTrig(0, mode);
				else
				{
					aquireDataDual(1);
					findTrigger();
					displayWaveformTrig(0, mode);
				}
			}
			else
			{
				//unavailable
//				aquireDataQuad();
//				//find trigger
//				//display waveform
			}
			break;
		}
		case 5:
		{
			if(dso.timeDiv < timeDiv_10us)
			{
				if(mode)
				{
					displayWaveformTrig(1, mode);
					displayWaveformTrig(3, mode);
				}
				else
				{
					aquireDataParallel_13();
					findTrigger();
					displayWaveformTrig(1, mode);
					displayWaveformTrig(3, mode);
				}
			}
			else if(dso.timeDiv < timeDiv_2_5us)
			{
				if(mode)
					displayWaveformTrig(0, mode);
				else
				{
					aquireDataDualParallel();
					findTrigger();
					displayWaveformTrig(0, mode);
				}
			}
			else
			{
//				aquireDataQuad();
//				//find trigger
//				//display waveform
			}
			break;
		}
		case 6:
		{
			if(dso.timeDiv < timeDiv_10us)
			{
				if(mode)
				{
					displayWaveformTrig(2, mode);
					displayWaveformTrig(3, mode);
				}
				else
				{
					aquireDataParallel_23();
					findTrigger();
					displayWaveformTrig(2, mode);
					displayWaveformTrig(3, mode);
				}
			}
			else if(dso.timeDiv < timeDiv_2_5us)
			{
//				aquireDataDualParallel();
//				//find trigger
//				//display waveform
			}
			else
			{
//				aquireDataQuad();
//				//find trigger
//				//display waveform
			}
			break;
		}
		case 7:
		{
			if(dso.timeDiv < timeDiv_10us)
			{
				if(mode)
				{
					displayWaveformTrig(1, mode);
					displayWaveformTrig(2, mode);
					displayWaveformTrig(3, mode);
				}
				else
				{
					aquireDataAllParallel();
					findTrigger();
					displayWaveformTrig(1, mode);
					displayWaveformTrig(2, mode);
					displayWaveformTrig(3, mode);
				}
			}
			else if(dso.timeDiv < timeDiv_2_5us)
			{
			}
			else
			{
			}
			break;
		}
		case 8:
		{
			if(dso.timeDiv < timeDiv_10us)
			{
				if(mode)
					displayWaveformTrig(4, mode);
				else
				{
					aquireDataSingle(&dso.ch4);
					findTrigger();
					displayWaveformTrig(4, mode);
				}
			}
			else if(dso.timeDiv < timeDiv_2_5us)
			{
//				aquireDataDualParallel();
//				//find trigger
//				//display waveform
			}
			else
			{
//				aquireDataQuad();
//				//find trigger
//				//display waveform
			}
			break;
		}
		case 9:
		{
			if(dso.timeDiv < timeDiv_10us)
			{
				if(mode)
				{
					displayWaveformTrig(1, mode);
					displayWaveformTrig(4, mode);
				}
				else
				{
					aquireDataParallel_14();
					findTrigger();
					displayWaveformTrig(1, mode);
					displayWaveformTrig(4, mode);
				}
			}
			else if(dso.timeDiv < timeDiv_2_5us)
			{
//				aquireDataDualParallel();
//				//find trigger
//				//display waveform
			}
			else
			{
//				aquireDataQuad();
//				//find trigger
//				//display waveform
			}
			break;
		}
		case 10:
		{
			if(dso.timeDiv < timeDiv_10us)
			{
				if(mode)
				{
					displayWaveformTrig(2, mode);
					displayWaveformTrig(4, mode);
				}
				else
				{
					aquireDataParallel_24();
					findTrigger();
					displayWaveformTrig(2, mode);
					displayWaveformTrig(4, mode);
				}
			}
			else if(dso.timeDiv < timeDiv_2_5us)
			{
//				aquireDataDualParallel();
//				//find trigger
//				//display waveform
			}
			else
			{
//				aquireDataQuad();
//				//find trigger
//				//display waveform
			}
			break;
		}
		case 11:
		{
			if(dso.timeDiv < timeDiv_10us)
			{
				if(mode)
				{
					displayWaveformTrig(1, mode);
					displayWaveformTrig(2, mode);
					displayWaveformTrig(4, mode);
				}
				else
				{
					aquireDataParallel_124();
					findTrigger();
					displayWaveformTrig(1, mode);
					displayWaveformTrig(2, mode);
					displayWaveformTrig(4, mode);
				}
			}
			else if(dso.timeDiv < timeDiv_2_5us)
			{
//				aquireDataDualParallel();
//				//find trigger
//				//display waveform
			}
			else
			{
//				aquireDataQuad();
//				//find trigger
//				//display waveform
			}
			break;
		}
		case 12:
		{
			if(dso.timeDiv < timeDiv_10us)
			{
				if(mode)
				{
					displayWaveformTrig(3, mode);
					displayWaveformTrig(4, mode);
				}
				else
				{
					aquireDataParallel(1);
					findTrigger();
					displayWaveformTrig(3, mode);
					displayWaveformTrig(4, mode);
				}
				//display waveform
			}
			else if(dso.timeDiv < timeDiv_2_5us)
			{
//				aquireDataParallel(1);
//				//find trigger
//				//display waveform
			}
			else
			{
//				aquireDataQuad();
//				//find trigger
//				//display waveform
			}
			break;
		}
		case 13:
		{
			if(dso.timeDiv < timeDiv_10us)
			{
				if(mode)
				{
					displayWaveformTrig(1, mode);
					displayWaveformTrig(3, mode);
					displayWaveformTrig(4, mode);
				}
				else
				{
					aquireDataAllParallel();
					findTrigger();
					displayWaveformTrig(1, mode);
					displayWaveformTrig(3, mode);
					displayWaveformTrig(4, mode);
				}
				//display waveform
			}
			else if(dso.timeDiv < timeDiv_2_5us)
			{
//				aquireDataDualParallel();
//				//find trigger
//				//display waveform
			}
			else
			{
//				aquireDataQuad();
//				//find trigger
//				//display waveform
			}
			break;
		}
		case 14:
		{
			if(dso.timeDiv < timeDiv_10us)
			{
				if(mode)
				{
					displayWaveformTrig(2, mode);
					displayWaveformTrig(3, mode);
					displayWaveformTrig(4, mode);
				}
				else
				{
					aquireDataParallel_234();
					findTrigger();
					displayWaveformTrig(2, mode);
					displayWaveformTrig(3, mode);
					displayWaveformTrig(4, mode);
				}
			}
			else if(dso.timeDiv < timeDiv_2_5us)
			{
//				aquireDataDualParallel();
//				//find trigger
//				//display waveform
			}
			else
			{
//				aquireDataQuad();
//				//find trigger
//				//display waveform
			}
			break;
		}
		case 15:
		{
			if(dso.timeDiv < timeDiv_10us)
			{
				if(mode)
				{
					displayWaveformTrig(1, mode);
					displayWaveformTrig(2, mode);
					displayWaveformTrig(3, mode);
					displayWaveformTrig(4, mode);
				}
				else
				{
					aquireDataAllParallel();
					findTrigger();
					displayWaveformTrig(1, mode);
					displayWaveformTrig(2, mode);
					displayWaveformTrig(3, mode);
					displayWaveformTrig(4, mode);
				}
			}
			else if(dso.timeDiv < timeDiv_2_5us)
			{
//				aquireDataDualParallel();
//				//find trigger
//				//display waveform
			}
			else
			{
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
