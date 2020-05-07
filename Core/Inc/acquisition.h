/*
 * acquisition.h
 *
 *  Created on: Apr 26, 2020
 *      Author: Stanimir Manev
 */

#ifndef INC_ACQUISITION_H_
#define INC_ACQUISITION_H_

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
}tmdv_t;

typedef enum{
	v_100mDiv,
	v_200mDiv,
	v_500mDiv,
	v_1Div,
	v_2Div,
	v_5Div,
	v_10Div
}voltDiv_t;

typedef struct{
	uint32_t id;
	uint32_t enabled;
	uint32_t selected;
	voltDiv_t voltsDiv;
	uint32_t vertical;
	uint32_t coupling;
	uint32_t buffSize;
} channel_t;

typedef struct{
	tmdv_t timeDiv;
	uint32_t horizontal;
	uint32_t triggerPoint;
	uint32_t triggerEdge;
	uint32_t triggerSource;
	uint32_t triggerValue;
	uint32_t quad;
	uint32_t dual1;
	uint32_t dual2;
	channel_t ch1;
	channel_t ch2;
	channel_t ch3;
	channel_t ch4;
} dso_config_t;

void aquireData(uint32_t mode);

#endif /* INC_ACQUISITION_H_ */
