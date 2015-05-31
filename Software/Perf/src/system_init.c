/*
 *
 *
 *
 *
 */

#include "system_init.h"




void systickInit(int inDelay);

void sysInit(void)
{
	sysGpioInit();
	sysComInit();
	sysTimInit();


	sysIrqInit();
}



