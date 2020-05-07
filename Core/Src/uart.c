#include "main.h"

static uint8_t uart_setup_buff[11];		/**< UART buffer for the DSO setup */

/**
 * @brief Setup UAR
 */
void setupUART(void)
{
	USART1->CR2 = 0;
	USART1->BRR = 0x0000004E; //baud rate set at 115200 bps
	USART1->CR1 = USART_CR1_RE | USART_CR1_UE | USART_CR1_TE; //enable uart and enable receive
}

/**
 * @brief Send data over UART in blocking mode
 * @param buff Pointer to the buffer containing the data to be sent
 * @param bytes Number of bytes to send
 */
static void UART_send(uint8_t *buff, uint32_t bytes)
{
	uint32_t cnt = 0;
	while (cnt < bytes)
	{
		while (!(USART1->ISR & USART_ISR_TXE));
		USART1->TDR = buff[cnt];
		cnt++;
	}
}

/**
 * @brief Receive data over UART in blocking mode
 * @param buff Pointer to a buffer where the received data will be placed
 * @param bytes Number of bytes to receive
 */
static void UART_receive(uint8_t *buff, uint32_t bytes)
{
	uint32_t cnt = 0;
	while (cnt < bytes)
	{
		while (!(USART1->ISR & USART_ISR_RXNE));
		buff[cnt] = USART1->RDR;
		cnt++;
	}
}

/**
 * @brief Function for handling serial communication of the device with the desktop App
 */
void serial(void)
{
	if ((USART1->ISR & USART_ISR_RXNE))
	{
		uint8_t cmd = USART1->RDR;
		if (cmd == 0x24)
		{
			switch (channel_state)
			{
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
		}
		else
		{
			if (cmd == 0x3C)
			{
				uint8_t ack = 0xAC;
				UART_send(&ack, 1);
				UART_receive(uart_setup_buff, 11);
				dso.timeDiv = (tmdv_t) (uart_setup_buff[0] & 0x0F);
				dso.triggerEdge = (uint32_t) ((uart_setup_buff[0] & 0x10) >> 4);
				dso.quad = (uint32_t) ((uart_setup_buff[0] & 0x20) >> 5);
				dso.dual1 = (uint32_t) ((uart_setup_buff[0] & 0x40) >> 6);
				dso.dual2 = (uint32_t) ((uart_setup_buff[0] & 0x80) >> 7);
				dso.triggerValue = (uint32_t) uart_setup_buff[1];
				dso.triggerValue = dso.triggerValue
						+ (uint32_t) ((uart_setup_buff[2] & 0x0F) << 8);
				dso.triggerSource = (uint32_t) ((uart_setup_buff[2] & 0x30) >> 4);

				dso.ch1.enabled = (uint32_t) (uart_setup_buff[3] & 0x01);
				dso.ch1.voltsDiv = (voltDiv_t) ((uart_setup_buff[3] & 0x0E) >> 1);
				dso.ch1.coupling = (uint32_t) ((uart_setup_buff[3] & 0x30) >> 4);
				dso.ch1.vertical = (uint32_t) uart_setup_buff[4];

				dso.ch2.enabled = (uint32_t) (uart_setup_buff[5] & 0x01);
				dso.ch2.voltsDiv = (voltDiv_t) ((uart_setup_buff[5] & 0x0E) >> 1);
				dso.ch2.coupling = (uint32_t) ((uart_setup_buff[5] & 0x30) >> 4);
				dso.ch2.vertical = (uint32_t) uart_setup_buff[6];

				dso.ch3.enabled = (uint32_t) (uart_setup_buff[7] & 0x01);
				dso.ch3.voltsDiv = (voltDiv_t) ((uart_setup_buff[7] & 0x0E) >> 1);
				dso.ch3.coupling = (uint32_t) ((uart_setup_buff[7] & 0x30) >> 4);
				dso.ch3.vertical = (uint32_t) uart_setup_buff[8];

				dso.ch4.enabled = (uint32_t) (uart_setup_buff[9] & 0x01);
				dso.ch4.voltsDiv = (voltDiv_t) ((uart_setup_buff[9] & 0x0E) >> 1);
				dso.ch4.coupling = (uint32_t) ((uart_setup_buff[9] & 0x30) >> 4);
				dso.ch4.vertical = (uint32_t) uart_setup_buff[10];

				channelChange();
			}
			else if (cmd == 0xAA)
			{
				uart_setup_buff[0] = (uint8_t)(dso.timeDiv + (dso.triggerEdge << 4));
				uart_setup_buff[0] = (uint8_t)(uart_setup_buff[0] + (dso.quad << 5) + (dso.dual1 << 6));
				uart_setup_buff[0] = (uint8_t)(uart_setup_buff[0] + (dso.dual2 << 7));
				uart_setup_buff[1] = (uint8_t)(dso.triggerValue & 0x000000FF);
				uart_setup_buff[2] = (uint8_t)(((dso.triggerValue & 0x00000F00) >> 8) + (dso.triggerSource << 4));

				uart_setup_buff[3] = (uint8_t)(dso.ch1.enabled + ((uint32_t)dso.ch1.voltsDiv << 1));
				uart_setup_buff[3] = (uint8_t)(uart_setup_buff[3] + (dso.ch1.coupling << 4));
				uart_setup_buff[4] = (uint8_t)(dso.ch1.vertical);

				uart_setup_buff[5] = (uint8_t)(dso.ch2.enabled + ((uint32_t)dso.ch2.voltsDiv << 1));
				uart_setup_buff[5] = (uint8_t)(uart_setup_buff[5] + (dso.ch2.coupling << 4));
				uart_setup_buff[6] = (uint8_t)(dso.ch2.vertical);

				uart_setup_buff[7] = (uint8_t)(dso.ch3.enabled + ((uint32_t)dso.ch3.voltsDiv << 1));
				uart_setup_buff[7] = (uint8_t)(uart_setup_buff[7] + (dso.ch3.coupling << 4));
				uart_setup_buff[8] = (uint8_t)(dso.ch3.vertical);

				uart_setup_buff[9] = (uint8_t)(dso.ch4.enabled + ((uint32_t)dso.ch4.voltsDiv << 1));
				uart_setup_buff[9] = (uint8_t)(uart_setup_buff[9] + (dso.ch4.coupling << 4));
				uart_setup_buff[10] = (uint8_t)(dso.ch4.vertical);

				UART_send(uart_setup_buff, 11);
			}
		}
	}
}
