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

#define CH1_XTextStart 	0
#define CH2_XTextStart 	80
#define CH3_XTextStart 	160
#define CH4_XTextStart 	240
#define CH1_YTextStart 	228
#define CH2_YTextStart 	228
#define CH3_YTextStart 	228
#define CH4_YTextStart 	228
#define CH_StrPixWidth 	80
#define X_PIXELS 		280
#define Y_PIXELS 		216

#define RISING_EDGE 	1
#define FALLING_EDGE 	0
#define COUPLING_GND 	0
#define COUPLING_DC 	1
#define COUPLING_AC 	2

#define CS_AS1_LOW 		GPIOB->BRR = 0x00001000;  // PB12
#define CS_AS1_HIGH 	GPIOB->BSRR = 0x00001000; // PB12
#define CS_AS2_LOW 		GPIOB->BRR = 0x00000800;  // PB11
#define CS_AS2_HIGH 	GPIOB->BSRR = 0x00000800; // PB11
#define CS_AS3_LOW 		GPIOB->BRR = 0x00000004;  // PB2
#define CS_AS3_HIGH 	GPIOB->BSRR = 0x00000004; // PB2
#define CS_AS4_LOW 		GPIOB->BRR = 0x00008000;  // PB15
#define CS_AS4_HIGH 	GPIOB->BSRR = 0x00008000; // PB15
#define CS_AS5_LOW 		GPIOB->BRR = 0x00004000;  // PB13
#define CS_AS5_HIGH 	GPIOB->BSRR = 0x00004000; // PB13

#define BUFF_SIZE 		2048

extern dso_config_t dso;						/**< Configuration structure for the DSO */

extern uint8_t sendScreenBuffer[X_PIXELS*4];	/**< Allocate RAM for the screenbuffers */
extern uint8_t *sendScreenBuffer1;				/**< Screen buffer for channel 1 */
extern uint8_t *sendScreenBuffer2;				/**< Screen buffer for channel 2 */
extern uint8_t *sendScreenBuffer3;				/**< Screen buffer for channel 3 */
extern uint8_t *sendScreenBuffer4;				/**< Screen buffer for channel 4 */
extern uint32_t buffer1[BUFF_SIZE*4];			/**< Buffer for channel 1. (allocates memory for all four channels) */
extern uint32_t *buffer2;						/**< Acquisition buffer for channel 2 */
extern uint32_t *buffer3;						/**< Acquisition buffer for channel 2 */
extern uint32_t *buffer4;						/**< Acquisition buffer for channel 2 */
extern uint16_t AS1;							/**< Variable for Analog switch 1. Each bit of the upper byte acts on
													one switch. The lower byte is needed to cycle out previous config */
extern uint16_t AS2;							/**< Variable for Analog switch 2 */
extern uint16_t AS3;							/**< Variable for Analog switch 3 */
extern uint16_t AS4;							/**< Variable for Analog switch 4 */
extern uint16_t AS5;							/**< Variable for Analog switch 5 */


extern uint32_t channel_state;					/**< Indicates which of the four channels is enabled */

extern button_t currentButton;					/**< The current button that is pressed */
extern button_t prevButton;						/**< The previous button */

void delay_ms(int ms);
void assertError(uint8_t errno);
void toggle(uint32_t *var);
void timeDivChange(void);
void channelChange(void);
void setSwitch(uint32_t num);

#endif
