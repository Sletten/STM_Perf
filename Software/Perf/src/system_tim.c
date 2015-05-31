/*
 *
 * configuration of the timers
 *
 */



#include "system_init.h"


#include "stm32f10x_tim.h"
#include "stm32f10x_rcc.h"



void sysTimInit(void)
{
	// tim 2 - light


	// tim 3 - thruster
	// tim 4 - thruster

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

	TIM_TimeBaseInitTypeDef timTbItd;

	timTbItd.TIM_Period = 48000;
	timTbItd.TIM_Prescaler = (uint16_t) (SystemCoreClock / 2400000) - 1;
	timTbItd.TIM_ClockDivision = 0;
	timTbItd.TIM_CounterMode = TIM_CounterMode_Up;

	TIM_TimeBaseInit(TIM3, &timTbItd);
	TIM_TimeBaseInit(TIM4, &timTbItd);

	TIM_OCInitTypeDef timOcItd;

	timOcItd.TIM_OCMode = TIM_OCMode_PWM1;
	timOcItd.TIM_OutputState = TIM_OutputState_Enable;
	timOcItd.TIM_Pulse = 2500;
	timOcItd.TIM_OCPolarity = TIM_OCPolarity_High;

	TIM_OC1Init(TIM3, &timOcItd);
	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);

	TIM_OC2Init(TIM3, &timOcItd);
	TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);

	TIM_OC3Init(TIM3, &timOcItd);
	TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);

	TIM_OC4Init(TIM3, &timOcItd);
	TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);

	TIM_ARRPreloadConfig(TIM3, ENABLE);


	TIM_OC1Init(TIM4, &timOcItd);
	TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);

	TIM_OC2Init(TIM4, &timOcItd);
	TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);

	TIM_OC3Init(TIM4, &timOcItd);
	TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);

	TIM_OC4Init(TIM4, &timOcItd);
	TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);

	TIM_ARRPreloadConfig(TIM4, ENABLE);



	TIM_Cmd(TIM3, ENABLE);
	TIM_Cmd(TIM4, ENABLE);



	// tim 12 - Manip
	// tim 13 - manip
	// tim 14 - manip
	// tim 15 - manip
}
