#include "disp.h"

char ch1[CH_STR_LNGTH] = {'1',':',' ',' ','1','V',' ',' ','D','C'};
char ch2[CH_STR_LNGTH] = {'2',':',' ',' ','1','V',' ',' ','D','C'};
char ch3[CH_STR_LNGTH] = {'3',':',' ',' ','1','V',' ',' ','D','C'};
char ch4[CH_STR_LNGTH] = {'4',':',' ',' ','1','V',' ',' ','D','C'};
char trig[TRIG_STR_LNGTH] = {'T', 'r', 'i', 'g', ':', 'C', 'H', '1', 128, ' ', '0', '.', '0', '0', 'V', ' '};
char tDiv[TDIV_STR_LNGTH] = {'T', 'i', 'm', 'e', '/', 'D', 'i', 'v', ':', ' ', '5', '0', 'u', 's'};

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
	LCD_WriteData(0x00000078);// pre-charge timing set to 2 unit

	LCD_WriteCommand(ILI9341_DTCB); // driver timing control B
	LCD_WriteData(0x00000000); // EQ to GND
	LCD_WriteData(0x00000000);

	LCD_WriteCommand(ILI9341_POSC); // power on sequence control
	LCD_WriteData(0x00000064); // CP1, CP23: soft start keep 1 frame
	LCD_WriteData(0x00000003); // VCL enabled on 1st frame, ddvdh enabled on 4th frame
	LCD_WriteData(0X00000012); // VGH enabled on 2nd frame, VGL enabled on 3rd frame
	LCD_WriteData(0X00000081); // ddvdh enhance mode on

	LCD_WriteCommand(ILI9341_PRC);// pump control ratio
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
	LCD_WriteData(0x00000018);// 79Hz ( 24 clocks per line)

	LCD_WriteCommand(ILI9341_DFUNCTR);    // Display Function Control
	LCD_WriteData(0x00000008);// ......
	LCD_WriteData(0x00000082);// normally white display, source and gate output scan line set to S(G)1 -> S(G)-> 720(320)
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

void displayInit(void){
	LCD_Background(ILI9341_BLACK);
//	char x[4], y[4];
//	aquireData4ch();
	LCD_RectFill(288, 75, 321, 137, ILI9341_LIGHTGREY);
	LCD_RectFill(289, 139, 321, 201, ILI9341_LIGHTGREY);
//	LCD_DrawRect(289, 75, 321, 137, ILI9341_WHITE);
//	LCD_DrawRect(289, 138, 321, 200, ILI9341_WHITE);
	LCD_DrawLineX(292, 120, 304, 90, ILI9341_BLACK);
	LCD_DrawLineX(304, 90, 316, 120, ILI9341_BLACK);
	LCD_DrawLineX(292, 150, 304, 180, ILI9341_BLACK);
	LCD_DrawLineX(304, 180, 316, 150, ILI9341_BLACK);

	LCD_DrawReticle();
//	PutSignedInt(x, 'h', 3, cnt1);
//	PutUnsignedInt(x, ' ', 4, dso1.triggerValue);
//	PutUnsignedInt(y, ' ', 1, dso1.timeDiv);
	LCD_Print(0, 0, trig, TRIG_STR_LNGTH, ILI9341_YELLOW);
	LCD_Print(TRIG_STR_LNGTH*fontXSizeSmall, 0, tDiv, TDIV_STR_LNGTH, ILI9341_YELLOW);
	LCD_Print(CH1_XTextStart, CH1_YTextStart, ch1, CH_STR_LNGTH, CH1_COLOR);
	LCD_Print(CH2_XTextStart, CH2_YTextStart, ch2, CH_STR_LNGTH, CH2_COLOR);
	LCD_Print(CH3_XTextStart, CH3_YTextStart, ch3, CH_STR_LNGTH, CH3_COLOR);
	LCD_Print(CH4_XTextStart, CH4_YTextStart, ch4, CH_STR_LNGTH, CH4_COLOR);
//	LCD_DrawLine(17, 17, 220, 220, ILI9341_CYAN);
//	LCD_DrawLine(17, 160, 220, 17, ILI9341_CYAN);
}

void LCD_WriteCommand(uint32_t command){
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
void LCD_WriteData(uint32_t data){
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

void LCD_PutPixel(uint32_t x, uint32_t y, uint32_t color)
{
	LCD_WriteCommand(ILI9341_PASET);
	LCD_WriteData(x>>8);
	LCD_WriteData(x&0x000000ff);
	LCD_WriteData((x+1)>>8);
	LCD_WriteData((x+1)&0x000000ff);

	LCD_WriteCommand(ILI9341_CASET);
	LCD_WriteData(y>>8);
	LCD_WriteData(y&0x000000ff);
	LCD_WriteData((y+1)>>8);
	LCD_WriteData((y+1)&0x000000ff);

//	LCD_SetWindow(x, y, x+1, y+1);

	LCD_WriteCommand(ILI9341_RAMWR); // write data
	LCD_WriteData(color);
}

void LCD_SetWindow(uint32_t x0, uint32_t y0, uint32_t x1, uint32_t y1){
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
	LCD_WriteData(x0>>8);
	LCD_WriteData(x0&0x000000ff);
	LCD_WriteData(x1>>8);
	LCD_WriteData(x1&0x000000ff);

	LCD_WriteCommand(ILI9341_CASET);
	LCD_WriteData(y0>>8);
	LCD_WriteData(y0&0x000000ff);
	LCD_WriteData(y1>>8);
	LCD_WriteData(y1&0x000000ff);
}

void LCD_DrawLineH(uint32_t x0, uint32_t x1, uint32_t y, uint32_t color){
	LCD_SetWindow(x0, y, x1, y);
	int Xlength = x1 - x0 + 1;
	LCD_WriteCommand(ILI9341_RAMWR); // write data
	while(Xlength){
		LCD_WriteData(color);
		Xlength--;
	}
}

void LCD_DrawLineVX(uint32_t y0, uint32_t y1, uint32_t x, uint32_t color){
	if(y0>y1){
		uint32_t temp = y0;
		y0 = y1;
		y1 = temp;
	}
	LCD_SetWindow(x, y0, x, y1);
	uint32_t Xlength = y1 - y0 + 1;
	LCD_WriteCommand(ILI9341_RAMWR); // write data
	while(Xlength){
		LCD_WriteData(color);
		Xlength--;
	}
}

void LCD_DrawLineV(uint32_t y0, uint32_t y1, uint32_t x, uint32_t color){

//	if(x>=288) x=288-1;
	if(y0>227) y0=227;

//	if(x<=7) x=8;
	if(y0<12) y0=12;

	// check auf Limit
	if(y1>227) y1=227;

	if(y1<12) y1=12;

	if(y0 == y1){
		LCD_PutPixel(x, y0, color);
		return;
	}

	if(y0>y1){
		uint32_t temp = y0;
		y0 = y1;
		y1 = temp;
	}
	LCD_SetWindow(x, y0, x, y1);
	uint32_t Xlength = y1 - y0 + 1;
	LCD_WriteCommand(ILI9341_RAMWR); // write data
	while(Xlength){
		LCD_WriteData(color);
		Xlength--;
	}
}

void LCD_Background(uint32_t color){
	LCD_SetWindow(0, 0, RESX, RESY);
	uint32_t cnt = 76800;
	LCD_WriteCommand(ILI9341_RAMWR); // write data
	while(cnt){
		LCD_WriteData(color);
		cnt--;
	}
}


int LCD_Print(uint32_t x, uint32_t y, char *text, uint32_t length, uint32_t color){
//	if(length*fontXSizeSmall + x > RESX)
//		return 1;
//	if(fontYSizeSmall + y > RESY)
//		return 2;
	uint32_t l;
	uint32_t i;
	uint32_t j;
	char *ptr = text;
	uint8_t line;
	for(l = 0; l < length; l++){
		for(j = 0; j < fontYSizeSmall; j++){
			line = smallFont[((*ptr - 32)*fontYSizeSmall)+j];
//			line = console_font_6x8[((*ptr)*fontYSizeSmall)+j];
			for(i = 0; i < fontXSizeSmall; i++){
					if(line & 0x80){
						LCD_PutPixel((x+(l*(fontXSizeSmall))+i), y+j, color);
					}
					line = line << 1;
			}
		}
		ptr++;
	}
	return 0;
}

void LCD_RectFill(uint32_t x0, uint32_t y0, uint32_t x1, uint32_t y1, uint32_t color){
	LCD_SetWindow(x0, y0, x1, y1);
	uint32_t cnt = (x1-x0+1)*(y1-y0+1);
	LCD_WriteCommand(ILI9341_RAMWR); // write data
	while(cnt){
		LCD_WriteData(color);
		cnt--;
	}
}
void LCD_DrawRect(uint32_t x0, uint32_t y0, uint32_t x1, uint32_t y1, uint32_t color){
	LCD_DrawLineH(x0, x1, y0, color);
	LCD_DrawLineH(x0, x1, y1, color);
	LCD_DrawLineVX(y0, y1, x0, color);
	LCD_DrawLineVX(y0, y1, x1, color);
}

//void LCD_DrawLine(uint32_t x0, uint32_t y0, uint32_t x1, uint32_t y1, uint32_t color){
//
//}

void LCD_DrawReticle(void){
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

//void DrawDLineH(uint32_t x0, uint32_t x1, uint32_t y, uint32_t color)
//

void LCD_DrawLine(uint32_t x0, uint32_t y0, uint32_t x1, uint32_t y1, uint32_t color) {
//	uint32_t steep = (y1 - y0) > (x1 - x0);
//    if (steep){
//		uint32_t temp = y0;
//		y0 = x0;
//		x0 = temp;
//		temp = y1;
//		y1 = x1;
//		x1 = temp;
//    }
//	if(x0>x1){
//		uint32_t temp = y0;
//		y0 = y1;
//		y1 = temp;
//		temp = x0;
//		x0 = x1;
//		x1 = temp;
//	}
//
//    uint32_t dx, dy;
//    dx = x1 - x0;
//    dy = y1 - y0;
//
//    uint32_t err = dx / 2;
//
//    for (; x0<=x1; x0++) {
//    	LCD_PutPixel(x0, y0, color);
//        err -= dy;
//        if (err < 0) {
//            y0++;
//            err += dx;
//        }
//    }

	  int32_t x, y, t, dx, dy, incx, incy, pdx, pdy, ddx, ddy, es, el, err;

//	  // check auf Limit
	  if(x0>=288) x0=288-1;
	  if(y0>=228) y0=228-1;

	  if(x0<=7) x0=8;
	  if(y0<=11) y0=12;

	  // check auf Limit
	  if(x1>=288) x1=288-1;
	  if(y1>=228) y1=228-1;

	  if(x1<=7) x1=8;
	  if(y1<=11) y1=12;

	  // Entfernung in beiden Dimensionen berechnen
	  dx = x1 - x0;
	  dy = y1 - y0;

	  // Vorzeichen des Inkrements bestimmen
	  incx = P_Graphic2D_sgn(dx);
	  incy = P_Graphic2D_sgn(dy);
	  if(dx<0) dx = -dx;
	  if(dy<0) dy = -dy;

	  // feststellen, welche Entfernung grцЯer ist
	  if (dx>dy) {
	    // x ist schnelle Richtung
	    pdx=incx; pdy=0;    // pd. ist Parallelschritt
	    ddx=incx; ddy=incy; // dd. ist Diagonalschritt
	    es =dy;   el =dx;   // Fehlerschritte schnell, langsam
	  }
	  else {
	    // y ist schnelle Richtung
	    pdx=0;    pdy=incy; // pd. ist Parallelschritt
	    ddx=incx; ddy=incy; // dd. ist Diagonalschritt
	    es =dx;   el =dy;   // Fehlerschritte schnell, langsam
	  }

	  // Initialisierungen vor Schleifenbeginn
	  x = x0;
	  y = y0;
	  err = (el>>1);
	  LCD_PutPixel(x,y,color);

	  // Pixel berechnen
	  for(t=0; t<el; ++t) // t zaehlt die Pixel, el ist auch Anzahl
	  {
	    // Aktualisierung Fehlerterm
	    err -= es;
	    if(err<0) {
	      // Fehlerterm wieder positiv (>=0) machen
	      err += el;
	      // Schritt in langsame Richtung, Diagonalschritt
	      x += ddx;
	      y += ddy;
	    }
	    else {
	      // Schritt in schnelle Richtung, Parallelschritt
	      x += pdx;
	      y += pdy;
	    }
	    LCD_PutPixel(x,y,color);
	  }
}

void LCD_DrawLineX(uint32_t x0, uint32_t y0, uint32_t x1, uint32_t y1, uint32_t color) {
	  int32_t x, y, t, dx, dy, incx, incy, pdx, pdy, ddx, ddy, es, el, err;

//	  // check auf Limit
	  if(x0>=320) x0=319;
	  if(y0>=240) y0=239;

	  if(x0<0) x0=0;
	  if(y0<0) y0=0;

  //	  // check auf Limit
	  if(x1>=320) x1=319;
	  if(y1>=240) y1=239;

	  if(x1<0) x1=0;
	  if(y1<0) y1=0;

	  // Entfernung in beiden Dimensionen berechnen
	  dx = x1 - x0;
	  dy = y1 - y0;

	  // Vorzeichen des Inkrements bestimmen
	  incx = P_Graphic2D_sgn(dx);
	  incy = P_Graphic2D_sgn(dy);
	  if(dx<0) dx = -dx;
	  if(dy<0) dy = -dy;

	  // feststellen, welche Entfernung grцЯer ist
	  if (dx>dy) {
	    // x ist schnelle Richtung
	    pdx=incx; pdy=0;    // pd. ist Parallelschritt
	    ddx=incx; ddy=incy; // dd. ist Diagonalschritt
	    es =dy;   el =dx;   // Fehlerschritte schnell, langsam
	  }
	  else {
	    // y ist schnelle Richtung
	    pdx=0;    pdy=incy; // pd. ist Parallelschritt
	    ddx=incx; ddy=incy; // dd. ist Diagonalschritt
	    es =dx;   el =dy;   // Fehlerschritte schnell, langsam
	  }

	  // Initialisierungen vor Schleifenbeginn
	  x = x0;
	  y = y0;
	  err = (el>>1);
	  LCD_PutPixel(x,y,color);

	  // Pixel berechnen
	  for(t=0; t<el; ++t) // t zaehlt die Pixel, el ist auch Anzahl
	  {
	    // Aktualisierung Fehlerterm
	    err -= es;
	    if(err<0) {
	      // Fehlerterm wieder positiv (>=0) machen
	      err += el;
	      // Schritt in langsame Richtung, Diagonalschritt
	      x += ddx;
	      y += ddy;
	    }
	    else {
	      // Schritt in schnelle Richtung, Parallelschritt
	      x += pdx;
	      y += pdy;
	    }
	    LCD_PutPixel(x,y,color);
	  }
}

int32_t P_Graphic2D_sgn(int32_t x)
{
  return (x > 0) ? 1 : (x < 0) ? -1 : 0;
}
