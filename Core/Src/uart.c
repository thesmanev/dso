#include "uart.h"

void setupUART(void){
	USART1->CR2 = 0;
	USART1->BRR = 0x0000004E; //baud rate set at 115200 bps
	USART1->CR1 = USART_CR1_RE | USART_CR1_UE | USART_CR1_TE; ; //enable uart and enable receive
}

void UART_send(uint8_t *buff, uint32_t bytes){
	uint32_t cnt = 0;
	while(cnt < bytes){
		while(!(USART1->ISR & USART_ISR_TXE));
		USART1->TDR = buff[cnt];
		cnt++;
	}
}

void UART_receive(uint8_t *buff, uint32_t bytes){
	uint32_t cnt = 0;
	while(cnt < bytes){
		while(!(USART1->ISR & USART_ISR_RXNE));
		buff[cnt] = USART1->RDR;
		cnt++;
	}
}


void serial(void){
	if((USART1->ISR & USART_ISR_RXNE)){
		uint8_t cmd = USART1->RDR;
		if(cmd == 0x24){
			switch(x){
			case 0:
				break;
			case 1:
				UART_send(sendScreenBuffer1, 280);
				break;
			case 2:
				UART_send(sendScreenBuffer2, 280);
				break;
			case 3:
				UART_send(sendScreenBuffer1, 280);
				UART_send(sendScreenBuffer2, 280);
				break;
			case 4:
				UART_send(sendScreenBuffer3, 280);
				break;
			case 5:
				UART_send(sendScreenBuffer1, 280);
				UART_send(sendScreenBuffer3, 280);
				break;
			case 6:
				UART_send(sendScreenBuffer2, 280);
				UART_send(sendScreenBuffer3, 280);
				break;
			case 7:
				UART_send(sendScreenBuffer1, 280);
				UART_send(sendScreenBuffer2, 280);
				UART_send(sendScreenBuffer3, 280);
				break;
			case 8:
				UART_send(sendScreenBuffer4, 280);
				break;
			case 9:
				UART_send(sendScreenBuffer1, 280);
				UART_send(sendScreenBuffer4, 280);
				break;
			case 10:
				UART_send(sendScreenBuffer2, 280);
				UART_send(sendScreenBuffer4, 280);
				break;
			case 11:
				UART_send(sendScreenBuffer1, 280);
				UART_send(sendScreenBuffer2, 280);
				UART_send(sendScreenBuffer4, 280);
				break;
			case 12:
				UART_send(sendScreenBuffer3, 280);
				UART_send(sendScreenBuffer4, 280);
				break;
			case 13:
				UART_send(sendScreenBuffer1, 280);
				UART_send(sendScreenBuffer3, 280);
				UART_send(sendScreenBuffer4, 280);
				break;
			case 14:
				UART_send(sendScreenBuffer2, 280);
				UART_send(sendScreenBuffer3, 280);
				UART_send(sendScreenBuffer4, 280);
				break;
			case 15:
				UART_send(sendScreenBuffer1, 280);
				UART_send(sendScreenBuffer2, 280);
				UART_send(sendScreenBuffer3, 280);
				UART_send(sendScreenBuffer4, 280);
				break;
			default:
				break;
			}

//			UART_send(sendScreenBuffer, 4*280);
		}
		else{
			if(cmd == 0x3C){
				uint8_t ack = 0xAC;
				UART_send(&ack, 1);
				UART_receive(setup, 11);
				dso1.timeDiv = (tmdv)(setup[0] & 0x0F);
				dso1.triggerEdge = (uint32_t)((setup[0] & 0x10) >> 4);
				dso1.quad = (uint32_t)((setup[0] & 0x20) >> 5);
				dso1.dual1 = (uint32_t)((setup[0] & 0x40) >> 6);
				dso1.dual2 = (uint32_t)((setup[0] & 0x80) >> 7);
				dso1.triggerValue = (uint32_t)setup[1];
				dso1.triggerValue = dso1.triggerValue + (uint32_t)((setup[2] & 0x0F) << 8);
				dso1.triggerSource = (uint32_t)((setup[2] & 0x30) >> 4);

				dso1.ch1.enabled = (uint32_t)(setup[3] & 0x01);
				dso1.ch1.voltsDiv = (voltDiv)((setup[3] & 0x0E) >> 1);
				dso1.ch1.coupling = (uint32_t)((setup[3] & 0x30) >> 4);
				dso1.ch1.vertical = (uint32_t)setup[4];

				dso1.ch2.enabled = (uint32_t)(setup[5] & 0x01);
				dso1.ch2.voltsDiv = (voltDiv)((setup[5] & 0x0E) >> 1);
				dso1.ch2.coupling = (uint32_t)((setup[5] & 0x30) >> 4);
				dso1.ch2.vertical = (uint32_t)setup[6];

				dso1.ch3.enabled = (uint32_t)(setup[7] & 0x01);
				dso1.ch3.voltsDiv = (voltDiv)((setup[7] & 0x0E) >> 1);
				dso1.ch3.coupling = (uint32_t)((setup[7] & 0x30) >> 4);
				dso1.ch3.vertical = (uint32_t)setup[8];

				dso1.ch4.enabled = (uint32_t)(setup[9] & 0x01);
				dso1.ch4.voltsDiv = (voltDiv)((setup[9] & 0x0E) >> 1);
				dso1.ch4.coupling = (uint32_t)((setup[9] & 0x30) >> 4);
				dso1.ch4.vertical = (uint32_t)setup[10];

				channelChange();
			}
			else if(cmd == 0xAA){
				setup[0] = (uint8_t)dso1.timeDiv + ((uint8_t)dso1.triggerEdge << 4);
				setup[0] = setup[0] + ((uint8_t)dso1.quad << 5) + ((uint8_t)dso1.dual1 << 6);
				setup[0] = setup[0] + ((uint8_t)dso1.dual2 << 7);
				setup[1] = (uint8_t)(dso1.triggerValue & 0x000000FF);
				setup[2] = ((uint8_t)((dso1.triggerValue & 0x00000F00) >> 8)) + ((uint8_t)(dso1.triggerSource << 4));

				setup[3] = (uint8_t)dso1.ch1.enabled + (uint8_t)(dso1.ch1.voltsDiv << 1);
				setup[3] = (uint8_t)(dso1.ch1.coupling << 4);
				setup[4] = (uint8_t)(dso1.ch1.vertical);

				setup[5] = (uint8_t)dso1.ch2.enabled + (uint8_t)(dso1.ch2.voltsDiv << 1);
				setup[5] = (uint8_t)(dso1.ch2.coupling << 4);
				setup[6] = (uint8_t)(dso1.ch2.vertical);

				setup[7] = (uint8_t)dso1.ch3.enabled + (uint8_t)(dso1.ch3.voltsDiv << 1);
				setup[7] = (uint8_t)(dso1.ch3.coupling << 4);
				setup[8] = (uint8_t)(dso1.ch3.vertical);

				setup[9] = (uint8_t)dso1.ch4.enabled + (uint8_t)(dso1.ch4.voltsDiv << 1);
				setup[9] = (uint8_t)(dso1.ch4.coupling << 4);
				setup[10] = (uint8_t)(dso1.ch4.vertical);

				UART_send(setup, 11);
			}
		}
	}
}
