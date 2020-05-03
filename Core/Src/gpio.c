#include "gpio.h"

void setupGPIO(void){
	//setup for display
	DATA_PORT->MODER = 0x55555555;// set pins as output
	CONTROL_PORT->MODER = 0x00000155;// set pins as output, PD15 used for touch IRQ

//	DATA_PORT->OTYPER = 0; // set output as push pull
//	CONTROL_PORT->OTYPER = 0;

	DATA_PORT->OSPEEDR = 0xFFFFFFFF; // set high-speed
	CONTROL_PORT->OSPEEDR = 0x000003FF;

//	DATA_PORT->PUPDR = 0; // set no-pull
	CONTROL_PORT->PUPDR = 0x40000000; // pull-up on PD15

	DATA_PORT->ODR = 0; // set all data pins to 0;
	
	//setup for touch

	// setup for SPI1
	//{

//	GPIOA->AFR[0] |= 0x55550000; // AF5 for PA7, PA6, PA5, PA4
//
//	GPIOA->MODER |= 0xC000AA00; // alternate function on PA7, PA6, PA5, PA4
//	GPIOA->MODER &= 0x7FFFFFFF; // PA15 output
//
////	GPIOA->OTYPER |= 0;
//
//	GPIOA->OSPEEDR |= 0xC000CF00;
//
//	GPIOA->PUPDR &= 0x3FFFFFFF; // no pull on PA15

	//}


	// setup for SPI4
	//{

	GPIOE->AFR[0] |= 0x05500500; // AF5 for PE6(MOSI), PE5(MISO), PE2(SCK)
								// CS -> PE3

	GPIOE->MODER |= 0x00002860; // alternate function on PE6, PE5, PE2, PE3 as output
	GPIOE->OSPEEDR |= 0x000030F0;//

	//GPIOE->PUPDR = 0;

	//}


	//CST_HIGH;
	//delay_ms(100);
	//CST_LOW;
	//delay_ms(100);
	//CST_HIGH;
	
	
	// setup for ADCs
	
	GPIOB->MODER |= (1 << 28) | (1 << 14) | 1; // setup led pins as output
	GPIOB->MODER |= 12; // setup PB1 as analog
	GPIOB->PUPDR &= ~(12);// setup no pull on PB1

	GPIOA->MODER |= 3 | (3 << 8);// setup PA0 and PA4 as analog
	GPIOA->PUPDR &= ~(3);// setup no pull-up or pull-down for PA0
	GPIOA->PUPDR &= ~(3 << 8);// setup no pull-up or pull-down for PA4

	GPIOE->MODER |= (3 << 28); // setup PE14 as analog
	GPIOE->PUPDR &= ~(3 << 28); // setup no pull on PE14


	//setup for UART
	GPIOA->AFR[1] |= 0x00000770; // AF7 (UART) for PA9(TX) and PA10(RX)
	GPIOA->MODER |= 0x00280000; // AF mode for PA9 and PA10
	GPIOA->OSPEEDR |= 0x000C0000; // high speed output for PA9 and PA10


	//lock configs

//	GPIOA->LCKR = 0x0001FFFF;
//	GPIOA->LCKR = 0x0000FFFF;
//	GPIOA->LCKR = 0x0001FFFF;
////	if(!(GPIOA->LCKR & 0x00010000)){
////		assertError();
////	}
//
//	GPIOB->LCKR = 0x0001FFFF;
//	GPIOB->LCKR = 0x0000FFFF;
//	GPIOB->LCKR = 0x0001FFFF;
////	if(!(GPIOB->LCKR & 0x00010000)){
////		assertError();
////	}
//
//	GPIOD->LCKR = 0x0001FFFF;
//	GPIOD->LCKR = 0x0000FFFF;
//	GPIOD->LCKR = 0x0001FFFF;
////	if(!(GPIOD->LCKR & 0x00010000)){
////		assertError();
////	}
//
//	GPIOE->LCKR = 0x0001FFFF;
//	GPIOE->LCKR = 0x0000FFFF;
//	GPIOE->LCKR = 0x0001FFFF;
////	if(!(GPIOE->LCKR & 0x00010000)){
////		assertError();
////	}
//
//	GPIOG->LCKR = 0x0001FFFF;
//	GPIOG->LCKR = 0x0000FFFF;
//	GPIOG->LCKR = 0x0001FFFF;
////	if(!(GPIOG->LCKR & 0x00010000)){
////		assertError();
////	}


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
