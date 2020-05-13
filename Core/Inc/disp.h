#ifndef DISP_H
#define DISP_H

#define DATA_PORT 			GPIOG
// D0-D15 -> PF0-PF15
#define CONTROL_PORT 		GPIOD
// RESET -> PD0
// RD -> PD1
// WR -> PD2
// RS -> PD3
// CS -> PD4

#define FONTXSIZESMALL 		8u
#define FONTYSIZESMALL 		12u
#define RESX 319
#define RESY 239


// bit set macros
#define RESET_LOW 			CONTROL_PORT->BRR  = 0x00000001
#define RESET_HIGH 			CONTROL_PORT->BSRR = 0x00000001
#define RD_LOW 				CONTROL_PORT->BRR  = 0x00000002
#define RD_HIGH 			CONTROL_PORT->BSRR = 0x00000002
#define WR_LOW 				CONTROL_PORT->BRR  = 0x00000004
#define WR_HIGH 			CONTROL_PORT->BSRR = 0x00000004
#define RS_LOW 				CONTROL_PORT->BRR  = 0x00000008
#define RS_HIGH 			CONTROL_PORT->BSRR = 0x00000008
#define CSD_LOW 			CONTROL_PORT->BRR  = 0x00000010
#define CSD_HIGH 			CONTROL_PORT->BSRR = 0x00000010

#define RS_COMMAND 			RS_LOW
#define RS_DATA    			RS_HIGH

#define RD_STROBE 			{RD_LOW; RD_HIGH;}
#define WR_STROBE 			{WR_LOW; WR_HIGH;}

//comands
#define ILI9341_NOP     	0x00000000U
#define ILI9341_SWRESET 	0x00000001U
#define ILI9341_RDII		0x00000004U
#define ILI9341_RDDST		0x00000009U
#define ILI9341_RDMODE  	0x0000000AU
#define ILI9341_RDMADCTL    0x0000000BU
#define ILI9341_RDPIXFMT    0x0000000CU
#define ILI9341_RDIMGFMT    0x0000000DU
#define ILI9341_RDSM		0x0000000EU
#define ILI9341_RDSELFDIAG  0x0000000FU
#define ILI9341_SLPIN   	0x00000010U
#define ILI9341_SLPOUT 		0x00000011U
#define ILI9341_PTLON   	0x00000012U
#define ILI9341_NORON   	0x00000013U
#define ILI9341_INVOFF  	0x00000020U
#define ILI9341_INVON   	0x00000021U
#define ILI9341_GAMMASET 	0x00000026U
#define ILI9341_DISPOFF 	0x00000028U
#define ILI9341_DISPON  	0x00000029U
#define ILI9341_CASET   	0x0000002AU
#define ILI9341_PASET   	0x0000002BU
#define ILI9341_RAMWR   	0x0000002CU
#define ILI9341_RAMRD   	0x0000002EU
#define ILI9341_PTLAR   	0x00000030U
#define ILI9341_VSD			0x00000033U
#define ILI9341_TELOFF		0x00000034U
#define ILI9341_TELON		0x00000035U
#define ILI9341_MADCTL  	0x00000036U
#define ILI9341_VSSA		0x00000037U
#define ILI9341_IMOFF		0x00000038U
#define ILI9341_IMON		0x00000039U
#define ILI9341_PIXFMT  	0x0000003AU
#define ILI9341_WMC 		0x0000003CU
#define ILI9341_RMC 		0x0000003EU
#define ILI9341_STS		 	0x00000044U
#define ILI9341_GS		 	0x00000045U
#define ILI9341_WDB		 	0x00000051U
#define ILI9341_RDB		 	0x00000052U
#define ILI9341_WCTRLD	 	0x00000053U
#define ILI9341_RCTRD	 	0x00000054U
#define ILI9341_WCABC	 	0x00000055U
#define ILI9341_RCABC	 	0x00000056U
#define ILI9341_WCABCMB	 	0x0000005EU
#define ILI9341_RCABCMB	 	0x0000005FU
#define ILI9341_RGBISM 		0x000000B0U
#define ILI9341_FRMCTR1 	0x000000B1U
#define ILI9341_FRMCTR2 	0x000000B2U
#define ILI9341_FRMCTR3 	0x000000B3U
#define ILI9341_INVCTR  	0x000000B4U
#define ILI9341_BPC			0x000000B5U
#define ILI9341_DFUNCTR 	0x000000B6U
#define ILI9341_EMS		 	0x000000B7U
#define ILI9341_BC1		 	0x000000B8U
#define ILI9341_BC2		 	0x000000B9U
#define ILI9341_BC3		 	0x000000BAU
#define ILI9341_BC4			0x000000BBU
#define ILI9341_BC5		 	0x000000BCU
#define ILI9341_BC6			0x000000BEU
#define ILI9341_BC7			0x000000BFU
#define ILI9341_PWCTR1  	0x000000C0U
#define ILI9341_PWCTR2  	0x000000C1U
#define ILI9341_PWCTR3  	0x000000C2U
#define ILI9341_PWCTR4  	0x000000C3U
#define ILI9341_PWCTR5  	0x000000C4U
#define ILI9341_VMCTR1  	0x000000C5U
#define ILI9341_VMCTR2  	0x000000C7U
#define ILI9341_PCA			0x000000CBU
#define ILI9341_PCB			0x000000CFU
#define ILI9341_NVMW	   	0x000000D0U
#define ILI9341_NVMPK   	0x000000D1U
#define ILI9341_NVMSR   	0x000000D2U
#define ILI9341_RDDID   	0x000000D3U
#define ILI9341_RDID1   	0x000000DAU
#define ILI9341_RDID2   	0x000000DBU
#define ILI9341_RDID3   	0x000000DCU
#define ILI9341_RDID4   	0x000000DDU
#define ILI9341_GMCTRP1 	0x000000E0U
#define ILI9341_GMCTRN1 	0x000000E1U
#define ILI9341_DGC1	 	0x000000E2U
#define ILI9341_DGC2	 	0x000000E3U
#define ILI9341_DTCAI		0x000000E8U
#define ILI9341_DTCAE		0x000000E9U
#define ILI9341_DTCB		0x000000EAU
#define ILI9341_POSC		0x000000EDU
#define ILI9341_E3G			0x000000F2U
#define ILI9341_PRC			0x000000F7U
#define ILI9341_IC		 	0x000000F6U
/*
#define ILI9341_PWCTR6  0xFC
*/

// Color definitions
#define ILI9341_BLACK       0x00000000U      /*   0,   0,   0 */
#define ILI9341_NAVY        0x0000000FU      /*   0,   0, 128 */
#define ILI9341_DARKGREEN   0x000003E0U      /*   0, 128,   0 */
#define ILI9341_DARKCYAN    0x000003EFU      /*   0, 128, 128 */
#define ILI9341_DARKCYAN_T	0x000000EFU
#define ILI9341_MAROON      0x00007800U      /* 128,   0,   0 */
#define ILI9341_PURPLE      0x0000780FU      /* 128,   0, 128 */
#define ILI9341_OLIVE       0x00007BE0U      /* 128, 128,   0 */
#define ILI9341_LIGHTGREY   0x0000C618U      /* 192, 192, 192 */
#define ILI9341_DARKGREY    0x00007BEFU      /* 128, 128, 128 */
#define ILI9341_BLUE        0x0000001FU      /*   0,   0, 255 */
#define ILI9341_GREEN       0x000007E0U      /*   0, 255,   0 */
#define ILI9341_CYAN        0x000007FFU      /*   0, 255, 255 */
#define ILI9341_RED         0x0000F800U      /* 255,   0,   0 */
#define ILI9341_MAGENTA     0x0000F81FU      /* 255,   0, 255 */
#define ILI9341_YELLOW      0x0000FFE0U      /* 255, 255,   0 */
#define ILI9341_WHITE       0x0000FFFFU      /* 255, 255, 255 */
#define ILI9341_ORANGE      0x0000FD20U      /* 255, 165,   0 */
#define ILI9341_GREENYELLOW 0x0000AFE5U      /* 173, 255,  47 */
#define ILI9341_PINK 		0x0000F81FU

#define MAX_X 				40U
#define MAX_Y 				320U

#define CH1_COLOR 			ILI9341_GREEN
#define CH2_COLOR 			ILI9341_BLUE
#define CH3_COLOR 			ILI9341_RED
#define CH4_COLOR 			ILI9341_ORANGE

#define CH_STR_LNGTH 		10U
#define TRIG_STR_LNGTH 		16U
#define TDIV_STR_LNGTH 		14U

extern uint8_t ch1[CH_STR_LNGTH];
extern uint8_t ch2[CH_STR_LNGTH];
extern uint8_t ch3[CH_STR_LNGTH];
extern uint8_t ch4[CH_STR_LNGTH];
extern uint8_t tDiv[TDIV_STR_LNGTH];
extern uint8_t trig[TRIG_STR_LNGTH];

// functions
void LCD_Init(void);
void LCD_PutPixel(uint32_t x, uint32_t y, uint32_t color);
void LCD_WriteCommand(uint32_t command);
void LCD_WriteData(uint32_t data);
void LCD_DrawLineH(uint32_t x0, uint32_t x1, uint32_t y, uint32_t color);
void LCD_SetWindow(uint32_t x0, uint32_t y0, uint32_t x1, uint32_t y1);
void LCD_Background(uint32_t color);
void LCD_DrawLineV(uint32_t y0, uint32_t y1, uint32_t x, uint32_t color);
void LCD_Print(uint32_t x, uint32_t y, uint8_t *text, uint32_t length, uint32_t color);
void LCD_RectFill(uint32_t x0, uint32_t y0, uint32_t x1, uint32_t y1, uint32_t color);
void LCD_DrawRect(uint32_t x0, uint32_t y0, uint32_t x1, uint32_t y1, uint32_t color);
void LCD_DrawReticle(void);
void displayInit(void);
void LCD_DrawLine(uint32_t x0, uint32_t y0, uint32_t x1, uint32_t y1, uint32_t color);
void LCD_DrawLineX(uint32_t x0, uint32_t y0, uint32_t x1, uint32_t y1, uint32_t color);

#endif
