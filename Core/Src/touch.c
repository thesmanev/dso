#include "main.h"

void timeDivChangeRedraw(void);
void triggerValueChangeRedraw(uint32_t up_down);
void SelectDeselect(uint32_t select);
void deselect(void);
void incDec(uint32_t mode);

static uint32_t selected = 0;

/**
 * @brief Setup touch detection
 */
void setupTouch(void)
{
	SPIx->CR1 = 0x002C; // disabled, baud-rate fpclk/32, master
	SPIx->CR2 = 0x0F04; // 16-bit, SS output disabled, RXNE at 1/2 full RX FIFO
	touch_readX();
	touch_readY();
}

/**
 * @brief Read the 12 bit ADC conversion for the Y axis of the touch panel
 * @return 0 - 4095
 */
uint32_t touch_readY(void)
{
	uint16_t y1, y2, y;
	SPIx->DR = touchReadYcmd; // send read X command
	CST_LOW;
	SPIx->CR1 = 0x006C; // enabled, baud-rate f-pclk/64, master
	while (!(SPIx->SR & SPI_SR_TXE)); // wait for receiving to complete
	while (!(SPIx->SR & SPI_SR_RXNE)); // wait for receiving to complete
	y1 = SPIx->DR; // read input
	SPIx->DR = 0; // send zero
	while (!(SPIx->SR & SPI_SR_TXE)); // wait for receiving to complete
	while (!(SPIx->SR & SPI_SR_RXNE)); // wait for receiving to complete
	y2 = SPIx->DR; // read input
	CST_HIGH;
	SPIx->CR1 = 0x002C; // disabled, baud-rate fpclk/64, master
	y = (uint32_t) (((y1 << 5) & 0x0FE0) + ((y2 >> 11) & 0x001F));
	return y;
}

/**
 * @brief Read the 12 bit ADC conversion for the Xs axis of the touch panel
 * @return 0 - 4095
 */
uint32_t touch_readX(void)
{
	uint16_t x1, x2, x;
	CST_LOW;
	SPIx->CR1 = 0x006C; // enabled, baud-rate f-pclk/64, master
	SPIx->DR = touchReadXcmd; // send read X command
	while (!(SPIx->SR & SPI_SR_TXE)); // wait for receiving to complete
	while (!(SPIx->SR & SPI_SR_RXNE)); // wait for receiving to complete
	x1 = SPIx->DR; // read input
	SPIx->DR = 0; // send zero
	while (!(SPIx->SR & SPI_SR_TXE)); // wait for receiving to complete
	while (!(SPIx->SR & SPI_SR_RXNE)); // wait for receiving to complete
	x2 = SPIx->DR; // read input
	CST_HIGH;
	SPIx->CR1 = 0x002C; // disabled, baud-rate fpclk/64, master
	x = (uint32_t) (((x1 << 5) & 0x0FE0) + ((x2 >> 11) & 0x001F));
	return x;
}

/**
 * @brief Get the Y pixel coordinate of the touch
 * @return The pixel coordinate
 */
uint32_t get_Y_touch(void)
{
	// X and Y of touch are inverted in relation to display (X is Y and vice versa)
	uint32_t X_TOUCH = touch_readX();
	int32_t pix;
	int32_t yt = 240 - ((X_TOUCH - 320) / 14);
	pix = (int32_t) yt;
	if (pix > 240)
		pix = 241;
	if (pix < 0)
		pix = 0;
	return (uint32_t) pix;
}

/**
 * @brief Get the X pixel coordinate of the touch
 * @return The pixel coordinate
 */
uint32_t get_X_touch(void)
{
	// X and Y of touch are inverted in relation to display (X is Y and vice versa)
	uint32_t Y_TOUCH = touch_readY();
	int32_t pix;
	int32_t xt = (Y_TOUCH - 490) / 10;
	pix = (int32_t) xt;
	if (pix > 320)
		pix = 321;
	if (pix < 0)
		pix = 0;
	return (uint32_t) pix;
}

/**
 * @brief Check for a touch on the screen and perform actions if a certain object has been touched
 */
void pollForTouch(void)
{
	uint32_t xt = 0, yt = 0;
	if (!(GPIOD->IDR & GPIO_IDR_15))
	{
		yt = get_Y_touch();
		xt = get_X_touch();

		// increment
		if ((yt >= 75) && (yt <= 137) && (xt >= 288) && (xt <= RESX))
		{
			currentButton = inc_pressed;
			if (currentButton != prevButton)
			{
				incDec(1);
			}
		}
		// decrement
		if ((yt >= 139) && (yt <= 201) && (xt >= 288) && (xt <= RESX))
		{
			currentButton = dec_pressed;
			if (currentButton != prevButton)
			{
				incDec(0);
			}
		}

		//trigger source
		if ((yt >= 0) && (yt <= (fontYSizeSmall))
				&& (xt >= (4 * fontXSizeSmall)) && (xt <= (7 * fontXSizeSmall)))
		{
			currentButton = trigSource_pressed;
			if (currentButton != prevButton)
			{
				if (selected != 1)
				{
					SelectDeselect(0);
					selected = 1;
					SelectDeselect(1);
				}
			}
		}

		//trigger edge
		if ((yt >= 0) && (yt <= (fontYSizeSmall))
				&& (xt >= (7 * fontXSizeSmall))
				&& (xt <= ((8 * fontXSizeSmall))))
		{
			currentButton = trigEdge_pressed;
			if (currentButton != prevButton)
			{
				if (selected != 2)
				{
					SelectDeselect(0);
					selected = 2;
					SelectDeselect(1);
				}
			}
		}

		//trigger value
		if ((yt >= 0) && (yt <= (fontYSizeSmall))
				&& (xt >= (8 * fontXSizeSmall))
				&& (xt <= (TRIG_STR_LNGTH * fontXSizeSmall)))
		{
			currentButton = trigValue_pressed;
			if (currentButton != prevButton)
			{
				if (selected != 4)
				{
					SelectDeselect(0);
					selected = 4;
					SelectDeselect(1);
				}
			}
		}

		//timeDiv
		if ((yt >= 0) && (yt <= (fontYSizeSmall))
				&& (xt >= (TRIG_STR_LNGTH * fontXSizeSmall))
				&& (xt
						<= ((TDIV_STR_LNGTH * fontXSizeSmall)
								+ (TRIG_STR_LNGTH * fontXSizeSmall))))
		{
			currentButton = timeDiv_pressed;
			if (currentButton != prevButton)
			{
				if (selected != 8)
				{
					SelectDeselect(0);
					selected = 8;
					SelectDeselect(1);
				}
			}
		}

		//CH1 voltsDiv
		if ((yt >= CH1_YTextStart) && (yt <= MAX_Y)
				&& (xt >= (2 * fontXSizeSmall + CH1_XTextStart))
				&& (xt <= (6 * fontXSizeSmall + CH2_XTextStart)))
		{
			currentButton = ch1VDiv_pressed;
			if (currentButton != prevButton)
			{
				if (selected != 16)
				{
					SelectDeselect(0);
					selected = 16;
					SelectDeselect(1);
				}
			}
		}

		//ch1 coupling
		if ((yt >= CH1_YTextStart) && (yt <= MAX_Y)
				&& (xt >= (6 * fontXSizeSmall + CH1_XTextStart))
				&& (xt <= (9 * fontXSizeSmall + CH2_XTextStart)))
		{
			currentButton = ch1Cpl_pressed;
			if (currentButton != prevButton)
			{
				if (selected != 32)
				{
					SelectDeselect(0);
					selected = 32;
					SelectDeselect(1);
				}
			}
		}

		//CH2 voltsDiv
		if ((yt >= CH1_YTextStart) && (yt <= MAX_Y)
				&& (xt >= (2 * fontXSizeSmall + CH2_XTextStart))
				&& (xt <= (6 * fontXSizeSmall + CH2_XTextStart)))
		{
			currentButton = ch2VDiv_pressed;
			if (currentButton != prevButton)
			{
				if (selected != 64)
				{
					SelectDeselect(0);
					selected = 64;
					SelectDeselect(1);
				}
			}
		}

		//ch2 coupling
		if ((yt >= CH1_YTextStart) && (yt <= MAX_Y)
				&& (xt >= (6 * fontXSizeSmall + CH2_XTextStart))
				&& (xt <= (9 * fontXSizeSmall + CH2_XTextStart)))
		{
			currentButton = ch2Cpl_pressed;
			if (currentButton != prevButton)
			{
				if (selected != 128)
				{
					SelectDeselect(0);
					selected = 128;
					SelectDeselect(1);
				}
			}
		}

		//CH3 voltsDiv
		if ((yt >= CH1_YTextStart) && (yt <= MAX_Y)
				&& (xt >= (2 * fontXSizeSmall + CH3_XTextStart))
				&& (xt <= (6 * fontXSizeSmall + CH3_XTextStart)))
		{
			currentButton = ch3VDiv_pressed;
			if (currentButton != prevButton)
			{
				if (selected != 256)
				{
					SelectDeselect(0);
					selected = 256;
					SelectDeselect(1);
				}
			}
		}

		//ch3 coupling
		if ((yt >= CH1_YTextStart) && (yt <= MAX_Y)
				&& (xt >= (6 * fontXSizeSmall + CH3_XTextStart))
				&& (xt <= (9 * fontXSizeSmall + CH3_XTextStart)))
		{
			currentButton = ch3Cpl_pressed;
			if (currentButton != prevButton)
			{
				if (selected != 512)
				{
					SelectDeselect(0);
					selected = 512;
					SelectDeselect(1);
				}
			}
		}

		//CH4 voltsDiv
		if ((yt >= CH1_YTextStart) && (yt <= MAX_Y)
				&& (xt >= (2 * fontXSizeSmall + CH4_XTextStart))
				&& (xt <= (5 * fontXSizeSmall + CH4_XTextStart)))
		{
			currentButton = ch4VDiv_pressed;
			if (currentButton != prevButton)
			{
				if (selected != 1024)
				{
					SelectDeselect(0);
					selected = 1024;
					SelectDeselect(1);
				}
			}
		}

		//ch4 coupling
		if ((yt >= CH1_YTextStart) && (yt <= MAX_Y)
				&& (xt >= (6 * fontXSizeSmall + CH4_XTextStart))
				&& (xt <= (9 * fontXSizeSmall + CH4_XTextStart)))
		{
			currentButton = ch4Cpl_pressed;
			if (currentButton != prevButton)
			{
				if (selected != 2048)
				{
					SelectDeselect(0);
					selected = 2048;
					SelectDeselect(1);
				}
			}
		}

		// ch1
		if ((yt <= MAX_Y) && (yt >= CH1_YTextStart) && (xt >= CH1_XTextStart)
				&& (xt <= CH1_XTextStart + 16))
		{
			currentButton = ch1_pressed;
			if (currentButton != prevButton)
			{
				if (dso1.ch1.selected)
				{
					dso1.ch1.enabled = 0;
					dso1.ch1.selected = 0;
					LCD_RectFill(CH1_XTextStart, CH1_YTextStart,
							CH1_XTextStart + 16,
							(CH1_YTextStart + fontYSizeSmall - 2),
							ILI9341_BLACK);
					LCD_Print(CH1_XTextStart, CH1_YTextStart, ch1, 2,
							ILI9341_GREEN);
					channelChange();
				}
				else
				{
					if (dso1.ch1.enabled)
					{
						if (selected != 4096)
						{
							SelectDeselect(0);
							selected = 4096;
							SelectDeselect(1);
						}
						dso1.ch1.selected = 1;
						dso1.ch2.selected = 0;
						dso1.ch3.selected = 0;
						dso1.ch4.selected = 0;
						deselect();
						LCD_DrawRect(CH1_XTextStart, CH1_YTextStart,
								CH1_XTextStart + 16,
								(CH1_YTextStart + fontYSizeSmall - 2),
								ILI9341_WHITE);
					}
					else
					{
						dso1.ch1.enabled = 1;
						dso1.ch1.selected = 0;
						LCD_RectFill(CH1_XTextStart, CH1_YTextStart,
								CH1_XTextStart + 16,
								(CH1_YTextStart + fontYSizeSmall - 2),
								ILI9341_GREEN);
						LCD_Print(CH1_XTextStart, CH1_YTextStart, ch1, 2,
								ILI9341_BLACK);
						channelChange();
					}
				}
			}
		}

		// ch2
		if ((yt <= MAX_Y) && (yt >= CH2_YTextStart) && (xt >= CH2_XTextStart)
				&& (xt <= CH2_XTextStart + 16))
		{
			currentButton = ch2_pressed;
			if (currentButton != prevButton)
			{
				if (dso1.ch2.selected)
				{
					dso1.ch2.enabled = 0;
					dso1.ch2.selected = 0;
					LCD_RectFill(CH2_XTextStart, CH2_YTextStart,
							CH2_XTextStart + 16,
							(CH2_YTextStart + fontYSizeSmall - 2),
							ILI9341_BLACK);
					LCD_Print(CH2_XTextStart, CH2_YTextStart, ch2, 2,
							ILI9341_BLUE);
					channelChange();
				}
				else
				{
					if (dso1.ch2.enabled)
					{
						if (selected != 8192)
						{
							SelectDeselect(0);
							selected = 8192;
							SelectDeselect(1);
						}
						dso1.ch1.selected = 0;
						dso1.ch2.selected = 1;
						dso1.ch3.selected = 0;
						dso1.ch4.selected = 0;
						deselect();
						LCD_DrawRect(CH2_XTextStart, CH2_YTextStart,
								CH2_XTextStart + 16,
								(CH2_YTextStart + fontYSizeSmall - 2),
								ILI9341_WHITE);

					}
					else
					{
						dso1.ch2.enabled = 1;
						dso1.ch2.selected = 0;
						LCD_RectFill(CH2_XTextStart, CH2_YTextStart,
								CH2_XTextStart + 16,
								(CH2_YTextStart + fontYSizeSmall - 2),
								ILI9341_BLUE);
						LCD_Print(CH2_XTextStart, CH2_YTextStart, ch2, 2,
								ILI9341_BLACK);
						channelChange();
					}
				}
			}
		}

		// ch3
		if ((yt <= MAX_Y) && (yt >= CH3_YTextStart) && (xt >= CH3_XTextStart)
				&& (xt <= CH3_XTextStart + 16))
		{
			currentButton = ch3_pressed;
			if (currentButton != prevButton)
			{
				if (dso1.ch3.selected)
				{
					dso1.ch3.enabled = 0;
					dso1.ch3.selected = 0;
					LCD_RectFill(CH3_XTextStart, CH3_YTextStart,
							CH3_XTextStart + 16,
							(CH3_YTextStart + fontYSizeSmall - 2),
							ILI9341_BLACK);
					LCD_Print(CH3_XTextStart, CH3_YTextStart, ch3, 2,
							ILI9341_RED);
					channelChange();
				}
				else
				{
					if (dso1.ch3.enabled)
					{
						if (selected != 16384)
						{
							SelectDeselect(0);
							selected = 16384;
							SelectDeselect(1);
						}
						dso1.ch1.selected = 0;
						dso1.ch2.selected = 0;
						dso1.ch3.selected = 1;
						dso1.ch4.selected = 0;
						deselect();
						LCD_DrawRect(CH3_XTextStart, CH3_YTextStart,
								CH3_XTextStart + 16,
								(CH3_YTextStart + fontYSizeSmall - 2),
								ILI9341_WHITE);
					}
					else
					{
						dso1.ch3.enabled = 1;
						dso1.ch3.selected = 0;
						LCD_RectFill(CH3_XTextStart, CH3_YTextStart,
								CH3_XTextStart + 16,
								(CH3_YTextStart + fontYSizeSmall - 2),
								ILI9341_RED);
						LCD_Print(CH3_XTextStart, CH3_YTextStart, ch3, 2,
								ILI9341_BLACK);
						channelChange();
					}
				}
			}
		}

		// ch4
		if ((yt <= MAX_Y) && (yt >= CH4_YTextStart) && (xt >= CH4_XTextStart)
				&& (xt <= (CH4_XTextStart + 16)))
		{
			currentButton = ch4_pressed;
			if (currentButton != prevButton)
			{
				if (dso1.ch4.selected)
				{
					dso1.ch4.enabled = 0;
					dso1.ch4.selected = 0;
					LCD_RectFill(CH4_XTextStart, CH4_YTextStart,
							CH4_XTextStart + 16,
							(CH3_YTextStart + fontYSizeSmall - 2),
							ILI9341_BLACK);
					LCD_Print(CH4_XTextStart, CH4_YTextStart, ch4, 2,
							ILI9341_ORANGE);
					channelChange();
				}
				else
				{
					if (dso1.ch4.enabled)
					{
						if (selected != 32768)
						{
							SelectDeselect(0);
							selected = 32768;
							SelectDeselect(1);
						}
						dso1.ch1.selected = 0;
						dso1.ch2.selected = 0;
						dso1.ch3.selected = 0;
						dso1.ch4.selected = 1;
						deselect();
						LCD_DrawRect(CH4_XTextStart, CH4_YTextStart,
								CH4_XTextStart + 16,
								(CH3_YTextStart + fontYSizeSmall - 2),
								ILI9341_WHITE);
					}
					else
					{
						dso1.ch4.enabled = 1;
						dso1.ch4.selected = 0;
						LCD_RectFill(CH4_XTextStart, CH4_YTextStart,
								CH4_XTextStart + 16,
								(CH3_YTextStart + fontYSizeSmall - 2),
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
 * @brief Select/deselect a touch object
 * @param select 1 - select
 * 				 0 - deselect
 */
void SelectDeselect(uint32_t select)
{
	switch (selected)
	{
	case 1: // trigger source
		if (select)
			LCD_DrawRect(5 * fontXSizeSmall - 1, 0, 8 * fontXSizeSmall,
					fontYSizeSmall - 2, ILI9341_WHITE);
		else
			LCD_DrawRect(5 * fontXSizeSmall - 1, 0, 8 * fontXSizeSmall,
					fontYSizeSmall - 2, ILI9341_BLACK);
		break;

	case 2: // trigger edge
		if (select)
			LCD_DrawRect(8 * fontXSizeSmall, 0, 9 * fontXSizeSmall,
					fontYSizeSmall - 2, ILI9341_WHITE);
		else
			LCD_DrawRect(8 * fontXSizeSmall, 0, 9 * fontXSizeSmall,
					fontYSizeSmall - 2, ILI9341_BLACK);
		break;

	case 4: // trigger value
		if (select)
			LCD_DrawRect(9 * fontXSizeSmall, 0,
					TRIG_STR_LNGTH * fontXSizeSmall - 1, fontYSizeSmall - 2,
					ILI9341_WHITE);
		else
			LCD_DrawRect(9 * fontXSizeSmall, 0,
					TRIG_STR_LNGTH * fontXSizeSmall - 1, fontYSizeSmall - 2,
					ILI9341_BLACK);
		break;

	case 8: // timeDiv
		if (select)
			LCD_DrawRect(TRIG_STR_LNGTH * fontXSizeSmall - 1, 0,
					(TRIG_STR_LNGTH * fontXSizeSmall)
							+ (TDIV_STR_LNGTH * fontXSizeSmall),
					fontYSizeSmall - 2, ILI9341_WHITE);
		else
			LCD_DrawRect(TRIG_STR_LNGTH * fontXSizeSmall - 1, 0,
					(TRIG_STR_LNGTH * fontXSizeSmall)
							+ (TDIV_STR_LNGTH * fontXSizeSmall),
					fontYSizeSmall - 2, ILI9341_BLACK);
		break;

	case 16: // CH1 voltsDiv
		if (select)
			LCD_DrawRect(CH1_XTextStart + (2 * fontXSizeSmall) - 1,
					CH1_YTextStart, CH1_XTextStart + (7 * fontXSizeSmall) - 2,
					(CH1_YTextStart + fontYSizeSmall - 2), ILI9341_WHITE);
		else
			LCD_DrawRect(CH1_XTextStart + (2 * fontXSizeSmall) - 1,
					CH1_YTextStart, CH1_XTextStart + (7 * fontXSizeSmall) - 2,
					(CH1_YTextStart + fontYSizeSmall - 2), ILI9341_BLACK);
		break;

	case 32: // CH1 coupling
		if (select)
			LCD_DrawRect(CH1_XTextStart + (7 * fontXSizeSmall) - 1,
					CH1_YTextStart, CH1_XTextStart + (10 * fontXSizeSmall) - 2,
					(CH1_YTextStart + fontYSizeSmall - 2), ILI9341_WHITE);
		else
			LCD_DrawRect(CH1_XTextStart + (7 * fontXSizeSmall) - 1,
					CH1_YTextStart, CH1_XTextStart + (10 * fontXSizeSmall) - 2,
					(CH1_YTextStart + fontYSizeSmall - 2), ILI9341_BLACK);
		break;

	case 64: // CH2 voltsDiv
		if (select)
			LCD_DrawRect(CH2_XTextStart + (2 * fontXSizeSmall) - 1,
					CH2_YTextStart, CH2_XTextStart + (7 * fontXSizeSmall) - 2,
					(CH2_YTextStart + fontYSizeSmall - 2), ILI9341_WHITE);
		else
			LCD_DrawRect(CH2_XTextStart + (2 * fontXSizeSmall) - 1,
					CH2_YTextStart, CH2_XTextStart + (7 * fontXSizeSmall) - 2,
					(CH2_YTextStart + fontYSizeSmall - 2), ILI9341_BLACK);
		break;

	case 128: // CH2 coupling
		if (select)
			LCD_DrawRect(CH2_XTextStart + (7 * fontXSizeSmall) - 1,
					CH2_YTextStart, CH2_XTextStart + (10 * fontXSizeSmall) - 2,
					(CH2_YTextStart + fontYSizeSmall - 2), ILI9341_WHITE);
		else
			LCD_DrawRect(CH2_XTextStart + (7 * fontXSizeSmall) - 1,
					CH2_YTextStart, CH2_XTextStart + (10 * fontXSizeSmall) - 2,
					(CH2_YTextStart + fontYSizeSmall - 2), ILI9341_BLACK);
		break;

	case 256: // CH3 voltsDiv
		if (select)
			LCD_DrawRect(CH3_XTextStart + (2 * fontXSizeSmall) - 1,
					CH3_YTextStart, CH3_XTextStart + (7 * fontXSizeSmall) - 2,
					(CH3_YTextStart + fontYSizeSmall - 2), ILI9341_WHITE);
		else
			LCD_DrawRect(CH3_XTextStart + (2 * fontXSizeSmall) - 1,
					CH3_YTextStart, CH3_XTextStart + (7 * fontXSizeSmall) - 2,
					(CH3_YTextStart + fontYSizeSmall - 2), ILI9341_BLACK);
		break;

	case 512: // CH3 coupling
		if (select)
			LCD_DrawRect(CH3_XTextStart + (7 * fontXSizeSmall) - 1,
					CH3_YTextStart, CH3_XTextStart + (10 * fontXSizeSmall) - 2,
					(CH3_YTextStart + fontYSizeSmall - 2), ILI9341_WHITE);
		else
			LCD_DrawRect(CH3_XTextStart + (7 * fontXSizeSmall) - 1,
					CH3_YTextStart, CH3_XTextStart + (10 * fontXSizeSmall) - 2,
					(CH3_YTextStart + fontYSizeSmall - 2), ILI9341_BLACK);
		break;

	case 1024: // CH4 voltsDiv
		if (select)
			LCD_DrawRect(CH4_XTextStart + (2 * fontXSizeSmall) - 1,
					CH4_YTextStart, CH4_XTextStart + (7 * fontXSizeSmall) - 2,
					(CH4_YTextStart + fontYSizeSmall - 2), ILI9341_WHITE);
		else
			LCD_DrawRect(CH4_XTextStart + (2 * fontXSizeSmall) - 1,
					CH4_YTextStart, CH4_XTextStart + (7 * fontXSizeSmall) - 2,
					(CH4_YTextStart + fontYSizeSmall - 2), ILI9341_BLACK);
		break;

	case 2048: // CH4 coupling
		if (select)
			LCD_DrawRect(CH4_XTextStart + (7 * fontXSizeSmall) - 1,
					CH4_YTextStart, CH4_XTextStart + (10 * fontXSizeSmall) - 2,
					(CH4_YTextStart + fontYSizeSmall - 2), ILI9341_WHITE);
		else
			LCD_DrawRect(CH4_XTextStart + (7 * fontXSizeSmall) - 1,
					CH4_YTextStart, CH4_XTextStart + (10 * fontXSizeSmall) - 2,
					(CH4_YTextStart + fontYSizeSmall - 2), ILI9341_BLACK);
		break;

	default:
		break;
	}
}

/**
 * @brief Function for de-selecting a channel
 */
void deselect(void)
{
	if (!dso1.ch1.selected)
	{
		if (dso1.ch1.enabled)
		{
			LCD_DrawRect(CH1_XTextStart, CH1_YTextStart,
					CH1_XTextStart + (2 * fontXSizeSmall),
					(CH1_YTextStart + fontYSizeSmall - 2), CH1_COLOR);
		}
	}

	if (!dso1.ch2.selected)
	{
		if (dso1.ch2.enabled)
		{
			LCD_DrawRect(CH2_XTextStart, CH2_YTextStart,
					CH2_XTextStart + (2 * fontXSizeSmall),
					(CH2_YTextStart + fontYSizeSmall - 2), CH2_COLOR);
		}
	}

	if (!dso1.ch3.selected)
	{
		if (dso1.ch3.enabled)
		{
			LCD_DrawRect(CH3_XTextStart, CH3_YTextStart,
					CH3_XTextStart + (2 * fontXSizeSmall),
					(CH3_YTextStart + fontYSizeSmall - 2), CH3_COLOR);
		}
	}

	if (!dso1.ch4.selected)
	{
		if (dso1.ch4.enabled)
		{
			LCD_DrawRect(CH4_XTextStart, CH4_YTextStart,
					CH4_XTextStart + (2 * fontXSizeSmall),
					(CH3_YTextStart + fontYSizeSmall - 2), CH4_COLOR);
		}
	}
}

/**
 * @brief Re-draw(re-print) Time-base when it is changed
 */
void timeDivChangeRedraw(void)
{
	switch (dso1.timeDiv)
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
	LCD_RectFill((TRIG_STR_LNGTH + 9) * fontXSizeSmall - 1, 1,
			(TRIG_STR_LNGTH + TDIV_STR_LNGTH) * fontXSizeSmall - 1,
			fontYSizeSmall - 3, ILI9341_BLACK);
	LCD_Print((TRIG_STR_LNGTH + 9) * fontXSizeSmall, 0, &tDiv[9], 5,
			ILI9341_YELLOW);
}

/**
 * @brief Re-draw(re-print) Volts per division when it is changed
 * @param chx Pointer to a channel structure.
 */
void voltDivChangeRedraw(channel *chx)
{
	uint32_t x1, x2, color;
	uint16_t *AS;
	char *ch;

	switch (chx->id)
	{
	case 1:
		x1 = 2 * fontXSizeSmall;
		x2 = 7 * fontXSizeSmall;
		ch = &ch1[2];
		color = CH1_COLOR;
		AS = &AS1;
		break;
	case 2:
		x1 = 2 * fontXSizeSmall + CH2_XTextStart;
		x2 = 7 * fontXSizeSmall + CH2_XTextStart;
		ch = &ch2[2];
		color = CH2_COLOR;
		AS = &AS2;
		break;
	case 3:
		x1 = 2 * fontXSizeSmall + CH3_XTextStart;
		x2 = 7 * fontXSizeSmall + CH3_XTextStart;
		ch = &ch3[2];
		color = CH3_COLOR;
		AS = &AS3;
		break;
	case 4:
		x1 = 2 * fontXSizeSmall + CH4_XTextStart;
		x2 = 7 * fontXSizeSmall + CH4_XTextStart;
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
void triggerSourceChangeRedraw()
{
	switch (dso1.triggerSource)
	{
	case 1:
		trig[7] = '1';
		break;
	case 2:
		trig[7] = '2';
		break;
	case 3:
		trig[7] = '3';
		break;
	case 4:
		trig[7] = '4';
		break;
	}
	LCD_RectFill((7 * fontXSizeSmall), 1, (8 * fontXSizeSmall) - 1,
			fontYSizeSmall - 3, ILI9341_BLACK);
	LCD_Print((7 * fontXSizeSmall), 0, &trig[7], 1, ILI9341_YELLOW);
}

/**
 * @brief Re-draw the trigger edge when it is changed.
 */
void triggerEdgeChangeRedraw()
{
	if (dso1.triggerEdge)
		trig[8] = 127; //rising
	else
		trig[8] = 128; //falling

	LCD_RectFill((8 * fontXSizeSmall) + 1, 1, (9 * fontXSizeSmall) - 1,
			fontYSizeSmall - 3, ILI9341_BLACK);
	LCD_Print((8 * fontXSizeSmall), 0, &trig[8], 1, ILI9341_YELLOW);
}

/**
 * @brief Re-print the coupling for the channel when it is changed.
 * @param chx Pointer to a channel structure
 */
void CouplingChangeRedraw(channel *chx)
{
	uint32_t x1, x2, color;
	char *ch;
	uint16_t *AS;

	switch (chx->id)
	{
	case 1:
		x1 = 7 * fontXSizeSmall;
		x2 = 10 * fontXSizeSmall;
		ch = &ch1[7];
		color = CH1_COLOR;
		AS = &AS1;
		break;
	case 2:
		x1 = 7 * fontXSizeSmall + CH2_XTextStart;
		x2 = 10 * fontXSizeSmall + CH2_XTextStart;
		ch = &ch2[7];
		color = CH2_COLOR;
		AS = &AS2;
		break;
	case 3:
		x1 = 7 * fontXSizeSmall + CH3_XTextStart;
		x2 = 10 * fontXSizeSmall + CH3_XTextStart;
		ch = &ch3[7];
		color = CH3_COLOR;
		AS = &AS3;
		break;
	case 4:
		x1 = 7 * fontXSizeSmall + CH4_XTextStart;
		x2 = 10 * fontXSizeSmall + CH4_XTextStart;
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
	case 0: //gnd
		ch[0] = 'G';
		ch[1] = 'N';
		ch[2] = 'D';
		//			***
		*AS |= 0b0001000000000000;
		*AS &= 0b1111001100000000;
		break;
	case 1: //DC
		ch[0] = ' ';
		ch[1] = 'D';
		ch[2] = 'C';
		//			***
		*AS |= 0b0000010000000000;
		*AS &= 0b1110011100000000;
		break;
	case 2: //AC
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
void triggerValueChangeRedraw(uint32_t up_down)
{
	channel *chx;
	int32_t val;
	switch (dso1.triggerSource)
	{
	case 1:
		chx = &dso1.ch1;
		break;
	case 2:
		chx = &dso1.ch2;
		break;
	case 3:
		chx = &dso1.ch3;
		break;
	case 4:
		chx = &dso1.ch4;
		break;
	default:
		chx = &dso1.ch4;
		break;
	}

	switch (chx->voltsDiv)
	{
	case v_100mDiv:

		if (up_down)
			dso1.triggerValue += 27; // + 4 mV
		else
			dso1.triggerValue -= 27; // - 4 mV

		val = dso1.triggerValue - 2048;
		val = val * 4;
		val = val / 27;
		trig[13] = 'm';
		trig[14] = 'V';
		trig[15] = ' ';
		PutSignedInt(&trig[9], ' ', 4, val);
		break;
	case v_200mDiv:
		if (up_down)
			dso1.triggerValue += 27; // + 8 mV
		else
			dso1.triggerValue -= 27; // - 8 mV

		val = dso1.triggerValue - 2048;
		val = val * 8;
		val = val / 27;
		if ((val < 1000) && (val > (-1000)))
		{
			trig[13] = 'm';
			trig[14] = 'V';
			trig[15] = ' ';
			PutSignedInt(&trig[9], ' ', 4, val);
		}
		else
		{
			uint32_t x = 0;
			if (val < 0)
			{
				x = -val;
				trig[9] = '-';
			}
			else
			{
				trig[9] = ' ';
				x = val;
			}
			trig[15] = ' ';
			trig[14] = 'V';
			x = x / 10;
			trig[13] = (uint8_t) (x % 10) + '0';
			x = x / 10;
			trig[12] = (uint8_t) (x % 10) + '0';
			trig[11] = '.';
			trig[10] = (uint8_t) (x / 10) + '0';
		}
		break;
	case v_500mDiv:
		if (up_down)
			dso1.triggerValue += 27; // + 20 mV
		else
			dso1.triggerValue -= 27; // - 20 mV

		val = dso1.triggerValue - 2048;
		val = val * 20;
		val = val / 27;
		if ((val < 1000) && (val > (-1000)))
		{
			trig[13] = 'm';
			trig[14] = 'V';
			trig[15] = ' ';
			PutSignedInt(&trig[9], ' ', 4, val);
		}
		else
		{
			uint32_t x = 0;
			if (val < 0)
			{
				x = -val;
				trig[9] = '-';
			}
			else
			{
				trig[9] = ' ';
				x = val;
			}
			trig[15] = ' ';
			trig[14] = 'V';
			x = x / 10;
			trig[13] = (uint8_t) (x % 10) + '0';
			x = x / 10;
			trig[12] = (uint8_t) (x % 10) + '0';
			trig[11] = '.';
			trig[10] = (uint8_t) (x / 10) + '0';
		}
		break;
	case v_1Div:
		if (up_down)
			dso1.triggerValue += 27; // + 40 mV
		else
			dso1.triggerValue -= 27; // - 40 mV

		val = dso1.triggerValue - 2048;
		val = val * 40;
		val = val / 27;
		if ((val < 1000) && (val > (-1000)))
		{
			trig[13] = 'm';
			trig[14] = 'V';
			trig[15] = ' ';
			PutSignedInt(&trig[9], ' ', 4, val);
		}
		else
		{
			uint32_t x = 0;
			if (val < 0)
			{
				x = -val;
				trig[9] = '-';
			}
			else
			{
				trig[9] = ' ';
				x = val;
			}
			trig[15] = ' ';
			trig[14] = 'V';
			x = x / 10;
			trig[13] = (uint8_t) (x % 10) + '0';
			x = x / 10;
			trig[12] = (uint8_t) (x % 10) + '0';
			trig[11] = '.';
			trig[10] = (uint8_t) (x / 10) + '0';
		}
		break;
	case v_2Div:
		if (up_down)
			dso1.triggerValue += 27; // + 80 mV
		else
			dso1.triggerValue -= 27; // - 80 mV

		val = dso1.triggerValue - 2048;
		val = val * 80;
		val = val / 27;
		if ((val < 1000) && (val > (-1000)))
		{
			trig[13] = 'm';
			trig[14] = 'V';
			trig[15] = ' ';
			PutSignedInt(&trig[9], ' ', 4, val);
		}
		else
		{
			uint32_t x = 0;
			if (val < 0)
			{
				x = -val;
				trig[9] = '-';
			}
			else
			{
				trig[9] = ' ';
				x = val;
			}
			trig[15] = ' ';
			trig[14] = 'V';
			x = x / 10;
			trig[13] = (uint8_t) (x % 10) + '0';
			x = x / 10;
			trig[12] = (uint8_t) (x % 10) + '0';
			trig[11] = '.';
			trig[10] = (uint8_t) (x / 10) + '0';
		}
		break;
	case v_5Div:
		if (up_down)
			dso1.triggerValue += 27; // + 80 mV
		else
			dso1.triggerValue -= 27; // - 80 mV

		val = dso1.triggerValue - 2048;
		val = val * 200;
		val = val / 27;
		if ((val < 1000) && (val > (-1000)))
		{
			trig[13] = 'm';
			trig[14] = 'V';
			trig[15] = ' ';
			PutSignedInt(&trig[9], ' ', 4, val);
		}
		else
		{
			uint32_t x = 0;
			if (val < 0)
			{
				x = -val;
				trig[9] = '-';
			}
			else
			{
				trig[9] = ' ';
				x = val;
			}
			if (x > 10000)
			{
				trig[15] = 'V';
				x = x / 10;
				trig[14] = (uint8_t) (x % 10) + '0';
				x = x / 10;
				trig[13] = (uint8_t) (x % 10) + '0';
				trig[12] = '.';
				x = x / 10;
				trig[11] = (uint8_t) (x % 10) + '0';
				trig[10] = (uint8_t) (x / 10) + '0';
			}
			else
			{
				trig[15] = ' ';
				trig[14] = 'V';
				x = x / 10;
				trig[13] = (uint8_t) (x % 10) + '0';
				x = x / 10;
				trig[12] = (uint8_t) (x % 10) + '0';
				trig[11] = '.';
				trig[10] = (uint8_t) (x / 10) + '0';
			}
		}
		break;
	case v_10Div:
		if (up_down)
			dso1.triggerValue += 27; // + 80 mV
		else
			dso1.triggerValue -= 27; // - 80 mV

		val = dso1.triggerValue - 2048;
		val = val * 400;
		val = val / 27;
		if ((val < 1000) && (val > (-1000)))
		{
			trig[13] = 'm';
			trig[14] = 'V';
			trig[15] = ' ';
			PutSignedInt(&trig[9], ' ', 4, val);
		}
		else
		{
			uint32_t x = 0;
			if (val < 0)
			{
				x = -val;
				trig[9] = '-';
			}
			else
			{
				trig[9] = ' ';
				x = val;
			}
			if (x > 10000)
			{
				trig[15] = 'V';
				x = x / 10;
				trig[14] = (uint8_t) (x % 10) + '0';
				x = x / 10;
				trig[13] = (uint8_t) (x % 10) + '0';
				trig[12] = '.';
				x = x / 10;
				trig[11] = (uint8_t) (x % 10) + '0';
				trig[10] = (uint8_t) (x / 10) + '0';
			}
			else
			{
				trig[15] = ' ';
				trig[14] = 'V';
				x = x / 10;
				trig[13] = (uint8_t) (x % 10) + '0';
				x = x / 10;
				trig[12] = (uint8_t) (x % 10) + '0';
				trig[11] = '.';
				trig[10] = (uint8_t) (x / 10) + '0';
			}
		}
		break;
	default:
		break;
	}
	LCD_RectFill(fontXSizeSmall * 9 + 1, 1,
			(TRIG_STR_LNGTH * fontXSizeSmall) - 2, fontYSizeSmall - 3,
			ILI9341_BLACK);
	LCD_Print(fontXSizeSmall * 9, 0, &trig[9], 6, ILI9341_YELLOW);
}

/**
 * @brief Increment/decrement an object
 * @param mode 1 - increment, 0 - decrement
 */
void incDec(uint32_t mode)
{
	switch (selected)
	{
	case 1: // trigger source
		if (mode)
		{
			if (dso1.triggerSource == 4)
				dso1.triggerSource = 1;
			else
				dso1.triggerSource++;
		}
		else
		{
			if (dso1.triggerSource == 1)
				dso1.triggerSource = 4;
			else
				dso1.triggerSource--;
		}
		triggerSourceChangeRedraw();
		break;

	case 2: // trigger edge
		toggle(&dso1.triggerEdge);
		triggerEdgeChangeRedraw();
		break;

	case 4: // trigger value
		if (mode)
		{
			triggerValueChangeRedraw(1);
		}
		else
		{
			triggerValueChangeRedraw(0);
		}
		break;

	case 8: // timeDiv
		if (mode)
		{
			dso1.timeDiv++;
		}
		else
		{
			dso1.timeDiv--;
		}
		timeDivChangeRedraw();
		channelChange();
		break;

	case 16: // CH1 voltsDiv
		if (mode)
		{
			dso1.ch1.voltsDiv++;
		}
		else
		{
			dso1.ch1.voltsDiv--;
		}
		voltDivChangeRedraw(&dso1.ch1);
		break;

	case 32: // CH1 coupling
		if (mode)
		{
			if (dso1.ch1.coupling == COUPLING_AC)
				dso1.ch1.coupling = COUPLING_GND;
			else
				dso1.ch1.coupling++;
		}
		else
		{
			if (dso1.ch1.coupling == COUPLING_GND)
				dso1.ch1.coupling = COUPLING_AC;
			else
				dso1.ch1.coupling--;
		}
		CouplingChangeRedraw(&dso1.ch1);
		break;

	case 64: // CH2 voltsDiv
		if (mode)
		{
			dso1.ch2.voltsDiv++;
		}
		else
		{
			dso1.ch2.voltsDiv--;
		}
		voltDivChangeRedraw(&dso1.ch2);
		break;

	case 128: // CH2 coupling
		if (mode)
		{
			if (dso1.ch2.coupling == COUPLING_AC)
				dso1.ch2.coupling = COUPLING_GND;
			else
				dso1.ch2.coupling++;
		}
		else
		{
			if (dso1.ch2.coupling == COUPLING_GND)
				dso1.ch2.coupling = COUPLING_AC;
			else
				dso1.ch2.coupling--;
		}
		CouplingChangeRedraw(&dso1.ch2);
		break;

	case 256: // CH3 voltsDiv
		if (mode)
		{
			dso1.ch3.voltsDiv++;
		}
		else
		{
			dso1.ch3.voltsDiv--;
		}
		voltDivChangeRedraw(&dso1.ch3);
		break;

	case 512: // CH3 coupling
		if (mode)
		{
			if (dso1.ch3.coupling == COUPLING_AC)
				dso1.ch3.coupling = COUPLING_GND;
			else
				dso1.ch3.coupling++;
		}
		else
		{
			if (dso1.ch3.coupling == COUPLING_GND)
				dso1.ch3.coupling = COUPLING_AC;
			else
				dso1.ch3.coupling--;
		}
		CouplingChangeRedraw(&dso1.ch3);
		break;

	case 1024: // CH4 voltsDiv
		if (mode)
		{
			dso1.ch4.voltsDiv++;
		}
		else
		{
			dso1.ch4.voltsDiv--;
		}
		voltDivChangeRedraw(&dso1.ch4);
		break;

	case 2048: // CH4 coupling
		if (mode)
		{
			if (dso1.ch4.coupling == COUPLING_AC)
				dso1.ch4.coupling = COUPLING_GND;
			else
				dso1.ch4.coupling++;
		}
		else
		{
			if (dso1.ch4.coupling == COUPLING_GND)
				dso1.ch4.coupling = COUPLING_AC;
			else
				dso1.ch4.coupling--;
		}
		CouplingChangeRedraw(&dso1.ch4);
		break;

		//ch1 vertical
	case 4096:
		if (mode)
			dso1.ch1.vertical += 4;
		else
			dso1.ch1.vertical -= 4;
		break;

		//ch2 vertical
	case 8192:
		if (mode)
			dso1.ch2.vertical += 4;
		else
			dso1.ch2.vertical -= 4;
		break;

		//ch3 vertical
	case 16384:
		if (mode)
			dso1.ch3.vertical += 4;
		else
			dso1.ch3.vertical -= 4;
		break;

		//ch4 vertical
	case 32768:
		if (mode)
			dso1.ch4.vertical += 4;
		else
			dso1.ch4.vertical -= 4;
		break;
	}
}
