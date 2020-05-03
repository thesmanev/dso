#ifndef MAIN_H
#define MAIN_H

#include "stm32f3xx.h"
#include "stdint.h"
#include "acquisition.h"
#include "touch.h"
#include "disp.h"
#include "gpio.h"
#include "dma.h"
#include "adc.h"
#include "font.h"
#include "uart.h"
#include "printf.h"
#include "trigger.h"

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

extern volatile int flags[3];
extern volatile int interrupts[5];
extern volatile int flags2[3];
extern volatile int overrun;
extern volatile int inc;
extern uint8_t sendScreenBuffer[X_PIXELS*4];
extern uint8_t *sendScreenBuffer1;
extern uint8_t *sendScreenBuffer2;
extern uint8_t *sendScreenBuffer3;
extern uint8_t *sendScreenBuffer4;
extern uint8_t setup[11];
extern uint32_t buffer1[BUFF_SIZE*4];
extern uint32_t *buffer2;
extern uint32_t *buffer3;
extern uint32_t *buffer4;
extern uint16_t AS1, AS2, AS3, AS4, AS5;

extern dso dso1;
extern uint32_t channel_state;

extern button currentButton;
extern button prevButton;

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
