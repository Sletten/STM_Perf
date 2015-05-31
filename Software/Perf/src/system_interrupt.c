/*
 *
 * Setting up the interrupts
 *
 *
 *
 */

#include "system_init.h"

#include "misc.h"


void systickInit(int inDelay);


void sysIrqInit(void)
{



	// configure systick interrupt for 50Hz
	systickInit(20);
}


// setting up the systick interrupt with a delay set by the 'delay' parameter in milliseconds
void systickInit(int inDelay)
{
	NVIC_InitTypeDef nvicItd;

	nvicItd.NVIC_IRQChannel = SysTick_IRQn;
	nvicItd.NVIC_IRQChannelPreemptionPriority = 0;
	nvicItd.NVIC_IRQChannelSubPriority = 0;
	nvicItd.NVIC_IRQChannel = ENABLE;

	NVIC_Init(&nvicItd);

	// set up SysTick to call interrupt 50 times each second
	SysTick_Config(SystemCoreClock/inDelay);
}
