#include "main.h"

void disableADC(uint32_t num);
void enableADC(uint32_t num);

/**
 * @brief Initialize ADC
 * @param chx pointer to a channel structure. Each channel has it's own dedicated ADC
 */
void initADC(channel *chx)
{
	calibrateADC(chx);
	// all ADCs are initialized in single mode
	switch (chx->id)
	{
	case 1:
	{
		disableADC(1);
		ADC12_COMMON->CCR = 0x00010000; //synchronous clock
		ADC1->CFGR = 0x00003001; // Continuous mode, overrun enabled, 12bit, right align, DMA one-shot, disabled
		ADC1->SQR1 = 0x00000040; //SQ1 = 0x01, startuvaj //konverzija na ADC1_IN1
		enableADC(1);
		break;
	}
	case 2:
	{
		disableADC(2);
		ADC12_COMMON->CCR = 0x00010000; //synchronous clock
		ADC2->CFGR = 0x00003001; // Continuous mode, overrun enabled, 12bit, right align,
		ADC2->SQR1 = 0x00000040; //SQ1 = 0x01, startuvaj //konverzija na ADC2_IN1
		enableADC(2);
		break;
	}
	case 3:
	{
		disableADC(3);
		ADC34_COMMON->CCR = 0x00010000; //synchronous clock
		ADC3->CFGR = 0x00003001; // Continuous mode, overrun enabled, 12bit, right align,
		ADC3->SQR1 = 0x00000040; //SQ1 = 0x01, startuvaj //konverzija na ADC2_IN1
		enableADC(3);
		break;
	}
	case 4:
	{
		disableADC(4);
		ADC34_COMMON->CCR = 0x00010000; //synchronous clock
		ADC4->CFGR = 0x00003001; // Continuous mode, overrun enabled, 12bit, right align,
		ADC4->SQR1 = 0x00000040; //SQ1 = 0x01, startuvaj //konverzija na ADC2_IN1
		enableADC(4);
		break;
	}
	}
}

/**
 * @brief Setup a single ADC
 * @param chx Pointer to a channel structure. Each channel has it's own, dedicated ADC.
 */
void setupADCSingle(channel *chx)
{
	switch (chx->id)
	{
	case 1:
	{
		switch (dso1.timeDiv)
		{
		case timeDiv_1ms:
		{ // 1 ms
			ADC1->SMPR1 = 0x00000038; // => sampling time 614 cycles (117.264 KS/s)
			break;
		}
		case timeDiv_500us:
		{ // 500 us
			ADC1->SMPR1 = 0x00000030; // => sampling time 194 cycles (371.31 KS/s)
			break;
		}
		case timeDiv_250us:
		{ //250 us
			ADC1->SMPR1 = 0x00000028; // => sampling time 74 cycles (973 KS/s)
			break;
		}
		case timeDiv_100us:
		{ // 100 us
			ADC1->SMPR1 = 0x00000018; // => sampling time 20 cycles (3.6 MS/s)
			break;
		}
		case timeDiv_50us:
		{ //50 us
			ADC1->SMPR1 = 0x00000000; // => sampling time 14 cycles (5.14 MS/s)
			break;
		}
		case timeDiv_25us:
		{ //25 us
			ADC1->SMPR1 = 0x00000000; // => sampling time 14 cycles (5.14 MS/s)
			break;
		}
		case timeDiv_10us:
		{ //10 us
			ADC1->SMPR1 = 0x00000000; // => sampling time 14 cycles (5.14 MS/s)
			break;
		}
		case timeDiv_5us:
		{ //5 us
			ADC1->SMPR1 = 0x00000000; // => sampling time 14 cycles (5.14 MS/s)
			break;
		}
		case timeDiv_2_5us:
		{ //2.5 us
			ADC1->SMPR1 = 0x00000000; // => sampling time 14 cycles (5.14 MS/s)
			break;
		}
		case timeDiv_1us:
		{ //1 us
			ADC1->SMPR1 = 0x00000000; // => sampling time 14 cycles (5.14 MS/s)
			break;
		}
		default:
		{
			ADC1->SMPR1 = 0x00000000; // => sampling time 14 cycles (5.14 MS/s)
			break;
		}
		}
	}
	case 2:
	{
		switch (dso1.timeDiv)
		{
		case timeDiv_1ms:
		{ // 1 ms
			ADC2->SMPR1 = 0x00000038; // => sampling time 614 cycles (117.264 KS/s)
			break;
		}
		case timeDiv_500us:
		{ // 500 us
			ADC2->SMPR1 = 0x00000030; // => sampling time 194 cycles (371.31 KS/s)
			break;
		}
		case timeDiv_250us:
		{ //250 us
			ADC2->SMPR1 = 0x00000028; // => sampling time 74 cycles (973 KS/s)
			break;
		}
		case timeDiv_100us:
		{ // 100 us
			ADC2->SMPR1 = 0x00000018; // => sampling time 20 cycles (3.6 MS/s)
			break;
		}
		case timeDiv_50us:
		{ //50 us
			ADC2->SMPR1 = 0x00000000; // => sampling time 14 cycles (5.14 MS/s)
			break;
		}
		case timeDiv_25us:
		{ //25 us
			ADC2->SMPR1 = 0x00000000; // => sampling time 14 cycles (5.14 MS/s)
			break;
		}
		case timeDiv_10us:
		{ //10 us
			ADC2->SMPR1 = 0x00000000; // => sampling time 14 cycles (5.14 MS/s)
			break;
		}
		case timeDiv_5us:
		{ //5 us
			ADC2->SMPR1 = 0x00000000; // => sampling time 14 cycles (5.14 MS/s)
			break;
		}
		case timeDiv_2_5us:
		{ //2.5 us
			ADC2->SMPR1 = 0x00000000; // => sampling time 14 cycles (5.14 MS/s)
			break;
		}
		case timeDiv_1us:
		{ //1 us
			ADC2->SMPR1 = 0x00000000; // => sampling time 14 cycles (5.14 MS/s)
			break;
		}
		default:
		{
			ADC2->SMPR1 = 0x00000000; // => sampling time 14 cycles (5.14 MS/s)
			break;
		}
		}
	}
	case 3:
	{
		switch (dso1.timeDiv)
		{
		case timeDiv_1ms:
		{ // 1 ms
			ADC3->SMPR1 = 0x00000038; // => sampling time 614 cycles (117.264 KS/s)
			break;
		}
		case timeDiv_500us:
		{ // 500 us
			ADC3->SMPR1 = 0x00000030; // => sampling time 194 cycles (371.31 KS/s)
			break;
		}
		case timeDiv_250us:
		{ //250 us
			ADC3->SMPR1 = 0x00000028; // => sampling time 74 cycles (973 KS/s)
			break;
		}
		case timeDiv_100us:
		{ // 100 us
			ADC3->SMPR1 = 0x00000018; // => sampling time 20 cycles (3.6 MS/s)
			break;
		}
		case timeDiv_50us:
		{ //50 us
			ADC3->SMPR1 = 0x00000000; // => sampling time 14 cycles (5.14 MS/s)
			break;
		}
		case timeDiv_25us:
		{ //25 us
			ADC3->SMPR1 = 0x00000000; // => sampling time 14 cycles (5.14 MS/s)
			break;
		}
		case timeDiv_10us:
		{ //10 us
			ADC3->SMPR1 = 0x00000000; // => sampling time 14 cycles (5.14 MS/s)
			break;
		}
		case timeDiv_5us:
		{ //5 us
			ADC3->SMPR1 = 0x00000000; // => sampling time 14 cycles (5.14 MS/s)
			break;
		}
		case timeDiv_2_5us:
		{ //2.5 us
			ADC3->SMPR1 = 0x00000000; // => sampling time 14 cycles (5.14 MS/s)
			break;
		}
		case timeDiv_1us:
		{ //1 us
			ADC3->SMPR1 = 0x00000000; // => sampling time 14 cycles (5.14 MS/s)
			break;
		}
		default:
		{
			ADC3->SMPR1 = 0x00000000; // => sampling time 14 cycles (5.14 MS/s)
			break;
		}
		}
	}
	case 4:
	{
		switch (dso1.timeDiv)
		{
		case timeDiv_1ms:
		{ // 1 ms
			ADC4->SMPR1 = 0x00000038; // => sampling time 614 cycles (117.264 KS/s)
			break;
		}
		case timeDiv_500us:
		{ // 500 us
			ADC4->SMPR1 = 0x00000030; // => sampling time 194 cycles (371.31 KS/s)
			break;
		}
		case timeDiv_250us:
		{ //250 us
			ADC4->SMPR1 = 0x00000028; // => sampling time 74 cycles (973 KS/s)
			break;
		}
		case timeDiv_100us:
		{ // 100 us
			ADC4->SMPR1 = 0x00000018; // => sampling time 20 cycles (3.6 MS/s)
			break;
		}
		case timeDiv_50us:
		{ //50 us
			ADC4->SMPR1 = 0x00000000; // => sampling time 14 cycles (5.14 MS/s)
			break;
		}
		case timeDiv_25us:
		{ //25 us
			ADC4->SMPR1 = 0x00000000; // => sampling time 14 cycles (5.14 MS/s)
			break;
		}
		case timeDiv_10us:
		{ //10 us
			ADC4->SMPR1 = 0x00000000; // => sampling time 14 cycles (5.14 MS/s)
			break;
		}
		case timeDiv_5us:
		{ //5 us
			ADC4->SMPR1 = 0x00000000; // => sampling time 14 cycles (5.14 MS/s)
			break;
		}
		case timeDiv_2_5us:
		{ //2.5 us
			ADC4->SMPR1 = 0x00000000; // => sampling time 14 cycles (5.14 MS/s)
			break;
		}
		case timeDiv_1us:
		{ //1 us
			ADC4->SMPR1 = 0x00000000; // => sampling time 14 cycles (5.14 MS/s)
			break;
		}
		default:
		{
			ADC4->SMPR1 = 0x00000000; // => sampling time 14 cycles (5.14 MS/s)
			break;
		}
		}
	}
	}
}

/**
 * @brief Setup ADCs 1 & 2 or 3 & 4 for work in interleaved mode
 * @param num 0 for ADCs 1 & 2, 1 for ADCs 3 & 4
 */
void setupADCDual(uint32_t num)
{
	// dual mode used for 10 MS/s only (nanosecond timeDiv)
	if (num)
	{	// ADC3_4 in dual
		disableADC(3);
		disableADC(4);
		ADC34_COMMON->CCR = 0x00018407;	//mdma one shot, 12 bit, delay 5 TADC, interleaved mode
		enableADC(3);
		enableADC(4);
		ADC3->SMPR1 = 0; // max sample rate
		ADC4->SMPR1 = 0; // max sample rate
		//setup DMA
	}
	else
	{
		disableADC(1);
		disableADC(2);
		// wait for adc2 to disable
		ADC12_COMMON->CCR = 0x00018407;	//mdma one shot, 12 bit, delay 5 TADC, interleaved mode
		enableADC(1);
		enableADC(2);
		ADC1->SMPR1 = 0; // max sample rate
		ADC2->SMPR1 = 0; // max sample rate
		// setup DMA
	}
}
/**
 * @brief Setup ADCs 1 & 2, or 3 & 4 to run in parallel or regular simultaneous mode
 * @param num 0 -> 1 & 2, 1 -> 3 & 4
 */
void setupADCParallel(uint32_t num)
{
	// dual mode used for 10 MS/s only (nanosecond timeDiv)
	if (num)
	{	// ADC3_4 in dual
		disableADC(3);
		disableADC(4);
		// wait for adc4 to disable
		ADC34_COMMON->CCR = 0x00010006;	//no mdma, regular simultaneous mode
		enableADC(3);
		enableADC(4);
		setupADCSingle(&dso1.ch3);
		setupADCSingle(&dso1.ch4);
		//setup DMA
	}
	else
	{
		disableADC(1);
		disableADC(2);
		// wait for adc2 to disable
		ADC12_COMMON->CCR = 0x00010006;	// no mdma, regular simultaneous mode
		enableADC(1);
		enableADC(2);
		setupADCSingle(&dso1.ch3);
		setupADCSingle(&dso1.ch4);
		// setup DMA
	}
}

/**
 * @brief Setup all four ADCs for running in QUAD or interleaved mode
 */
void setupADCQuad(void)
{
	disableADC(1);
	disableADC(2);
	disableADC(3);
	disableADC(4);
	while (ADC4->CR & ADC_CR_ADDIS);// ADDIS bit is high until ADC gets disabled
	ADC12_COMMON->CCR = 0x00018107; // synchronous clock, 12 bit MDMA circular mode, delay 2 T-ADC interleaved mode only
	ADC34_COMMON->CCR = 0x00018107; // synchronous clock, 12 bit MDMA circular mode, delay 2 T-ADC interleaved mode only
	enableADC(1);
	enableADC(2);
	enableADC(3);
	enableADC(4);
	ADC1->SMPR1 = 0; // max sample rate
	ADC2->SMPR1 = 0; // max sample rate
	ADC3->SMPR1 = 0; // max sample rate
	ADC4->SMPR1 = 0; // max sample rate
}

/**
 * @brief Interrupt handler for ADCs 1 & 2
 */
void ADC1_2_IRQHandler(void)
{
	interrupts[2] = 1; // if interrupt is from overrun -> stop
	if ((ADC1->ISR & ADC_ISR_OVR) | (ADC2->ISR & ADC_ISR_OVR))
	{
		ADC1->CR |= ADC_CR_ADSTP; //stop adc1 (and ADC2 bcs dual mode)
		ADC3->CR |= ADC_CR_ADSTP; //stop adc3 (and ADC4 bcs dual mode)
		overrun = 1;
		DMA1_Channel1->CCR = 0x00002aaa; //
		DMA2_Channel5->CCR = 0x00002aaa; // DMA disabled
		assertError(1);
	}
}

/**
 * @brief Interrupt handler for ADC3
 */
void ADC3_IRQHandler(void)
{
	interrupts[3] = 1; // if interrupt is from overrun -> stop
	if ((ADC3->ISR & ADC_ISR_OVR))
	{
		ADC3->CR |= ADC_CR_ADSTP; //stop adc3 (and ADC4 bcs dual mode)
		ADC1->CR |= ADC_CR_ADSTP; //stop adc1 (and ADC2 bcs dual mode)
		overrun = 2; // overrun from adc3
		DMA2_Channel5->CCR = 0x00002aaa; // DMA disabled
		DMA1_Channel1->CCR = 0x00002aaa; //
		assertError(2);
	}
}

/**
 * @brief Iterrupt handler for ADC4
 */
void ADC4_IRQHandler(void)
{
	interrupts[4] = 1; // if interrupt is from overrun -> stop
	if ((ADC4->ISR & ADC_ISR_OVR))
	{
		ADC3->CR |= ADC_CR_ADSTP; //stop adc3 (and ADC4 bcs dual mode)
		ADC1->CR |= ADC_CR_ADSTP; //stop adc1 (and ADC2 bcs dual mode)
		overrun = 3; // overrun from adc4
		DMA2_Channel5->CCR = 0x00002aaa; // DMA disabled
		DMA1_Channel1->CCR = 0x00002aaa; //
		assertError(3);
	}
}

/**
 * @brief Configure clocks for the ADCs
 */
void ADC_Clocks(void)
{
	ADC12_COMMON->CCR = 0x00010000; //synchronous clock
	ADC34_COMMON->CCR = 0x00010000; //synchronous clock
}

/**
 * @brief Calibrate an ADC
 * @param chx Pointer to a channel structure. Each channel has it's own, dedicated ADC.
 */
void calibrateADC(channel *chx)
{
	uint32_t i;
	switch (chx->id)
	{
	case 1:
	{
		disableADC(chx->id);
		ADC1->CR &= ~(0x3 << 28); //00: ADC Intermediate state
		ADC1->CR |= 0x1 << 28; //01: ADC Voltage regulator enabled
		for (i = 0; i < 1000; i++); // some delay
		ADC1->CR &= ~(0x1 << 30); //kalibracija vo Single-ended inputs Mode.
		ADC1->CR |= 0x1 << 31; //Start ADC calibration
		while (ADC1->CR & (0x1 << 31)); //wait until calibration done
		break;
	}
	case 2:
	{
		disableADC(chx->id);
		ADC2->CR &= ~(0x3 << 28); //00: ADC Intermediate state
		ADC2->CR |= 0x1 << 28; //01: ADC Voltage regulator enabled
		for (i = 0; i < 1000; i++);
		ADC2->CR &= ~(0x1 << 30); //kalibracija vo Single-ended inputs Mode.
		ADC2->CR |= 0x1 << 31; //Start ADC calibration
		while (ADC2->CR & (0x1 << 31)); //wait until calibration done
		break;
	}
	case 3:
	{
		disableADC(chx->id);
		ADC3->CR &= ~(0x3 << 28); //00: ADC Intermediate state
		ADC3->CR |= 0x1 << 28; //01: ADC Voltage regulator enabled
		for (i = 0; i < 1000; i++);
		ADC3->CR &= ~(0x1 << 30); //kalibracija vo Single-ended inputs Mode.
		ADC3->CR |= 0x1 << 31; //Start ADC calibration
		while (ADC3->CR & (0x1 << 31)); //wait until calibration done
		break;
	}
	case 4:
	{
		disableADC(chx->id);
		ADC4->CR &= ~(0x3 << 28); //00: ADC Intermediate state
		ADC4->CR |= 0x1 << 28; //01: ADC Voltage regulator enabled
		for (i = 0; i < 1000; i++);
		ADC4->CR &= ~(0x1 << 30); //kalibracija vo Single-ended inputs Mode.
		ADC4->CR |= 0x1 << 31; //Start ADC calibration
		while (ADC4->CR & (0x1 << 31)); //wait until calibration done
		break;
	}
	}
}

/**
 * @brief Disable an ADC
 * @param num The number of the ADC 1 to 4
 */
void disableADC(uint32_t num)
{
	switch (num)
	{
	case 1:
		if (ADC1->CR & ADC_CR_ADEN)
		{
			ADC1->CR |= ADC_CR_ADDIS;
			while (ADC1->CR & ADC_CR_ADDIS); //ADDIS bit is high until ADC gets disabled
		}
		break;

	case 2:
		if (ADC2->CR & ADC_CR_ADEN)
		{
			ADC2->CR |= ADC_CR_ADDIS;
			while (ADC2->CR & ADC_CR_ADDIS); //ADDIS bit is high until ADC gets disabled
		}
		break;

	case 3:
		if (ADC3->CR & ADC_CR_ADEN)
		{
			ADC3->CR |= ADC_CR_ADDIS;
			while (ADC3->CR & ADC_CR_ADDIS); //ADDIS bit is high until ADC gets disabled
		}
		break;

	case 4:
		if (ADC4->CR & ADC_CR_ADEN)
		{
			ADC4->CR |= ADC_CR_ADDIS;
			while (ADC4->CR & ADC_CR_ADDIS); //ADDIS bit is high until ADC gets disabled
		}
		break;
	}
}

/**
 * @brief Enable an ADC
 * @param num Number of the ADC 1 to 4
 */
void enableADC(uint32_t num)
{
	switch (num)
	{
	case 1:
		if (!(ADC1->CR & ADC_CR_ADEN))
		{
			ADC1->CR |= ADC_CR_ADEN; //Enable ADC1
			while (!ADC1->ISR & ADC_ISR_ADRD); //wait for ADRDY
		}
		break;

	case 2:
		if (!(ADC2->CR & ADC_CR_ADEN))
		{
			ADC2->CR |= ADC_CR_ADEN; //Enable ADC1
			while (!ADC2->ISR & ADC_ISR_ADRD); //wait for ADRDY
		}
		break;

	case 3:
		if (!(ADC3->CR & ADC_CR_ADEN))
		{
			ADC3->CR |= ADC_CR_ADEN; //Enable ADC1
			while (!ADC3->ISR & ADC_ISR_ADRD); //wait for ADRDY
		}
		break;

	case 4:
		if (!(ADC4->CR & ADC_CR_ADEN))
		{
			ADC4->CR |= ADC_CR_ADEN; //Enable ADC1
			while (!ADC4->ISR & ADC_ISR_ADRD); //wait for ADRDY
		}
		break;
	}
}

