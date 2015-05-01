#include "stm32f10x_gpio.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_rcc.h"


#define START_DATA 	215

//void GPIO_Blink(void);


int main(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);


	GPIO_InitTypeDef gpioStruct;

	// USART1 pins setup


	RCC_APB2PeriphClockCmd( RCC_APB2Periph_AFIO  |  RCC_APB2Periph_GPIOA, ENABLE);


	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);



	// initsierer gpioa pin 9 for usart1 TX
	gpioStruct.GPIO_Pin = GPIO_Pin_9;
	gpioStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	gpioStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &gpioStruct);

	// initsierer gpioa pin 10 for usart1 RX
	gpioStruct.GPIO_Pin = GPIO_Pin_10;
	gpioStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &gpioStruct);





	USART_InitTypeDef usartStruct;


	usartStruct.USART_BaudRate = 56000;
	usartStruct.USART_WordLength = USART_WordLength_8b;
	usartStruct.USART_StopBits = USART_StopBits_1;
	usartStruct.USART_Parity = USART_Parity_No;
	usartStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	usartStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	USART_Init(USART1, &usartStruct);

	// enable usart
	USART_Cmd(USART1, ENABLE);


	while(1)
    {

        /* Toggle LEDs which connected to PC6*/
        GPIOC->ODR ^= GPIO_Pin_6;
        USART_SendData(USART1, START_DATA);
        while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET){}
        GPIOC->ODR ^= GPIO_Pin_6;





        /* delay */
        int i = 0;
        for(i=0;i<0x100000;i++);


        GPIOC->ODR ^= GPIO_Pin_9;

        /* delay */
        for(i=0;i<0x100000;i++);

    }
}

