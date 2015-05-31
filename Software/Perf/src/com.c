/**
 * @file		com.c
 * @author		Sletten
 * @version		v0.0.1
 * @date		30.05.15
 * @brief
 *
 *
 *
 *
 * @history
 *
 */

/*
 *
 */

#include "com.h"

#include "stm32f10x_usart.h"
#include "misc.h"

//DEBUG:
#include "stm32f10x_gpio.h"

uint16_t data[10];

int i = 0;
char done = 0;
char rec = 0;

#define stopData 255


int getData(int inGet)
{
	return (int)data[inGet];
}



void USART1_IRQHandler(void)
{
    //ta i mot data
    if ((USART1->SR & USART_FLAG_RXNE) != (u16)RESET)
	{
    	uint16_t inData = USART_ReceiveData(USART1);

    	if(inData == stopData || i >= 9)
		{
			done = 1;
			i = 0;
		}
		else
		{
			data[i++] = inData;
		}
	}

    //send data
    if(done)
    {

		while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
		{
		}
		i = 0;
		while(i < 9) //mcu: 6 tcu 7
		{
			USART_SendData(USART1, data[i]);

			while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
			{
			}

			i++;
		}

		rec = 0; // start recieving?
	    done = 0; // done recieving?
	    i = 0;
    }
}


