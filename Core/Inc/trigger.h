/*
 * trigger.h
 *
 *  Created on: Apr 26, 2020
 *      Author: Stanimir Manev
 */

#ifndef INC_TRIGGER_H_
#define INC_TRIGGER_H_

void displayWaveformTrig(uint32_t chNum) __attribute__((section(".ccmram")));
void findTrigger(void) __attribute__((section(".ccmram")));
void clearWaveform(void) __attribute__((section(".ccmram")));


#endif /* INC_TRIGGER_H_ */
