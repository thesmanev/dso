#ifndef TOUCH_H
#define TOUCH_H

#include "main.h"

void setupTouch(void);
uint32_t touch_readY(void);
uint32_t touch_readX(void);
uint32_t get_X_touch(void);
uint32_t get_Y_touch(void);
void pollForTouch(void);
void deselectChannel(void);


#define SPIx SPI4

// setup for SPI1
//#define CST_LOW GPIOA->BRR = 0x00008000; // PA15
//#define CST_HIGH GPIOA->BSRR = 0x00008000;

// setup for SPI4
#define CST_LOW GPIOE->BRR = 0x00000008; // PE3
#define CST_HIGH GPIOE->BSRR = 0x00000008; // PE3


#define touchReadXcmd 0xD000
#define touchReadYcmd 0x9000

#endif
