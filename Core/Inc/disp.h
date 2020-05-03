#ifndef DISP_H
#define DISP_H

#include "main.h"


// functions

void LCD_Init(void);
void LCD_PutPixel(uint32_t x, uint32_t y, uint32_t color);
void LCD_WriteCommand(uint32_t command);
void LCD_WriteData(uint32_t data);
void LCD_DrawLineH(uint32_t x0, uint32_t x1, uint32_t y, uint32_t color);
void LCD_SetWindow(uint32_t x0, uint32_t y0, uint32_t x1, uint32_t y1);
void LCD_Background(uint32_t color);
void LCD_DrawLineV(uint32_t y0, uint32_t y1, uint32_t x, uint32_t color);
//void LCD_image();
int LCD_Print(uint32_t x, uint32_t y, char *text, uint32_t length, uint32_t color);
void LCD_RectFill(uint32_t x0, uint32_t y0, uint32_t x1, uint32_t y1, uint32_t color);
void LCD_DrawRect(uint32_t x0, uint32_t y0, uint32_t x1, uint32_t y1, uint32_t color);
void LCD_DrawReticle(void);
void displayInit(void);
void LCD_DrawLine(uint32_t x0, uint32_t y0, uint32_t x1, uint32_t y1, uint32_t color);
void LCD_DrawLineX(uint32_t x0, uint32_t y0, uint32_t x1, uint32_t y1, uint32_t color);

int32_t P_Graphic2D_sgn(int32_t x);

#define DATA_PORT GPIOG
// D0-D15 -> PF0-PF15
#define CONTROL_PORT GPIOD
// RESET -> PD0
// RD -> PD1
// WR -> PD2
// RS -> PD3
// CS -> PD4

#define fontXSizeSmall 8
#define fontYSizeSmall 12
//#define fontYSizeSmall 8
#define RESX 319
#define RESY 239


// bit set macros
#define RESET_LOW 	CONTROL_PORT->BRR  = 0x00000001
#define RESET_HIGH 	CONTROL_PORT->BSRR = 0x00000001
#define RD_LOW 		CONTROL_PORT->BRR  = 0x00000002
#define RD_HIGH 	CONTROL_PORT->BSRR = 0x00000002
#define WR_LOW 		CONTROL_PORT->BRR  = 0x00000004
#define WR_HIGH 	CONTROL_PORT->BSRR = 0x00000004
#define RS_LOW 		CONTROL_PORT->BRR  = 0x00000008
#define RS_HIGH 	CONTROL_PORT->BSRR = 0x00000008
#define CSD_LOW 	CONTROL_PORT->BRR  = 0x00000010
#define CSD_HIGH 	CONTROL_PORT->BSRR = 0x00000010

#define RS_COMMAND RS_LOW
#define RS_DATA    RS_HIGH

#define RD_STROBE {RD_LOW; RD_HIGH;}
#define WR_STROBE {WR_LOW; WR_HIGH;}

//comands
#define ILI9341_NOP     	0x00000000
#define ILI9341_SWRESET 	0x00000001
#define ILI9341_RDII		0x00000004
#define ILI9341_RDDST		0x00000009
#define ILI9341_RDMODE  	0x0000000A
#define ILI9341_RDMADCTL    0x0000000B
#define ILI9341_RDPIXFMT    0x0000000C
#define ILI9341_RDIMGFMT    0x0000000D
#define ILI9341_RDSM		0x0000000E
#define ILI9341_RDSELFDIAG  0x0000000F
#define ILI9341_SLPIN   	0x00000010
#define ILI9341_SLPOUT 		0x00000011
#define ILI9341_PTLON   	0x00000012
#define ILI9341_NORON   	0x00000013
#define ILI9341_INVOFF  	0x00000020
#define ILI9341_INVON   	0x00000021
#define ILI9341_GAMMASET 	0x00000026
#define ILI9341_DISPOFF 	0x00000028
#define ILI9341_DISPON  	0x00000029
#define ILI9341_CASET   	0x0000002A
#define ILI9341_PASET   	0x0000002B
#define ILI9341_RAMWR   	0x0000002C
#define ILI9341_RAMRD   	0x0000002E
#define ILI9341_PTLAR   	0x00000030
#define ILI9341_VSD			0x00000033
#define ILI9341_TELOFF		0x00000034
#define ILI9341_TELON		0x00000035
#define ILI9341_MADCTL  	0x00000036
#define ILI9341_VSSA		0x00000037
#define ILI9341_IMOFF		0x00000038
#define ILI9341_IMON		0x00000039
#define ILI9341_PIXFMT  	0x0000003A
#define ILI9341_WMC 		0x0000003C
#define ILI9341_RMC 		0x0000003E
#define ILI9341_STS		 	0x00000044
#define ILI9341_GS		 	0x00000045
#define ILI9341_WDB		 	0x00000051
#define ILI9341_RDB		 	0x00000052
#define ILI9341_WCTRLD	 	0x00000053
#define ILI9341_RCTRD	 	0x00000054
#define ILI9341_WCABC	 	0x00000055
#define ILI9341_RCABC	 	0x00000056
#define ILI9341_WCABCMB	 	0x0000005E
#define ILI9341_RCABCMB	 	0x0000005F
#define ILI9341_RGBISM 		0x000000B0
#define ILI9341_FRMCTR1 	0x000000B1
#define ILI9341_FRMCTR2 	0x000000B2
#define ILI9341_FRMCTR3 	0x000000B3
#define ILI9341_INVCTR  	0x000000B4
#define ILI9341_BPC			0x000000B5
#define ILI9341_DFUNCTR 	0x000000B6
#define ILI9341_EMS		 	0x000000B7
#define ILI9341_BC1		 	0x000000B8
#define ILI9341_BC2		 	0x000000B9
#define ILI9341_BC3		 	0x000000BA
#define ILI9341_BC4			0x000000BB
#define ILI9341_BC5		 	0x000000BC
#define ILI9341_BC6			0x000000BE
#define ILI9341_BC7			0x000000BF
#define ILI9341_PWCTR1  	0x000000C0
#define ILI9341_PWCTR2  	0x000000C1
#define ILI9341_PWCTR3  	0x000000C2
#define ILI9341_PWCTR4  	0x000000C3
#define ILI9341_PWCTR5  	0x000000C4
#define ILI9341_VMCTR1  	0x000000C5
#define ILI9341_VMCTR2  	0x000000C7
#define ILI9341_PCA			0x000000CB
#define ILI9341_PCB			0x000000CF
#define ILI9341_NVMW	   	0x000000D0
#define ILI9341_NVMPK   	0x000000D1
#define ILI9341_NVMSR   	0x000000D2
#define ILI9341_RDDID   	0x000000D3
#define ILI9341_RDID1   	0x000000DA
#define ILI9341_RDID2   	0x000000DB
#define ILI9341_RDID3   	0x000000DC
#define ILI9341_RDID4   	0x000000DD
#define ILI9341_GMCTRP1 	0x000000E0
#define ILI9341_GMCTRN1 	0x000000E1
#define ILI9341_DGC1	 	0x000000E2
#define ILI9341_DGC2	 	0x000000E3
#define ILI9341_DTCAI		0x000000E8
#define ILI9341_DTCAE		0x000000E9
#define ILI9341_DTCB		0x000000EA
#define ILI9341_POSC		0x000000ED
#define ILI9341_E3G			0x000000F2
#define ILI9341_PRC			0x000000F7
#define ILI9341_IC		 	0x000000F6
/*
#define ILI9341_PWCTR6  0xFC
*/

// Color definitions
#define ILI9341_BLACK       0x00000000      /*   0,   0,   0 */
#define ILI9341_NAVY        0x0000000F      /*   0,   0, 128 */
#define ILI9341_DARKGREEN   0x000003E0      /*   0, 128,   0 */
#define ILI9341_DARKCYAN    0x000003EF      /*   0, 128, 128 */
#define ILI9341_DARKCYAN_T	0x000000EF
#define ILI9341_MAROON      0x00007800      /* 128,   0,   0 */
#define ILI9341_PURPLE      0x0000780F      /* 128,   0, 128 */
#define ILI9341_OLIVE       0x00007BE0      /* 128, 128,   0 */
#define ILI9341_LIGHTGREY   0x0000C618      /* 192, 192, 192 */
#define ILI9341_DARKGREY    0x00007BEF      /* 128, 128, 128 */
#define ILI9341_BLUE        0x0000001F      /*   0,   0, 255 */
#define ILI9341_GREEN       0x000007E0      /*   0, 255,   0 */
#define ILI9341_CYAN        0x000007FF      /*   0, 255, 255 */
#define ILI9341_RED         0x0000F800      /* 255,   0,   0 */
#define ILI9341_MAGENTA     0x0000F81F      /* 255,   0, 255 */
#define ILI9341_YELLOW      0x0000FFE0      /* 255, 255,   0 */
#define ILI9341_WHITE       0x0000FFFF      /* 255, 255, 255 */
#define ILI9341_ORANGE      0x0000FD20      /* 255, 165,   0 */
#define ILI9341_GREENYELLOW 0x0000AFE5      /* 173, 255,  47 */
#define ILI9341_PINK 		0x0000F81F

#define MAX_X 240
#define MAX_Y 320

#define CH1_COLOR ILI9341_GREEN
#define CH2_COLOR ILI9341_BLUE
#define CH3_COLOR ILI9341_RED
#define CH4_COLOR ILI9341_ORANGE

#define CH_STR_LNGTH 10
#define TRIG_STR_LNGTH 16
#define TDIV_STR_LNGTH 14

#endif
