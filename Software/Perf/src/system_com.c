/*
 *
 * Setting up all communication modules
 *
 *
 *
 */

#include "system_init.h"

#include "stm32f10x_usart.h"
#include "stm32f10x_i2c.h"
#include "stm32f10x_rcc.h"

void usart1Config(void);
void i2c1Config(void);
void i2c2Config(void);

void sysComInit()
{
	usart1Config();


}


void usart1Config(void)
{

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);



	USART_InitTypeDef usart1Itd;


	  /* USART1 configured as follow:
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

	usart1Itd.USART_BaudRate = 56000;
	usart1Itd.USART_WordLength = USART_WordLength_8b;
	usart1Itd.USART_StopBits = USART_StopBits_1;
	usart1Itd.USART_Parity = USART_Parity_No;
	usart1Itd.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	usart1Itd.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	USART_Init(USART1, &usart1Itd);

	// enable usart
	USART_Cmd(USART1, ENABLE);
}

void i2c2Config(void)
{

}
