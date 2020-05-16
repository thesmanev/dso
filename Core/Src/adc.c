#include "main.h"

static void calibrateADC(channel_t *chx);
static void disableADC(uint32_t num);
static void enableADC(uint32_t num);

/**
 * @brief Initialize ADC
 * @param chx pointer to a channel structure. Each channel has it's own dedicated ADC
 */
void initADC(channel_t *chx)
{
	calibrateADC(chx);
	// all ADCs are initialized in single mode
	switch (chx->id)
	{
		default:
		case CH1_ID:
		{
			disableADC(CH1_ID);
			//synchronous clock mode, hclk / 1
			ADC12_COMMON->CCR = ADC12_CCR_CKMODE_0;
			 // Continuous mode, overrun enabled, 12bit, right align, DMA one-shot, disabled
			ADC1->CFGR = ADC_CFGR_CONT | ADC_CFGR_OVRMOD | ADC_CFGR_DMAEN;
			//SQ1 = 0x01, startuvaj //konverzija na ADC1_IN1
			ADC1->SQR1 = ADC_SQR1_SQ1_0;
			enableADC(CH1_ID);
			break;
		}
		case CH2_ID:
		{
			disableADC(CH2_ID);
			//synchronous clock mode, hclk / 1
			ADC12_COMMON->CCR = ADC12_CCR_CKMODE_0;
			 // Continuous mode, overrun enabled, 12bit, right align, DMA one-shot, disabled
			ADC2->CFGR = ADC_CFGR_CONT | ADC_CFGR_OVRMOD | ADC_CFGR_DMAEN;
			//SQ1 = 0x01, startuvaj //konverzija na ADC2_IN1
			ADC2->SQR1 = ADC_SQR1_SQ1_0;
			enableADC(CH2_ID);
			break;
		}
		case CH3_ID:
		{
			disableADC(CH3_ID);
			//synchronous clock mode, hclk / 1
			ADC34_COMMON->CCR = ADC34_CCR_CKMODE_0;
			 // Continuous mode, overrun enabled, 12bit, right align, DMA one-shot, disabled
			ADC3->CFGR = ADC_CFGR_CONT | ADC_CFGR_OVRMOD | ADC_CFGR_DMAEN;
			//SQ1 = 0x01, startuvaj //konverzija na ADC3_IN1
			ADC3->SQR1 = ADC_SQR1_SQ1_0;
			enableADC(CH3_ID);
			break;
		}
		case CH4_ID:
		{
			disableADC(CH4_ID);
			//synchronous clock mode, hclk / 1
			ADC34_COMMON->CCR = ADC34_CCR_CKMODE_0;
			 // Continuous mode, overrun enabled, 12bit, right align, DMA one-shot, disabled
			ADC4->CFGR = ADC_CFGR_CONT | ADC_CFGR_OVRMOD | ADC_CFGR_DMAEN;
			//SQ1 = 0x01, startuvaj //konverzija na ADC4_IN1
			ADC4->SQR1 = ADC_SQR1_SQ1_0;
			enableADC(CH4_ID);
			break;
		}
	}
}

/**
 * @brief Setup a single ADC
 * @param chx Pointer to a channel structure. Each channel has it's own, dedicated ADC.
 */
void setupADCSingle(channel_t *chx)
{
	switch (chx->id)
	{
		default:
		case CH1_ID:
		{
			switch (dso.timeDiv)
			{
				case timeDiv_1ms:
				{ // 1 ms
					ADC1->SMPR1 = ADC_SMPL_RATE_117_264_KSPS; // => sampling time 614 cycles (117.264 KS/s)
					break;
				}
				case timeDiv_500us:
				{ // 500 us
					ADC1->SMPR1 = ADC_SMPL_RATE_371_31_KSPS; // => sampling time 194 cycles (371.31 KS/s)
					break;
				}
				case timeDiv_250us:
				{ //250 us
					ADC1->SMPR1 = ADC_SMPL_RATE_973_KSPS; // => sampling time 74 cycles (973 KS/s)
					break;
				}
				case timeDiv_100us:
				{ // 100 us
					ADC1->SMPR1 = ADC_SMPL_RATE_3_6_MSPS; // => sampling time 20 cycles (3.6 MS/s)
					break;
				}
				case timeDiv_50us:
				{ //50 us
					ADC1->SMPR1 = ADC_SMPL_RATE_5_14_MSPS; // => sampling time 14 cycles (5.14 MS/s)
					break;
				}
				case timeDiv_25us:
				{ //25 us
					ADC1->SMPR1 = ADC_SMPL_RATE_5_14_MSPS; // => sampling time 14 cycles (5.14 MS/s)
					break;
				}
				case timeDiv_10us:
				{ //10 us
					ADC1->SMPR1 = ADC_SMPL_RATE_5_14_MSPS; // => sampling time 14 cycles (5.14 MS/s)
					break;
				}
				case timeDiv_5us:
				{ //5 us
					ADC1->SMPR1 = ADC_SMPL_RATE_5_14_MSPS; // => sampling time 14 cycles (5.14 MS/s)
					break;
				}
				case timeDiv_2_5us:
				{ //2.5 us
					ADC1->SMPR1 = ADC_SMPL_RATE_5_14_MSPS; // => sampling time 14 cycles (5.14 MS/s)
					break;
				}
				case timeDiv_1us:
				{ //1 us
					ADC1->SMPR1 = ADC_SMPL_RATE_5_14_MSPS; // => sampling time 14 cycles (5.14 MS/s)
					break;
				}
				default:
				{
					ADC1->SMPR1 = ADC_SMPL_RATE_5_14_MSPS; // => sampling time 14 cycles (5.14 MS/s)
					break;
				}
			}
		}
		case CH2_ID:
		{
			switch (dso.timeDiv)
			{
				case timeDiv_1ms:
				{ // 1 ms
					ADC2->SMPR1 = ADC_SMPL_RATE_117_264_KSPS; // => sampling time 614 cycles (117.264 KS/s)
					break;
				}
				case timeDiv_500us:
				{ // 500 us
					ADC2->SMPR1 = ADC_SMPL_RATE_371_31_KSPS; // => sampling time 194 cycles (371.31 KS/s)
					break;
				}
				case timeDiv_250us:
				{ //250 us
					ADC2->SMPR1 = ADC_SMPL_RATE_973_KSPS; // => sampling time 74 cycles (973 KS/s)
					break;
				}
				case timeDiv_100us:
				{ // 100 us
					ADC2->SMPR1 = ADC_SMPL_RATE_3_6_MSPS; // => sampling time 20 cycles (3.6 MS/s)
					break;
				}
				case timeDiv_50us:
				{ //50 us
					ADC2->SMPR1 = ADC_SMPL_RATE_5_14_MSPS; // => sampling time 14 cycles (5.14 MS/s)
					break;
				}
				case timeDiv_25us:
				{ //25 us
					ADC2->SMPR1 = ADC_SMPL_RATE_5_14_MSPS; // => sampling time 14 cycles (5.14 MS/s)
					break;
				}
				case timeDiv_10us:
				{ //10 us
					ADC2->SMPR1 = ADC_SMPL_RATE_5_14_MSPS; // => sampling time 14 cycles (5.14 MS/s)
					break;
				}
				case timeDiv_5us:
				{ //5 us
					ADC2->SMPR1 = ADC_SMPL_RATE_5_14_MSPS; // => sampling time 14 cycles (5.14 MS/s)
					break;
				}
				case timeDiv_2_5us:
				{ //2.5 us
					ADC2->SMPR1 = ADC_SMPL_RATE_5_14_MSPS; // => sampling time 14 cycles (5.14 MS/s)
					break;
				}
				case timeDiv_1us:
				{ //1 us
					ADC2->SMPR1 = ADC_SMPL_RATE_5_14_MSPS; // => sampling time 14 cycles (5.14 MS/s)
					break;
				}
				default:
				{
					ADC2->SMPR1 = ADC_SMPL_RATE_5_14_MSPS; // => sampling time 14 cycles (5.14 MS/s)
					break;
				}
			}
		}
		case CH3_ID:
		{
			switch (dso.timeDiv)
			{
				case timeDiv_1ms:
				{ // 1 ms
					ADC3->SMPR1 = ADC_SMPL_RATE_117_264_KSPS; // => sampling time 614 cycles (117.264 KS/s)
					break;
				}
				case timeDiv_500us:
				{ // 500 us
					ADC3->SMPR1 = ADC_SMPL_RATE_371_31_KSPS; // => sampling time 194 cycles (371.31 KS/s)
					break;
				}
				case timeDiv_250us:
				{ //250 us
					ADC3->SMPR1 = ADC_SMPL_RATE_973_KSPS; // => sampling time 74 cycles (973 KS/s)
					break;
				}
				case timeDiv_100us:
				{ // 100 us
					ADC3->SMPR1 = ADC_SMPL_RATE_3_6_MSPS; // => sampling time 20 cycles (3.6 MS/s)
					break;
				}
				case timeDiv_50us:
				{ //50 us
					ADC3->SMPR1 = ADC_SMPL_RATE_5_14_MSPS; // => sampling time 14 cycles (5.14 MS/s)
					break;
				}
				case timeDiv_25us:
				{ //25 us
					ADC3->SMPR1 = ADC_SMPL_RATE_5_14_MSPS; // => sampling time 14 cycles (5.14 MS/s)
					break;
				}
				case timeDiv_10us:
				{ //10 us
					ADC3->SMPR1 = ADC_SMPL_RATE_5_14_MSPS; // => sampling time 14 cycles (5.14 MS/s)
					break;
				}
				case timeDiv_5us:
				{ //5 us
					ADC3->SMPR1 = ADC_SMPL_RATE_5_14_MSPS; // => sampling time 14 cycles (5.14 MS/s)
					break;
				}
				case timeDiv_2_5us:
				{ //2.5 us
					ADC3->SMPR1 = ADC_SMPL_RATE_5_14_MSPS; // => sampling time 14 cycles (5.14 MS/s)
					break;
				}
				case timeDiv_1us:
				{ //1 us
					ADC3->SMPR1 = ADC_SMPL_RATE_5_14_MSPS; // => sampling time 14 cycles (5.14 MS/s)
					break;
				}
				default:
				{
					ADC3->SMPR1 = ADC_SMPL_RATE_5_14_MSPS; // => sampling time 14 cycles (5.14 MS/s)
					break;
				}
			}
		}
		case CH4_ID:
		{
			switch (dso.timeDiv)
			{
				case timeDiv_1ms:
				{ // 1 ms
					ADC4->SMPR1 = ADC_SMPL_RATE_117_264_KSPS; // => sampling time 614 cycles (117.264 KS/s)
					break;
				}
				case timeDiv_500us:
				{ // 500 us
					ADC4->SMPR1 = ADC_SMPL_RATE_371_31_KSPS; // => sampling time 194 cycles (371.31 KS/s)
					break;
				}
				case timeDiv_250us:
				{ //250 us
					ADC4->SMPR1 = ADC_SMPL_RATE_973_KSPS; // => sampling time 74 cycles (973 KS/s)
					break;
				}
				case timeDiv_100us:
				{ // 100 us
					ADC4->SMPR1 = ADC_SMPL_RATE_3_6_MSPS; // => sampling time 20 cycles (3.6 MS/s)
					break;
				}
				case timeDiv_50us:
				{ //50 us
					ADC4->SMPR1 = ADC_SMPL_RATE_5_14_MSPS; // => sampling time 14 cycles (5.14 MS/s)
					break;
				}
				case timeDiv_25us:
				{ //25 us
					ADC4->SMPR1 = ADC_SMPL_RATE_5_14_MSPS; // => sampling time 14 cycles (5.14 MS/s)
					break;
				}
				case timeDiv_10us:
				{ //10 us
					ADC4->SMPR1 = ADC_SMPL_RATE_5_14_MSPS; // => sampling time 14 cycles (5.14 MS/s)
					break;
				}
				case timeDiv_5us:
				{ //5 us
					ADC4->SMPR1 = ADC_SMPL_RATE_5_14_MSPS; // => sampling time 14 cycles (5.14 MS/s)
					break;
				}
				case timeDiv_2_5us:
				{ //2.5 us
					ADC4->SMPR1 = ADC_SMPL_RATE_5_14_MSPS; // => sampling time 14 cycles (5.14 MS/s)
					break;
				}
				case timeDiv_1us:
				{ //1 us
					ADC4->SMPR1 = ADC_SMPL_RATE_5_14_MSPS; // => sampling time 14 cycles (5.14 MS/s)
					break;
				}
				default:
				{
					ADC4->SMPR1 = ADC_SMPL_RATE_5_14_MSPS; // => sampling time 14 cycles (5.14 MS/s)
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
	if(num == ACQ_DUAL_34)
	{	// ADC3_4 in dual
		disableADC(CH3_ID);
		disableADC(CH4_ID);
		//mdma one shot, 12 bit, delay 5 TADC, interleaved mode
		ADC34_COMMON->CCR = ADC34_CCR_MULTI_0 | ADC34_CCR_MULTI_1 | ADC34_CCR_MULTI_2 |
							ADC34_CCR_DELAY_2 | ADC34_CCR_MDMA_1 | ADC34_CCR_CKMODE_0;
		enableADC(CH3_ID);
		enableADC(CH4_ID);
		ADC3->SMPR1 = ADC_SMPL_RATE_5_14_MSPS; // max sample rate
		ADC4->SMPR1 = ADC_SMPL_RATE_5_14_MSPS; // max sample rate
		//setup DMA
	}
	else if(num == ACQ_DUAL_12)
	{
		disableADC(CH1_ID);
		disableADC(CH2_ID);
		// wait for adc2 to disable
		ADC12_COMMON->CCR = ADC12_CCR_MULTI_0 | ADC12_CCR_MULTI_1 | ADC12_CCR_MULTI_2 |
							ADC12_CCR_DELAY_2 | ADC12_CCR_MDMA_1 | ADC12_CCR_CKMODE_0;
		enableADC(CH1_ID);
		enableADC(CH2_ID);
		ADC1->SMPR1 = ADC_SMPL_RATE_5_14_MSPS; // max sample rate
		ADC2->SMPR1 = ADC_SMPL_RATE_5_14_MSPS; // max sample rate
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
	if(num == ACQ_PARALLEL_34)
	{	// ADC3_4 in dual
		disableADC(CH3_ID);
		disableADC(CH4_ID);
		// wait for adc4 to disable
		//no mdma, regular simultaneous mode
		ADC34_COMMON->CCR = ADC34_CCR_MULTI_1 | ADC34_CCR_MULTI_2 | ADC34_CCR_CKMODE_0;
		enableADC(CH3_ID);
		enableADC(CH4_ID);
		setupADCSingle(&dso.ch3);
		setupADCSingle(&dso.ch4);
		//setup DMA
	}
	else if(num == ACQ_PARALLEL_12)
	{
		disableADC(CH1_ID);
		disableADC(CH2_ID);
		//no mdma, regular simultaneous mode
		ADC12_COMMON->CCR = ADC12_CCR_MULTI_1 | ADC12_CCR_MULTI_2 | ADC12_CCR_CKMODE_0;
		enableADC(CH1_ID);
		enableADC(CH2_ID);
		setupADCSingle(&dso.ch1);
		setupADCSingle(&dso.ch2);
		// setup DMA
	}

}

/**
 * @brief Setup all four ADCs for running in QUAD or interleaved mode
 */
void setupADCQuad(void)
{
	disableADC(CH1_ID);
	disableADC(CH2_ID);
	disableADC(CH3_ID);
	disableADC(CH4_ID);

	// synchronous clock, 12 bit MDMA circular mode, delay 2 T-ADC interleaved mode only
	ADC12_COMMON->CCR = ADC12_CCR_MULTI_0 | ADC12_CCR_MULTI_1 | ADC12_CCR_MULTI_2 |
			            ADC12_CCR_DELAY_0 | ADC12_CCR_MDMA_1 | ADC12_CCR_CKMODE_0;

	// synchronous clock, 12 bit MDMA circular mode, delay 2 T-ADC interleaved mode only
	ADC34_COMMON->CCR = ADC34_CCR_MULTI_0 | ADC34_CCR_MULTI_1 | ADC34_CCR_MULTI_2 |
            			ADC34_CCR_DELAY_0 | ADC34_CCR_MDMA_1 | ADC34_CCR_CKMODE_0;
	enableADC(CH1_ID);
	enableADC(CH2_ID);
	enableADC(CH3_ID);
	enableADC(CH4_ID);
	ADC1->SMPR1 = ADC_SMPL_RATE_5_14_MSPS; // max sample rate
	ADC2->SMPR1 = ADC_SMPL_RATE_5_14_MSPS; // max sample rate
	ADC3->SMPR1 = ADC_SMPL_RATE_5_14_MSPS; // max sample rate
	ADC4->SMPR1 = ADC_SMPL_RATE_5_14_MSPS; // max sample rate
}

/**
 * @brief Interrupt handler for ADCs 1 & 2
 */
void ADC1_2_IRQHandler(void)
{
	if((ADC1->ISR & ADC_ISR_OVR) | (ADC2->ISR & ADC_ISR_OVR))
	{
		ADC1->CR |= ADC_CR_ADSTP; //stop adc1 (and ADC2 bcs dual mode)
		ADC3->CR |= ADC_CR_ADSTP; //stop adc3 (and ADC4 bcs dual mode)
		DMA1_Channel1->CCR &= ~DMA_CCR_EN; //
		DMA2_Channel5->CCR &= ~DMA_CCR_EN; // DMA disabled
		assertError(1);
	}
}

/**
 * @brief Interrupt handler for ADC3
 */
void ADC3_IRQHandler(void)
{
	if((ADC3->ISR & ADC_ISR_OVR))
	{
		ADC3->CR |= ADC_CR_ADSTP; //stop adc3 (and ADC4 bcs dual mode)
		ADC1->CR |= ADC_CR_ADSTP; //stop adc1 (and ADC2 bcs dual mode)
		DMA2_Channel5->CCR &= ~DMA_CCR_EN; // DMA disabled
		DMA1_Channel1->CCR &= ~DMA_CCR_EN; //
		assertError(2);
	}
}

/**
 * @brief Iterrupt handler for ADC4
 */
void ADC4_IRQHandler(void)
{
	if((ADC4->ISR & ADC_ISR_OVR))
	{
		ADC3->CR |= ADC_CR_ADSTP; //stop adc3 (and ADC4 bcs dual mode)
		ADC1->CR |= ADC_CR_ADSTP; //stop adc1 (and ADC2 bcs dual mode)
		DMA2_Channel5->CCR &= ~DMA_CCR_EN; // DMA disabled
		DMA1_Channel1->CCR &= ~DMA_CCR_EN; //
		assertError(3);
	}
}

/**
 * @brief Configure clocks for the ADCs
 */
void ADC_Clocks(void)
{
	ADC12_COMMON->CCR = ADC12_CCR_CKMODE_0; //synchronous clock
	ADC34_COMMON->CCR = ADC34_CCR_CKMODE_0; //synchronous clock
}

/**
 * @brief Calibrate an ADC
 * @param chx Pointer to a channel structure. Each channel has it's own, dedicated ADC.
 */
static void calibrateADC(channel_t *chx)
{
	uint32_t i;
	switch (chx->id)
	{
		default:
		case CH1_ID:
		{
			disableADC(chx->id);
			//00: ADC Intermediate state
			ADC1->CR &= ~(ADC_CR_ADVREGEN_0 | ADC_CR_ADVREGEN_1);
			//01: ADC Voltage regulator enabled
			ADC1->CR |= ADC_CR_ADVREGEN_0;
			for (i = 0; i < 1000; i++);     	// some delay
			//calibrate in Single-ended inputs Mode.
			ADC1->CR &= ~ADC_CR_ADCALDIF;
			//Start ADC calibration
			ADC1->CR |= ADC_CR_ADCAL;
			//wait until calibration done
			while (ADC1->CR & ADC_CR_ADCAL);
			break;
		}
		case CH2_ID:
		{
			disableADC(chx->id);
			//00: ADC Intermediate state
			ADC2->CR &= ~(ADC_CR_ADVREGEN_0 | ADC_CR_ADVREGEN_1);
			//01: ADC Voltage regulator enabled
			ADC2->CR |= ADC_CR_ADVREGEN_0;
			for (i = 0; i < 1000; i++);     	// some delay
			//calibrate in Single-ended inputs Mode.
			ADC2->CR &= ~ADC_CR_ADCALDIF;
			//Start ADC calibration
			ADC2->CR |= ADC_CR_ADCAL;
			//wait until calibration done
			while (ADC2->CR & ADC_CR_ADCAL);
			break;
		}
		case CH3_ID:
		{
			disableADC(chx->id);
			//00: ADC Intermediate state
			ADC3->CR &= ~(ADC_CR_ADVREGEN_0 | ADC_CR_ADVREGEN_1);
			//01: ADC Voltage regulator enabled
			ADC3->CR |= ADC_CR_ADVREGEN_0;
			for (i = 0; i < 1000; i++);     	// some delay
			//calibrate in Single-ended inputs Mode.
			ADC3->CR &= ~ADC_CR_ADCALDIF;
			//Start ADC calibration
			ADC3->CR |= ADC_CR_ADCAL;
			//wait until calibration done
			while (ADC3->CR & ADC_CR_ADCAL);
			break;
		}
		case CH4_ID:
		{
			disableADC(chx->id);
			//00: ADC Intermediate state
			ADC4->CR &= ~(ADC_CR_ADVREGEN_0 | ADC_CR_ADVREGEN_1);
			//01: ADC Voltage regulator enabled
			ADC4->CR |= ADC_CR_ADVREGEN_0;
			for (i = 0; i < 1000; i++);     	// some delay
			//calibrate in Single-ended inputs Mode.
			ADC4->CR &= ~ADC_CR_ADCALDIF;
			//Start ADC calibration
			ADC4->CR |= ADC_CR_ADCAL;
			//wait until calibration done
			while (ADC4->CR & ADC_CR_ADCAL);
			break;
		}
	}
}

/**
 * @brief Disable an ADC
 * @param num The number of the ADC 1 to 4
 */
static void disableADC(uint32_t num)
{
	switch (num)
	{
		default:
		case CH1_ID:
			if(ADC1->CR & ADC_CR_ADEN)
			{
				ADC1->CR |= ADC_CR_ADDIS;
				while(ADC1->CR & ADC_CR_ADDIS); //ADDIS bit is high until ADC gets disabled
			}
			break;

		case CH2_ID:
			if(ADC2->CR & ADC_CR_ADEN)
			{
				ADC2->CR |= ADC_CR_ADDIS;
				while (ADC2->CR & ADC_CR_ADDIS); //ADDIS bit is high until ADC gets disabled
			}
			break;

		case CH3_ID:
			if(ADC3->CR & ADC_CR_ADEN)
			{
				ADC3->CR |= ADC_CR_ADDIS;
				while (ADC3->CR & ADC_CR_ADDIS); //ADDIS bit is high until ADC gets disabled
			}
			break;

		case CH4_ID:
			if(ADC4->CR & ADC_CR_ADEN)
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
static void enableADC(uint32_t num)
{
	switch (num)
	{
		default:
		case CH1_ID:
			if(!(ADC1->CR & ADC_CR_ADEN))
			{
				ADC1->CR |= ADC_CR_ADEN; //Enable ADC1
				while (!(ADC1->ISR & ADC_ISR_ADRD)); //wait for ADRDY
			}
			break;

		case CH2_ID:
			if(!(ADC2->CR & ADC_CR_ADEN))
			{
				ADC2->CR |= ADC_CR_ADEN; //Enable ADC1
				while (!(ADC2->ISR & ADC_ISR_ADRD)); //wait for ADRDY
			}
			break;

		case CH3_ID:
			if(!(ADC3->CR & ADC_CR_ADEN))
			{
				ADC3->CR |= ADC_CR_ADEN; //Enable ADC1
				while (!(ADC3->ISR & ADC_ISR_ADRD)); //wait for ADRDY
			}
			break;

		case CH4_ID:
			if(!(ADC4->CR & ADC_CR_ADEN))
			{
				ADC4->CR |= ADC_CR_ADEN; //Enable ADC1
				while (!(ADC4->ISR & ADC_ISR_ADRD)); //wait for ADRDY
			}
			break;
	}
}

