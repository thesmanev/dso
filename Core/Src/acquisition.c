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
	DMA1_Channel1->CCR = DMA_CCR_EN | DMA_CCR_TCIE | DMA_CCR_TEIE | DMA_CCR_MINC | DMA_CCR_PSIZE_1 | DMA_CCR_MSIZE_1 | DMA_CCR_PL_1; // Configure and start DMA1 channel 1
	// expected duration 5 cycles
	DMA2_Channel5->CCR = DMA_CCR_EN | DMA_CCR_TCIE | DMA_CCR_TEIE | DMA_CCR_MINC | DMA_CCR_PSIZE_1 | DMA_CCR_MSIZE_1 | DMA_CCR_PL_1;	// Configure and start DMA2 channel 5
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
	DMA1_Channel1->CCR = DMA_CCR_EN | DMA_CCR_TCIE | DMA_CCR_TEIE | DMA_CCR_MINC | DMA_CCR_PSIZE_1 | DMA_CCR_MSIZE_1 | DMA_CCR_PL_1; // Configure and start DMA1 channel 1
	// expected duration 5 cycles
	DMA2_Channel5->CCR = DMA_CCR_EN | DMA_CCR_TCIE | DMA_CCR_TEIE | DMA_CCR_MINC | DMA_CCR_PSIZE_1 | DMA_CCR_MSIZE_1 | DMA_CCR_PL_1;	// Configure and start DMA2 channel 5
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
		DMA2_Channel5->CCR = DMA_CCR_EN | DMA_CCR_TCIE | DMA_CCR_TEIE | DMA_CCR_MINC | DMA_CCR_PSIZE_1 | DMA_CCR_MSIZE_1 | DMA_CCR_PL_1; // Configure and start DMA1 channel 1
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
		DMA1_Channel1->CCR = DMA_CCR_EN | DMA_CCR_TCIE | DMA_CCR_TEIE | DMA_CCR_MINC | DMA_CCR_PSIZE_1 | DMA_CCR_MSIZE_1 | DMA_CCR_PL_1; // Configure and start DMA1 channel 1
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
		DMA2_Channel5->CCR = DMA_CCR_EN | DMA_CCR_TCIE | DMA_CCR_TEIE | DMA_CCR_MINC | DMA_CCR_PSIZE_1 | DMA_CCR_MSIZE_0 | DMA_CCR_PL_1; // Configure and start DMA2 channel 5, mem 16bit, periph 32bit
		DMA2_Channel2->CCR = DMA_CCR_EN | DMA_CCR_TCIE | DMA_CCR_TEIE | DMA_CCR_MINC | DMA_CCR_PSIZE_1 | DMA_CCR_MSIZE_0 | DMA_CCR_PL_1; // Configure and start DMA2 channel 2, mem 16bit, periph 32bit
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
		DMA1_Channel1->CCR = DMA_CCR_EN | DMA_CCR_TCIE | DMA_CCR_TEIE | DMA_CCR_MINC | DMA_CCR_PSIZE_1 | DMA_CCR_MSIZE_0 | DMA_CCR_PL_1; // Configure and start DMA1 channel 1, mem 16bit, periph 32bit
		DMA2_Channel1->CCR = DMA_CCR_EN | DMA_CCR_TCIE | DMA_CCR_TEIE | DMA_CCR_MINC | DMA_CCR_PSIZE_1 | DMA_CCR_MSIZE_0 | DMA_CCR_PL_1; // Configure and start DMA2 channel 1, mem 16bit, periph 32bit
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
	DMA1_Channel1->CCR = DMA_CCR_EN | DMA_CCR_TCIE | DMA_CCR_TEIE | DMA_CCR_MINC | DMA_CCR_PSIZE_1 | DMA_CCR_MSIZE_0 | DMA_CCR_PL_1; // Configure and start DMA1 channel 1, mem 16bit, periph 32bit
	DMA2_Channel1->CCR = DMA_CCR_EN | DMA_CCR_TCIE | DMA_CCR_TEIE | DMA_CCR_MINC | DMA_CCR_PSIZE_1 | DMA_CCR_MSIZE_0 | DMA_CCR_PL_1; // Configure and start DMA1 channel 1, mem 16bit, periph 32bit
	// expected duration 5 cycles
	DMA2_Channel5->CCR = DMA_CCR_EN | DMA_CCR_TCIE | DMA_CCR_TEIE | DMA_CCR_MINC | DMA_CCR_PSIZE_1 | DMA_CCR_MSIZE_0 | DMA_CCR_PL_1;	// Configure and start DMA2 channel 5, mem 16bit, periph 32bit
	DMA2_Channel2->CCR = DMA_CCR_EN | DMA_CCR_TCIE | DMA_CCR_TEIE | DMA_CCR_MINC | DMA_CCR_PSIZE_1 | DMA_CCR_MSIZE_0 | DMA_CCR_PL_1;	// Configure and start DMA1 channel 1, mem 16bit, periph 32bit
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
			DMA1_Channel1->CCR = DMA_CCR_EN | DMA_CCR_TCIE | DMA_CCR_TEIE | DMA_CCR_MINC | DMA_CCR_PSIZE_1 | DMA_CCR_MSIZE_0 | DMA_CCR_PL_1; // Configure and start DMA1 channel 1, mem 16bit, periph 32bit
			asm("wfi");
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
			DMA2_Channel1->CCR = DMA_CCR_EN | DMA_CCR_TCIE | DMA_CCR_TEIE | DMA_CCR_MINC | DMA_CCR_PSIZE_1 | DMA_CCR_MSIZE_0 | DMA_CCR_PL_1; // Configure and start DMA1 channel 1, mem 16bit, periph 32bit
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
			DMA2_Channel5->CCR = DMA_CCR_EN | DMA_CCR_TCIE | DMA_CCR_TEIE | DMA_CCR_MINC | DMA_CCR_PSIZE_1 | DMA_CCR_MSIZE_0 | DMA_CCR_PL_1; // Configure and start DMA1 channel 1, mem 16bit, periph 32bit
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
			DMA2_Channel2->CCR = DMA_CCR_EN | DMA_CCR_TCIE | DMA_CCR_TEIE | DMA_CCR_MINC | DMA_CCR_PSIZE_1 | DMA_CCR_MSIZE_0 | DMA_CCR_PL_1; // Configure and start DMA1 channel 1, mem 16bit, periph 32bit
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
	DMA2_Channel1->CCR = DMA_CCR_EN | DMA_CCR_TCIE | DMA_CCR_TEIE | DMA_CCR_MINC | DMA_CCR_PSIZE_1 | DMA_CCR_MSIZE_0 | DMA_CCR_PL_1; // Configure and start DMA2 channel 1, mem 16bit, periph 32bit
	DMA2_Channel5->CCR = DMA_CCR_EN | DMA_CCR_TCIE | DMA_CCR_TEIE | DMA_CCR_MINC | DMA_CCR_PSIZE_1 | DMA_CCR_MSIZE_0 | DMA_CCR_PL_1; // Configure and start DMA2 channel 5, mem 16bit, periph 32bit
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
	DMA2_Channel1->CCR = DMA_CCR_EN | DMA_CCR_TCIE | DMA_CCR_TEIE | DMA_CCR_MINC | DMA_CCR_PSIZE_1 | DMA_CCR_MSIZE_0 | DMA_CCR_PL_1; // Configure and start DMA2 channel 1, mem 16bit, periph 32bit
	DMA2_Channel5->CCR = DMA_CCR_EN | DMA_CCR_TCIE | DMA_CCR_TEIE | DMA_CCR_MINC | DMA_CCR_PSIZE_1 | DMA_CCR_MSIZE_0 | DMA_CCR_PL_1; // Configure and start DMA2 channel 5, mem 16bit, periph 32bit
	DMA2_Channel1->CCR = DMA_CCR_EN | DMA_CCR_TCIE | DMA_CCR_TEIE | DMA_CCR_MINC | DMA_CCR_PSIZE_1 | DMA_CCR_MSIZE_0 | DMA_CCR_PL_1; //, mem 16bit, periph 32bit
	asm("wfi");
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
	DMA2_Channel1->CCR = DMA_CCR_EN | DMA_CCR_TCIE | DMA_CCR_TEIE | DMA_CCR_MINC | DMA_CCR_PSIZE_1 | DMA_CCR_MSIZE_0 | DMA_CCR_PL_1; // Configure and start DMA2 channel 1, mem 16bit, periph 32bit
	DMA2_Channel2->CCR = DMA_CCR_EN | DMA_CCR_TCIE | DMA_CCR_TEIE | DMA_CCR_MINC | DMA_CCR_PSIZE_1 | DMA_CCR_MSIZE_0 | DMA_CCR_PL_1; // Configure and start DMA2 channel 2, mem 16bit, periph 32bit
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
	DMA1_Channel1->CCR = DMA_CCR_EN | DMA_CCR_TCIE | DMA_CCR_TEIE | DMA_CCR_MINC | DMA_CCR_PSIZE_1 | DMA_CCR_MSIZE_0 | DMA_CCR_PL_1; // Configure and start DMA1 channel 1, mem 16bit, periph 32bit
	DMA2_Channel2->CCR = DMA_CCR_EN | DMA_CCR_TCIE | DMA_CCR_TEIE | DMA_CCR_MINC | DMA_CCR_PSIZE_1 | DMA_CCR_MSIZE_0 | DMA_CCR_PL_1; // Configure and start DMA2 channel 2, mem 16bit, periph 32bit
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
	DMA1_Channel1->CCR = DMA_CCR_EN | DMA_CCR_TCIE | DMA_CCR_TEIE | DMA_CCR_MINC | DMA_CCR_PSIZE_1 | DMA_CCR_MSIZE_0 | DMA_CCR_PL_1; // Configure and start DMA1 channel 1, mem 16bit, periph 32bit
	DMA2_Channel1->CCR = DMA_CCR_EN | DMA_CCR_TCIE | DMA_CCR_TEIE | DMA_CCR_MINC | DMA_CCR_PSIZE_1 | DMA_CCR_MSIZE_0 | DMA_CCR_PL_1; //, mem 16bit, periph 32bit
	DMA2_Channel2->CCR = DMA_CCR_EN | DMA_CCR_TCIE | DMA_CCR_TEIE | DMA_CCR_MINC | DMA_CCR_PSIZE_1 | DMA_CCR_MSIZE_0 | DMA_CCR_PL_1; // Configure and start DMA2 channel 2, mem 16bit, periph 32bit
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
	DMA1_Channel1->CCR = DMA_CCR_EN | DMA_CCR_TCIE | DMA_CCR_TEIE | DMA_CCR_MINC | DMA_CCR_PSIZE_1 | DMA_CCR_MSIZE_0 | DMA_CCR_PL_1; // Configure and start DMA1 channel 1
	// expected duration 5 cycles
	DMA2_Channel5->CCR = DMA_CCR_EN | DMA_CCR_TCIE | DMA_CCR_TEIE | DMA_CCR_MINC | DMA_CCR_PSIZE_1 | DMA_CCR_MSIZE_0 | DMA_CCR_PL_1;	// Configure and start DMA2 channel 5
	asm("wfi");
}

/**
 * @Brief Depending on the enabled channels and the timebase, this function determines how to acquire/sample data.
 * @param mode
 */
void aquireData(void)
{
	switch (channel_state)
	{
		case ENABLED_CH_NONE:
			break;
		case ENABLED_CH_1:
		{
			if(dso.timeDiv < timeDiv_10us)
			{
				aquireDataSingle(&dso.ch1);
				findTrigger();
				displayWaveformTrig(CH1_ID);
			}
			else if(dso.timeDiv < timeDiv_2_5us)
			{
				aquireDataDual(0);
				findTrigger();
				displayWaveformTrig(CH_ID_NONE);
			}
			else
			{
				aquireDataQuad();
				findTrigger();
				displayWaveformTrig(CH_ID_NONE);
			}
			break;
		}
		case ENABLED_CH_2:
		{
			if(dso.timeDiv < timeDiv_10us)
			{
				aquireDataSingle(&dso.ch2);
				findTrigger();
				displayWaveformTrig(CH2_ID);
			}
			break;
		}
		case ENABLED_CH_12:
		{
			if(dso.timeDiv < timeDiv_10us)
			{
				aquireDataParallel(0);
				findTrigger();
				displayWaveformTrig(CH1_ID);
				displayWaveformTrig(CH2_ID);
			}
		}
		case ENABLED_CH_3:
		{
			if(dso.timeDiv < timeDiv_10us)
			{
				aquireDataSingle(&dso.ch3);
				findTrigger();
				displayWaveformTrig(CH3_ID);
			}
			else if(dso.timeDiv < timeDiv_2_5us)
			{
				aquireDataDual(1);
				findTrigger();
				displayWaveformTrig(CH_ID_NONE);
			}
			break;
		}
		case ENABLED_CH_13:
		{
			if(dso.timeDiv < timeDiv_10us)
			{
				aquireDataParallel_13();
				findTrigger();
				displayWaveformTrig(CH1_ID);
				displayWaveformTrig(CH3_ID);
			}
			else if(dso.timeDiv < timeDiv_2_5us)
			{
				aquireDataDualParallel();
				findTrigger();
				displayWaveformTrig(CH_ID_NONE);
			}
			break;
		}
		case ENABLED_CH_23:
		{
			if(dso.timeDiv < timeDiv_10us)
			{
				aquireDataParallel_23();
				findTrigger();
				displayWaveformTrig(CH2_ID);
				displayWaveformTrig(CH3_ID);
			}
			break;
		}
		case ENABLED_CH_123:
		{
			if(dso.timeDiv < timeDiv_10us)
			{
				aquireDataAllParallel();
				findTrigger();
				displayWaveformTrig(CH1_ID);
				displayWaveformTrig(CH2_ID);
				displayWaveformTrig(CH3_ID);
			}
			break;
		}
		case ENABLED_CH_4:
		{
			if(dso.timeDiv < timeDiv_10us)
			{
				aquireDataSingle(&dso.ch4);
				findTrigger();
				displayWaveformTrig(CH4_ID);
			}
			break;
		}
		case ENABLED_CH_14:
		{
			if(dso.timeDiv < timeDiv_10us)
			{
				aquireDataParallel_14();
				findTrigger();
				displayWaveformTrig(CH1_ID);
				displayWaveformTrig(CH4_ID);
			}
			break;
		}
		case ENABLED_CH_24:
		{
			if(dso.timeDiv < timeDiv_10us)
			{
				aquireDataParallel_24();
				findTrigger();
				displayWaveformTrig(CH2_ID);
				displayWaveformTrig(CH4_ID);
			}
			break;
		}
		case ENABLED_CH_124:
		{
			if(dso.timeDiv < timeDiv_10us)
			{
				aquireDataParallel_124();
				findTrigger();
				displayWaveformTrig(CH1_ID);
				displayWaveformTrig(CH2_ID);
				displayWaveformTrig(CH4_ID);
			}
			break;
		}
		case ENABLED_CH_34:
		{
			if(dso.timeDiv < timeDiv_10us)
			{
				aquireDataParallel(1);
				findTrigger();
				displayWaveformTrig(CH3_ID);
				displayWaveformTrig(CH4_ID);
			}
			break;
		}
		case ENABLED_CH_134:
		{
			if(dso.timeDiv < timeDiv_10us)
			{
				aquireDataAllParallel();
				findTrigger();
				displayWaveformTrig(CH1_ID);
				displayWaveformTrig(CH3_ID);
				displayWaveformTrig(CH4_ID);
			}
			break;
		}
		case ENABLED_CH_234:
		{
			if(dso.timeDiv < timeDiv_10us)
			{
				aquireDataParallel_234();
				findTrigger();
				displayWaveformTrig(CH2_ID);
				displayWaveformTrig(CH3_ID);
				displayWaveformTrig(CH4_ID);
			}
			break;
		}
		case ENABLED_CH_1234:
		{
			if(dso.timeDiv < timeDiv_10us)
			{
				aquireDataAllParallel();
				findTrigger();
				displayWaveformTrig(CH1_ID);
				displayWaveformTrig(CH2_ID);
				displayWaveformTrig(CH3_ID);
				displayWaveformTrig(CH4_ID);
			}
			break;
		}
		default:
			break;
	}
}
