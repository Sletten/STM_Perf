/**
 * @file		eeprom.h
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
#pragma once

#include "stdint.h"

void writeToEEPROM();


void writeByte(uint8_t inData, uint16_t inAddress);
void writePage(uint8_t inData[], uint8_t inLength, uint16_t inAddress);
uint8_t readByte(uint16_t inAddress);
