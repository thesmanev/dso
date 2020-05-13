#include "main.h"

static int32_t P_Graphic2D_sgn(int32_t x);

/* Initial screen test strings */
uint8_t ch1[CH_STR_LNGTH] = { '1', ':', ' ', ' ', '1', 'V', ' ', ' ', 'D', 'C' };
uint8_t ch2[CH_STR_LNGTH] = { '2', ':', ' ', ' ', '1', 'V', ' ', ' ', 'D', 'C' };
uint8_t ch3[CH_STR_LNGTH] = { '3', ':', ' ', ' ', '1', 'V', ' ', ' ', 'D', 'C' };
uint8_t ch4[CH_STR_LNGTH] = { '4', ':', ' ', ' ', '1', 'V', ' ', ' ', 'D', 'C' };
uint8_t trig[TRIG_STR_LNGTH] = { 'T', 'r', 'i', 'g', ':', 'C', 'H', '1', 128, ' ', '0', '.', '0', '0', 'V', ' ' };
uint8_t tDiv[TDIV_STR_LNGTH] = { 'T', 'i', 'm', 'e', '/', 'D', 'i', 'v', ':', ' ', '5', '0', 'u', 's' };

/**
 * @brief Initialize LCD and LCD driver chip (ILI9341)
 */
void LCD_Init(void)
{
	// VCI=2.8V
	RD_HIGH;
	CSD_HIGH;
	WR_HIGH;
	RS_HIGH;

	//************* Reset LCD Driver ****************//
	RESET_HIGH;
	delay_ms(1);      // Delay 1ms
	RESET_LOW;
	delay_ms(10);       // Delay 10ms // This delay time is necessary
	RESET_HIGH;
	delay_ms(120);       // Delay 120 ms

	LCD_WriteCommand(ILI9341_SLPOUT);    //Exit Sleep
	delay_ms(120);
	//************* Start Initial Sequence **********//
	LCD_WriteCommand(ILI9341_PCA); //power control A
	LCD_WriteData(0x00000039);
	LCD_WriteData(0x0000002c);
	LCD_WriteData(0x00000000);
	LCD_WriteData(0x00000034); // set vcore to 1.6V
	LCD_WriteData(0x00000002); // set ddvdh to 5.6V

	LCD_WriteCommand(ILI9341_PCB); // power control B
	LCD_WriteData(0x00000000);
	LCD_WriteData(0x000000C1); // PCEQ enabled
	LCD_WriteData(0x00000030); // default

	LCD_WriteCommand(ILI9341_DTCAI); // driver timing control A internal
	LCD_WriteData(0x00000085); // overlap: default + 1 unit
	LCD_WriteData(0x00000000); // CR and EQU timing to 1 unit
	LCD_WriteData(0x00000078); // pre-charge timing set to 2 unit

	LCD_WriteCommand(ILI9341_DTCB); // driver timing control B
	LCD_WriteData(0x00000000); // EQ to GND
	LCD_WriteData(0x00000000);

	LCD_WriteCommand(ILI9341_POSC); // power on sequence control
	LCD_WriteData(0x00000064); // CP1, CP23: soft start keep 1 frame
	LCD_WriteData(0x00000003); // VCL enabled on 1st frame, ddvdh enabled on 4th frame
	LCD_WriteData(0X00000012); // VGH enabled on 2nd frame, VGL enabled on 3rd frame
	LCD_WriteData(0X00000081); // ddvdh enhance mode on

	LCD_WriteCommand(ILI9341_PRC); // pump control ratio
	LCD_WriteData(0x00000020); // set ddvdh to 2*VCI

	LCD_WriteCommand(ILI9341_PWCTR1);    //Power control 1
	LCD_WriteData(0x00000023);   // set GVDD to 4.60V

	LCD_WriteCommand(ILI9341_PWCTR2);    //Power control 2
	LCD_WriteData(0x00000010);   //VGH = VCI*7, VGL = - 4*VCI

	LCD_WriteCommand(ILI9341_VMCTR1);    //VCM control
	LCD_WriteData(0x0000003E); // set VCOMH to 4.225V
	LCD_WriteData(0x00000028); // set VCOML to -1.5V

	LCD_WriteCommand(ILI9341_VMCTR2);    //VCM control2
	LCD_WriteData(0x00000086); // offset enabled, VCOM offset set to VMH-58

	LCD_WriteCommand(ILI9341_MADCTL);    // Memory Access Control
	LCD_WriteData(0x00000C8); // MV set to 0,(landscape, x->320, y->240)
	LCD_WriteCommand(ILI9341_PIXFMT); // pixel format
	LCD_WriteData(0x00000055);   // 16Bits per pixel RGB 565

	LCD_WriteCommand(ILI9341_FRMCTR1); // frame rate control (normal mode/full colors)
	LCD_WriteData(0x00000000); // division ratio for internal clocks when normal mode set to f-osc
	LCD_WriteData(0x00000018); // 79Hz ( 24 clocks per line)

	LCD_WriteCommand(ILI9341_DFUNCTR);    // Display Function Control
	LCD_WriteData(0x00000008);    // ......
	LCD_WriteData(0x00000082); // normally white display, source and gate output scan line set to S(G)1 -> S(G)-> 720(320)
	//	ISC set to 5 frames
	LCD_WriteData(0x00000027); // number of lines set to 320

	//LCD_WriteCommand(0xF6);
	//LCD_WriteData(0x01);
	//LCD_WriteData(0x30);

	LCD_WriteCommand(ILI9341_E3G);    // 3Gamma Function Disable
	LCD_WriteData(0x00000000);

//	LCD_WriteCommand(ILI9341_GAMMASET);    //Gamma curve selected
//	LCD_WriteData(0x00000001); // gamma curve 1 selected (2.2)
//
//	LCD_WriteCommand(ILI9341_GMCTRP1);    //Set Gamma
//	LCD_WriteData(0x0000000F);
//	LCD_WriteData(0x00000026);
//	LCD_WriteData(0x00000022);
//	LCD_WriteData(0x0000000A);
//	LCD_WriteData(0x00000010);
//	LCD_WriteData(0x0000000A);
//	LCD_WriteData(0x0000004C);
//	LCD_WriteData(0X000000CA);
//	LCD_WriteData(0x00000036);
//	LCD_WriteData(0x00000000);
//	LCD_WriteData(0x00000015);
//	LCD_WriteData(0x00000000);
//	LCD_WriteData(0x00000010);
//	LCD_WriteData(0x00000010);
//	LCD_WriteData(0x00000000);
//
//	LCD_WriteCommand(ILI9341_GMCTRN1);    //Set Gamma
//	LCD_WriteData(0x00000000);
//	LCD_WriteData(0x00000019);
//	LCD_WriteData(0x0000001B);
//	LCD_WriteData(0x00000005);
//	LCD_WriteData(0x0000000F);
//	LCD_WriteData(0x00000005);
//	LCD_WriteData(0x00000033);
//	LCD_WriteData(0x00000035);
//	LCD_WriteData(0x00000049);
//	LCD_WriteData(0x0000000F);
//	LCD_WriteData(0x0000001F);
//	LCD_WriteData(0x0000000F);
//	LCD_WriteData(0x0000003F);
//	LCD_WriteData(0x0000003F);
//	LCD_WriteData(0x0000000F);

	LCD_WriteCommand(ILI9341_SLPOUT);    //Exit Sleep
	delay_ms(120);

	LCD_WriteCommand(ILI9341_DISPON);    //Display on
	displayInit();
}

/**
 * @brief Draw initial screen
 */
void displayInit(void)
{
	LCD_Background(ILI9341_BLACK);

	LCD_RectFill(288, 75, 321, 137, ILI9341_LIGHTGREY);
	LCD_RectFill(289, 139, 321, 201, ILI9341_LIGHTGREY);
	LCD_DrawLineX(292, 120, 304, 90, ILI9341_BLACK);
	LCD_DrawLineX(304, 90, 316, 120, ILI9341_BLACK);
	LCD_DrawLineX(292, 150, 304, 180, ILI9341_BLACK);
	LCD_DrawLineX(304, 180, 316, 150, ILI9341_BLACK);

	LCD_DrawReticle();

	LCD_Print(0, 0, trig, TRIG_STR_LNGTH, ILI9341_YELLOW);
	LCD_Print(TRIG_STR_LNGTH * FONTXSIZESMALL, 0, tDiv, TDIV_STR_LNGTH,
			ILI9341_YELLOW);
	LCD_Print(CH1_XTextStart, CH1_YTextStart, ch1, CH_STR_LNGTH, CH1_COLOR);
	LCD_Print(CH2_XTextStart, CH2_YTextStart, ch2, CH_STR_LNGTH, CH2_COLOR);
	LCD_Print(CH3_XTextStart, CH3_YTextStart, ch3, CH_STR_LNGTH, CH3_COLOR);
	LCD_Print(CH4_XTextStart, CH4_YTextStart, ch4, CH_STR_LNGTH, CH4_COLOR);
}

/**
 * @brief Function for sending commands to the ILI9341
 * @param command The command to be sent. Check macros in disp.h
 */
void LCD_WriteCommand(uint32_t command)
{
//	RS_COMMAND;//!
//	CSD_LOW;
//	DATA_PORT->ODR = command;
//	WR_STROBE;
//	CSD_HIGH;
	//optimized
	DATA_PORT->ODR = command;
	CONTROL_PORT->BSRR = 0x001C0000;
	CONTROL_PORT->BSRR = 0x00000014;
}

/**
 * @brief Send data to the ILI9341
 * @param data The 16 bits of data to be sent.
 */
void LCD_WriteData(uint32_t data)
{
//	RS_DATA;//!
//	CSD_LOW;
//	DATA_PORT->ODR = data;
//	WR_STROBE;
//	CSD_HIGH;
	//optimized
	DATA_PORT->ODR = data;
	CONTROL_PORT->BSRR = 0x00140008;
	CONTROL_PORT->BSRR = 0x00000014;
}

/**
 * @brief Set a pixel on the display to the specified color
 * @param x X coordinate of the pixel
 * @param y Y coordinate of the pixel
 * @param color The pixel color. Check disp.h macros for guidance.
 */
void LCD_PutPixel(uint32_t x, uint32_t y, uint32_t color)
{
	LCD_WriteCommand(ILI9341_PASET);
	LCD_WriteData(x >> 8);
	LCD_WriteData(x & 0x000000ff);
	LCD_WriteData((x + 1) >> 8);
	LCD_WriteData((x + 1) & 0x000000ff);

	LCD_WriteCommand(ILI9341_CASET);
	LCD_WriteData(y >> 8);
	LCD_WriteData(y & 0x000000ff);
	LCD_WriteData((y + 1) >> 8);
	LCD_WriteData((y + 1) & 0x000000ff);

	LCD_WriteCommand(ILI9341_RAMWR); // write data
	LCD_WriteData(color);
}

/**
 * @brief Set a window where pixels are going to be modified
 * @param x0 X coordinate of the top-left of the window rectangle
 * @param y0 Y coordinate of the top-left of the window rectangle
 * @param x1 X coordinate of the bottom-right of the window rectangle
 * @param y1 Y coordinate of the bottom-right of the window rectangle
 */
void LCD_SetWindow(uint32_t x0, uint32_t y0, uint32_t x1, uint32_t y1)
{
//	if (x1 >= MAX_X)
//	{
//		x1 = MAX_X - 1;
//	}
//	if (x0 >= MAX_X)
//	{
//		x0 = MAX_X - 1;
//	}
//	if (y1 >= MAX_Y)
//	{
//		y1 = MAX_Y - 1;
//	}
//	if (y0 >= MAX_Y)
//	{
//		y0 = MAX_Y - 1;
//	}

	LCD_WriteCommand(ILI9341_PASET);
	LCD_WriteData(x0 >> 8);
	LCD_WriteData(x0 & 0x000000ff);
	LCD_WriteData(x1 >> 8);
	LCD_WriteData(x1 & 0x000000ff);

	LCD_WriteCommand(ILI9341_CASET);
	LCD_WriteData(y0 >> 8);
	LCD_WriteData(y0 & 0x000000ff);
	LCD_WriteData(y1 >> 8);
	LCD_WriteData(y1 & 0x000000ff);
}

/**
 * @brief Draw a horizontal line with a width of 1 pixel
 * @param x0 Starting coordinate on the X axis
 * @param x1 End coordinate on the X axis
 * @param y Y coordinate of the line
 * @param color line color
 */
void LCD_DrawLineH(uint32_t x0, uint32_t x1, uint32_t y, uint32_t color)
{
	LCD_SetWindow(x0, y, x1, y);
	uint32_t Xlength = x1 - x0 + 1;
	LCD_WriteCommand(ILI9341_RAMWR); // write data
	while(Xlength)
	{
		LCD_WriteData(color);
		Xlength--;
	}
}

/**
 * @brief Draw a vertical line with a width of 1 pixel.
 * @param y0 Starting Y coordinate of the line
 * @param y1 End Y coordinate of the line
 * @param x X coordinate of the line
 * @param color Color of the line
 * @note This function has no safety checks.
 */
void LCD_DrawLineVX(uint32_t y0, uint32_t y1, uint32_t x, uint32_t color)
{
	if (y0 > y1)
	{
		uint32_t temp = y0;
		y0 = y1;
		y1 = temp;
	}
	LCD_SetWindow(x, y0, x, y1);
	uint32_t Xlength = y1 - y0 + 1;
	LCD_WriteCommand(ILI9341_RAMWR); // write data
	while (Xlength)
	{
		LCD_WriteData(color);
		Xlength--;
	}
}

/**
 * @brief Draw a vertical line with a width of 1 pixel within the reticle.
 * @param y0 Starting Y coordinate of the line
 * @param y1 End Y coordinate of the line
 * @param x X coordinate of the line
 * @param color Color of the line
 */
void LCD_DrawLineV(uint32_t y0, uint32_t y1, uint32_t x, uint32_t color)
{
	// check Limit
	if (y0 > 227)
		y0 = 227;

	if (y0 < 12)
		y0 = 12;

	if (y1 > 227)
		y1 = 227;

	if (y1 < 12)
		y1 = 12;

	if (y0 == y1)
	{
		LCD_PutPixel(x, y0, color);
		return;
	}

	if (y0 > y1)
	{
		uint32_t temp = y0;
		y0 = y1;
		y1 = temp;
	}
	LCD_SetWindow(x, y0, x, y1);
	uint32_t Xlength = y1 - y0 + 1;
	LCD_WriteCommand(ILI9341_RAMWR); // write data
	while (Xlength)
	{
		LCD_WriteData(color);
		Xlength--;
	}
}

/**
 * @brief, function for setting the background color, or clearing the whole screen with a color.
 * @param color The color.
 */
void LCD_Background(uint32_t color)
{
	LCD_SetWindow(0, 0, RESX, RESY);
	uint32_t cnt = 76800;
	LCD_WriteCommand(ILI9341_RAMWR); // write data
	while (cnt)
	{
		LCD_WriteData(color);
		cnt--;
	}
}

/**
 * @brief Print text on the screen
 * @param x X coordinate of the upper left corner of the text
 * @param y Y coordinate of the upper lefr corner of the text
 * @param text Pointer to a char array containing the text.
 * @param length The number of characters to be printed out
 * @param color The color of the text
 */
void LCD_Print(uint32_t x, uint32_t y, uint8_t *text, uint32_t length,
		uint32_t color)
{
	uint32_t l;
	uint32_t i;
	uint32_t j;
	uint8_t *ptr = text;
	uint8_t line;
	for (l = 0; l < length; l++)
	{
		for (j = 0; j < FONTYSIZESMALL; j++)
		{
			line = smallFont[((uint32_t)(*ptr - 32) * FONTYSIZESMALL) + j];
			for (i = 0; i < FONTXSIZESMALL; i++)
			{
				if (line & 0x80)
				{
					LCD_PutPixel((x + (l * (FONTXSIZESMALL)) + i), y + j,
							color);
				}
				line = (uint8_t)(line << 1u);
			}
		}
		ptr++;
	}
}

/**
 * @brief Draw a filled rectangle on the screen
 * @param x0 X coordinate of the upper left corner
 * @param y0 Y coordinate of the upper left corner
 * @param x1 X coordinate of the lower right corner
 * @param y1 Y coordinate of the lower right corner
 * @param color Color of the rectangle
 */
void LCD_RectFill(uint32_t x0, uint32_t y0, uint32_t x1, uint32_t y1,
		uint32_t color)
{
	LCD_SetWindow(x0, y0, x1, y1);
	uint32_t cnt = (x1 - x0 + 1) * (y1 - y0 + 1);
	LCD_WriteCommand(ILI9341_RAMWR); // write data
	while (cnt)
	{
		LCD_WriteData(color);
		cnt--;
	}
}

/**
 * @brief Draw a rectangle on the screen.
 * @param x0 X coordinate of the upper left corner
 * @param y0 Y coordinate of the upper left corner
 * @param x1 X coordinate of the lower right corner
 * @param y1 Y coordinate of the lower right corner
 * @param color Color of the line forming the rectangle.
 */
void LCD_DrawRect(uint32_t x0, uint32_t y0, uint32_t x1, uint32_t y1,
		uint32_t color)
{
	LCD_DrawLineH(x0, x1, y0, color);
	LCD_DrawLineH(x0, x1, y1, color);
	LCD_DrawLineVX(y0, y1, x0, color);
	LCD_DrawLineVX(y0, y1, x1, color);
}

/**
 * @brief Draw the oscilloscope reticle
 */
void LCD_DrawReticle(void)
{
	LCD_DrawRect(7, 11, 288, 228, ILI9341_WHITE);
	LCD_DrawLineH(7, 288, 47, ILI9341_WHITE); // x -> 35 pix
	LCD_DrawLineH(7, 288, 83, ILI9341_WHITE); // y -> 36 pix
	LCD_DrawLineH(7, 288, 119, ILI9341_WHITE);
	LCD_DrawLineH(7, 288, 155, ILI9341_WHITE);
	LCD_DrawLineH(7, 288, 191, ILI9341_WHITE);
	LCD_DrawLineVX(11, 228, 42, ILI9341_WHITE);
	LCD_DrawLineVX(11, 228, 77, ILI9341_WHITE);
	LCD_DrawLineVX(11, 228, 112, ILI9341_WHITE);
	LCD_DrawLineVX(11, 228, 147, ILI9341_WHITE);
	LCD_DrawLineVX(11, 228, 182, ILI9341_WHITE);
	LCD_DrawLineVX(11, 228, 217, ILI9341_WHITE);
	LCD_DrawLineVX(11, 228, 252, ILI9341_WHITE);
}

/**
 * @brief Draw a line within the oscilloscope reticle
 * @param x0 Starting X coordinate
 * @param y0 Starting Y coordinate
 * @param x1 Ending X coordinate
 * @param y1 Ending Y coordinate
 * @param color Color of the line
 */
void LCD_DrawLine(uint32_t x0, uint32_t y0, uint32_t x1, uint32_t y1,
		uint32_t color)
{

	int32_t x, y, t, dx, dy, incx, incy, pdx, pdy, ddx, ddy, es, el, err;

//	  // check Limit
	if (x0 >= 288)
		x0 = 288 - 1;
	if (y0 >= 228)
		y0 = 228 - 1;

	if (x0 <= 7)
		x0 = 8;
	if (y0 <= 11)
		y0 = 12;

	if (x1 >= 288)
		x1 = 288 - 1;
	if (y1 >= 228)
		y1 = 228 - 1;

	if (x1 <= 7)
		x1 = 8;
	if (y1 <= 11)
		y1 = 12;

	dx = (int32_t)x1 - (int32_t)x0;
	dy = (int32_t)y1 - (int32_t)y0;

	incx = P_Graphic2D_sgn(dx);
	incy = P_Graphic2D_sgn(dy);
	if (dx < 0)
		dx = -dx;
	if (dy < 0)
		dy = -dy;

	if (dx > dy)
	{
		pdx = incx;
		pdy = 0;
		ddx = incx;
		ddy = incy;
		es = dy;
		el = dx;
	}
	else
	{
		pdx = 0;
		pdy = incy;
		ddx = incx;
		ddy = incy;
		es = dx;
		el = dy;
	}

	x = (int32_t)x0;
	y = (int32_t)y0;
	err = (el >> 1);
	LCD_PutPixel((uint32_t)x, (uint32_t)y, color);

	for (t = 0; t < el; ++t)
	{
		err -= es;
		if (err < 0)
		{
			err += el;
			x += ddx;
			y += ddy;
		}
		else
		{
			x += pdx;
			y += pdy;
		}
		LCD_PutPixel((uint32_t)x, (uint32_t)y, color);
	}
}

/**
 * @brief Draw a line on the screen
 * @param x0 Starting X coordinate
 * @param y0 Starting Y coordinate
 * @param x1 Ending X coordinate
 * @param y1 Ending Y coordinate
 * @param color Color of the line
 */
void LCD_DrawLineX(uint32_t x0, uint32_t y0, uint32_t x1, uint32_t y1,
		uint32_t color)
{
	int32_t x, y, t, dx, dy, incx, incy, pdx, pdy, ddx, ddy, es, el, err;

//	  check  Limit
	if (x0 >= 320)
		x0 = 319;
	if (y0 >= 240)
		y0 = 239;

	if (x1 >= 320)
		x1 = 319;
	if (y1 >= 240)
		y1 = 239;

	dx = (int32_t)x1 - (int32_t)x0;
	dy = (int32_t)y1 - (int32_t)y0;

	incx = P_Graphic2D_sgn(dx);
	incy = P_Graphic2D_sgn(dy);
	if (dx < 0)
		dx = -dx;
	if (dy < 0)
		dy = -dy;

	if (dx > dy)
	{
		pdx = incx;
		pdy = 0;
		ddx = incx;
		ddy = incy;
		es = dy;
		el = dx;
	}
	else
	{
		pdx = 0;
		pdy = incy;
		ddx = incx;
		ddy = incy;
		es = dx;
		el = dy;
	}

	x = (int32_t)x0;
	y = (int32_t)y0;
	err = (el >> 1);
	LCD_PutPixel((uint32_t)x, (uint32_t)y, color);

	for (t = 0; t < el; ++t)
	{
		err -= es;
		if (err < 0)
		{
			err += el;
			x += ddx;
			y += ddy;
		}
		else
		{
			x += pdx;
			y += pdy;
		}
		LCD_PutPixel((uint32_t)x, (uint32_t)y, color);
	}
}

/**
 * @brief Function for determining sign of the input variable
 * @param x Input
 * @return 1 when x > 0
 * 		   0 when x = 0
 * 		   -1 when x < 0
 */
static int32_t P_Graphic2D_sgn(int32_t x)
{
	return (x > 0) ? 1 : (x < 0) ? -1 : 0;
}
