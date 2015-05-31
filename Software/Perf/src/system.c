#include "system.h"

#include "system_init.h"
#include "com.h"
#include "thruster.h"




void run(void);


void sysStart(void)
{
	// initialize the system
	sysInit();

	// run the system
	run();
}

// handler for the systick interrupt, performing periodical operations
void SysTick_Handler(void)
{

}


void run(void)
{

}


