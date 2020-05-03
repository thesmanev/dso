/*
 * trigger.c
 *
 *  Created on: Apr 26, 2020
 *      Author: Stanimir Manev
 */
#include "main.h"

void displayWaveformTrig(uint32_t chNum, uint32_t mode){
	uint32_t i;
	uint32_t pixcnt = 8; // comes from waveform screen size
	uint32_t color;

	if(dso1.quad){
		uint16_t *buff1 = (uint16_t*)buffer1;
		uint16_t *buff2 = (uint16_t*)buffer3;
		uint32_t y1 = 0, y2 = 0, y3 = 0, y4 = 0, x0 = 8, x1 = 0;

		if(mode)
			color = ILI9341_BLACK;
		else
			color = CH1_COLOR; // ch1 color
		switch(dso1.timeDiv){
			case timeDiv_2_5us:{
				y1 = 227 - (buff1[dso1.triggerPoint] / 19) + dso1.ch1.vertical;// 212 comes from waveform screen size
				y2 = 227 - (buff1[dso1.triggerPoint+1] / 19) + dso1.ch1.vertical;// 4096 / 212 = 19.316
				y3 = 227 - (buff2[dso1.triggerPoint] / 19) + dso1.ch1.vertical; // needs refinement!!!!
				y4 = 227 - (buff2[dso1.triggerPoint+1] / 19) + dso1.ch1.vertical;
				sendScreenBuffer1[pixcnt-8] = y1;
				LCD_DrawLineV(y1, y2, pixcnt++, color);
				sendScreenBuffer1[pixcnt-8] = y2;
				LCD_DrawLineV(y2, y3, pixcnt++, color);
				sendScreenBuffer1[pixcnt-8] = y3;
				LCD_DrawLineV(y3, y4, pixcnt++, color);
				sendScreenBuffer1[pixcnt-8] = y4;
				for(i = dso1.triggerPoint+2; i < (dso1.triggerPoint+(X_PIXELS/2)); i += 2){
					y1 = 227 - (buff1[i] / 19) + dso1.ch1.vertical;
					LCD_DrawLineV(y4, y1, pixcnt++, color);
					sendScreenBuffer1[pixcnt-8] = y1;
					y2 = 227 - (buff1[i+1] / 19) + dso1.ch1.vertical;
					y3 = 227 - (buff2[i] / 19) + dso1.ch1.vertical;
					y4 = 227 - (buff2[i+1] / 19) + dso1.ch1.vertical;
					LCD_DrawLineV(y1, y2, pixcnt++, color);
					sendScreenBuffer1[pixcnt-8] = y2;
					LCD_DrawLineV(y2, y3, pixcnt++, color);
					sendScreenBuffer1[pixcnt-8] = y3;
					LCD_DrawLineV(y3, y4, pixcnt++, color);
					sendScreenBuffer1[pixcnt-8] = y4;
				}
			break;
			}
			case timeDiv_1us:{
				y1 = 227 - (buff1[dso1.triggerPoint] / 19) + dso1.ch1.vertical;// 212 comes from waveform screen size
				y2 = 227 - (buff1[dso1.triggerPoint+1] / 19) + dso1.ch1.vertical;// 4096 / 212 = 19.316
				y3 = 227 - (buff2[dso1.triggerPoint] / 19) + dso1.ch1.vertical; // needs refinement!!!!
				y4 = 227 - (buff2[dso1.triggerPoint+1] / 19) + dso1.ch1.vertical;
				x1 = (pixcnt*871) / 512;
				LCD_DrawLine(x0, y1, x1, y2, color);
				sendScreenBuffer1[0] = y1;
				pixcnt++;
				x0 = x1;
				x1 = (pixcnt*871) / 512;
				LCD_DrawLine(x0, y2, x1, y3, color);
				sendScreenBuffer1[1] = y2;
				pixcnt++;
				x0 = x1;
				x1 = (pixcnt*871) / 512;
				LCD_DrawLine(x0, y3, x1, y4, color);
				sendScreenBuffer1[2] = y3;
				pixcnt++;
				x0 = x1;
				for(i = dso1.triggerPoint+2; i <dso1.triggerPoint + 163; i += 2){
					y1 = 227 - (buff1[i] / 19) + dso1.ch1.vertical;
					x1 = (pixcnt*871) / 512;
					LCD_DrawLine(x0, y4, x1, y1, color);
					sendScreenBuffer1[pixcnt-8] = y4;
					pixcnt++;
					x0 = x1;
					x1 = (pixcnt*871) / 512;
					y2 = 227 - (buff1[i+1] / 19) + dso1.ch1.vertical;
					y3 = 227 - (buff2[i] / 19) + dso1.ch1.vertical;
					y4 = 227 - (buff2[i+1] / 19) + dso1.ch1.vertical;
					LCD_DrawLine(x0, y1, x1, y2, color);
					sendScreenBuffer1[pixcnt-8] = y1;
					pixcnt++;
					x0 = x1;
					x1 = (pixcnt*871) / 512;
					LCD_DrawLine(x0, y2, x1, y3, color);
					sendScreenBuffer1[pixcnt-8] = y2;
					pixcnt++;
					x0 = x1;
					x1 = (pixcnt*871) / 512;
					LCD_DrawLine(x0, y3, x1, y4, color);
					sendScreenBuffer1[pixcnt-8] = y3;
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
	if(dso1.dual1){
		uint16_t *buff1 = (uint16_t*)buffer1;
		uint32_t y1 = 0, y2 = 0, x = 0;

		if(mode)
			color = ILI9341_BLACK;
		else
			color = CH1_COLOR; // ch1 color

		switch(dso1.timeDiv){
			case 6:{
				y1 = 227 - (buff1[dso1.triggerPoint] / 19) + dso1.ch1.vertical;// 212 comes from waveform screen size
				sendScreenBuffer1[0] = y1;
				for(i = 1; i < X_PIXELS; i++){
					x = (i*47)/16 + dso1.triggerPoint;
					y2 = 227 - (buff1[x] / 19) + dso1.ch1.vertical;
					LCD_DrawLineV(y1, y2, pixcnt++, color);
					sendScreenBuffer1[i] = y2;
					y1 = y2;
				}
				break;
			}
			case 7:{
				y1 = 227 - (buff1[dso1.triggerPoint] / 19) + dso1.ch1.vertical;// 212 comes from waveform screen size
				sendScreenBuffer1[0] = y1;
				for(i = 1; i < X_PIXELS; i++){
					x = (i*47)/32 + dso1.triggerPoint;
					y2 = 227 - (buff1[x] / 19) + dso1.ch1.vertical;
					LCD_DrawLineV(y1, y2, pixcnt++, color);
					sendScreenBuffer1[i] = y2;
					y1 = y2;
				}
				break;
			}
			default:
				assertError(16); // 16 - wrong timeDiv for displayWaveform

		}
		pixcnt = 8;
	}

	if(dso1.dual2){
		uint16_t *buff1 = (uint16_t*)buffer3;
		uint32_t y1 = 0, y2 = 0, x = 0;

		if(mode)
			color = ILI9341_BLACK;
		else
			color = CH3_COLOR; // ch3 color

		switch(dso1.timeDiv){
			case 6:{
				y1 = 227 - (buff1[dso1.triggerPoint] / 19) + dso1.ch3.vertical;// 212 comes from waveform screen size
				sendScreenBuffer3[0] = y1;
				for(i = 1; i < X_PIXELS; i++){
					x = (i*47)/16 + dso1.triggerPoint;
					y2 = 227 - (buff1[x] / 19) + dso1.ch3.vertical;
					LCD_DrawLineV(y1, y2, pixcnt++, color);
					sendScreenBuffer3[i] = y2;
					y1 = y2;
				}
				break;
			}
			case 7:{
				y1 = 227 - (buff1[dso1.triggerPoint] / 19) + dso1.ch3.vertical;// 212 comes from waveform screen size
				sendScreenBuffer3[0] = y1;
				for(i = 1; i < X_PIXELS; i++){
					x = (i*47)/32 + dso1.triggerPoint;
					y2 = 227 - (buff1[x] / 19) + dso1.ch3.vertical;
					LCD_DrawLineV(y1, y2, pixcnt++, color);
					sendScreenBuffer3[i] = y2;
					y1 = y2;
				}
				break;
			}
			default:
				assertError(16); // 16 - wrong timeDiv for displayWaveform

		}
		pixcnt = 8;
	}
	if(chNum){
		uint16_t *buff1;
		uint32_t y1 = 0, y2 = 0, x = 0, vert = 0;
		uint8_t *scrbuff;
		switch(chNum){
			case 1:{
				vert = dso1.ch1.vertical;
				if(mode)
					color = ILI9341_BLACK;
				else
					color = CH1_COLOR; // ch1 color
				buff1 = (uint16_t*)buffer1;
				scrbuff = sendScreenBuffer1;
				break;
			}
			case 2:{
				vert = dso1.ch2.vertical;
				if(mode)
					color = ILI9341_BLACK;
				else
					color = CH2_COLOR; // ch1 color
				buff1 = (uint16_t*)buffer2;
				scrbuff = sendScreenBuffer2;
				break;
			}
			case 3:{
				vert = dso1.ch3.vertical;
				if(mode)
					color = ILI9341_BLACK;
				else
					color = CH3_COLOR; // ch1 color
				buff1 = (uint16_t*)buffer3;
				scrbuff = sendScreenBuffer3;
				break;
			}
			case 4:{
				vert = dso1.ch4.vertical;
				if(mode)
					color = ILI9341_BLACK;
				else
					color = CH4_COLOR; // ch1 color
				buff1 = (uint16_t*)buffer4;
				scrbuff = sendScreenBuffer4;
				break;
			}
		}
		switch(dso1.timeDiv){
			case timeDiv_1ms:{
				y1 = 227 - (buff1[dso1.triggerPoint] / 19) + vert;// 212 comes from waveform screen size
				scrbuff[0] = y1;
				for(i = 1; i<X_PIXELS; i++){
					x = ((i*439143)/131072) + dso1.triggerPoint ;
					y2 = 227 - (buff1[x] / 19) + vert;
					LCD_DrawLineV(y1, y2, pixcnt++, color);
					scrbuff[i] = y2;
					y1 = y2;
				}
				break;
			}
			case timeDiv_500us:{
				y1 = 227 - (buff1[dso1.triggerPoint] / 19) + vert;// 212 comes from waveform screen size
				scrbuff[0] = y1;
				for(i = 1; i<X_PIXELS; i++){
					x = ((i*910925)/65536) + dso1.triggerPoint;
					y2 = 227 - (buff1[x] / 19) + vert;
					LCD_DrawLineV(y1, y2, pixcnt++, color);
					scrbuff[i] = y2;
					y1 = y2;
				}
				break;
			}
			case timeDiv_250us:{
				y1 = 227 - (buff1[dso1.triggerPoint] / 19) + vert;// 212 comes from waveform screen size
				scrbuff[0] = y1;
				for(i = 1; i<X_PIXELS; i++){
					x = (i*7) + dso1.triggerPoint;
					y2 = 227 - (buff1[x] / 19) + vert;
					LCD_DrawLineV(y1, y2, pixcnt++, color);
					scrbuff[i] = y2;
					y1 = y2;
				}
				break;
			}
			case timeDiv_100us:{
				y1 = 227 - (buff1[dso1.triggerPoint] / 19) + vert;// 212 comes from waveform screen size
				scrbuff[0] = y1;
				for(i = 1; i<X_PIXELS; i++){
					x = ((i*421307)/65536) + dso1.triggerPoint;
					y2 = 227 - (buff1[x] / 19) + vert;
					LCD_DrawLineV(y1, y2, pixcnt++, color);
					scrbuff[i] = y2;
					y1 = y2;
				}
				break;
			}
			case timeDiv_50us:{
				y1 = 227 - (buff1[dso1.triggerPoint] / 19) + vert;// 212 comes from waveform screen size
				scrbuff[0] = y1;
				for(i = 1; i<X_PIXELS; i++){
					x = ((i*1389865)/262144) + dso1.triggerPoint;
					y2 = 227 - (buff1[x] / 19) + vert;
					LCD_DrawLineV(y1, y2, pixcnt++, color);
					scrbuff[i] = y2;
					y1 = y2;
				}
				break;
			}
			case timeDiv_25us:{
				y1 = 227 - (buff1[dso1.triggerPoint] / 19) + vert;// 212 comes from waveform screen size
				scrbuff[0] = y1;
				for(i = 1; i<X_PIXELS; i++){
					x = ((i*30093)/8192) + dso1.triggerPoint;
					y2 = 227 - (buff1[x] / 19) + vert;
					LCD_DrawLineV(y1, y2, pixcnt++, color);
					scrbuff[i] = y2;
					y1 = y2;
				}
				break;
			}
			default:
				assertError(16);
				break;
		}
	}
}

void findTrigger(void){
	uint32_t i;
//	uint32_t increment = 1;
	if(dso1.quad){
		uint16_t *buff1 = (uint16_t*)(&buffer1[40]);
		uint16_t *buff2 = (uint16_t*)(&buffer3[40]);
		if(dso1.triggerEdge){
			for(i=0; i<BUFF_SIZE*2; i++){ // BUFF_SIZE*2 bcs (32->16) * 2(2 buffs) -160(points till last pixel)
								  // 320/2(bcs 2 pointers) = 160
				if((buff1[i] < dso1.triggerValue) && (buff2[i] >= dso1.triggerValue)){
					dso1.triggerPoint = i + 80;//40*2(32->16) = 8
					break;
				}//comparison is done sample[x] and sample[x+2]
			}
		}
		else{
			for(i=0; i<BUFF_SIZE*2; i++){ // 640(all samples) - 160(half screen) = 480
								  // 480/2(bcs 2 pointers) = 240
				if((buff1[i] > dso1.triggerValue) && (buff2[i] <= dso1.triggerValue)){
					dso1.triggerPoint = i + 80;//40*2(32->16) = 8
					break;
				}//comparison is done sample[x] and sample[x+2]
			}
		}
	}
	else if(dso1.dual1 && (dso1.triggerSource == 1)){
		uint16_t *buff1 = (uint16_t*)(&buffer1[80]);
		if(dso1.triggerEdge){
			for(i=0; i<BUFF_SIZE; i++){ // BUFF_SIZE*2 bcs (32->16) -160(points till last pixel))
								  // 320/2(bcs 2 pointers) = 160
				if((buff1[i] < dso1.triggerValue) && (buff1[i+10] >= dso1.triggerValue)){
					dso1.triggerPoint = i + 160;//40*2(32->16) = 8
					break;
				}//comparison is done sample[x] and sample[x+2]
			}
		}
		else{
			for(i=0; i<BUFF_SIZE; i++){ // BUFF_SIZE*2 bcs (32->16) -160(points till last pixel)
								  // 480/2(bcs 2 pointers) = 240
				if((buff1[i] > dso1.triggerValue) && (buff1[i+10] <= dso1.triggerValue)){
					dso1.triggerPoint = i + 160;//40*2(32->16) = 8
					break;
				}//comparison is done sample[x] and sample[x+2]
			}
		}
	}
	else if(dso1.dual2 && (dso1.triggerSource == 3)){
		uint16_t *buff1 = (uint16_t*)(&buffer3[40]);
		if(dso1.triggerEdge){
			for(i=0; i<((BUFF_SIZE*2)-160); i++){ // BUFF_SIZE*2 bcs (32->16) -160(points till last pixel))
								  // 320/2(bcs 2 pointers) = 160
				if((buff1[i] < dso1.triggerValue) && (buff1[i+4] >= dso1.triggerValue)){
					dso1.triggerPoint = i + 80;//40*2(32->16) = 8
					break;
				}//comparison is done sample[x] and sample[x+2]
			}
		}
		else{
			for(i=0; i<((BUFF_SIZE*2)-160); i++){ // BUFF_SIZE*2 bcs (32->16) -160(points till last pixel)
								  // 480/2(bcs 2 pointers) = 240
				if((buff1[i] > dso1.triggerValue) && (buff1[i+4] <= dso1.triggerValue)){
					dso1.triggerPoint = i + 80;//40*2(32->16) = 8
					break;
				}//comparison is done sample[x] and sample[x+2]
			}
		}
	}
	else{
		uint16_t *buff1;
		switch(dso1.triggerSource){
			case 1:
				buff1 = (uint16_t*)(&buffer1[40]);
				break;
			case 2:
				buff1 = (uint16_t*)(&buffer2[40]);
				break;
			case 3:
				buff1 = (uint16_t*)(&buffer3[40]);
				break;
			case 4:
				buff1 = (uint16_t*)(&buffer4[40]);
				break;
			default:
				buff1 = (uint16_t*)(&buffer1[40]);
				break;
		}
		if(dso1.triggerEdge){
			for(i=0; i<(BUFF_SIZE/2); i++){// BUFF_SIZE -160(points till last pixel)
								  // 320/2(bcs 2 pointers) = 160
				if((buff1[i] < dso1.triggerValue) && (buff1[i+2] >= dso1.triggerValue)){
					dso1.triggerPoint = i + 80;//40*2(32->16) = 8
					break;
				}//comparison is done sample[x] and sample[x+2]
			}
		}
		else{
			for(i=0; i<(BUFF_SIZE/2); i++){ // BUFF_SIZE -160(points till last pixel)
								  // 480/2(bcs 2 pointers) = 240
				if((buff1[i] > dso1.triggerValue) && (buff1[i+2] <= dso1.triggerValue)){
					dso1.triggerPoint = i + 80;//40*2(32->16) = 8
					break;
				}//comparison is done sample[x] and sample[x+2]
			}
		}
	}
}
