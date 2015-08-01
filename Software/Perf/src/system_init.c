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

	sysComInit();
	sysGpioInit();
	sysTimInit();


	sysIrqInit();
}



