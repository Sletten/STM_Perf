/**
 * @file		com.h
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


#ifndef _COM_H_
#define _COM_H_

void comInit();

void communicate();

void prepareTsData();
void sendTS();
void routeTsData();
void sendPCU();
void routePcuData();
void sendMCU();
void routeMcuData();
void sendTCU();
void routeTcuData();

char getData(int inData);
void initComTimer(void);


//com_gpio.c
void gpioInit(void);

//com_usart.c
void usartInit(void);


#endif
