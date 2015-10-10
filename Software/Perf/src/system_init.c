/*
 *
 *
 *
 *
 */

#include "system_init.h"
#include "thrusterSimple.h"



void systickInit(int inDelay);

void sysInit(void)
{

	sysComInit();
	sysGpioInit();
	sysTimInit();

	initThrusters();


	sysIrqInit();
}



