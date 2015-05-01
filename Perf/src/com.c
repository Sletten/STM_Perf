/**
 * @file		com.c
 * @author		Sletten
 * @version		v0.0.1
 * @date		10.04.2014
 * @brief
 *
 *
 *
 *
 * @history
 *
 */

/*
 * Communication system for the CCU
 *
 */

#include "com.h"

#include "stm32f10x.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_gpio.h"
#include "misc.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_tim.h"



#define START_DATA 	255
#define STOP_DATA	251

// ROV_data contains all data sendt and recieved on the ROV
#define ROV_DATA_SIZE 72
unsigned char ROV_data[ROV_DATA_SIZE];

// Placement of data in ROV_data, and its size
#define TS_TO_CCU_START 	0
#define TS_TO_CCU_SIZE		13

#define MCU_TO_CCU_START 	TS_TO_CCU_START + TS_TO_CCU_SIZE
#define MCU_TO_CCU_SIZE		6

#define TCU_TO_CCU_START 	MCU_TO_CCU_START + MCU_TO_CCU_SIZE
#define TCU_TO_CCU_SIZE		7

#define PCU_TO_CCU_START 	TCU_TO_CCU_START + TCU_TO_CCU_SIZE
#define PCU_TO_CCU_SIZE		4

#define CCU_TO_TS_START		PCU_TO_CCU_START + PCU_TO_CCU_SIZE
#define CCU_TO_TS_SIZE		7

#define CCU_TO_MCU_START	CCU_TO_TS_START + CCU_TO_TS_SIZE
#define CCU_TO_MCU_SIZE		6

#define CCU_TO_TCU_START	CCU_TO_MCU_START + CCU_TO_MCU_SIZE
#define CCU_TO_TCU_SIZE		9

#define CCU_TO_PCU_START	CCU_TO_TCU_START + CCU_TO_TCU_SIZE
#define CCU_TO_PCU_SIZE		1

// maximun wait time in ms before skipping to next node
#define MAX_RESPOND_TIME 500
// number of skipps allowed from TS before stopping all moving systems on ROV
#define PROTECTION_LIMIT 3
// counted skipps to topside, restarts each successful data transfer.
int protectionModeCounter = 0;
void protectmode();


// variables used for for-loops.
int i = 0;
int j = 0;

/*
 * Initialize the communication system
 */
void comInit()
{
	gpioInit();
	usartInit();
	initComTimer();
}

/*
 * performs the communication routine
 */
void communicate(void)
{
	prepareTsData();
	sendTS();
	//routeTsData();

	//sendPCU();
	//routePcuData();

	//sendTCU();
	//routeTcuData();

	//sendMCU();
	//routeMcuData();
}

/*
 * prepare data on ROV for sending to TopSide
 */
void prepareTsData()
{
	//set cameraTilt
	//ROV_data[CCU_TO_TS_START + 1] = getCameraTilt(1);
	//ROV_data[CCU_TO_TS_START + 2] = getCameraTilt(2);


	//TODO: debug
	//ROV_data[CCU_TO_TS_START + 6] = TIM6->CNT/1000;
}

/*
 * Performs communication with TopSide
 */
void sendTS(void)
{
	// set rs-485 drive enable
	//GPIO_WriteBit(GPIOA, GPIO_Pin_11, Bit_SET);

	//while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET){}

	// send start byte
	USART_SendData(USART1, START_DATA);
	//while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET){}

	// send data to topside
	//for(i = CCU_TO_TS_START; i < CCU_TO_TS_START + CCU_TO_TS_SIZE; i++)
	{
		//USART_SendData(USART1, ROV_data[i]);
		//while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET){}
	}

	// send stop byte
	//USART_SendData(USART1, STOP_DATA);
	//while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET){}

	// reset rs-485 drive enable
	//GPIO_WriteBit(GPIOA, GPIO_Pin_11, Bit_RESET);


	//
	//	Recieve data
	//
	/*
	uint16_t time = TIM6->CNT;
	if(time >= TIM6->ARR - MAX_RESPOND_TIME)
	{
		time = 0;
		TIM6->CNT = 0;
	}

	int inData;
	char rec = 0; // start recieving?
	char done = 0; // done recieving?
	while (!done)
	{

		if(TIM6->CNT >= time + MAX_RESPOND_TIME)
		{
			done = 1;
			if (protectionModeCounter++ >= PROTECTION_LIMIT)
			{
				protectMode();
			}
		}


		//wait for start byte
		if ((USART1->SR & USART_FLAG_RXNE) != (u16)RESET)
		{
			inData = USART_ReceiveData(USART1);

			if(inData == START_DATA)
			{
				// setup for receiving data
				rec = 1;
				i = TS_TO_CCU_START;
			}
			else if((inData == STOP_DATA))
			//else if((inData == STOP_DATA) || (i >= TS_TO_CCU_START + TS_TO_CCU_SIZE))
			{
				protectionModeCounter = 0;
				done = 1;
			}
			else if(rec && (inData != STOP_DATA))
			{
				ROV_data[i++] = inData;
			}
		}
	}
	*/
}

/*
 * Routes data from TS to the correct places in ROV_data
 */
void routeTsData()
{
	// copy pilot axis 1-4
	for(i = 0; i < 4 ; i++)
	{
		ROV_data[CCU_TO_TCU_START+i] = ROV_data[TS_TO_CCU_START+i];
	}
	// copy pilot buttons
	ROV_data[CCU_TO_TCU_START+8] = ROV_data[TS_TO_CCU_START+5];


	// copy co-pilot axis 1-4
	for(i = 0; i < 4 ; i++)
	{
		ROV_data[CCU_TO_MCU_START+i] = ROV_data[TS_TO_CCU_START+6+i];
		//ROV_data[CCU_TO_MCU_START+i] = ROV_data[TS_TO_CCU_START+i];
	}
	// copy co-pilot buttons
	ROV_data[CCU_TO_MCU_START+4] = ROV_data[TS_TO_CCU_START+11];
	ROV_data[CCU_TO_MCU_START+5] = ROV_data[TS_TO_CCU_START+4];

}

/*
 * Performs communication with the MCU
 */
void sendMCU(void)
{
	//set MCU rs-485 read enable
	GPIO_WriteBit(GPIOC, GPIO_Pin_5, Bit_RESET);

	//set CCU rs-485 drive enable, and rs read disable
	GPIO_WriteBit(GPIOA, GPIO_Pin_4, Bit_SET);
	while(USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET){}

	USART_SendData(USART2, START_DATA);
	while(USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET){}

	for(i = CCU_TO_MCU_START; i < (CCU_TO_MCU_START + CCU_TO_MCU_SIZE); i++)
	{
		USART_SendData(USART2, ROV_data[i]);
		while(USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET){}
	}

	USART_SendData(USART2, STOP_DATA);
	while(USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET){}

	//set CCU rs-485 drive disable, and rs read enable
	GPIO_WriteBit(GPIOA, GPIO_Pin_4, Bit_RESET);

	//set MCU rs read disable
	GPIO_WriteBit(GPIOC, GPIO_Pin_5, Bit_SET);


	//
	//	Recieve data
	//
	uint16_t time = TIM6->CNT;
	if(time >= TIM6->ARR - MAX_RESPOND_TIME)
	{
		time = 0;
		TIM6->CNT = 0;
	}

	int inData;

	char rec = 0; // start recieving?
	char done = 0; // done recieving?

	while (!done)
	{
		if(TIM6->CNT >= time + MAX_RESPOND_TIME)
		{
			done = 1;
		}

		//wait for start byte
		if ((USART2->SR & USART_FLAG_RXNE) != (u16)RESET)
		{
			inData = USART_ReceiveData(USART2);

			if(inData == START_DATA)
			{
				rec = 1;
				i = MCU_TO_CCU_START;
			}
			else if(inData == STOP_DATA /*|| (i >= (MCU_TO_CCU_START + MCU_TO_CCU_SIZE))*/)
			{
				done = 1;
			}
			else if(rec)
			{
				ROV_data[i++] = inData;
			}
		}
	}
}
/*
 * Routes data from MCU to the correct places in ROV_data
 */
void routeMcuData()
{
}

/*
 * Performs communication with the TCU
 */
void sendTCU(void)
{
	GPIO_WriteBit(GPIOC, GPIO_Pin_5, Bit_SET);



	// set tcu rs read enable
	GPIO_WriteBit(GPIOC, GPIO_Pin_4, Bit_RESET);

	//set ccu rs write enable
	GPIO_WriteBit(GPIOA, GPIO_Pin_4, Bit_SET);

	while(USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET){}

	//Send start byte
	USART_SendData(USART2, START_DATA);
	while(USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET){}

	// send data bytes
	for(i = CCU_TO_TCU_START; i < CCU_TO_TCU_START + CCU_TO_TCU_SIZE; i++)
	{
		USART_SendData(USART2, ROV_data[i]);
		while(USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET){}
	}

	//send stop byte
	USART_SendData(USART2, STOP_DATA);
	while(USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET){}

	GPIO_WriteBit(GPIOC, GPIO_Pin_4, Bit_SET);
	GPIO_WriteBit(GPIOA, GPIO_Pin_4, Bit_RESET);


	//
	//	Recieve data
	//
	uint16_t time = TIM6->CNT;
	if(time >= TIM6->ARR - MAX_RESPOND_TIME)
	{
		time = 0;
		TIM6->CNT = 0;
	}

	int inData;

	char rec = 0; // start recieving?
	char done = 0; // done recieving?
	while (!done)
	{
		if(TIM6->CNT >= time + MAX_RESPOND_TIME)
		{
			done = 1;
		}

		//wait for start byte
		if ((USART2->SR & USART_FLAG_RXNE) != (u16)RESET)
		{
			inData = USART_ReceiveData(USART2);

			if(inData == START_DATA)
			{
				rec = 1;
				i = TCU_TO_CCU_START;
			}
			else if(inData == STOP_DATA /*|| (i > TCU_TO_CCU_START + TCU_TO_CCU_SIZE)*/)
			{
				done = 1;
			}
			else if(rec)
			{
				ROV_data[i++] = inData;
			}
		}
	}
}
/*
 * Routes data from TCU to the correct places in ROV_data
 */
void routeTcuData()
{

}

/*
 * Performs communication with the PCU
 */
void sendPCU()
{
	//set PCU rs-485 read enable
	GPIO_WriteBit(GPIOA, GPIO_Pin_5, Bit_RESET);

	//set CCU rs-485 drive enable, and rs read disable
	GPIO_WriteBit(GPIOA, GPIO_Pin_4, Bit_SET);
	while(USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET){}

	USART_SendData(USART2, START_DATA);
	while(USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET){}

	for(i = CCU_TO_PCU_START; i < (CCU_TO_PCU_START + CCU_TO_PCU_SIZE); i++)
	{
		USART_SendData(USART2, ROV_data[i]);
		while(USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET){}
	}

	USART_SendData(USART2, STOP_DATA);
	while(USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET){}

	//set CCU rs-485 drive disable, and rs read enable
	GPIO_WriteBit(GPIOA, GPIO_Pin_4, Bit_RESET);

	//set PCU rs read disable
	GPIO_WriteBit(GPIOA, GPIO_Pin_5, Bit_SET);


	//
	//	Recieve data
	//
	uint16_t time = TIM6->CNT;
	if(time >= TIM6->ARR - MAX_RESPOND_TIME)
	{
		time = 0;
		TIM6->CNT = 0;
	}

	int inData;

	char rec = 0; // start recieving?
	char done = 0; // done recieving?

	while (!done)
	{
		if(TIM6->CNT >= time + MAX_RESPOND_TIME)
		{
			done = 1;
		}

		//wait for start byte
		if ((USART2->SR & USART_FLAG_RXNE) != (u16)RESET)
		{
			inData = USART_ReceiveData(USART2);

			if(inData == START_DATA)
			{
				rec = 1;
				i = PCU_TO_CCU_START;
			}
			else if(inData == STOP_DATA)
			{
				done = 1;
			}
			else if(rec)
			{
				ROV_data[i++] = inData;
			}
		}
	}
}
/*
 * Routes data from PCU to the correct places in ROV_data
 */
void routePcuData()
{
	for(i = 0; i < 4 ; i++)
	{
		ROV_data[CCU_TO_TCU_START+4+i] = ROV_data[PCU_TO_CCU_START+i];
		ROV_data[CCU_TO_TS_START +3+i] = ROV_data[PCU_TO_CCU_START+i];
	}
}

/*
 * Returns requested data from ROV_data
 */
char getData(int inData)
{
	return ROV_data[inData];
}

/*
 * Initialize timer 6 for
 */
void initComTimer()
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);

	TIM6->PSC = 24000-1;
	TIM6->ARR = 65535;

	TIM_Cmd(TIM6, ENABLE);
}

void protectMode()
{
	// set all incoming data to zero
	for(i = TS_TO_CCU_START; i < TS_TO_CCU_START + TS_TO_CCU_SIZE; i++)
		ROV_data[i] = 0;

	// set all TCU inputs to zero to ensure shutdown of thrusters
	for(i = CCU_TO_TCU_START; i < CCU_TO_TCU_START + 4; i++)
		ROV_data[i] = 125;
	//TODO: set TCU control bit with "shutdown message" to high
	//TODO: set data from PCU to TCU in stable position with no correction

	// Set aøø MCU inputs to zero to ensure shutdown of manipulator
	for(i = CCU_TO_MCU_START; i < CCU_TO_MCU_START + 4; i++)
		ROV_data[i] = 125;
	//TODO: set MCU control bit with "shutdown message" to high

	//TODO: set other ROV functions not installed yet to stop.
}
