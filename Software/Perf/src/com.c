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






static uint8_t data[512];

int i = 0;
char done = 0;
char rec = 0;


#define STARTOFPACKAGE_1	0
#define	STARTOFPACKAGE_2	1
#define DATATYPE			2
#define PAYLOADLENGTH 		3

#define STARTBYTE_1			155
#define STARTBYTE_2			185
#define STOPBYTE			255


//Data types

#define COMMAND				0x01
#define CONTROL				0x08
#define SYSTEM				0x10

#define GETINFO				0x01
#define GETVERSION			0x02
#define GETSTATUS			0x03

#define THRUSTER			0x11
#define MANIP				0x13
#define LIGHT				0x15
#define CAMERA				0x17


int getData(int inGet)
{
	return (int)data[inGet];
}



void USART1_IRQHandler(void)
{
	// set com status led
	GPIOE->ODR |= GPIO_Pin_0;


	static uint8_t receiveState = 0;// Receiving state
									// 0 - not receiving
									// 1 - first start byte correct
									// 2 - second start byte correct, starting receiving
									// 3 - waiting for payload length
									// 4 - done receiving, do checksum
									// 5 - checksum verified, route data to modules.

	static uint8_t packageSize = 0; // size of package
	static uint8_t i = 0;


    //receive data
    if ((USART1->SR & USART_FLAG_RXNE) != (u16)RESET)
	{
    	uint16_t inData = USART_ReceiveData(USART1); //received byte

    	switch(receiveState){
    	case 0:
    		if(inData == startByte1)
    		{
    			receiveState = 1;
    			data[i++] = inData;
    		}

    		break;
    	case 1:
    		if(inData == startByte2)
			{
				receiveState = 2;
				data[i++] = inData;
			}
    		else
    		{
    			receiveState = 0;
			}
    		break;
    	case 2:
    		if(i == 3)	// payload length is the fourth data element.
    		{
    			packageSize = inData;
    			receiveState = 3;
    		}

    		data[i++] = inData;
    		break;
    	case 3:
    		data[i++] = inData;


    		if(i >= packageSize + 6) // package + 4 start, 2 checksum
    			receiveState = 4;
    		break;
    	}

    	if(receiveState == 4)
    	{
    		//TODO: perform checksum stuff

    		//TODO: Check data type, handle datas acordingly
    		// transfer data.

    		while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
			{
			}
			i = 4;
			while(i < packageSize + 4)
			{
				USART_SendData(USART1, data[i]);

				while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
				{
				}

				i++;
			}
			i = 0;

			receiveState = 0;
    	}
	}

    // reset com status led
    GPIOE->ODR ^= GPIO_Pin_0;
}


// test function for data communication
void handleData()
{
	uint8_t payLength = data[PAYLOADLENGTH];
	switch()
	{
	case COMMANDS:
		break;

	}
	//read datatype header -> check if payload contains one or several data packs
	//find payload length
	//read out data, send to respective PERF modules.
	//repeat if applicable
}


