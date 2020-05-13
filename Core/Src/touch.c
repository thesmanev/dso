#include "main.h"

static uint16_t touch_readY(void);
static uint16_t touch_readX(void);
static uint32_t get_Y_touch(void);
static uint32_t get_X_touch(void);
static void select(void);
static void deselect(void);
static void deselect_channel(void);
static void timeDivChangeRedraw(void);
static void voltDivChangeRedraw(channel_t *chx);
static void triggerSourceChangeRedraw(void);
static void triggerEdgeChangeRedraw(void);
static void CouplingChangeRedraw(channel_t *chx);
static void triggerValueChangeRedraw(uint32_t up_down);
static void incDec(uint32_t mode);

typedef enum
{
	none_selected = 		0,
	trig_src_selected = 	1,
	trig_edge_selected = 	2,
	trig_val_selected = 	4,
	timediv_selected = 		8,
	ch1_vdiv_selected = 	16,
	ch1_cpl_selected = 		32,
	ch2_vdiv_selected = 	64,
	ch2_cpl_selected = 		128,
	ch3_vdiv_selected = 	256,
	ch3_cpl_selected = 		512,
	ch4_vdiv_selected = 	1024,
	ch4_cpl_selected = 		2048,
	ch1_selected = 			4096,
	ch2_selected = 			8192,
	ch3_selected = 			16384,
	ch4_selected = 			32768
}obj_selected_t;

static obj_selected_t selected = none_selected;

/**
 * @brief Setup touch detection
 */
void setupTouch(void)
{
	// disabled, baud-rate fpclk/64, master
	SPIx->CR1 = SPI_CR1_MSTR | SPI_CR1_BR_0 | SPI_CR1_BR_2;
	// 16-bit, SS output disabled, RXNE at 1/2 full RX FIFO
	SPIx->CR2 = SPI_CR2_DS | SPI_CR2_SSOE;
	touch_readX();
	touch_readY();
}

/**
 * @brief Read the 12 bit ADC conversion for the Y axis of the touch panel
 * @return 0 - 4095
 */
static uint16_t touch_readY(void)
{
	uint16_t y1, y2, y;
	SPIx->DR = TOUCHREADYCMD; // send read X command
	CST_LOW;
	 // enabled, baud-rate f-pclk/64, master
	SPIx->CR1 = SPI_CR1_MSTR | SPI_CR1_BR_0 | SPI_CR1_BR_2 | SPI_CR1_SPE;
	while (!(SPIx->SR & SPI_SR_TXE)); // wait for receiving to complete
	while (!(SPIx->SR & SPI_SR_RXNE)); // wait for receiving to complete
	y1 = SPIx->DR; // read input
	SPIx->DR = 0; // send zero
	while (!(SPIx->SR & SPI_SR_TXE)); // wait for receiving to complete
	while (!(SPIx->SR & SPI_SR_RXNE)); // wait for receiving to complete
	y2 = SPIx->DR; // read input
	CST_HIGH;
	 // disabled, baud-rate fpclk/64, master
	SPIx->CR1 = SPI_CR1_MSTR | SPI_CR1_BR_0 | SPI_CR1_BR_2;

	/* Part of the data is received when sending the read command, and a second part with the second transmission.
	 * Combine the data to get the 12 bit value.
	 */
	y = (uint16_t)(((y1 << 5) & 0x0FE0) + ((y2 >> 11) & 0x001F));
	return y;
}

/**
 * @brief Read the 12 bit ADC conversion for the Xs axis of the touch panel
 * @return 0 - 4095
 */
static uint16_t touch_readX(void)
{
	uint16_t x1, x2, x;
	CST_LOW;
	 // enabled, baud-rate f-pclk/64, master
	SPIx->CR1 = SPI_CR1_MSTR | SPI_CR1_BR_0 | SPI_CR1_BR_2 | SPI_CR1_SPE;
	SPIx->DR = TOUCHREADXCMD; // send read X command
	while (!(SPIx->SR & SPI_SR_TXE)); // wait for receiving to complete
	while (!(SPIx->SR & SPI_SR_RXNE)); // wait for receiving to complete
	x1 = SPIx->DR; // read input
	SPIx->DR = 0; // send zero
	while (!(SPIx->SR & SPI_SR_TXE)); // wait for receiving to complete
	while (!(SPIx->SR & SPI_SR_RXNE)); // wait for receiving to complete
	x2 = SPIx->DR; // read input
	CST_HIGH;
	 // disabled, baud-rate fpclk/64, master
	SPIx->CR1 = SPI_CR1_MSTR | SPI_CR1_BR_0 | SPI_CR1_BR_2;

	/* Part of the data is received when sending the read command, and a second part with the second transmission.
	 * Combine the data to get the 12 bit value.
	 */
	x = (uint16_t) (((x1 << 5) & 0x0FE0) + ((x2 >> 11) & 0x001F));
	return x;
}

/**
 * @brief Get the Y pixel coordinate of the touch
 * @return The pixel coordinate
 */
static uint32_t get_Y_touch(void)
{
	// X and Y of touch are inverted in relation to display (X is Y and vice versa)
	uint16_t x_touch = touch_readX();
	int32_t pix;
	int32_t yt = Y_PIXELS - (((int32_t) x_touch - 320) / 14);
	pix = (int32_t) yt;
	if(pix > 240)
		pix = 241;
	if(pix < 0)
		pix = 0;
	return (uint32_t) pix;
}

/**
 * @brief Get the X pixel coordinate of the touch
 * @return The pixel coordinate
 */
static uint32_t get_X_touch(void)
{
	// X and Y of touch are inverted in relation to display (X is Y and vice versa)
	uint16_t y_touch = touch_readY();
	int32_t pix;
	int32_t xt = ((int32_t) y_touch - 490) / 10;
	pix = (int32_t) xt;
	if(pix > 320)
		pix = 321;
	if(pix < 0)
		pix = 0;
	return (uint32_t) pix;
}

/**
 * @brief Check for a touch on the screen and perform actions if a certain object has been touched
 */
void pollForTouch(void)
{
	uint32_t xt = 0, yt = 0;
	/* Check if touch is detected */
	if(!(GPIOD->IDR & GPIO_IDR_15))
	{
		yt = get_Y_touch();
		xt = get_X_touch();

		// increment
		if((yt >= 75) && (yt <= 137) && (xt >= 288) && (xt <= RESX))
		{
			currentButton = inc_pressed;
			if(currentButton != prevButton)
			{
				incDec(INCREMENT);
			}
		}
		// decrement
		if((yt >= 139) && (yt <= 201) && (xt >= 288) && (xt <= RESX))
		{
			currentButton = dec_pressed;
			if(currentButton != prevButton)
			{
				incDec(DECREMENT);
			}
		}

		//trigger source
		if((yt <= FONTXSIZESMALL) && (xt >= (4 * FONTXSIZESMALL)) && (xt <= (7 * FONTXSIZESMALL)))
		{
			currentButton = trigSource_pressed;
			if(currentButton != prevButton)
			{
				if(selected != trig_src_selected)
				{
					deselect();
					selected = trig_src_selected;
					select();
				}
			}
		}

		//trigger edge
		if((yt <= (FONTXSIZESMALL)) && (xt >= (7 * FONTXSIZESMALL)) && (xt <= ((8 * FONTXSIZESMALL))))
		{
			currentButton = trigEdge_pressed;
			if(currentButton != prevButton)
			{
				if(selected != trig_edge_selected)
				{
					deselect();
					selected = trig_edge_selected;
					select();
				}
			}
		}

		//trigger value
		if((yt <= (FONTXSIZESMALL)) && (xt >= (8 * FONTXSIZESMALL)) && (xt <= (TRIG_STR_LNGTH * FONTXSIZESMALL)))
		{
			currentButton = trigValue_pressed;
			if(currentButton != prevButton)
			{
				if(selected != trig_val_selected)
				{
					deselect();
					selected = trig_val_selected;
					select();
				}
			}
		}

		//timeDiv
		if((yt <= (FONTXSIZESMALL)) && (xt >= (TRIG_STR_LNGTH * FONTXSIZESMALL))
				&& (xt <= ((TDIV_STR_LNGTH * FONTXSIZESMALL) + (TRIG_STR_LNGTH * FONTXSIZESMALL))))
		{
			currentButton = timeDiv_pressed;
			if(currentButton != prevButton)
			{
				if(selected != timediv_selected)
				{
					deselect();
					selected = timediv_selected;
					select();
				}
			}
		}

		//CH1 voltsDiv
		if((yt >= CH1_YTextStart) && (yt <= MAX_Y) && (xt >= (2 * FONTXSIZESMALL + CH1_XTextStart)) && (xt <= (6 * FONTXSIZESMALL + CH2_XTextStart)))
		{
			currentButton = ch1VDiv_pressed;
			if(currentButton != prevButton)
			{
				if(selected != ch1_vdiv_selected)
				{
					deselect();
					selected = ch1_vdiv_selected;
					select();
				}
			}
		}

		//ch1 coupling
		if((yt >= CH1_YTextStart) && (yt <= MAX_Y) && (xt >= (6 * FONTXSIZESMALL + CH1_XTextStart)) && (xt <= (9 * FONTXSIZESMALL + CH2_XTextStart)))
		{
			currentButton = ch1Cpl_pressed;
			if(currentButton != prevButton)
			{
				if(selected != ch1_cpl_selected)
				{
					deselect();
					selected = ch1_cpl_selected;
					select();
				}
			}
		}

		//CH2 voltsDiv
		if((yt >= CH1_YTextStart) && (yt <= MAX_Y) && (xt >= (2 * FONTXSIZESMALL + CH2_XTextStart)) && (xt <= (6 * FONTXSIZESMALL + CH2_XTextStart)))
		{
			currentButton = ch2VDiv_pressed;
			if(currentButton != prevButton)
			{
				if(selected != ch2_vdiv_selected)
				{
					deselect();
					selected = ch2_vdiv_selected;
					select();
				}
			}
		}

		//ch2 coupling
		if((yt >= CH1_YTextStart) && (yt <= MAX_Y) && (xt >= (6 * FONTXSIZESMALL + CH2_XTextStart)) && (xt <= (9 * FONTXSIZESMALL + CH2_XTextStart)))
		{
			currentButton = ch2Cpl_pressed;
			if(currentButton != prevButton)
			{
				if(selected != ch2_cpl_selected)
				{
					deselect();
					selected = ch2_cpl_selected;
					select();
				}
			}
		}

		//CH3 voltsDiv
		if((yt >= CH1_YTextStart) && (yt <= MAX_Y) && (xt >= (2 * FONTXSIZESMALL + CH3_XTextStart)) && (xt <= (6 * FONTXSIZESMALL + CH3_XTextStart)))
		{
			currentButton = ch3VDiv_pressed;
			if(currentButton != prevButton)
			{
				if(selected != ch3_vdiv_selected)
				{
					deselect();
					selected = ch3_vdiv_selected;
					select();
				}
			}
		}

		//ch3 coupling
		if((yt >= CH1_YTextStart) && (yt <= MAX_Y) && (xt >= (6 * FONTXSIZESMALL + CH3_XTextStart)) && (xt <= (9 * FONTXSIZESMALL + CH3_XTextStart)))
		{
			currentButton = ch3Cpl_pressed;
			if(currentButton != prevButton)
			{
				if(selected != ch3_cpl_selected)
				{
					deselect();
					selected = ch3_cpl_selected;
					select();
				}
			}
		}

		//CH4 voltsDiv
		if((yt >= CH1_YTextStart) && (yt <= MAX_Y) && (xt >= (2 * FONTXSIZESMALL + CH4_XTextStart)) && (xt <= (5 * FONTXSIZESMALL + CH4_XTextStart)))
		{
			currentButton = ch4VDiv_pressed;
			if(currentButton != prevButton)
			{
				if(selected != ch4_vdiv_selected)
				{
					deselect();
					selected = ch4_vdiv_selected;
					select();
				}
			}
		}

		//ch4 coupling
		if((yt >= CH1_YTextStart) && (yt <= MAX_Y) && (xt >= (6 * FONTXSIZESMALL + CH4_XTextStart)) && (xt <= (9 * FONTXSIZESMALL + CH4_XTextStart)))
		{
			currentButton = ch4Cpl_pressed;
			if(currentButton != prevButton)
			{
				if(selected != ch4_cpl_selected)
				{
					deselect();
					selected = ch4_cpl_selected;
					select();
				}
			}
		}

		// ch1
		if((yt <= MAX_Y) && (yt >= CH1_YTextStart) && (xt <= CH1_XTextStart + 16))
		{
			currentButton = ch1_pressed;
			if(currentButton != prevButton)
			{
				if(dso.ch1.selected)
				{
					dso.ch1.enabled = 0;
					dso.ch1.selected = 0;
					LCD_RectFill(CH1_XTextStart, CH1_YTextStart,
					CH1_XTextStart + 16, (CH1_YTextStart + FONTYSIZESMALL - 2),
					ILI9341_BLACK);
					LCD_Print(CH1_XTextStart, CH1_YTextStart, ch1, 2,
					ILI9341_GREEN);
					channelChange();
				}
				else
				{
					if(dso.ch1.enabled)
					{
						if(selected != ch1_selected)
						{
							deselect();
							selected = ch1_selected;
							select();
						}
						dso.ch1.selected = 1;
						dso.ch2.selected = 0;
						dso.ch3.selected = 0;
						dso.ch4.selected = 0;
						deselect_channel();
						LCD_DrawRect(CH1_XTextStart, CH1_YTextStart,
						CH1_XTextStart + 16, (CH1_YTextStart + FONTYSIZESMALL - 2),
						ILI9341_WHITE);
					}
					else
					{
						dso.ch1.enabled = 1;
						dso.ch1.selected = 0;
						LCD_RectFill(CH1_XTextStart, CH1_YTextStart,
						CH1_XTextStart + 16, (CH1_YTextStart + FONTYSIZESMALL - 2),
						ILI9341_GREEN);
						LCD_Print(CH1_XTextStart, CH1_YTextStart, ch1, 2,
						ILI9341_BLACK);
						channelChange();
					}
				}
			}
		}

		// ch2
		if((yt <= MAX_Y) && (yt >= CH2_YTextStart) && (xt >= CH2_XTextStart) && (xt <= CH2_XTextStart + 16))
		{
			currentButton = ch2_pressed;
			if(currentButton != prevButton)
			{
				if(dso.ch2.selected)
				{
					dso.ch2.enabled = 0;
					dso.ch2.selected = 0;
					LCD_RectFill(CH2_XTextStart, CH2_YTextStart,
					CH2_XTextStart + 16, (CH2_YTextStart + FONTYSIZESMALL - 2),
					ILI9341_BLACK);
					LCD_Print(CH2_XTextStart, CH2_YTextStart, ch2, 2,
					ILI9341_BLUE);
					channelChange();
				}
				else
				{
					if(dso.ch2.enabled)
					{
						if(selected != ch2_selected)
						{
							deselect();
							selected = ch2_selected;
							select();
						}
						dso.ch1.selected = 0;
						dso.ch2.selected = 1;
						dso.ch3.selected = 0;
						dso.ch4.selected = 0;
						deselect_channel();
						LCD_DrawRect(CH2_XTextStart, CH2_YTextStart,
						CH2_XTextStart + 16, (CH2_YTextStart + FONTYSIZESMALL - 2),
						ILI9341_WHITE);

					}
					else
					{
						dso.ch2.enabled = 1;
						dso.ch2.selected = 0;
						LCD_RectFill(CH2_XTextStart, CH2_YTextStart,
						CH2_XTextStart + 16, (CH2_YTextStart + FONTYSIZESMALL - 2),
						ILI9341_BLUE);
						LCD_Print(CH2_XTextStart, CH2_YTextStart, ch2, 2,
						ILI9341_BLACK);
						channelChange();
					}
				}
			}
		}

		// ch3
		if((yt <= MAX_Y) && (yt >= CH3_YTextStart) && (xt >= CH3_XTextStart) && (xt <= CH3_XTextStart + 16))
		{
			currentButton = ch3_pressed;
			if(currentButton != prevButton)
			{
				if(dso.ch3.selected)
				{
					dso.ch3.enabled = 0;
					dso.ch3.selected = 0;
					LCD_RectFill(CH3_XTextStart, CH3_YTextStart,
					CH3_XTextStart + 16, (CH3_YTextStart + FONTYSIZESMALL - 2),
					ILI9341_BLACK);
					LCD_Print(CH3_XTextStart, CH3_YTextStart, ch3, 2,
					ILI9341_RED);
					channelChange();
				}
				else
				{
					if(dso.ch3.enabled)
					{
						if(selected != ch3_selected)
						{
							deselect();
							selected = ch3_selected;
							select();
						}
						dso.ch1.selected = 0;
						dso.ch2.selected = 0;
						dso.ch3.selected = 1;
						dso.ch4.selected = 0;
						deselect_channel();
						LCD_DrawRect(CH3_XTextStart, CH3_YTextStart,
						CH3_XTextStart + 16, (CH3_YTextStart + FONTYSIZESMALL - 2),
						ILI9341_WHITE);
					}
					else
					{
						dso.ch3.enabled = 1;
						dso.ch3.selected = 0;
						LCD_RectFill(CH3_XTextStart, CH3_YTextStart,
						CH3_XTextStart + 16, (CH3_YTextStart + FONTYSIZESMALL - 2),
						ILI9341_RED);
						LCD_Print(CH3_XTextStart, CH3_YTextStart, ch3, 2,
						ILI9341_BLACK);
						channelChange();
					}
				}
			}
		}

		// ch4
		if((yt <= MAX_Y) && (yt >= CH4_YTextStart) && (xt >= CH4_XTextStart) && (xt <= (CH4_XTextStart + 16)))
		{
			currentButton = ch4_pressed;
			if(currentButton != prevButton)
			{
				if(dso.ch4.selected)
				{
					dso.ch4.enabled = 0;
					dso.ch4.selected = 0;
					LCD_RectFill(CH4_XTextStart, CH4_YTextStart,
					CH4_XTextStart + 16, (CH3_YTextStart + FONTYSIZESMALL - 2),
					ILI9341_BLACK);
					LCD_Print(CH4_XTextStart, CH4_YTextStart, ch4, 2,
					ILI9341_ORANGE);
					channelChange();
				}
				else
				{
					if(dso.ch4.enabled)
					{
						if(selected != ch4_selected)
						{
							deselect();
							selected = ch4_selected;
							select();
						}
						dso.ch1.selected = 0;
						dso.ch2.selected = 0;
						dso.ch3.selected = 0;
						dso.ch4.selected = 1;
						deselect_channel();
						LCD_DrawRect(CH4_XTextStart, CH4_YTextStart,
						CH4_XTextStart + 16, (CH3_YTextStart + FONTYSIZESMALL - 2),
						ILI9341_WHITE);
					}
					else
					{
						dso.ch4.enabled = 1;
						dso.ch4.selected = 0;
						LCD_RectFill(CH4_XTextStart, CH4_YTextStart,
						CH4_XTextStart + 16, (CH3_YTextStart + FONTYSIZESMALL - 2),
						ILI9341_ORANGE);
						LCD_Print(CH4_XTextStart, CH4_YTextStart, ch4, 2,
						ILI9341_BLACK);
						channelChange();
					}
				}
			}
		}
		prevButton = currentButton;
	}
	else
	{
		prevButton = none_pressed;
		currentButton = none_pressed;
	}
}

/**
 * @brief Select a touch object (draw a white line around it)
 */
static void select(void)
{
	switch (selected)
	{
		case trig_src_selected: // trigger source
			LCD_DrawRect(5 * FONTXSIZESMALL - 1, 0, 8 * FONTXSIZESMALL, FONTYSIZESMALL - 2, ILI9341_WHITE);
			break;

		case trig_edge_selected: // trigger edge
			LCD_DrawRect(8 * FONTXSIZESMALL, 0, 9 * FONTXSIZESMALL, FONTYSIZESMALL - 2, ILI9341_WHITE);
			break;

		case trig_val_selected: // trigger value
			LCD_DrawRect(9 * FONTXSIZESMALL, 0, TRIG_STR_LNGTH * FONTXSIZESMALL - 1, FONTYSIZESMALL - 2, ILI9341_WHITE);
			break;

		case timediv_selected: // timeDiv
			LCD_DrawRect(TRIG_STR_LNGTH * FONTXSIZESMALL - 1, 0, (TRIG_STR_LNGTH * FONTXSIZESMALL) + (TDIV_STR_LNGTH * FONTXSIZESMALL),
						FONTYSIZESMALL - 2, ILI9341_WHITE);
			break;

		case ch1_vdiv_selected: // CH1 voltsDiv
			LCD_DrawRect(CH1_XTextStart + (2 * FONTXSIZESMALL) - 1, CH1_YTextStart, CH1_XTextStart + (7 * FONTXSIZESMALL) - 2,
					(CH1_YTextStart + FONTYSIZESMALL - 2), ILI9341_WHITE);
			break;

		case ch1_cpl_selected: // CH1 coupling
			LCD_DrawRect(CH1_XTextStart + (7 * FONTXSIZESMALL) - 1, CH1_YTextStart, CH1_XTextStart + (10 * FONTXSIZESMALL) - 2,
					(CH1_YTextStart + FONTYSIZESMALL - 2), ILI9341_WHITE);

			break;

		case ch2_vdiv_selected: // CH2 voltsDiv
			LCD_DrawRect(CH2_XTextStart + (2 * FONTXSIZESMALL) - 1,
				CH2_YTextStart, CH2_XTextStart + (7 * FONTXSIZESMALL) - 2, (CH2_YTextStart + FONTYSIZESMALL - 2), ILI9341_WHITE);
			break;

		case ch2_cpl_selected: // CH2 coupling
			LCD_DrawRect(CH2_XTextStart + (7 * FONTXSIZESMALL) - 1, CH2_YTextStart, CH2_XTextStart + (10 * FONTXSIZESMALL) - 2,
					(CH2_YTextStart + FONTYSIZESMALL - 2), ILI9341_WHITE);
			break;

		case ch3_vdiv_selected: // CH3 voltsDiv
			LCD_DrawRect(CH3_XTextStart + (2 * FONTXSIZESMALL) - 1, CH3_YTextStart, CH3_XTextStart + (7 * FONTXSIZESMALL) - 2,
					(CH3_YTextStart + FONTYSIZESMALL - 2), ILI9341_WHITE);
			break;

		case ch3_cpl_selected: // CH3 coupling
			LCD_DrawRect(CH3_XTextStart + (7 * FONTXSIZESMALL) - 1, CH3_YTextStart, CH3_XTextStart + (10 * FONTXSIZESMALL) - 2,
					(CH3_YTextStart + FONTYSIZESMALL - 2), ILI9341_WHITE);
			break;

		case ch4_vdiv_selected: // CH4 voltsDiv
			LCD_DrawRect(CH4_XTextStart + (2 * FONTXSIZESMALL) - 1, CH4_YTextStart, CH4_XTextStart + (7 * FONTXSIZESMALL) - 2,
					(CH4_YTextStart + FONTYSIZESMALL - 2), ILI9341_WHITE);
			break;

		case ch4_cpl_selected: // CH4 coupling
			LCD_DrawRect(CH4_XTextStart + (7 * FONTXSIZESMALL) - 1, CH4_YTextStart, CH4_XTextStart + (10 * FONTXSIZESMALL) - 2,
					(CH4_YTextStart + FONTYSIZESMALL - 2), ILI9341_WHITE);
			break;

		default:
			break;
	}
}

/**
 * @brief De-select a touch object (remove white line around object)
 */
static void deselect(void)
{
	switch (selected)
	{
		case trig_src_selected: // trigger source
			LCD_DrawRect(5 * FONTXSIZESMALL - 1, 0, 8 * FONTXSIZESMALL, FONTYSIZESMALL - 2, ILI9341_BLACK);
			break;

		case trig_edge_selected: // trigger edge
			LCD_DrawRect(8 * FONTXSIZESMALL, 0, 9 * FONTXSIZESMALL, FONTYSIZESMALL - 2, ILI9341_BLACK);
			break;

		case trig_val_selected: // trigger value
			LCD_DrawRect(9 * FONTXSIZESMALL, 0, TRIG_STR_LNGTH * FONTXSIZESMALL - 1, FONTYSIZESMALL - 2, ILI9341_BLACK);
			break;

		case timediv_selected: // timeDiv
			LCD_DrawRect(TRIG_STR_LNGTH * FONTXSIZESMALL - 1, 0, (TRIG_STR_LNGTH * FONTXSIZESMALL) + (TDIV_STR_LNGTH * FONTXSIZESMALL),
						FONTYSIZESMALL - 2, ILI9341_BLACK);
			break;

		case ch1_vdiv_selected: // CH1 voltsDiv
			LCD_DrawRect(CH1_XTextStart + (2 * FONTXSIZESMALL) - 1, CH1_YTextStart, CH1_XTextStart + (7 * FONTXSIZESMALL) - 2,
					(CH1_YTextStart + FONTYSIZESMALL - 2), ILI9341_BLACK);
			break;

		case ch1_cpl_selected: // CH1 coupling
			LCD_DrawRect(CH1_XTextStart + (7 * FONTXSIZESMALL) - 1, CH1_YTextStart, CH1_XTextStart + (10 * FONTXSIZESMALL) - 2,
					(CH1_YTextStart + FONTYSIZESMALL - 2), ILI9341_BLACK);
			break;

		case ch2_vdiv_selected: // CH2 voltsDiv
			LCD_DrawRect(CH2_XTextStart + (2 * FONTXSIZESMALL) - 1, CH2_YTextStart, CH2_XTextStart + (7 * FONTXSIZESMALL) - 2,
					(CH2_YTextStart + FONTYSIZESMALL - 2), ILI9341_BLACK);
			break;

		case ch2_cpl_selected: // CH2 coupling
			LCD_DrawRect(CH2_XTextStart + (7 * FONTXSIZESMALL) - 1, CH2_YTextStart, CH2_XTextStart + (10 * FONTXSIZESMALL) - 2,
					(CH2_YTextStart + FONTYSIZESMALL - 2), ILI9341_BLACK);
			break;

		case ch3_vdiv_selected: // CH3 voltsDiv
			LCD_DrawRect(CH3_XTextStart + (2 * FONTXSIZESMALL) - 1, CH3_YTextStart, CH3_XTextStart + (7 * FONTXSIZESMALL) - 2,
					(CH3_YTextStart + FONTYSIZESMALL - 2), ILI9341_BLACK);
			break;

		case ch3_cpl_selected: // CH3 coupling
			LCD_DrawRect(CH3_XTextStart + (7 * FONTXSIZESMALL) - 1, CH3_YTextStart, CH3_XTextStart + (10 * FONTXSIZESMALL) - 2,
					(CH3_YTextStart + FONTYSIZESMALL - 2), ILI9341_BLACK);
			break;

		case ch4_vdiv_selected: // CH4 voltsDiv
			LCD_DrawRect(CH4_XTextStart + (2 * FONTXSIZESMALL) - 1, CH4_YTextStart, CH4_XTextStart + (7 * FONTXSIZESMALL) - 2,
					(CH4_YTextStart + FONTYSIZESMALL - 2), ILI9341_BLACK);
			break;

		case ch4_cpl_selected: // CH4 coupling
			LCD_DrawRect(CH4_XTextStart + (7 * FONTXSIZESMALL) - 1, CH4_YTextStart, CH4_XTextStart + (10 * FONTXSIZESMALL) - 2, (CH4_YTextStart + FONTYSIZESMALL - 2), ILI9341_BLACK);
			break;

		default:
			// ignore other
			break;
	}
}

/**
 * @brief Function for de-selecting a channel
 */
static void deselect_channel(void)
{
	if(!dso.ch1.selected)
	{
		if(dso.ch1.enabled)
		{
			LCD_DrawRect(CH1_XTextStart, CH1_YTextStart,
			CH1_XTextStart + (2 * FONTXSIZESMALL), (CH1_YTextStart + FONTYSIZESMALL - 2), CH1_COLOR);
		}
	}

	if(!dso.ch2.selected)
	{
		if(dso.ch2.enabled)
		{
			LCD_DrawRect(CH2_XTextStart, CH2_YTextStart,
			CH2_XTextStart + (2 * FONTXSIZESMALL), (CH2_YTextStart + FONTYSIZESMALL - 2), CH2_COLOR);
		}
	}

	if(!dso.ch3.selected)
	{
		if(dso.ch3.enabled)
		{
			LCD_DrawRect(CH3_XTextStart, CH3_YTextStart,
			CH3_XTextStart + (2 * FONTXSIZESMALL), (CH3_YTextStart + FONTYSIZESMALL - 2), CH3_COLOR);
		}
	}

	if(!dso.ch4.selected)
	{
		if(dso.ch4.enabled)
		{
			LCD_DrawRect(CH4_XTextStart, CH4_YTextStart,
			CH4_XTextStart + (2 * FONTXSIZESMALL), (CH3_YTextStart + FONTYSIZESMALL - 2), CH4_COLOR);
		}
	}
}

/**
 * @brief Re-draw(re-print) Time-base when it is changed
 */
static void timeDivChangeRedraw(void)
{
	switch (dso.timeDiv)
	{
		case timeDiv_1ms:
			tDiv[9] = ' ';
			tDiv[10] = ' ';
			tDiv[11] = '1';
			tDiv[12] = 'm';
			tDiv[13] = 's';
			break;
		case timeDiv_500us:
			tDiv[9] = '5';
			tDiv[10] = '0';
			tDiv[11] = '0';
			tDiv[12] = 'u';
			tDiv[13] = 's';
			break;
		case timeDiv_250us:
			tDiv[9] = '2';
			tDiv[10] = '5';
			tDiv[11] = '0';
			tDiv[12] = 'u';
			tDiv[13] = 's';
			break;
		case timeDiv_100us:
			tDiv[9] = '1';
			tDiv[10] = '0';
			tDiv[11] = '0';
			tDiv[12] = 'u';
			tDiv[13] = 's';
			break;
		case timeDiv_50us:
			tDiv[9] = ' ';
			tDiv[10] = '5';
			tDiv[11] = '0';
			tDiv[12] = 'u';
			tDiv[13] = 's';
			break;
		case timeDiv_25us:
			tDiv[9] = ' ';
			tDiv[10] = '2';
			tDiv[11] = '5';
			tDiv[12] = 'u';
			tDiv[13] = 's';
			break;
		case timeDiv_10us:
			tDiv[9] = ' ';
			tDiv[10] = '1';
			tDiv[11] = '0';
			tDiv[12] = 'u';
			tDiv[13] = 's';
			break;
		case timeDiv_5us:
			tDiv[9] = ' ';
			tDiv[10] = ' ';
			tDiv[11] = '5';
			tDiv[12] = 'u';
			tDiv[13] = 's';
			break;
		case timeDiv_2_5us:
			tDiv[9] = '2';
			tDiv[10] = '.';
			tDiv[11] = '5';
			tDiv[12] = 'u';
			tDiv[13] = 's';
			break;
		case timeDiv_1us:
			tDiv[9] = ' ';
			tDiv[10] = ' ';
			tDiv[11] = '1';
			tDiv[12] = 'u';
			tDiv[13] = 's';
			break;
		default:
			break;
	}
	LCD_RectFill((TRIG_STR_LNGTH + 9) * FONTXSIZESMALL - 1, 1, (TRIG_STR_LNGTH + TDIV_STR_LNGTH) * FONTXSIZESMALL - 1,
			FONTYSIZESMALL - 3, ILI9341_BLACK);
	LCD_Print((TRIG_STR_LNGTH + 9) * FONTXSIZESMALL, 0, &tDiv[9], 5, ILI9341_YELLOW);
}

/**
 * @brief Re-draw(re-print) Volts per division when it is changed
 * @param chx Pointer to a channel structure.
 */
static void voltDivChangeRedraw(channel_t *chx)
{
	uint32_t x1, x2, color;
	uint16_t *AS;
	uint8_t *ch;

	switch (chx->id)
	{
		case CH1_ID:
			x1 = 2 * FONTXSIZESMALL;
			x2 = 7 * FONTXSIZESMALL;
			ch = &ch1[2];
			color = CH1_COLOR;
			AS = &AS1;
			break;
		case CH2_ID:
			x1 = 2 * FONTXSIZESMALL + CH2_XTextStart;
			x2 = 7 * FONTXSIZESMALL + CH2_XTextStart;
			ch = &ch2[2];
			color = CH2_COLOR;
			AS = &AS2;
			break;
		case CH3_ID:
			x1 = 2 * FONTXSIZESMALL + CH3_XTextStart;
			x2 = 7 * FONTXSIZESMALL + CH3_XTextStart;
			ch = &ch3[2];
			color = CH3_COLOR;
			AS = &AS3;
			break;
		case CH4_ID:
			x1 = 2 * FONTXSIZESMALL + CH4_XTextStart;
			x2 = 7 * FONTXSIZESMALL + CH4_XTextStart;
			ch = &ch4[2];
			color = CH4_COLOR;
			AS = &AS4;
			break;
		default:
			x1 = 0;
			x2 = 0;
			ch = ((void*) 0);
			color = 0;
			AS = ((void*) 0);
			break;
	}

	switch (chx->voltsDiv)
	{
		case v_100mDiv:
			ch[0] = '1';
			ch[1] = '0';
			ch[2] = '0';
			ch[3] = 'm';
			ch[4] = 'V';
			//		 ***   **
			*AS |= 0b1000000100000000;
			*AS &= 0b1001110100000000;
			break;
		case v_200mDiv:
			ch[0] = '2';
			ch[1] = '0';
			ch[2] = '0';
			ch[3] = 'm';
			ch[4] = 'V';
			//		 ***   **
			*AS |= 0b1100000100000000;
			*AS &= 0b1101110100000000;
			break;
		case v_500mDiv:
			ch[0] = '5';
			ch[1] = '0';
			ch[2] = '0';
			ch[3] = 'm';
			ch[4] = 'V';
			//		 ***   **
			*AS |= 0b0100000100000000;
			*AS &= 0b0101110100000000;
			break;
		case v_1Div:
			ch[0] = ' ';
			ch[1] = ' ';
			ch[2] = '1';
			ch[3] = 'V';
			ch[4] = ' ';
			//		 ***   **
			*AS |= 0b0010000100000000;
			*AS &= 0b0011110100000000;
			break;
		case v_2Div:
			ch[0] = ' ';
			ch[1] = ' ';
			ch[2] = '2';
			ch[3] = 'V';
			ch[4] = ' ';
			//		 ***   **
			*AS |= 0b1100001000000000;
			*AS &= 0b1101111000000000;
			break;
		case v_5Div:
			ch[0] = ' ';
			ch[1] = ' ';
			ch[2] = '5';
			ch[3] = 'V';
			ch[4] = ' ';
			//		 ***   **
			*AS |= 0b0110001000000000;
			*AS &= 0b0111111000000000;
			break;
		case v_10Div:
			ch[0] = ' ';
			ch[1] = '1';
			ch[2] = '0';
			ch[3] = 'V';
			ch[4] = ' ';
			//		 ***   **
			*AS |= 0b0010001000000000;
			*AS &= 0b0011111000000000;
			break;
		default:
			break;
	}
	setSwitch(chx->id);
	LCD_RectFill(x1, CH1_YTextStart + 1, x2 - 2, RESY - 2, ILI9341_BLACK);
	LCD_Print(x1, CH1_YTextStart, ch, 5, color);
}

/**
 * @brief Re-print the trigger source when it is changed
 */
static void triggerSourceChangeRedraw(void)
{
	switch (dso.triggerSource)
	{
		default:
		case TRIG_SRC_CH1:
			trig[7] = '1';
			break;
		case TRIG_SRC_CH2:
			trig[7] = '2';
			break;
		case TRIG_SRC_CH3:
			trig[7] = '3';
			break;
		case TRIG_SRC_CH4:
			trig[7] = '4';
			break;
	}
	LCD_RectFill((7 * FONTXSIZESMALL), 1, (8 * FONTXSIZESMALL) - 1,
			FONTYSIZESMALL - 3, ILI9341_BLACK);
	LCD_Print((7 * FONTXSIZESMALL), 0, &trig[7], 1, ILI9341_YELLOW);
}

/**
 * @brief Re-draw the trigger edge when it is changed.
 */
static void triggerEdgeChangeRedraw(void)
{
	if(dso.triggerEdge == TRIG_EDGE_RISING)
		trig[8] = 127; //rising
	else
		trig[8] = 128; //falling

	LCD_RectFill((8 * FONTXSIZESMALL) + 1, 1, (9 * FONTXSIZESMALL) - 1,
			FONTYSIZESMALL - 3, ILI9341_BLACK);
	LCD_Print((8 * FONTXSIZESMALL), 0, &trig[8], 1, ILI9341_YELLOW);
}

/**
 * @brief Re-print the coupling for the channel when it is changed.
 * @param chx Pointer to a channel structure
 */
static void CouplingChangeRedraw(channel_t *chx)
{
	uint32_t x1, x2, color;
	uint8_t *ch;
	uint16_t *AS;

	switch (chx->id)
	{
		case CH1_ID:
			x1 = 7 * FONTXSIZESMALL;
			x2 = 10 * FONTXSIZESMALL;
			ch = &ch1[7];
			color = CH1_COLOR;
			AS = &AS1;
			break;
		case CH2_ID:
			x1 = 7 * FONTXSIZESMALL + CH2_XTextStart;
			x2 = 10 * FONTXSIZESMALL + CH2_XTextStart;
			ch = &ch2[7];
			color = CH2_COLOR;
			AS = &AS2;
			break;
		case CH3_ID:
			x1 = 7 * FONTXSIZESMALL + CH3_XTextStart;
			x2 = 10 * FONTXSIZESMALL + CH3_XTextStart;
			ch = &ch3[7];
			color = CH3_COLOR;
			AS = &AS3;
			break;
		case CH4_ID:
			x1 = 7 * FONTXSIZESMALL + CH4_XTextStart;
			x2 = 10 * FONTXSIZESMALL + CH4_XTextStart;
			ch = &ch4[7];
			color = CH4_COLOR;
			AS = &AS4;
			break;
		default:
			x1 = 0;
			x2 = 0;
			ch = ((void*) 0);
			color = 0;
			AS = ((void*) 0);
			break;
	}
	switch (chx->coupling)
	{
		case CH_COUPLING_GND: //gnd
			ch[0] = 'G';
			ch[1] = 'N';
			ch[2] = 'D';
			//			***
			*AS |= 0b0001000000000000;
			*AS &= 0b1111001100000000;
			break;
		case CH_COUPLING_DC: //DC
			ch[0] = ' ';
			ch[1] = 'D';
			ch[2] = 'C';
			//			***
			*AS |= 0b0000010000000000;
			*AS &= 0b1110011100000000;
			break;
		case CH_COUPLING_AC: //AC
			ch[0] = ' ';
			ch[1] = 'A';
			ch[2] = 'C';
			//			***
			*AS |= 0b0000100000000000;
			*AS &= 0b1110101100000000;
			break;
		default:
			ch[0] = 'G';
			ch[1] = 'N';
			ch[2] = 'D';
			//			***
			*AS |= 0b0001000000000000;
			*AS &= 0b1111001100000000;
			break;
	}
	setSwitch(chx->id);
	LCD_RectFill(x1, CH1_YTextStart + 2, x2 - 3, RESY - 2, ILI9341_BLACK);
	LCD_Print(x1, CH1_YTextStart, ch, 3, color);
}

/**
 * @brief Re-print the trigger level when it is changed.
 * @param up_down 1 for trigger level increment
 * 				  0 for trigger level decrement
 */
static void triggerValueChangeRedraw(uint32_t up_down)
{
	channel_t *chx;
	int32_t val = 0;;
	switch (dso.triggerSource)
	{
		case CH1_ID:
			chx = &dso.ch1;
			break;
		case CH2_ID:
			chx = &dso.ch2;
			break;
		case CH3_ID:
			chx = &dso.ch3;
			break;
		case CH4_ID:
			chx = &dso.ch4;
			break;
		default:
			chx = &dso.ch4;
			break;
	}

	switch (chx->voltsDiv)
	{
		case v_100mDiv:

			if(up_down)
				dso.triggerValue += 27; // + 4 mV
			else
				dso.triggerValue -= 27; // - 4 mV

			val = (int32_t)dso.triggerValue - 2048;
			val = val * 4;
			val = val / 27;
			trig[13] = 'm';
			trig[14] = 'V';
			trig[15] = ' ';
			PutSignedInt((char *)(&trig[9]), ' ', 4, val);
			break;
		case v_200mDiv:
			if(up_down)
				dso.triggerValue += 27; // + 8 mV
			else
				dso.triggerValue -= 27; // - 8 mV

			val = (int32_t)dso.triggerValue - 2048;
			val = val * 8;
			val = val / 27;
			if((val < 1000) && (val > (-1000)))
			{
				trig[13] = 'm';
				trig[14] = 'V';
				trig[15] = ' ';
				PutSignedInt((char *)(&trig[9]), ' ', 4, val);
			}
			else
			{
				uint32_t x = 0;
				if(val < 0)
				{
					x = (uint32_t)(-val);
					trig[9] = '-';
				}
				else
				{
					trig[9] = ' ';
					x = (uint32_t)val;
				}
				trig[15] = ' ';
				trig[14] = 'V';
				x = x / 10;
				trig[13] = (uint8_t)((x % 10) + '0');
				x = x / 10;
				trig[12] = (uint8_t)((x % 10) + '0');
				trig[11] = '.';
				trig[10] = (uint8_t)((x / 10) + '0');
			}
			break;
		case v_500mDiv:
			if(up_down)
				dso.triggerValue += 27; // + 20 mV
			else
				dso.triggerValue -= 27; // - 20 mV

			val = (int32_t)dso.triggerValue - 2048;
			val = val * 20;
			val = val / 27;
			if((val < 1000) && (val > (-1000)))
			{
				trig[13] = 'm';
				trig[14] = 'V';
				trig[15] = ' ';
				PutSignedInt((char *)(&trig[9]), ' ', 4, val);
			}
			else
			{
				uint32_t x = 0;
				if(val < 0)
				{
					x = (uint32_t)(-val);
					trig[9] = '-';
				}
				else
				{
					trig[9] = ' ';
					x = (uint32_t)val;
				}
				trig[15] = ' ';
				trig[14] = 'V';
				x = x / 10;
				trig[13] = (uint8_t)((x % 10) + '0');
				x = x / 10;
				trig[12] = (uint8_t)((x % 10) + '0');
				trig[11] = '.';
				trig[10] = (uint8_t)((x / 10) + '0');
			}
			break;
		case v_1Div:
			if(up_down)
				dso.triggerValue += 27; // + 40 mV
			else
				dso.triggerValue -= 27; // - 40 mV

			val = (int32_t)dso.triggerValue - 2048;
			val = val * 40;
			val = val / 27;
			if((val < 1000) && (val > (-1000)))
			{
				trig[13] = 'm';
				trig[14] = 'V';
				trig[15] = ' ';
				PutSignedInt((char *)(&trig[9]), ' ', 4, val);
			}
			else
			{
				uint32_t x = 0;
				if(val < 0)
				{
					x = (uint32_t)(-val);
					trig[9] = '-';
				}
				else
				{
					trig[9] = ' ';
					x = (uint32_t)val;
				}
				trig[15] = ' ';
				trig[14] = 'V';
				x = x / 10;
				trig[13] = (uint8_t)((x % 10) + '0');
				x = x / 10;
				trig[12] = (uint8_t)((x % 10) + '0');
				trig[11] = '.';
				trig[10] = (uint8_t)((x / 10) + '0');
			}
			break;
		case v_2Div:
			if(up_down)
				dso.triggerValue += 27; // + 80 mV
			else
				dso.triggerValue -= 27; // - 80 mV

			val = (int32_t)dso.triggerValue - 2048;
			val = val * 80;
			val = val / 27;
			if((val < 1000) && (val > (-1000)))
			{
				trig[13] = 'm';
				trig[14] = 'V';
				trig[15] = ' ';
				PutSignedInt((char *)(&trig[9]), ' ', 4, val);
			}
			else
			{
				uint32_t x = 0;
				if(val < 0)
				{
					x = (uint32_t)(-val);
					trig[9] = '-';
				}
				else
				{
					trig[9] = ' ';
					x = (uint32_t)val;
				}
				trig[15] = ' ';
				trig[14] = 'V';
				x = x / 10;
				trig[13] = (uint8_t)((x % 10) + '0');
				x = x / 10;
				trig[12] = (uint8_t)((x % 10) + '0');
				trig[11] = '.';
				trig[10] = (uint8_t)((x / 10) + '0');
			}
			break;
		case v_5Div:
			if(up_down)
				dso.triggerValue += 27; // + 80 mV
			else
				dso.triggerValue -= 27; // - 80 mV

			val = (int32_t)dso.triggerValue - 2048;
			val = val * 200;
			val = val / 27;
			if((val < 1000) && (val > (-1000)))
			{
				trig[13] = 'm';
				trig[14] = 'V';
				trig[15] = ' ';
				PutSignedInt((char *)(&trig[9]), ' ', 4, val);
			}
			else
			{
				uint32_t x = 0;
				if(val < 0)
				{
					x = (uint32_t)(-val);
					trig[9] = '-';
				}
				else
				{
					trig[9] = ' ';
					x = (uint32_t)val;
				}
				if(x > 10000)
				{
					trig[15] = 'V';
					x = x / 10;
					trig[14] = (uint8_t)((x % 10) + '0');
					x = x / 10;
					trig[13] = (uint8_t)((x % 10) + '0');
					trig[12] = '.';
					x = x / 10;
					trig[11] = (uint8_t)((x % 10) + '0');
					trig[10] = (uint8_t)((x / 10) + '0');
				}
				else
				{
					trig[15] = ' ';
					trig[14] = 'V';
					x = x / 10;
					trig[13] = (uint8_t)((x % 10) + '0');
					x = x / 10;
					trig[12] = (uint8_t)((x % 10) + '0');
					trig[11] = '.';
					trig[10] = (uint8_t)((x / 10) + '0');
				}
			}
			break;
		case v_10Div:
			if(up_down)
				dso.triggerValue += 27; // + 80 mV
			else
				dso.triggerValue -= 27; // - 80 mV

			val = (int32_t)dso.triggerValue - 2048;
			val = val * 400;
			val = val / 27;
			if((val < 1000) && (val > (-1000)))
			{
				trig[13] = 'm';
				trig[14] = 'V';
				trig[15] = ' ';
				PutSignedInt((char *)(&trig[9]), ' ', 4, val);
			}
			else
			{
				uint32_t x = 0;
				if(val < 0)
				{
					x = (uint32_t)(-val);
					trig[9] = '-';
				}
				else
				{
					trig[9] = ' ';
					x = (uint32_t)val;
				}
				if(x > 10000)
				{
					trig[15] = 'V';
					x = x / 10;
					trig[14] = (uint8_t)((x % 10) + '0');
					x = x / 10;
					trig[13] = (uint8_t)((x % 10) + '0');
					trig[12] = '.';
					x = x / 10;
					trig[11] = (uint8_t)((x % 10) + '0');
					trig[10] = (uint8_t)((x / 10) + '0');
				}
				else
				{
					trig[15] = ' ';
					trig[14] = 'V';
					x = x / 10;
					trig[13] = (uint8_t)((x % 10) + '0');
					x = x / 10;
					trig[12] = (uint8_t)((x % 10) + '0');
					trig[11] = '.';
					trig[10] = (uint8_t)((x / 10) + '0');
				}
			}
			break;
		default:
			break;
	}
	LCD_RectFill(FONTXSIZESMALL * 9 + 1, 1, (TRIG_STR_LNGTH * FONTXSIZESMALL) - 2, FONTYSIZESMALL - 3, ILI9341_BLACK);
	LCD_Print(FONTXSIZESMALL * 9, 0, &trig[9], 6, ILI9341_YELLOW);
}

/**
 * @brief Increment/decrement an object
 * @param mode 1 - increment, 0 - decrement
 */
static void incDec(uint32_t mode)
{
	switch (selected)
	{
		default:
		case trig_src_selected: // trigger source
			if(mode)
			{
				if(dso.triggerSource == TRIG_SRC_CH4)
					dso.triggerSource = TRIG_SRC_CH1;
				else
					dso.triggerSource++;
			}
			else
			{
				if(dso.triggerSource == TRIG_SRC_CH1)
					dso.triggerSource = TRIG_SRC_CH4;
				else
					dso.triggerSource--;
			}
			triggerSourceChangeRedraw();
			break;

		case trig_edge_selected: // trigger edge
			toggle(&dso.triggerEdge);
			triggerEdgeChangeRedraw();
			break;

		case trig_val_selected: // trigger value
			if(mode)
			{
				triggerValueChangeRedraw(1);
			}
			else
			{
				triggerValueChangeRedraw(0);
			}
			break;

		case timediv_selected: // timeDiv
			if(mode)
			{
				dso.timeDiv++;
			}
			else
			{
				dso.timeDiv--;
			}
			timeDivChangeRedraw();
			channelChange();
			break;

		case ch1_vdiv_selected: // CH1 voltsDiv
			if(mode)
			{
				dso.ch1.voltsDiv++;
			}
			else
			{
				dso.ch1.voltsDiv--;
			}
			voltDivChangeRedraw(&dso.ch1);
			break;

		case ch1_cpl_selected: // CH1 coupling
			if(mode)
			{
				if(dso.ch1.coupling == CH_COUPLING_AC)
					dso.ch1.coupling = CH_COUPLING_GND;
				else
					dso.ch1.coupling++;
			}
			else
			{
				if(dso.ch1.coupling == CH_COUPLING_GND)
					dso.ch1.coupling = CH_COUPLING_AC;
				else
					dso.ch1.coupling--;
			}
			CouplingChangeRedraw(&dso.ch1);
			break;

		case ch2_vdiv_selected: // CH2 voltsDiv
			if(mode)
			{
				dso.ch2.voltsDiv++;
			}
			else
			{
				dso.ch2.voltsDiv--;
			}
			voltDivChangeRedraw(&dso.ch2);
			break;

		case ch2_cpl_selected: // CH2 coupling
			if(mode)
			{
				if(dso.ch2.coupling == CH_COUPLING_AC)
					dso.ch2.coupling = CH_COUPLING_GND;
				else
					dso.ch2.coupling++;
			}
			else
			{
				if(dso.ch2.coupling == CH_COUPLING_GND)
					dso.ch2.coupling = CH_COUPLING_AC;
				else
					dso.ch2.coupling--;
			}
			CouplingChangeRedraw(&dso.ch2);
			break;

		case ch3_vdiv_selected: // CH3 voltsDiv
			if(mode)
			{
				dso.ch3.voltsDiv++;
			}
			else
			{
				dso.ch3.voltsDiv--;
			}
			voltDivChangeRedraw(&dso.ch3);
			break;

		case ch3_cpl_selected: // CH3 coupling
			if(mode)
			{
				if(dso.ch3.coupling == CH_COUPLING_AC)
					dso.ch3.coupling = CH_COUPLING_GND;
				else
					dso.ch3.coupling++;
			}
			else
			{
				if(dso.ch3.coupling == CH_COUPLING_GND)
					dso.ch3.coupling = CH_COUPLING_AC;
				else
					dso.ch3.coupling--;
			}
			CouplingChangeRedraw(&dso.ch3);
			break;

		case ch4_vdiv_selected: // CH4 voltsDiv
			if(mode)
			{
				dso.ch4.voltsDiv++;
			}
			else
			{
				dso.ch4.voltsDiv--;
			}
			voltDivChangeRedraw(&dso.ch4);
			break;

		case ch4_cpl_selected: // CH4 coupling
			if(mode)
			{
				if(dso.ch4.coupling == CH_COUPLING_AC)
					dso.ch4.coupling = CH_COUPLING_GND;
				else
					dso.ch4.coupling++;
			}
			else
			{
				if(dso.ch4.coupling == CH_COUPLING_GND)
					dso.ch4.coupling = CH_COUPLING_AC;
				else
					dso.ch4.coupling--;
			}
			CouplingChangeRedraw(&dso.ch4);
			break;

			//ch1 vertical
		case ch1_selected:
			if(mode)
				dso.ch1.vertical += 4;
			else
				dso.ch1.vertical -= 4;
			break;

			//ch2 vertical
		case ch2_selected:
			if(mode)
				dso.ch2.vertical += 4;
			else
				dso.ch2.vertical -= 4;
			break;

			//ch3 vertical
		case ch3_selected:
			if(mode)
				dso.ch3.vertical += 4;
			else
				dso.ch3.vertical -= 4;
			break;

			//ch4 vertical
		case ch4_selected:
			if(mode)
				dso.ch4.vertical += 4;
			else
				dso.ch4.vertical -= 4;
			break;
	}
}
