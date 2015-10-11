/**
 * @file		eeprom.c
 * @author		Sletten
 * @version		v0.0.1
 * @date		10.10.2015
 * @brief
 *
 * Communication with the Motherboard EEPROM chip
 *
 *
 * @history
 *
 */

#include "eeprom.h"

#include "stm32f10x_i2c.h"

#include "stdint.h"

#define READ_ADDRESS 	0xa1
#define WRITE_ADDRESS 	0xa0



void writeByte(uint8_t inData, uint16_t inAddress)
{
	uint8_t highAddress = (inAddress & 0xff00) >> 8;
	uint8_t lowerAddress = inAddress & 0xff;

	I2C_GenerateSTART(I2C2, ENABLE);
	while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT));

    I2C_Send7bitAddress(I2C2, WRITE_ADDRESS, I2C_Direction_Transmitter);
    while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

	I2C_SendData(I2C2, highAddress);
	while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

	I2C_SendData(I2C2, lowerAddress);
	while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

	I2C_SendData(I2C2, inData);
	while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

	I2C_GenerateSTOP(I2C2, ENABLE);
}

void writePage(uint8_t inData[], uint8_t inLength, uint16_t inAddress)
{

}

uint8_t readByte(uint16_t inAddress)
{
	uint8_t highAddress = (inAddress & 0xff00) >> 8;
	uint8_t lowerAddress = inAddress & 0xff;

	I2C_GenerateSTART(I2C2, ENABLE);
	while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT));

	I2C_Send7bitAddress(I2C2, WRITE_ADDRESS, I2C_Direction_Transmitter);
	while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

	I2C_SendData(I2C2, highAddress);
	while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

	I2C_SendData(I2C2, lowerAddress);
	while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

	I2C_GenerateSTART(I2C2, ENABLE);
	while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT));

	I2C_Send7bitAddress(I2C2, WRITE_ADDRESS, I2C_Direction_Receiver);
	while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));

	I2C_AcknowledgeConfig(I2C2, DISABLE);

	I2C_GenerateSTOP(I2C2, ENABLE);

	uint8_t data = I2C_ReceiveData(I2C2);

	I2C_AcknowledgeConfig(I2C2, ENABLE);

	return data;
}
