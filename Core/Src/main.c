#include "main.h"

dso_config_t dso; /**< Configuration structure for the DSO */

uint32_t channel_state = ENABLED_CH_NONE; /**< Indicates which of the four channels is enabled */

uint8_t sendScreenBuffer[X_PIXELS * 4]; /**< Allocate RAM for the screenbuffers */
uint8_t *sendScreenBuffer1; /**< Screen buffer for channel 1 */
uint8_t *sendScreenBuffer2; /**< Screen buffer for channel 2 */
uint8_t *sendScreenBuffer3; /**< Screen buffer for channel 3 */
uint8_t *sendScreenBuffer4; /**< Screen buffer for channel 4 */
uint32_t *buffer1; /**< Buffer for channel 1. (allocates memory for all four channels) */
uint32_t *buffer2; /**< Acquisition buffer for channel 2 */
uint32_t *buffer3; /**< Acquisition buffer for channel 2 */
uint32_t *buffer4; /**< Acquisition buffer for channel 2 */
uint16_t AS1 = 0; /**< Variable for Analog switch 1. Each bit of the upper byte acts on
 	 	 	 	 	 one switch. The lower byte is needed to cycle out previous config */
uint16_t AS2 = 0; /**< Variable for Analog switch 2 */
uint16_t AS3 = 0; /**< Variable for Analog switch 3 */
uint16_t AS4 = 0; /**< Variable for Analog switch 4 */
uint16_t AS5 = 0; /**< Variable for Analog switch 5 */

button_t currentButton = none_pressed; /**< The current button that is pressed */
button_t prevButton = none_pressed; /**< The previous button */

void Error_Handler(void);

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void)
{
	RCC_OscInitTypeDef RCC_OscInitStruct = { 0 };
	RCC_ClkInitTypeDef RCC_ClkInitStruct = { 0 };
	RCC_PeriphCLKInitTypeDef PeriphClkInit = { 0 };

	/** Initializes the CPU, AHB and APB busses clocks
	 */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI | RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	RCC_OscInitStruct.LSIState = RCC_LSI_ON;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
	RCC_OscInitStruct.PLL.PREDIV = RCC_PREDIV_DIV1;
	if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
	{
		Error_Handler();
	}
	/** Initializes the CPU, AHB and APB busses clocks
	 */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

	if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
	{
		Error_Handler();
	}
	PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART3 | RCC_PERIPHCLK_RTC;
	PeriphClkInit.Usart3ClockSelection = RCC_USART3CLKSOURCE_PCLK1;
	PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;
	if(HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
	{
		Error_Handler();
	}
}

/**
 * @brief Setup the clocks for the peripherals.
 */
void setupClocks(void)
{
	//for touch and uart
	RCC->APB2ENR |= RCC_APB2ENR_SPI4EN | RCC_APB2ENR_USART1EN | RCC_APB2ENR_SPI1EN;

	//for display
	RCC->AHBENR |= RCC_AHBENR_GPIOGEN | RCC_AHBENR_GPIODEN;

	//for adc
	RCC->AHBENR |= RCC_AHBENR_GPIOBEN | RCC_AHBENR_ADC12EN | RCC_AHBENR_DMA1EN | RCC_AHBENR_GPIOAEN |
			RCC_AHBENR_DMA2EN | RCC_AHBENR_ADC34EN | RCC_AHBENR_GPIOEEN;
}

/**
 * @brief Setup priorities and enable DMA interrupts
 */
void enableInterrupts(void)
{
	HAL_NVIC_SetPriority(DMA1_Channel1_IRQn, DMA1_CHANNEL1_IRQ_PRIO, 0);
	HAL_NVIC_SetPriority(DMA2_Channel1_IRQn, DMA2_CHANNEL1_IRQ_PRIO, 0);
	HAL_NVIC_SetPriority(DMA2_Channel2_IRQn, DMA2_CHANNEL2_IRQ_PRIO, 0);
	HAL_NVIC_SetPriority(DMA2_Channel5_IRQn, DMA2_CHANNEL5_IRQ_PRIO, 0);
	NVIC_EnableIRQ(DMA1_Channel1_IRQn);
	NVIC_EnableIRQ(DMA2_Channel1_IRQn);
	NVIC_EnableIRQ(DMA2_Channel2_IRQn);
	NVIC_EnableIRQ(DMA2_Channel5_IRQn);
}

/**
 * @brief A simple, blocking delay function. Uses TIM6
 * @param ms delay in milliseconds
 */
void delay_ms(int ms)
{
	RCC->APB1ENR |= RCC_APB1ENR_TIM6EN;
	TIM6->PSC = 72 - 1; /* Prescale TIM6 clock by 71 (72 MHz -> 1 MHz) */
	TIM6->ARR = 1000 - 1; /* Reload after counting to 999 (1000 microseconds) */
	TIM6->CNT = 0; /* Start from 0 */
	TIM6->EGR |= TIM_EGR_UG; /* Enable generation of update event */
	TIM6->SR &= ~TIM_SR_UIF; /* Clear update interrupt flag */
	TIM6->CR1 |= TIM_CR1_CEN; /* Enable counter */

	while (ms > 0)
	{
		while ((TIM6->SR & TIM_SR_UIF) == 0);
		TIM6->SR &= ~TIM_SR_UIF;
		ms--;
	}
	TIM6->CR1 &= ~TIM_CR1_CEN; /* Disable counter */
	RCC->APB1ENR &= ~RCC_APB1ENR_TIM6EN;
}

/**
 * @brief Error handling function. Prints out the error on the LCD display
 * @param errno Error number
 */
void assertError(uint8_t errno)
{
	uint8_t str[7] =
	{ 'e', 'r', 'r', 'o', 'r', ' ', ' ' };
	str[6] = (uint8_t) (errno + 48); /* Convert error number to ASCII number */
	LCD_Background(ILI9341_BLUE);
	LCD_Print(12, 12, str, 7, ILI9341_WHITE);
	while (1);
}

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void)
{
	/* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */
	while (1)
		;
	/* USER CODE END Error_Handler_Debug */
}

/**
 * @brief Structure initialization function
 */
void initDSO(void)
{
	dso.dual1 = DUAL_OFF; // off
	dso.dual2 = DUAL_OFF; // off
	dso.horizontal = 0; // center
	dso.timeDiv = timeDiv_50us; // 50 us
	dso.quad = QUAD_OFF; //off
	dso.triggerEdge = TRIG_EDGE_RISING; // rising
	dso.triggerSource = TRIG_SRC_CH1; // ch1
	dso.triggerPoint = 50; // first sample (auto-triggering)
	dso.triggerValue = 2048; // 1.65v

	dso.ch1.buffSize = BUFF_SIZE;
	dso.ch1.coupling = CH_COUPLING_DC; //dc
	dso.ch1.enabled = CH_DISABLED; // off
	dso.ch1.selected = 0;
	dso.ch1.id = CH1_ID;
	dso.ch1.vertical = 0; // no offset
	dso.ch1.voltsDiv = v_1Div; // 1Vdiv

	dso.ch2.buffSize = BUFF_SIZE;
	dso.ch2.coupling = CH_COUPLING_DC; //dc
	dso.ch2.enabled = CH_DISABLED; // off
	dso.ch2.selected = 0;
	dso.ch2.id = CH2_ID;
	dso.ch2.vertical = 0; // no offset
	dso.ch2.voltsDiv = v_1Div; // 1Vdiv

	dso.ch3.buffSize = BUFF_SIZE;
	dso.ch3.coupling = CH_COUPLING_DC; //dc
	dso.ch3.enabled = CH_DISABLED; // off
	dso.ch3.selected = 0;
	dso.ch3.id = CH3_ID;
	dso.ch3.vertical = 0; // no offset
	dso.ch3.voltsDiv = v_1Div; // 1Vdiv

	dso.ch4.buffSize = BUFF_SIZE;
	dso.ch4.coupling = CH_COUPLING_DC; //dc
	dso.ch4.enabled = CH_DISABLED; // off
	dso.ch4.selected = 0;
	dso.ch4.id = CH4_ID;
	dso.ch4.vertical = 0; // no offset
	dso.ch4.voltsDiv = v_1Div; // 1Vdiv
}

/**
 * Toggle a uint32_t variable from 0 to 1 or the other way around.
 * @param var Pointer to the variable whose value is to be toggled
 * @note If the value of the variable is greater than 1, it is considered the same as being 1.
 */
void toggle(uint32_t *var)
{
	if(*var)
		*var = 0;
	else
	{
		*var = 1;
	}
}

/**
 * @brief This function handles changes in the system when a channel is enabled or disabled, or when the timebase is changed
 * 		It configures the ADCs and analog switches for the chosen channel and timebase combination.
 */
void channelChange(void)
{
	channel_state = dso.ch1.enabled;
	channel_state = channel_state + (dso.ch2.enabled << 1);
	channel_state = channel_state + (dso.ch3.enabled << 2);
	channel_state = channel_state + (dso.ch4.enabled << 3);

	switch (channel_state)
	{
		case 0:
		{ // all channels off
			break;
		}
		case 1:
		{ // channel 1 on
			if(dso.timeDiv < timeDiv_10us)
			{
				initADC(&dso.ch1);
				setupADCSingle(&dso.ch1);
				setupDMA(&dso.ch1);
				dso.dual1 = 0;
				dso.dual2 = 0;
				dso.quad = 0;
				/* Configure analog switches */
				AS5 |= 0b0000000100000000;
				AS5 &= 0b0000000100000000;
				setSwitch(5);
				setSwitch(1);
			}
			else
			{
				if(dso.timeDiv < timeDiv_2_5us)
				{
					setupADCDual(ACQ_DUAL_12);
					setupDMADual(ACQ_DUAL_12);
					dso.dual1 = 1;
					dso.dual2 = 0;
					dso.quad = 0;
					/* Configure analog switches */
					AS5 |= 0b0000001100000000;
					AS5 &= 0b0000001100000000;
					setSwitch(5);
					setSwitch(1);
				}
				else
				{
					setupADCQuad();
					setupDMAQuad();
					dso.dual1 = 0;
					dso.dual2 = 0;
					dso.quad = 1;
					/* Configure analog switches */
					AS5 |= 0b0000111100000000;
					AS5 &= 0b0000111100000000;
					setSwitch(5);
					setSwitch(1);
				}
			}
			break;
		}
		case 2:
		{ // ch2 only
			if(dso.timeDiv < timeDiv_10us)
			{
				initADC(&dso.ch2);
				setupADCSingle(&dso.ch2);
				setupDMA(&dso.ch2);
				dso.dual1 = 0;
				dso.dual2 = 0;
				dso.quad = 0;
				/* Configure analog switches */
				AS5 |= 0b0001000000000000;
				AS5 &= 0b0001000000000000;
				setSwitch(5);
				setSwitch(2);
			}
			else
			{
				//unavailable
			}
			break;
		}
		case 3:
		{
			if(dso.timeDiv < timeDiv_10us)
			{ //ch1 and ch2 parallel
				setupADCParallel(ACQ_PARALLEL_12);
				setupDMAParallel(ACQ_PARALLEL_12);
				dso.dual1 = 0;
				dso.dual2 = 0;
				dso.quad = 0;
				/* Configure analog switches */
				AS5 |= 0b0001000100000000;
				AS5 &= 0b0001000100000000;
				setSwitch(5);
				setSwitch(1);
				setSwitch(2);
			}
			else
			{
				//unavailable
			}
			break;
		}
		case 4:
		{ // channel 3 only
			if(dso.timeDiv < timeDiv_10us)
			{
				initADC(&dso.ch3);
				setupADCSingle(&dso.ch3);
				setupDMA(&dso.ch3);
				dso.dual1 = 0;
				dso.dual2 = 0;
				dso.quad = 0;
				/* Configure analog switches */
				AS5 |= 0b0010000000000000;
				AS5 &= 0b0010000000000000;
				setSwitch(5);
				setSwitch(3);
			}
			else
			{
				if(dso.timeDiv < timeDiv_2_5us)
				{
					setupADCDual(ACQ_DUAL_34);
					setupDMADual(ACQ_DUAL_34);
					dso.dual1 = 0;
					dso.dual2 = 1;
					dso.quad = 0;
					/* Configure analog switches */
					AS5 |= 0b0110000000000000;
					AS5 &= 0b0110000000000000;
					setSwitch(5);
					setSwitch(3);
				}
				else
				{
					//unavailable
				}
			}
			break;
		}
		case 5:
		{ // channels 1 and 3
			if(dso.timeDiv < timeDiv_10us)
			{
				initADC(&dso.ch1);
				initADC(&dso.ch3);
				setupADCSingle(&dso.ch1);
				setupDMA(&dso.ch1);
				setupADCSingle(&dso.ch3);
				setupDMA(&dso.ch3);
				dso.dual1 = 0;
				dso.dual2 = 0;
				dso.quad = 0;
				/* Configure analog switches */
				AS5 |= 0b0010000100000000;
				AS5 &= 0b0010000100000000;
				setSwitch(5);
				setSwitch(1);
				setSwitch(3);
			}
			else
			{
				if(dso.timeDiv < timeDiv_2_5us)
				{
					setupADCDual(ACQ_DUAL_12);
					setupDMADual(ACQ_DUAL_12);
					setupADCDual(ACQ_DUAL_34);
					setupDMADual(ACQ_DUAL_34);
					dso.dual1 = 1;
					dso.dual2 = 1;
					dso.quad = 0;
					/* Configure analog switches */
					AS5 |= 0b0110001100000000;
					AS5 &= 0b0110001100000000;
					setSwitch(5);
					setSwitch(1);
					setSwitch(3);
				}
				else
				{
					// unavailable
				}
			}
			break;
		}
		case 6:
		{ // channels 2 and 3
			if(dso.timeDiv < timeDiv_10us)
			{
				initADC(&dso.ch2);
				initADC(&dso.ch3);
				setupADCSingle(&dso.ch2);
				setupDMA(&dso.ch2);
				setupADCSingle(&dso.ch3);
				setupDMA(&dso.ch3);
				dso.dual1 = 0;
				dso.dual2 = 0;
				dso.quad = 0;
				/* Configure analog switches */
				AS5 |= 0b0011000000000000;
				AS5 &= 0b0011000000000000;
				setSwitch(5);
				setSwitch(2);
				setSwitch(3);
			}
			else
			{
				//unavailable
			}
			break;
		}
		case 7:
		{ // channels 1, 2 and 3
			if(dso.timeDiv < timeDiv_10us)
			{
				setupADCParallel(ACQ_PARALLEL_12);
				setupDMAParallel(ACQ_PARALLEL_12);
				initADC(&dso.ch3);
				setupADCSingle(&dso.ch3);
				setupDMA(&dso.ch3);
				dso.dual1 = 0;
				dso.dual2 = 0;
				dso.quad = 0;
				/* Configure analog switches */
				AS5 |= 0b0011000100000000;
				AS5 &= 0b0011000100000000;
				setSwitch(5);
				setSwitch(1);
				setSwitch(2);
				setSwitch(3);
			}
			else
			{
				//unavailable
			}
			break;
		}
		case 8:
		{ // channel 4 only
			if(dso.timeDiv < timeDiv_10us)
			{
				initADC(&dso.ch4);
				setupADCSingle(&dso.ch4);
				setupDMA(&dso.ch4);
				dso.dual1 = 0;
				dso.dual2 = 0;
				dso.quad = 0;
				/* Configure analog switches */
				AS5 |= 0b1000000000000000;
				AS5 &= 0b1000000000000000;
				setSwitch(5);
				setSwitch(4);
			}
			else
			{
				//unavailable
			}
			break;
		}
		case 9:
		{ // ch1 and ch4
			if(dso.timeDiv < timeDiv_10us)
			{
				initADC(&dso.ch1);
				initADC(&dso.ch4);
				setupADCSingle(&dso.ch1);
				setupDMA(&dso.ch1);
				setupADCSingle(&dso.ch4);
				setupDMA(&dso.ch4);
				dso.dual1 = 0;
				dso.dual2 = 0;
				dso.quad = 0;
				/* Configure analog switches */
				AS5 |= 0b1000000100000000;
				AS5 &= 0b1000000100000000;
				setSwitch(5);
				setSwitch(1);
				setSwitch(4);
			}
			else
			{
				//unavailable
			}
			break;
		}
		case 10:
		{ // ch2 and ch4
			if(dso.timeDiv < timeDiv_10us)
			{
				initADC(&dso.ch2);
				initADC(&dso.ch4);
				setupADCSingle(&dso.ch2);
				setupDMA(&dso.ch2);
				setupADCSingle(&dso.ch4);
				setupDMA(&dso.ch4);
				dso.dual1 = 0;
				dso.dual2 = 0;
				dso.quad = 0;
				/* Configure analog switches */
				AS5 |= 0b1001000000000000;
				AS5 &= 0b1001000000000000;
				setSwitch(5);
				setSwitch(2);
				setSwitch(4);
			}
			else
			{
				//unavailable
			}
			break;
		}
		case 11:
		{ //channels 1,2 and 4
			if(dso.timeDiv < timeDiv_10us)
			{
				setupADCParallel(ACQ_PARALLEL_12);
				setupDMAParallel(ACQ_PARALLEL_12);
				initADC(&dso.ch4);
				setupADCSingle(&dso.ch4);
				setupDMA(&dso.ch4);
				dso.dual1 = 0;
				dso.dual2 = 0;
				dso.quad = 0;
				/* Configure analog switches */
				AS5 |= 0b1001000100000000;
				AS5 &= 0b1001000100000000;
				setSwitch(5);
				setSwitch(1);
				setSwitch(2);
				setSwitch(4);
			}
			else
			{
				//unavailable
			}
			break;
		}
		case 12:
		{ // channels 3 and 4
			if(dso.timeDiv < timeDiv_10us)
			{
				setupADCParallel(ACQ_PARALLEL_34);
				setupDMAParallel(ACQ_PARALLEL_34);
				dso.dual1 = 0;
				dso.dual2 = 0;
				dso.quad = 0;
				/* Configure analog switches */
				AS5 |= 0b1010000000000000;
				AS5 &= 0b1010000000000000;
				setSwitch(5);
				setSwitch(3);
				setSwitch(4);
			}
			else
			{
				//unavailable
			}
			break;
		}
		case 13:
		{ // channels 1, 3 and 4
			if(dso.timeDiv < timeDiv_10us)
			{
				setupADCParallel(ACQ_PARALLEL_34);
				setupDMAParallel(ACQ_PARALLEL_34);
				initADC(&dso.ch1);
				setupADCSingle(&dso.ch1);
				setupDMA(&dso.ch1);
				dso.dual1 = 0;
				dso.dual2 = 0;
				dso.quad = 0;
				/* Configure analog switches */
				AS5 |= 0b1010000100000000;
				AS5 &= 0b1010000100000000;
				setSwitch(5);
				setSwitch(1);
				setSwitch(3);
				setSwitch(4);
			}
			else
			{
				//unavailable
			}
			break;
		}
		case 14:
		{ // channels 2, 3 and 4
			if(dso.timeDiv < timeDiv_10us)
			{
				setupADCParallel(ACQ_PARALLEL_34);
				setupDMAParallel(ACQ_PARALLEL_34);
				initADC(&dso.ch2);
				setupADCSingle(&dso.ch2);
				setupDMA(&dso.ch2);
				dso.dual1 = 0;
				dso.dual2 = 0;
				dso.quad = 0;
				/* Configure analog switches */
				AS5 |= 0b1011000000000000;
				AS5 &= 0b1011000000000000;
				setSwitch(5);
				setSwitch(2);
				setSwitch(3);
				setSwitch(4);
			}
			else
			{
				//unavailable
			}
			break;
		}
		case 15:
		{ // all channels
			if(dso.timeDiv < timeDiv_10us)
			{
				setupADCParallel(ACQ_PARALLEL_34);
				setupDMAParallel(ACQ_PARALLEL_34);
				setupADCParallel(ACQ_PARALLEL_12);
				setupDMAParallel(ACQ_PARALLEL_12);
				dso.dual1 = 0;
				dso.dual2 = 0;
				dso.quad = 0;
				//	     ********
				/* Configure analog switches */
				AS5 |= 0b1011000100000000;
				AS5 &= 0b1011000100000000;
				setSwitch(5);
				setSwitch(1);
				setSwitch(2);
				setSwitch(3);
				setSwitch(4);

			}
			else
			{
				//unavailable
			}
			break;
		}
		default:
		{
			break;
		}
	}
}

/**
 * @brief Setup analog swithces
 */
void setupSwitches(void)
{
	SPI1->CR1 = 0x002C; // disabled, baud-rate fpclk/32, master
	SPI1->CR2 = 0x0F04; // 16-bit, SS output disabled, RXNE at 1/2 full RX FIFO

	//		********
	AS1 = 0b0010010100000000;
	AS2 = 0b0010010100000000;
	AS3 = 0b0010010100000000;
	AS4 = 0b0010010100000000;

	setSwitch(1);
	setSwitch(2);
	setSwitch(3);
	setSwitch(4);
	setSwitch(5);
}

/**
 * Setup one of the analog swithces chip
 * @param num Number of the chip. Each of the four channels has it's own dedicated analog switch chip (1..4) with the
 * fifth analog chip being the common one for all of the channels
 */
void setSwitch(uint32_t num)
{
	switch (num)
	{
		case 1:
			SPI1->DR = AS1; // send open half
			CS_AS1_LOW;
			 // enabled, baud-rate f-pclk/64, master
			SPI1->CR1 = SPI_CR1_MSTR | SPI_CR1_BR_0 | SPI_CR1_BR_2 | SPI_CR1_SPE;
			while (!(SPI1->SR & SPI_SR_TXE)); // wait for transmission to complete
			while (!(SPI1->SR & SPI_SR_RXNE)); // wait for receiving to complete
			CS_AS1_HIGH;
			 //SPI1 disabled
			SPI1->CR1 &= ~SPI_CR1_SPE;
			break;
		case 2:
			SPI1->DR = AS2; // send open half
			CS_AS2_LOW;
			 // enabled, baud-rate f-pclk/64, master
			SPI1->CR1 = SPI_CR1_MSTR | SPI_CR1_BR_0 | SPI_CR1_BR_2 | SPI_CR1_SPE;
			while (!(SPI1->SR & SPI_SR_TXE)); // wait for transmission to complete
			while (!(SPI1->SR & SPI_SR_RXNE)); // wait for receiving to complete
			CS_AS2_HIGH;
			 //SPI1 disabled
			SPI1->CR1 &= ~SPI_CR1_SPE;
			break;
		case 3:
			SPI1->DR = AS3; // send open half
			CS_AS3_LOW;
			 // enabled, baud-rate f-pclk/64, master
			SPI1->CR1 = SPI_CR1_MSTR | SPI_CR1_BR_0 | SPI_CR1_BR_2 | SPI_CR1_SPE;
			while (!(SPI1->SR & SPI_SR_TXE)); // wait for transmission to complete
			while (!(SPI1->SR & SPI_SR_RXNE)); // wait for receiving to complete
			CS_AS3_HIGH;
			 //SPI1 disabled
			SPI1->CR1 &= ~SPI_CR1_SPE;
			break;
		case 4:
			SPI1->DR = AS4; // send open half
			CS_AS4_LOW;
			 // enabled, baud-rate f-pclk/64, master
			SPI1->CR1 = SPI_CR1_MSTR | SPI_CR1_BR_0 | SPI_CR1_BR_2 | SPI_CR1_SPE;
			while (!(SPI1->SR & SPI_SR_TXE)); // wait for transmission to complete
			while (!(SPI1->SR & SPI_SR_RXNE)); // wait for receiving to complete
			CS_AS4_HIGH;
			 //SPI1 disabled
			SPI1->CR1 &= ~SPI_CR1_SPE;
			break;
		case 5:
			SPI1->DR = AS5; // send open half
			CS_AS5_LOW;
			 // enabled, baud-rate f-pclk/64, master
			SPI1->CR1 = SPI_CR1_MSTR | SPI_CR1_BR_0 | SPI_CR1_BR_2 | SPI_CR1_SPE;
			while (!(SPI1->SR & SPI_SR_TXE)); // wait for transmission to complete
			while (!(SPI1->SR & SPI_SR_RXNE)); // wait for receiving to complete
			CS_AS5_HIGH;
			 //SPI1 disabled
			SPI1->CR1 &= ~SPI_CR1_SPE;
			break;
		default:
			break;
	}
}

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void)
{
	buffer1 = (uint32_t *)0x20000000;
	buffer2 = buffer1 + BUFF_SIZE;
	buffer3 = buffer2 + BUFF_SIZE;
	buffer4 = buffer3 + BUFF_SIZE;

	sendScreenBuffer1 = sendScreenBuffer;
	sendScreenBuffer2 = &sendScreenBuffer[280];
	sendScreenBuffer3 = &sendScreenBuffer[560];
	sendScreenBuffer4 = &sendScreenBuffer[840];

	SystemClock_Config();

	setupClocks();

	setupGPIO();

	setupTouch();

	setupSwitches();

	setupUART();

	enableInterrupts();

	initDSO();

	ADC_Clocks();

	initADC(&dso.ch1);
	initADC(&dso.ch2);
	initADC(&dso.ch3);
	initADC(&dso.ch4);

	LCD_Init();

	while (1)
	{
		pollForTouch();
		LCD_DrawReticle();
		aquireData();
		delay_ms(20);
		clearWaveform();
		serial();
		dso.triggerPoint = 50;
	}
}
