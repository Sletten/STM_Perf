/*
 *
 * Setting up all gpio with alternate function, and remapping
 *
 *
 */

#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"

void sysGpioInit(void);

void sysGpioInit(void)
{
	//enable clock for all GPIO
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOA |
							RCC_APB2Periph_GPIOB |
		                    RCC_APB2Periph_GPIOC |
		                    RCC_APB2Periph_GPIOD |
		                    RCC_APB2Periph_GPIOE |
		                    RCC_APB2Periph_GPIOF |
		                    RCC_APB2Periph_AFIO, ENABLE);

	// remapping GPIO pins
	GPIO_PinRemapConfig(GPIO_FullRemap_TIM1,ENABLE);
	GPIO_PinRemapConfig(GPIO_FullRemap_TIM3,ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_TIM4,ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_TIM12,ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_TIM13,ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_TIM14,ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_TIM15,ENABLE);

	GPIO_PinRemapConfig(GPIO_Remap_USART2,ENABLE);
	GPIO_PinRemapConfig(GPIO_FullRemap_USART3,ENABLE); //potensial tim 4 problem



	GPIO_InitTypeDef gpioItd;

	// Timer
	// TIM 1
	gpioItd.GPIO_Pin = 	GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 |
						GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14;
	gpioItd.GPIO_Mode = GPIO_Mode_AF_PP;
	gpioItd.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOE, &gpioItd);

	// TIM 2
	gpioItd.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
	gpioItd.GPIO_Mode = GPIO_Mode_AF_PP;
	gpioItd.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &gpioItd);

	// TIM 3
	gpioItd.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9;
	gpioItd.GPIO_Mode = GPIO_Mode_AF_PP;
	gpioItd.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &gpioItd);

	// TIM 4
	gpioItd.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	gpioItd.GPIO_Mode = GPIO_Mode_AF_PP;
	gpioItd.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOD, &gpioItd);

	// TIM 5
	// unused for now

	// TIM 12, 13, 14, 15, 16, and 17
	gpioItd.GPIO_Pin = 	GPIO_Pin_12 | GPIO_Pin_13 | // TIM 12
						GPIO_Pin_0 | // TIM 13
						GPIO_Pin_1 | // TIM 14
						GPIO_Pin_14 | GPIO_Pin_15 |	// TIM 15
						GPIO_Pin_8 | // TIM 16
						GPIO_Pin_9;	 // TIM 17
	gpioItd.GPIO_Mode = GPIO_Mode_AF_PP;
	gpioItd.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &gpioItd);



	// USART

	// USART 1
	gpioItd.GPIO_Pin =  GPIO_Pin_8 | // CK
						GPIO_Pin_9 | // TX
						GPIO_Pin_12; // RTS
	gpioItd.GPIO_Mode = GPIO_Mode_AF_PP;
	gpioItd.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &gpioItd);
	gpioItd.GPIO_Pin =  GPIO_Pin_10 | // RX
						GPIO_Pin_11;  // CTS
	gpioItd.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	gpioItd.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &gpioItd);

	// USART 2
	gpioItd.GPIO_Pin =  GPIO_Pin_8 | // CK
						GPIO_Pin_9 | // TX
						GPIO_Pin_12; // RTS
	gpioItd.GPIO_Mode = GPIO_Mode_AF_PP;
	gpioItd.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &gpioItd);
	gpioItd.GPIO_Pin =  GPIO_Pin_10 | // RX
						GPIO_Pin_11;  // CTS
	gpioItd.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	gpioItd.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &gpioItd);




	// SPI

	gpioItd.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
	//configuring pins for SPI 1
	GPIO_Init(GPIOA, &gpioItd);


	// I2C











}
