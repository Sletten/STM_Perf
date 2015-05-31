/*
 *
 * Setting up the interrupts
 *
 *
 *
 */

#include "system_init.h"

#include "stm32f10x_usart.h"
#include "misc.h"


void systickInit(int inDelay);
void usart1IrqInit(void);


void sysIrqInit(void)
{
	usart1IrqInit();


	// configure systick interrupt for 50Hz
	systickInit(20);
}


// setting up the systick interrupt with a delay set by the 'delay' parameter in milliseconds
void systickInit(int inDelay)
{
	NVIC_InitTypeDef nvicItd;

	nvicItd.NVIC_IRQChannel = SysTick_IRQn;
	nvicItd.NVIC_IRQChannelPreemptionPriority = 1;
	nvicItd.NVIC_IRQChannelSubPriority = 0;
	nvicItd.NVIC_IRQChannelCmd = ENABLE;

	NVIC_Init(&nvicItd);

	// set up SysTick to call interrupt 50 times each second
	SysTick_Config(SystemCoreClock/inDelay);
}

void usart1IrqInit(void)
{
	NVIC_InitTypeDef nvicItd;

	/* Enable the USARTx Interrupt */
	nvicItd.NVIC_IRQChannel = USART1_IRQn;
	nvicItd.NVIC_IRQChannelPreemptionPriority = 0;
	nvicItd.NVIC_IRQChannelSubPriority = 0;
	nvicItd.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvicItd);

	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
}
