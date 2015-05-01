/**
 * @file		gpio_conf.c
 * @author		Sletten
 * @version		v0.0.1
 * @date		20.01.2014
 * @brief
 *
 *
 *
 *
 * @history
 *				20.01.2014	-
 */

#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"


void gpioInit(void);
void gpioPeriphInit(void);
void gpioUsartInit(void);
void servoGpioInit(void);


void gpioInit(void)
{
	gpioPeriphInit();
	gpioUsartInit();
}

void gpioPeriphInit(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |
						   RCC_APB2Periph_GPIOB |
	                       RCC_APB2Periph_GPIOC |
	                       RCC_APB2Periph_AFIO, ENABLE);
}
void gpioUsartInit(void)
{
	GPIO_InitTypeDef gpioStruct;

	// USART1 pins setup

	// initsierer gpioa pin 9 for usart1 TX
	gpioStruct.GPIO_Pin = GPIO_Pin_9;
	gpioStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	gpioStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &gpioStruct);

	// initsierer gpioa pin 10 for usart1 RX
	gpioStruct.GPIO_Pin = GPIO_Pin_10;
	gpioStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &gpioStruct);


	// USART2 pins setup

	// initsierer gpioa pin 2 for usart2 TX
	gpioStruct.GPIO_Pin = GPIO_Pin_2;
	gpioStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	gpioStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &gpioStruct);

	// initsierer gpioa pin 3 for usart2 RX
	gpioStruct.GPIO_Pin = GPIO_Pin_3;
	gpioStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &gpioStruct);


	//initierer enable utganger
	gpioStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	gpioStruct.GPIO_Speed = GPIO_Speed_50MHz;

	gpioStruct.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_8 | GPIO_Pin_11;
	GPIO_Init(GPIOA, &gpioStruct);

	gpioStruct.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_4;
	GPIO_Init(GPIOC, &gpioStruct);

}

/*
void servoGpioInit(void)
{
*/
	/*
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB |
						   RCC_APB2Periph_AFIO, ENABLE);
*/
/*
	GPIO_InitTypeDef itd;


	itd.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9;
	itd.GPIO_Mode = GPIO_Mode_AF_PP;
	itd.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &itd);



	itd.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	itd.GPIO_Mode = GPIO_Mode_AF_PP;
	itd.GPIO_Speed = GPIO_Speed_50MHz;

    GPIO_Init(GPIOA, &itd);

    itd.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
    GPIO_Init(GPIOB, &itd);

}
*/
