#include "main.h"

uint32_t channel_state = 0;
dso dso1;

volatile int flags[3];
volatile int interrupts[5];
volatile int flags2[3];
volatile int overrun;
volatile int inc;
uint8_t sendScreenBuffer[X_PIXELS*4];
uint8_t *sendScreenBuffer1;
uint8_t *sendScreenBuffer2;
uint8_t *sendScreenBuffer3;
uint8_t *sendScreenBuffer4;
uint8_t setup[11];
uint32_t buffer1[BUFF_SIZE*4];
uint32_t *buffer2;
uint32_t *buffer3;
uint32_t *buffer4;
uint16_t AS1, AS2, AS3, AS4, AS5;

button currentButton;
button prevButton;

void Error_Handler(void);

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the CPU, AHB and APB busses clocks
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI|RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  RCC_OscInitStruct.PLL.PREDIV = RCC_PREDIV_DIV1;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART3|RCC_PERIPHCLK_RTC;
  PeriphClkInit.Usart3ClockSelection = RCC_USART3CLKSOURCE_PCLK1;
  PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
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
	RCC->APB2ENR |= RCC_APB2ENR_SPI4EN | RCC_APB2ENR_USART1EN
			| RCC_APB2ENR_SPI1EN;

	//for display
	RCC->AHBENR |= RCC_AHBENR_GPIOGEN | RCC_AHBENR_GPIODEN;

	//for adc
	RCC->AHBENR |= RCC_AHBENR_GPIOBEN | RCC_AHBENR_ADC12EN | RCC_AHBENR_DMA1EN
			| RCC_AHBENR_GPIOAEN | RCC_AHBENR_DMA2EN | RCC_AHBENR_ADC34EN
			| RCC_AHBENR_GPIOEEN;
}

/**
 * @brief Setup priorities and enable DMA interrupts
 */
void enableInterrupts(void)
{
	HAL_NVIC_SetPriority(DMA1_Channel1_IRQn, 6, 0);
	HAL_NVIC_SetPriority(DMA2_Channel1_IRQn, 6, 0);
	HAL_NVIC_SetPriority(DMA2_Channel2_IRQn, 6, 0);
	HAL_NVIC_SetPriority(DMA2_Channel5_IRQn, 6, 0);
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
	RCC->APB1ENR |= 0x1 << 4;
	TIM6->PSC = 72 - 1;
	TIM6->ARR = 1000 - 1;
	TIM6->CNT = 0;
	TIM6->EGR |= 0x1;
	TIM6->SR &= ~(0x1);
	TIM6->CR1 |= 0x1;

	while (ms > 0)
	{
		while ((TIM6->SR & 0x1) == 0)
			;
		TIM6->SR &= ~(0x1);
		ms--;
	}
	TIM6->CR1 &= ~(0x1);
	RCC->APB1ENR &= ~(0x1 << 4);
}

/**
 * @brief Error handling function. Prints out the error on the LCD display
 * @param errno Error number
 */
void assertError(uint32_t errno)
{
	char str[7] = { 'e', 'r', 'r', 'o', 'r', ' ', ' ' };
	str[6] = ((char) errno) + 48;
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
	while (1);
	/* USER CODE END Error_Handler_Debug */
}

/**
 * @brief Structure initialization function
 */
void initStructs(void)
{
	dso1.dual1 = 0; // off
	dso1.dual2 = 0; // off
	dso1.horizontal = 0; // center
	dso1.timeDiv = timeDiv_50us; // 50 us
	dso1.quad = 0; //off
	dso1.triggerEdge = 1; // rising
	dso1.triggerSource = 1; // ch1
	dso1.triggerPoint = 50; // first sample (auto-triggering)
	dso1.triggerValue = 2048; // 1.65v

	dso1.ch1.buffSize = BUFF_SIZE;
	dso1.ch1.coupling = 0; //dc
	dso1.ch1.enabled = 0; // off
	dso1.ch1.selected = 0;
	dso1.ch1.id = 1;
	dso1.ch1.vertical = 0; // no offset
	dso1.ch1.voltsDiv = v_1Div; // 1Vdiv

	dso1.ch2.buffSize = BUFF_SIZE;
	dso1.ch2.coupling = 0; //dc
	dso1.ch2.enabled = 0; // off
	dso1.ch2.selected = 0;
	dso1.ch2.id = 2;
	dso1.ch2.vertical = 0; // no offset
	dso1.ch2.voltsDiv = v_1Div; // 1Vdiv

	dso1.ch3.buffSize = BUFF_SIZE;
	dso1.ch3.coupling = 0; //dc
	dso1.ch3.enabled = 0; // off
	dso1.ch3.selected = 0;
	dso1.ch3.id = 3;
	dso1.ch3.vertical = 0; // no offset
	dso1.ch3.voltsDiv = v_1Div; // 1Vdiv

	dso1.ch4.buffSize = BUFF_SIZE;
	dso1.ch4.coupling = 0; //dc
	dso1.ch4.enabled = 0; // off
	dso1.ch4.selected = 0;
	dso1.ch4.id = 4;
	dso1.ch4.vertical = 0; // no offset
	dso1.ch4.voltsDiv = v_1Div; // 1Vdiv
}

/**
 * Toggle a uint32_t variable from 0 to 1 or the other way around.
 * @param var Pointer to the variable whose value is to be toggled
 * @note If the value of the variable is greater than 1, it is considered the same as being 1.
 */
void toggle(uint32_t *var)
{
	if (*var)
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
	channel_state = dso1.ch1.enabled;
	channel_state = channel_state + (dso1.ch2.enabled << 1);
	channel_state = channel_state + (dso1.ch3.enabled << 2);
	channel_state = channel_state + (dso1.ch4.enabled << 3);

	switch (channel_state)
	{
	case 0:
	{ // all channels off
		break;
	}
	case 1:
	{ // channel 1 on
		if (dso1.timeDiv < timeDiv_10us)
		{
			initADC(&dso1.ch1);
			setupADCSingle(&dso1.ch1);
			setupDMA(&dso1.ch1);
			dso1.dual1 = 0;
			dso1.dual2 = 0;
			dso1.quad = 0;
			/* Configure analog switches */
			AS5 |= 0b0000000100000000;
			AS5 &= 0b0000000100000000;
			setSwitch(5);
			setSwitch(1);
		}
		else
		{
			if (dso1.timeDiv < timeDiv_2_5us)
			{
				setupADCDual(0);
				setupDMADual(0);
				dso1.dual1 = 1;
				dso1.dual2 = 0;
				dso1.quad = 0;
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
				dso1.dual1 = 0;
				dso1.dual2 = 0;
				dso1.quad = 1;
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
		if (dso1.timeDiv < timeDiv_10us)
		{
			initADC(&dso1.ch2);
			setupADCSingle(&dso1.ch2);
			setupDMA(&dso1.ch2);
			dso1.dual1 = 0;
			dso1.dual2 = 0;
			dso1.quad = 0;
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
		if (dso1.timeDiv < timeDiv_10us)
		{ //ch1 and ch2 parallel
			setupADCParallel(0);
			setupDMAParallel(0);
			dso1.dual1 = 0;
			dso1.dual2 = 0;
			dso1.quad = 0;
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
		if (dso1.timeDiv < timeDiv_10us)
		{
			initADC(&dso1.ch3);
			setupADCSingle(&dso1.ch3);
			setupDMA(&dso1.ch3);
			dso1.dual1 = 0;
			dso1.dual2 = 0;
			dso1.quad = 0;
			/* Configure analog switches */
			AS5 |= 0b0010000000000000;
			AS5 &= 0b0010000000000000;
			setSwitch(5);
			setSwitch(3);
		}
		else
		{
			if (dso1.timeDiv < timeDiv_2_5us)
			{
				setupADCDual(1);
				setupDMADual(1);
				dso1.dual1 = 0;
				dso1.dual2 = 1;
				dso1.quad = 0;
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
		if (dso1.timeDiv < timeDiv_10us)
		{
			initADC(&dso1.ch1);
			initADC(&dso1.ch3);
			setupADCSingle(&dso1.ch1);
			setupDMA(&dso1.ch1);
			setupADCSingle(&dso1.ch3);
			setupDMA(&dso1.ch3);
			dso1.dual1 = 0;
			dso1.dual2 = 0;
			dso1.quad = 0;
			/* Configure analog switches */
			AS5 |= 0b0010000100000000;
			AS5 &= 0b0010000100000000;
			setSwitch(5);
			setSwitch(1);
			setSwitch(3);
		}
		else
		{
			if (dso1.timeDiv < timeDiv_2_5us)
			{
				setupADCDual(0);
				setupDMADual(0);
				setupADCDual(1);
				setupDMADual(1);
				dso1.dual1 = 1;
				dso1.dual2 = 1;
				dso1.quad = 0;
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
		if (dso1.timeDiv < timeDiv_10us)
		{
			initADC(&dso1.ch2);
			initADC(&dso1.ch3);
			setupADCSingle(&dso1.ch2);
			setupDMA(&dso1.ch2);
			setupADCSingle(&dso1.ch3);
			setupDMA(&dso1.ch3);
			dso1.dual1 = 0;
			dso1.dual2 = 0;
			dso1.quad = 0;
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
		if (dso1.timeDiv < timeDiv_10us)
		{
			setupADCParallel(0);
			setupDMAParallel(0);
			initADC(&dso1.ch3);
			setupADCSingle(&dso1.ch3);
			setupDMA(&dso1.ch3);
			dso1.dual1 = 0;
			dso1.dual2 = 0;
			dso1.quad = 0;
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
		if (dso1.timeDiv < timeDiv_10us)
		{
			initADC(&dso1.ch4);
			setupADCSingle(&dso1.ch4);
			setupDMA(&dso1.ch4);
			dso1.dual1 = 0;
			dso1.dual2 = 0;
			dso1.quad = 0;
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
		if (dso1.timeDiv < timeDiv_10us)
		{
			initADC(&dso1.ch1);
			initADC(&dso1.ch4);
			setupADCSingle(&dso1.ch1);
			setupDMA(&dso1.ch1);
			setupADCSingle(&dso1.ch4);
			setupDMA(&dso1.ch4);
			dso1.dual1 = 0;
			dso1.dual2 = 0;
			dso1.quad = 0;
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
		if (dso1.timeDiv < timeDiv_10us)
		{
			initADC(&dso1.ch2);
			initADC(&dso1.ch4);
			setupADCSingle(&dso1.ch2);
			setupDMA(&dso1.ch2);
			setupADCSingle(&dso1.ch4);
			setupDMA(&dso1.ch4);
			dso1.dual1 = 0;
			dso1.dual2 = 0;
			dso1.quad = 0;
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
		if (dso1.timeDiv < timeDiv_10us)
		{
			setupADCParallel(0);
			setupDMAParallel(0);
			initADC(&dso1.ch4);
			setupADCSingle(&dso1.ch4);
			setupDMA(&dso1.ch4);
			dso1.dual1 = 0;
			dso1.dual2 = 0;
			dso1.quad = 0;
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
		if (dso1.timeDiv < timeDiv_10us)
		{
			setupADCParallel(1);
			setupDMAParallel(1);
			dso1.dual1 = 0;
			dso1.dual2 = 0;
			dso1.quad = 0;
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
		if (dso1.timeDiv < timeDiv_10us)
		{
			setupADCParallel(1);
			setupDMAParallel(1);
			initADC(&dso1.ch1);
			setupADCSingle(&dso1.ch1);
			setupDMA(&dso1.ch1);
			dso1.dual1 = 0;
			dso1.dual2 = 0;
			dso1.quad = 0;
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
		if (dso1.timeDiv < timeDiv_10us)
		{
			setupADCParallel(1);
			setupDMAParallel(1);
			initADC(&dso1.ch2);
			setupADCSingle(&dso1.ch2);
			setupDMA(&dso1.ch2);
			dso1.dual1 = 0;
			dso1.dual2 = 0;
			dso1.quad = 0;
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
		if (dso1.timeDiv < timeDiv_10us)
		{
			setupADCParallel(1);
			setupDMAParallel(1);
			setupADCParallel(0);
			setupDMAParallel(0);
			dso1.dual1 = 0;
			dso1.dual2 = 0;
			dso1.quad = 0;
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
		SPI1->CR1 = 0x006C; // enabled, baud-rate f-pclk/64, master
		while (!(SPI1->SR & SPI_SR_TXE)); // wait for receiving to complete
		while (!(SPI1->SR & SPI_SR_RXNE)); // wait for receiving to complete
		CS_AS1_HIGH;
		SPI1->CR1 = 0x002C; // disabled, baud-rate fpclk/64, master
		break;
	case 2:
		SPI1->DR = AS2; // send open half
		CS_AS2_LOW;
		SPI1->CR1 = 0x006C; // enabled, baud-rate f-pclk/64, master
		while (!(SPI1->SR & SPI_SR_TXE)); // wait for receiving to complete
		while (!(SPI1->SR & SPI_SR_RXNE)); // wait for receiving to complete
		CS_AS2_HIGH;
		SPI1->CR1 = 0x002C; // disabled, baud-rate fpclk/64, master
		break;
	case 3:
		SPI1->DR = AS3; // send open half
		CS_AS3_LOW;
		SPI1->CR1 = 0x006C; // enabled, baud-rate f-pclk/64, master
		while (!(SPI1->SR & SPI_SR_TXE)); // wait for receiving to complete
		while (!(SPI1->SR & SPI_SR_RXNE)); // wait for receiving to complete
		CS_AS3_HIGH;
		SPI1->CR1 = 0x002C; // disabled, baud-rate fpclk/64, master
		break;
	case 4:
		SPI1->DR = AS4; // send open half
		CS_AS4_LOW;
		SPI1->CR1 = 0x006C; // enabled, baud-rate f-pclk/64, master
		while (!(SPI1->SR & SPI_SR_TXE)); // wait for receiving to complete
		while (!(SPI1->SR & SPI_SR_RXNE)); // wait for receiving to complete
		CS_AS4_HIGH;
		SPI1->CR1 = 0x002C; // disabled, baud-rate fpclk/64, master
		break;
	case 5:
		SPI1->DR = AS5; // send open half
		CS_AS5_LOW;
		SPI1->CR1 = 0x006C; // enabled, baud-rate f-pclk/64, master
		while (!(SPI1->SR & SPI_SR_TXE)); // wait for receiving to complete
		while (!(SPI1->SR & SPI_SR_RXNE)); // wait for receiving to complete
		CS_AS5_HIGH;
		SPI1->CR1 = 0x002C; // disabled, baud-rate fpclk/64, master
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
	currentButton = none_pressed;
	prevButton = none_pressed;

	buffer2 = &buffer1[2048];
	buffer3 = &buffer1[4096];
	buffer4 = &buffer1[6144];

	sendScreenBuffer1 = sendScreenBuffer;
	sendScreenBuffer2 = &sendScreenBuffer[280];
	sendScreenBuffer3 = &sendScreenBuffer[560];
	sendScreenBuffer4 = &sendScreenBuffer[840];

	AS1 = 0;
	AS2 = 0;
	AS3 = 0;
	AS4 = 0;
	AS5 = 0;

	setupClocks();

	setupGPIO();

	setupTouch();

	setupSwitches();

	setupUART();

	enableInterrupts();

	initStructs();

	ADC_Clocks();

	initADC(&dso1.ch1);
	initADC(&dso1.ch2);
	initADC(&dso1.ch3);
	initADC(&dso1.ch4);

	LCD_Init();
	//		********
	AS1 = 0b0010010100000000;
	AS2 = 0b0010010100000000;
	AS3 = 0b0010010100000000;
	AS4 = 0b0010010100000000;

    while(1)
    {
    	pollForTouch();
    	LCD_DrawReticle();
    	aquireData(0);
    	delay_ms(20);
    	clearWaveform();
    	serial();
    	dso1.triggerPoint = 50;
    }
}
