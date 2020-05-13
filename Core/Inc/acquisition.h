/*
 * acquisition.h
 *
 *  Created on: Apr 26, 2020
 *      Author: Stanimir Manev
 */

#ifndef INC_ACQUISITION_H_
#define INC_ACQUISITION_H_

#define DUAL_ON				1U
#define DUAL_OFF			0U

#define QUAD_ON				1U
#define QUAD_OFF			0U

#define TRIG_EDGE_RISING	1U
#define TRIG_EDGE_FALLING	0U

#define TRIG_SRC_CH1		1U
#define TRIG_SRC_CH2		2U
#define TRIG_SRC_CH3		3U
#define TRIG_SRC_CH4		4U

#define CH_COUPLING_GND		0U
#define CH_COUPLING_DC		1U
#define CH_COUPLING_AC		2U

#define CH_ENABLED			1U
#define CH_DISABLED			0U

#define CH_ID_NONE			0U
#define CH1_ID				1U
#define CH2_ID				2U
#define CH3_ID				3U
#define CH4_ID				4U

#define ENABLED_CH_NONE		0u
#define ENABLED_CH_1		(1u << 0)
#define ENABLED_CH_2		(1u << 1)
#define ENABLED_CH_3		(1u << 2)
#define ENABLED_CH_4		(1u << 3)

#define ENABLED_CH_12		(ENABLED_CH_1 | ENABLED_CH_2)
#define ENABLED_CH_13		(ENABLED_CH_1 | ENABLED_CH_3)
#define ENABLED_CH_23		(ENABLED_CH_2 | ENABLED_CH_3)
#define ENABLED_CH_123		(ENABLED_CH_1 | ENABLED_CH_2 | ENABLED_CH_3)
#define ENABLED_CH_14		(ENABLED_CH_1 | ENABLED_CH_4)
#define ENABLED_CH_24		(ENABLED_CH_2 | ENABLED_CH_4)
#define ENABLED_CH_124		(ENABLED_CH_1 | ENABLED_CH_2 | ENABLED_CH_4)
#define ENABLED_CH_34		(ENABLED_CH_3 | ENABLED_CH_4)
#define ENABLED_CH_134		(ENABLED_CH_1 | ENABLED_CH_3 | ENABLED_CH_4)
#define ENABLED_CH_234		(ENABLED_CH_2 | ENABLED_CH_3 | ENABLED_CH_4)
#define ENABLED_CH_1234		(ENABLED_CH_1 | ENABLED_CH_2 | ENABLED_CH_3 | ENABLED_CH_4)

#define ACQ_DUAL_12			0U
#define ACQ_DUAL_34			1U

#define ACQ_PARALLEL_12		0U
#define ACQ_PARALLEL_34		1U


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

void aquireData(void);

#endif /* INC_ACQUISITION_H_ */
