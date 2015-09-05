/**
 * @file		thrusterSimple.h
 * @author		Sletten
 * @version		v0.0.1
 * @date		30.08.2015
 * @brief
 *
 *
 *
 *
 * @history
 * 		30.08.2015	- First implementation
 *
 */

#pragma once
#include "stdint.h"


void initThrusters(void);


void setInput(uint8_t inX, uint8_t inY, uint8_t inZ, uint8_t inYaw, uint8_t inPitch, uint8_t inRoll);
void setInputDir(uint8_t inX, uint8_t inY, uint8_t inZ);
void setInputRot(uint8_t inYaw, uint8_t inPitch, uint8_t inRoll);

void updateThrusters();
