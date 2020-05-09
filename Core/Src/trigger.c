/*
 * trigger.c
 *
 *  Created on: Apr 26, 2020
 *      Author: Stanimir Manev
 */
#include "main.h"
#include "stdbool.h"

static bool clear_framebuffer1_f = false;
static bool clear_framebuffer2_f = false;
static bool clear_framebuffer3_f = false;
static bool clear_framebuffer4_f = false;

/**
 * @brief Clear waveforms from the screen
 */
void clearWaveform(void)
{
	uint8_t y0 = 0;
	uint8_t y1 = 0;

	if(clear_framebuffer1_f)
	{
		uint32_t j = 8; /* The Oscilloscope grid starts from the 8th pixel */
		for (uint32_t i = 0; i < X_PIXELS - 1; i++)
		{
			y0 = sendScreenBuffer1[i];
			y1 = sendScreenBuffer1[i + 1];
			LCD_DrawLineV(y0, y1, j, ILI9341_BLACK);
			j = j + 1;
			clear_framebuffer1_f = false;
		}
	}

	if(clear_framebuffer2_f)
	{
		uint32_t j = 8;
		for (uint32_t i = 0; i < X_PIXELS - 1; i++)
		{
			y0 = sendScreenBuffer2[i];
			y1 = sendScreenBuffer2[i + 1];
			LCD_DrawLineV(y0, y1, j, ILI9341_BLACK);
			j = j + 1;
			clear_framebuffer2_f = false;
		}
	}

	if(clear_framebuffer3_f)
	{
		uint32_t j = 8;
		for (uint32_t i = 0; i < X_PIXELS - 1; i++)
		{
			y0 = sendScreenBuffer3[i];
			y1 = sendScreenBuffer3[i + 1];
			LCD_DrawLineV(y0, y1, j, ILI9341_BLACK);
			j = j + 1;
			clear_framebuffer3_f = false;
		}
	}

	if(clear_framebuffer4_f)
	{
		uint32_t j = 8;
		for (uint32_t i = 0; i < X_PIXELS - 1; i++)
		{
			y0 = sendScreenBuffer4[i];
			y1 = sendScreenBuffer4[i + 1];
			LCD_DrawLineV(y0, y1, j, ILI9341_BLACK);
			j = j + 1;
			clear_framebuffer4_f = false;
		}
	}
}

/**
 * @brief Process acquired data and display it on the screen
 * @param chNum Channel Number
 * @param mode 1 for displaying, 0 for clearings
 */
void displayWaveformTrig(uint32_t chNum)
{
	uint32_t i;
	uint32_t pixcnt = 8; // comes from waveform screen size
	uint32_t color;

	if(dso.quad)
	{
		uint16_t *buff1 = (uint16_t*) buffer1;
		uint16_t *buff2 = (uint16_t*) buffer3;
		uint32_t y1 = 0, y2 = 0, y3 = 0, y4 = 0, x0 = 8, x1 = 0;
		clear_framebuffer1_f = true;

		color = CH1_COLOR; // ch1 color
		switch (dso.timeDiv)
		{
			case timeDiv_2_5us:
			{
				y1 = 227u - (buff1[dso.triggerPoint] / 19u) + dso.ch1.vertical; // 212 comes from waveform screen size
				y2 = 227u - (buff1[dso.triggerPoint + 1u] / 19u) + dso.ch1.vertical; // 4096 / 212 = 19.316
				y3 = 227u - (buff2[dso.triggerPoint] / 19u) + dso.ch1.vertical; // needs refinement!!!!
				y4 = 227u - (buff2[dso.triggerPoint + 1u] / 19u) + dso.ch1.vertical;
				sendScreenBuffer1[pixcnt - 8] = (uint8_t)y1;
				LCD_DrawLineV(y1, y2, pixcnt++, color);
				sendScreenBuffer1[pixcnt - 8] = (uint8_t)y2;
				LCD_DrawLineV(y2, y3, pixcnt++, color);
				sendScreenBuffer1[pixcnt - 8] = (uint8_t)y3;
				LCD_DrawLineV(y3, y4, pixcnt++, color);
				sendScreenBuffer1[pixcnt - 8] = (uint8_t)y4;
				for (i = dso.triggerPoint + 2; i < (dso.triggerPoint + (X_PIXELS / 2)); i += 2)
				{
					y1 = 227u - (buff1[i] / 19u) + dso.ch1.vertical;
					LCD_DrawLineV(y4, y1, pixcnt++, color);
					sendScreenBuffer1[pixcnt - 8u] = (uint8_t)y1;
					y2 = 227u - (buff1[i + 1u] / 19u) + dso.ch1.vertical;
					y3 = 227u - (buff2[i] / 19u) + dso.ch1.vertical;
					y4 = 227u - (buff2[i + 1u] / 19u) + dso.ch1.vertical;
					LCD_DrawLineV(y1, y2, pixcnt++, color);
					sendScreenBuffer1[pixcnt - 8] = (uint8_t)y2;
					LCD_DrawLineV(y2, y3, pixcnt++, color);
					sendScreenBuffer1[pixcnt - 8] = (uint8_t)y3;
					LCD_DrawLineV(y3, y4, pixcnt++, color);
					sendScreenBuffer1[pixcnt - 8] = (uint8_t)y4;
				}
				break;
			}
			case timeDiv_1us:
			{
				y1 = 227u - (buff1[dso.triggerPoint] / 19u) + dso.ch1.vertical; // 212 comes from waveform screen size
				y2 = 227u - (buff1[dso.triggerPoint + 1u] / 19u) + dso.ch1.vertical; // 4096 / 212 = 19.316
				y3 = 227u - (buff2[dso.triggerPoint] / 19u) + dso.ch1.vertical; // needs refinement!!!!
				y4 = 227u - (buff2[dso.triggerPoint + 1u] / 19u) + dso.ch1.vertical;
				x1 = (pixcnt * 871) / 512;
				LCD_DrawLine(x0, y1, x1, y2, color);
				sendScreenBuffer1[0] = (uint8_t)y1;
				pixcnt++;
				x0 = x1;
				x1 = (pixcnt * 871) / 512;
				LCD_DrawLine(x0, y2, x1, y3, color);
				sendScreenBuffer1[1] = (uint8_t)y2;
				pixcnt++;
				x0 = x1;
				x1 = (pixcnt * 871) / 512;
				LCD_DrawLine(x0, y3, x1, y4, color);
				sendScreenBuffer1[2] = (uint8_t)y3;
				pixcnt++;
				x0 = x1;
				for (i = dso.triggerPoint + 2; i < dso.triggerPoint + 163; i += 2)
				{
					y1 = 227u - (buff1[i] / 19) + dso.ch1.vertical;
					x1 = (pixcnt * 871u) / 512u;
					LCD_DrawLine(x0, y4, x1, y1, color);
					sendScreenBuffer1[pixcnt - 8] = (uint8_t)y4;
					pixcnt++;
					x0 = x1;
					x1 = (pixcnt * 871u) / 512u;
					y2 = 227u - (buff1[i + 1u] / 19u) + dso.ch1.vertical;
					y3 = 227u - (buff2[i] / 19u) + dso.ch1.vertical;
					y4 = 227u - (buff2[i + 1u] / 19u) + dso.ch1.vertical;
					LCD_DrawLine(x0, y1, x1, y2, color);
					sendScreenBuffer1[pixcnt - 8] = (uint8_t)y1;
					pixcnt++;
					x0 = x1;
					x1 = (pixcnt * 871u) / 512u;
					LCD_DrawLine(x0, y2, x1, y3, color);
					sendScreenBuffer1[pixcnt - 8] = (uint8_t)y2;
					pixcnt++;
					x0 = x1;
					x1 = (pixcnt * 871u) / 512u;
					LCD_DrawLine(x0, y3, x1, y4, color);
					sendScreenBuffer1[pixcnt - 8] = (uint8_t)y3;
					pixcnt++;
					x0 = x1;
				}
				break;
			}
			default:
				break;
		}
		return;
	}
	if(dso.dual1)
	{
		clear_framebuffer1_f = true;
		uint16_t *buff1 = (uint16_t*) buffer1;
		uint32_t y1 = 0, y2 = 0, x = 0;

		color = CH1_COLOR; // ch1 color

		switch(dso.timeDiv)
		{
			case timeDiv_10us:
			{
				y1 = 227u - (buff1[dso.triggerPoint] / 19u) + dso.ch1.vertical; // 212 comes from waveform screen size
				sendScreenBuffer1[0] = (uint8_t)y1;
				for (i = 1; i < X_PIXELS; i++)
				{
					x = (i * 47u) / 16u + dso.triggerPoint;
					y2 = 227u - (buff1[x] / 19u) + dso.ch1.vertical;
					LCD_DrawLineV(y1, y2, pixcnt++, color);
					sendScreenBuffer1[i] = (uint8_t)y2;
					y1 = y2;
				}
				break;
			}
			case timeDiv_5us:
			{
				y1 = 227u - (buff1[dso.triggerPoint] / 19u) + dso.ch1.vertical; // 212 comes from waveform screen size
				sendScreenBuffer1[0] = (uint8_t)y1;
				for (i = 1; i < X_PIXELS; i++)
				{
					x = (i * 47) / 32 + dso.triggerPoint;
					y2 = 227 - (buff1[x] / 19u) + dso.ch1.vertical;
					LCD_DrawLineV(y1, y2, pixcnt++, color);
					sendScreenBuffer1[i] = (uint8_t)y2;
					y1 = y2;
				}
				break;
			}
			default:
				assertError(16u); // 16 - wrong timeDiv for displayWaveform
		}
		pixcnt = 8;
	}

	if(dso.dual2)
	{
		clear_framebuffer3_f = true;
		uint16_t *buff1 = (uint16_t*) buffer3;
		uint32_t y1 = 0, y2 = 0, x = 0;

		color = CH3_COLOR; // ch3 color

		switch (dso.timeDiv)
		{
			case timeDiv_10us:
			{
				y1 = 227u - (buff1[dso.triggerPoint] / 19u) + dso.ch3.vertical; // 212 comes from waveform screen size
				sendScreenBuffer3[0] = (uint8_t)y1;
				for (i = 1; i < X_PIXELS; i++)
				{
					x = (i * 47u) / 16u + dso.triggerPoint;
					y2 = 227u - (buff1[x] / 19u) + dso.ch3.vertical;
					LCD_DrawLineV(y1, y2, pixcnt++, color);
					sendScreenBuffer3[i] = (uint8_t)y2;
					y1 = y2;
				}
				break;
			}
			case timeDiv_5us:
			{
				y1 = 227u - (buff1[dso.triggerPoint] / 19u) + dso.ch3.vertical; // 212 comes from waveform screen size
				sendScreenBuffer3[0] = (uint8_t)y1;
				for (i = 1; i < X_PIXELS; i++)
				{
					x = (i * 47u) / 32u + dso.triggerPoint;
					y2 = 227u - (buff1[x] / 19u) + dso.ch3.vertical;
					LCD_DrawLineV(y1, y2, pixcnt++, color);
					sendScreenBuffer3[i] = (uint8_t)y2;
					y1 = y2;
				}
				break;
			}
			default:
				assertError(16u); // 16 - wrong timeDiv for displayWaveform

		}
		pixcnt = 8;
	}
	if(chNum)
	{
		uint16_t *buff1;
		uint32_t y1 = 0, y2 = 0, x = 0, vert = 0;
		uint8_t *scrbuff;
		switch (chNum)
		{
			default:
			case 1:
			{
				vert = dso.ch1.vertical;
				color = CH1_COLOR; // ch1 color
				buff1 = (uint16_t*) buffer1;
				scrbuff = sendScreenBuffer1;
				clear_framebuffer1_f = true;
				break;
			}
			case 2:
			{
				vert = dso.ch2.vertical;
				color = CH2_COLOR; // ch1 color
				buff1 = (uint16_t*) buffer2;
				scrbuff = sendScreenBuffer2;
				clear_framebuffer2_f = true;
				break;
			}
			case 3:
			{
				vert = dso.ch3.vertical;
				color = CH3_COLOR; // ch1 color
				buff1 = (uint16_t*) buffer3;
				scrbuff = sendScreenBuffer3;
				clear_framebuffer3_f = true;
				break;
			}
			case 4:
			{
				vert = dso.ch4.vertical;
				color = CH4_COLOR; // ch1 color
				buff1 = (uint16_t*) buffer4;
				scrbuff = sendScreenBuffer4;
				clear_framebuffer4_f = true;
				break;
			}
		}
		switch (dso.timeDiv)
		{
			case timeDiv_1ms:
			{
				y1 = 227u - (buff1[dso.triggerPoint] / 19u) + vert; // 212 comes from waveform screen size
				scrbuff[0] = (uint8_t)y1;
				for (i = 1; i < X_PIXELS; i++)
				{
					x = ((i * 439143u) / 131072u) + dso.triggerPoint;
					y2 = 227u - (buff1[x] / 19u) + vert;
					LCD_DrawLineV(y1, y2, pixcnt++, color);
					scrbuff[i] = (uint8_t)y2;
					y1 = y2;
				}
				break;
			}
			case timeDiv_500us:
			{
				y1 = 227u - (buff1[dso.triggerPoint] / 19u) + vert; // 212 comes from waveform screen size
				scrbuff[0] = (uint8_t)y1;
				for (i = 1; i < X_PIXELS; i++)
				{
					x = ((i * 910925u) / 65536u) + dso.triggerPoint;
					y2 = 227 - (buff1[x] / 19u) + vert;
					LCD_DrawLineV(y1, y2, pixcnt++, color);
					scrbuff[i] = (uint8_t)y2;
					y1 = y2;
				}
				break;
			}
			case timeDiv_250us:
			{
				y1 = 227u - (buff1[dso.triggerPoint] / 19u) + vert; // 212 comes from waveform screen size
				scrbuff[0] = (uint8_t)y1;
				for (i = 1; i < X_PIXELS; i++)
				{
					x = (i * 7u) + dso.triggerPoint;
					y2 = 227u - (buff1[x] / 19u) + vert;
					LCD_DrawLineV(y1, y2, pixcnt++, color);
					scrbuff[i] = (uint8_t)y2;
					y1 = y2;
				}
				break;
			}
			case timeDiv_100us:
			{
				y1 = 227u - (buff1[dso.triggerPoint] / 19u) + vert; // 212 comes from waveform screen size
				scrbuff[0] = (uint8_t)y1;
				for (i = 1; i < X_PIXELS; i++)
				{
					x = ((i * 421307u) / 65536u) + dso.triggerPoint;
					y2 = 227 - (buff1[x] / 19u) + vert;
					LCD_DrawLineV(y1, y2, pixcnt++, color);
					scrbuff[i] = (uint8_t)y2;
					y1 = y2;
				}
				break;
			}
			case timeDiv_50us:
			{
				y1 = 227u - (buff1[dso.triggerPoint] / 19u) + vert; // 212 comes from waveform screen size
				scrbuff[0] = (uint8_t)y1;
				for (i = 1; i < X_PIXELS; i++)
				{
					x = ((i * 1389865) / 262144u) + dso.triggerPoint;
					y2 = 227u - (buff1[x] / 19u) + vert;
					LCD_DrawLineV(y1, y2, pixcnt++, color);
					scrbuff[i] = (uint8_t)y2;
					y1 = y2;
				}
				break;
			}
			case timeDiv_25us:
			{
				y1 = 227u - (buff1[dso.triggerPoint] / 19u) + vert; // 212 comes from waveform screen size
				scrbuff[0] = (uint8_t)y1;
				for (i = 1; i < X_PIXELS; i++)
				{
					x = ((i * 30093u) / 8192u) + dso.triggerPoint;
					y2 = 227 - (buff1[x] / 19u) + vert;
					LCD_DrawLineV(y1, y2, pixcnt++, color);
					scrbuff[i] = (uint8_t)y2;
					y1 = y2;
				}
				break;
			}
			default:
				assertError(16u);
				break;
		}
	}
}

/**
 * @brief Find the trigger point in the acquired data
 */
void findTrigger(void)
{
	uint32_t i;
	if(dso.quad)
	{
		uint16_t *buff1 = (uint16_t*) (&buffer1[40]);
		uint16_t *buff2 = (uint16_t*) (&buffer3[40]);
		if(dso.triggerEdge)
		{
			for (i = 0; i < BUFF_SIZE * 2; i++)
			{
				// BUFF_SIZE*2 bcs (32->16) * 2(2 buffs) -160(points till last pixel)
				// 320/2(bcs 2 pointers) = 160
				if((buff1[i] < dso.triggerValue) && (buff2[i] >= dso.triggerValue))
				{
					dso.triggerPoint = i + 80;				//40*2(32->16) = 8
					break;
				}				//comparison is done sample[x] and sample[x+2]
			}
		}
		else
		{
			for (i = 0; i < BUFF_SIZE * 2; i++)
			{
				// 640(all samples) - 160(half screen) = 480
				// 480/2(bcs 2 pointers) = 240
				if((buff1[i] > dso.triggerValue) && (buff2[i] <= dso.triggerValue))
				{
					dso.triggerPoint = i + 80;				//40*2(32->16) = 8
					break;
				}
				//comparison is done sample[x] and sample[x+2]
			}
		}
	}
	else if(dso.dual1 && (dso.triggerSource == 1))
	{
		uint16_t *buff1 = (uint16_t*) (&buffer1[80]);
		if(dso.triggerEdge)
		{
			for (i = 0; i < BUFF_SIZE; i++)
			{ // BUFF_SIZE*2 bcs (32->16) -160(points till last pixel))
			  // 320/2(bcs 2 pointers) = 160
				if((buff1[i] < dso.triggerValue) && (buff1[i + 10] >= dso.triggerValue))
				{
					dso.triggerPoint = i + 160;			//40*2(32->16) = 8
					break;
				}				//comparison is done sample[x] and sample[x+2]
			}
		}
		else
		{
			for (i = 0; i < BUFF_SIZE; i++)
			{ // BUFF_SIZE*2 bcs (32->16) -160(points till last pixel)
			  // 480/2(bcs 2 pointers) = 240
				if((buff1[i] > dso.triggerValue) && (buff1[i + 10] <= dso.triggerValue))
				{
					dso.triggerPoint = i + 160;			//40*2(32->16) = 8
					break;
				}				//comparison is done sample[x] and sample[x+2]
			}
		}
	}
	else if(dso.dual2 && (dso.triggerSource == 3))
	{
		uint16_t *buff1 = (uint16_t*) (&buffer3[40]);
		if(dso.triggerEdge)
		{
			for (i = 0; i < ((BUFF_SIZE * 2) - 160); i++)
			{ // BUFF_SIZE*2 bcs (32->16) -160(points till last pixel))
			  // 320/2(bcs 2 pointers) = 160
				if((buff1[i] < dso.triggerValue) && (buff1[i + 4] >= dso.triggerValue))
				{
					dso.triggerPoint = i + 80;				//40*2(32->16) = 8
					break;
				}				//comparison is done sample[x] and sample[x+2]
			}
		}
		else
		{
			for (i = 0; i < ((BUFF_SIZE * 2) - 160); i++)
			{ // BUFF_SIZE*2 bcs (32->16) -160(points till last pixel)
			  // 480/2(bcs 2 pointers) = 240
				if((buff1[i] > dso.triggerValue) && (buff1[i + 4] <= dso.triggerValue))
				{
					dso.triggerPoint = i + 80;				//40*2(32->16) = 8
					break;
				}				//comparison is done sample[x] and sample[x+2]
			}
		}
	}
	else
	{
		uint16_t *buff1;
		switch (dso.triggerSource)
		{
			case 1:
				buff1 = (uint16_t*) (&buffer1[40]);
				break;
			case 2:
				buff1 = (uint16_t*) (&buffer2[40]);
				break;
			case 3:
				buff1 = (uint16_t*) (&buffer3[40]);
				break;
			case 4:
				buff1 = (uint16_t*) (&buffer4[40]);
				break;
			default:
				buff1 = (uint16_t*) (&buffer1[40]);
				break;
		}
		if(dso.triggerEdge)
		{
			for (i = 0; i < (BUFF_SIZE / 2); i++)
			{						// BUFF_SIZE -160(points till last pixel)
				// 320/2(bcs 2 pointers) = 160
				if((buff1[i] < dso.triggerValue) && (buff1[i + 2] >= dso.triggerValue))
				{
					dso.triggerPoint = i + 80;				//40*2(32->16) = 8
					break;
				}				//comparison is done sample[x] and sample[x+2]
			}
		}
		else
		{
			for (i = 0; i < (BUFF_SIZE / 2); i++)
			{ // BUFF_SIZE -160(points till last pixel)
			  // 480/2(bcs 2 pointers) = 240
				if((buff1[i] > dso.triggerValue) && (buff1[i + 2] <= dso.triggerValue))
				{
					dso.triggerPoint = i + 80;				//40*2(32->16) = 8
					break;
				}				//comparison is done sample[x] and sample[x+2]
			}
		}
	}
}
