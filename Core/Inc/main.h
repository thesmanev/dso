#ifndef MAIN_H
#define MAIN_H



#include "stm32f3xx.h"

typedef enum{
	timeDiv_1ms,
	timeDiv_500us,
	timeDiv_250us,
	timeDiv_100us,
	timeDiv_50us,
	timeDiv_25us,
	timeDiv_10us,
	timeDiv_5us,
	timeDiv_2_5us,
	timeDiv_1us
}tmdv;

typedef enum{
	v_100mDiv,
	v_200mDiv,
	v_500mDiv,
	v_1Div,
	v_2Div,
	v_5Div,
	v_10Div
}voltDiv;

typedef struct{
	uint32_t id;
	uint32_t enabled;
	uint32_t selected;
	voltDiv voltsDiv;
	uint32_t vertical;
	uint32_t coupling;
	uint32_t buffSize;
} channel;

typedef struct{
	tmdv timeDiv;
	uint32_t horizontal;
	uint32_t triggerPoint;
	uint32_t triggerEdge;
	uint32_t triggerSource;
	uint32_t triggerValue;
	uint32_t quad;
	uint32_t dual1;
	uint32_t dual2;
	channel ch1;
	channel ch2;
	channel ch3;
	channel ch4;
} dso;

enum button{
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
}currentButton, prevButton ;


//enum button currentButton;
//enum button prevButton;

//typedef struct{
//	uint32_t ch1Btn;
//	uint32_t ch2Btn;
//	uint32_t ch3Btn;
//	uint32_t ch4Btn;
//	uint32_t timeDivUpBtn;
//	uint32_t timeDivDownBtn;
//	uint32_t trgiUpBtn;
//	uint32_t trigDwnBtn;
//} buttons;

//#include "semihosting/semihosting.h"
//#include "struct.h"
#include "disp.h"
#include "touch.h"
#include "gpio.h"
#include "dma.h"
#include "adc.h"
#include "font.h"
#include "uart.h"
#include "printf.h"
#include "acquisition.h"
#include "trigger.h"
//#include "struct.h"

//#define DEBUG_ON

#define CH1_XTextStart 0
#define CH2_XTextStart 80
#define CH3_XTextStart 160
#define CH4_XTextStart 240
#define CH1_YTextStart 228
#define CH2_YTextStart 228
#define CH3_YTextStart 228
#define CH4_YTextStart 228
#define CH_StrPixWidth 80
#define X_PIXELS 280
#define Y_PIXELS 216

#define RISING_EDGE 1
#define FALLING_EDGE 0
#define COUPLING_GND 0
#define COUPLING_DC 1
#define COUPLING_AC 2

#define CS_AS1_LOW GPIOB->BRR = 0x00001000; // PB12
#define CS_AS1_HIGH GPIOB->BSRR = 0x00001000; // PB12
#define CS_AS2_LOW GPIOB->BRR = 0x00000800; // PB11
#define CS_AS2_HIGH GPIOB->BSRR = 0x00000800; // PB11
#define CS_AS3_LOW GPIOB->BRR = 0x00000004; // PB2
#define CS_AS3_HIGH GPIOB->BSRR = 0x00000004; // PB2
#define CS_AS4_LOW GPIOB->BRR = 0x00008000; // PB15
#define CS_AS4_HIGH GPIOB->BSRR = 0x00008000; // PB15
#define CS_AS5_LOW GPIOB->BRR = 0x00004000; // PB13
#define CS_AS5_HIGH GPIOB->BSRR = 0x00004000; // PB13




#define BUFF_SIZE 2048
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
extern dso dso1;
extern uint32_t x;
//extern buttons btns;
extern char ch1[10];
extern char ch2[10];
extern char ch3[10];
extern char ch4[10];
extern char tDiv[];
extern char trig[];

void delay_ms(int ms);
void assertError(uint32_t errno);
void toggle(uint32_t *var);
void timeDivChange(void);
void channelChange(void);
void setSwitch(uint32_t num);

#endif
