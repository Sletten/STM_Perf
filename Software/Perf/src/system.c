#include "system.h"

#include "system_init.h"
#include "com.h"
#include "thrusterSimple.h"


#include "stm32f10x_tim.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_rcc.h"



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
	updateThrusters();
}

void run(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	// Init COM LED
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    // Init SYS LED
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOE, &GPIO_InitStructure);


	RCC_APB2PeriphClockCmd( RCC_APB2Periph_AFIO  |  RCC_APB2Periph_GPIOA, ENABLE);



	while(1)
    {
        int i = 0;
        for(i=0;i<0x1000;i++);
        {

        }

		// Set sys led
		GPIOE->ODR |= GPIO_Pin_1;

        for(i=0;i<0x200;i++);
        {

        }
    	// Reset sys led
    	GPIOE->ODR &= ~GPIO_Pin_1;


    }
}


