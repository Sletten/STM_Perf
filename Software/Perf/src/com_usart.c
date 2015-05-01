/**
 * @file		uart_com.c
 * @author		Sletten
 * @version		v0.0.1
 * @date		20.01.2014
 * @brief
 *
 *
 *
 *
 * @history
 *				20.01.2014	-	added first psudocode for usart communication
 *
 */


#include "stm32f10x_usart.h"
#include "stm32f10x_rcc.h"


void usartInit(void);
void usart1Config(void);
void usart2Config(void);



void usartInit(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);


	usart1Config();
	usart2Config();
}

void usart1Config(void)
{
	USART_InitTypeDef usartStruct;


	  /*
	   * copy from usart_rxtx example
	   *
	   * USART1 configured as follow:
	        - BaudRate = 115200 baud
	        - Word Length = 8 Bits
	        - One Stop Bit
	        - No parity
	        - Hardware flow control disabled (RTS and CTS signals)
	        - Receive and transmit enabled
	        - USART Clock disabled
	        - USART CPOL: Clock is active low
	        - USART CPHA: Data is captured on the middle
	        - USART LastBit: The clock pulse of the last data bit is not output to
	                         the SCLK pin
	  */

	usartStruct.USART_BaudRate = 9600;
	usartStruct.USART_WordLength = USART_WordLength_8b;
	usartStruct.USART_StopBits = USART_StopBits_1;
	usartStruct.USART_Parity = USART_Parity_No;
	usartStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	usartStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;


	USART_Init(USART1, &usartStruct);

	// enable usart
	USART_Cmd(USART1, ENABLE);
}

void usart2Config(void)
{
	USART_InitTypeDef usartStruct;


		  /*
		   * copy from usart_rxtx example
		   *
		   * USART1 configured as follow:
		        - BaudRate = 115200 baud
		        - Word Length = 8 Bits
		        - One Stop Bit
		        - No parity
		        - Hardware flow control disabled (RTS and CTS signals)
		        - Receive and transmit enabled
		        - USART Clock disabled
		        - USART CPOL: Clock is active low
		        - USART CPHA: Data is captured on the middle
		        - USART LastBit: The clock pulse of the last data bit is not output to
		                         the SCLK pin
		  */

		usartStruct.USART_BaudRate = 9600;
		usartStruct.USART_WordLength = USART_WordLength_8b;
		usartStruct.USART_StopBits = USART_StopBits_1;
		usartStruct.USART_Parity = USART_Parity_No;
		usartStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
		usartStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;


		USART_Init(USART2, &usartStruct);

		// enable usart
		USART_Cmd(USART2, ENABLE);
}
