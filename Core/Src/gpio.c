#include "main.h"

/**
 * @brief Setup GPIO pins
 */
void setupGPIO(void)
{
	//setup for display
	//set data port pins as output
	DATA_PORT->MODER = GPIO_MODER_MODER0_0 | GPIO_MODER_MODER1_0 | GPIO_MODER_MODER2_0 | GPIO_MODER_MODER3_0 |
			           GPIO_MODER_MODER4_0 | GPIO_MODER_MODER5_0 | GPIO_MODER_MODER6_0 | GPIO_MODER_MODER7_0 |
			           GPIO_MODER_MODER8_0 | GPIO_MODER_MODER9_0 | GPIO_MODER_MODER10_0 | GPIO_MODER_MODER11_0 |
			           GPIO_MODER_MODER12_0 | GPIO_MODER_MODER13_0 | GPIO_MODER_MODER14_0 | GPIO_MODER_MODER15_0;

	// set control port pins as output, PD15 used for touch IRQ
	CONTROL_PORT->MODER = GPIO_MODER_MODER0_0 | GPIO_MODER_MODER1_0 | GPIO_MODER_MODER2_0 | GPIO_MODER_MODER3_0 |
						  GPIO_MODER_MODER4_0;

	// set all data port pins as high-speed
	DATA_PORT->OSPEEDR = 0xFFFFFFFF;

	// set all control pins as high speed
	CONTROL_PORT->OSPEEDR = 0x000003FF;

	// pull-up on PD15
	CONTROL_PORT->PUPDR = GPIO_PUPDR_PUPDR15_0;

	DATA_PORT->ODR = 0; // set all data pins to 0;
	
	//setup for touch
	// setup for SPI4
	GPIOE->AFR[0] |= 0x05500500; // AF5 for PE6(MOSI), PE5(MISO), PE2(SCK)
								// CS -> PE3

	GPIOE->MODER |= 0x00002860; // alternate function on PE6, PE5, PE2, PE3 as output
	GPIOE->OSPEEDR |= 0x000030F0;//
	
	// setup for board specific pins (LD1, LD2, LD3 and Pushbutton 1)
	GPIOB->MODER |= (1u << 28) | (1u << 14) | 1u; // setup led pins as output
	GPIOB->MODER |= 12u; // setup PB1 as analog
	GPIOB->PUPDR &= ~(12u);// setup no pull on PB1

	// setup for ADCs
	GPIOA->MODER |= 3 | (3 << 8);// setup PA0 and PA4 as analog
	GPIOA->PUPDR &= ~(3u);// setup no pull-up or pull-down for PA0
	GPIOA->PUPDR &= ~(3u << 8);// setup no pull-up or pull-down for PA4

	GPIOE->MODER |= (3u << 28); // setup PE14 as analog
	GPIOE->PUPDR &= ~(3u << 28); // setup no pull on PE14

	//setup for UART
	GPIOA->AFR[1] |= 0x00000770; // AF7 (UART) for PA9(TX) and PA10(RX)
	GPIOA->MODER |= 0x00280000; // AF mode for PA9 and PA10
	GPIOA->OSPEEDR |= 0x000C0000; // high speed output for PA9 and PA10

	//setup for SPI1 (Analog switches)
	GPIOA->AFR[0] |= 0x55500000; // AF5 for PA7(MOSI), PA6(MISO), PA5(SCK)
								// CS_MAX395_1 -> PA8
								// CS_MAX395_2 -> PA11
								// CS_MAX395_3 -> PA12
								// CS_MAX395_4 -> PA13
								// CS_MAX395_5 -> PA14
	GPIOA->MODER |= 0x0000A800; // alternate function on PA5, PA6, PA7,
	GPIOA->OSPEEDR |= 0x0000CC00;// High speed for PA5, PA7
	GPIOB->MODER |= 0x45400014; // output on PB1, PB2, PB11, PB12, PB15
	GPIOB->OSPEEDR |= 0xCFC0003C;//high speed for PB1, PB2, PB11, PB12, PB15
}
