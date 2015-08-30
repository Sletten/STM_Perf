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


void setInput(int8_t inX, int8_t inY, int8_t inZ, int8_t inYaw, int8_t inPitch, int8_t inRoll);
void setInputDir(int8_t inX, int8_t inY, int8_t inZ);
void setInputRot(int8_t inYaw, int8_t inPitch, int8_t inRoll);
