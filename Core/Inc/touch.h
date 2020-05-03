#ifndef TOUCH_H
#define TOUCH_H

#define SPIx SPI4

// setup for SPI4
#define CST_LOW GPIOE->BRR = 0x00000008; // PE3
#define CST_HIGH GPIOE->BSRR = 0x00000008; // PE3

#define touchReadXcmd 0xD000
#define touchReadYcmd 0x9000

typedef enum{
	none_pressed,
	inc_pressed,
	dec_pressed,
	trigSource_pressed,
	trigEdge_pressed,
	trigValue_pressed,
	timeDiv_pressed,
	ch1VDiv_pressed,
	ch1Cpl_pressed,
	ch2VDiv_pressed,
	ch2Cpl_pressed,
	ch3VDiv_pressed,
	ch3Cpl_pressed,
	ch4VDiv_pressed,
	ch4Cpl_pressed,
	ch1_pressed,
	ch2_pressed,
	ch3_pressed,
	ch4_pressed,
}button;

void setupTouch(void);
uint32_t touch_readY(void);
uint32_t touch_readX(void);
uint32_t get_X_touch(void);
uint32_t get_Y_touch(void);
void pollForTouch(void);
void deselectChannel(void);

#endif
