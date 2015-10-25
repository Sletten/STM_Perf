/**
 * @file		thrusterSimple.h
 * @author		Sletten
 * @version		v0.1.1
 * @date		22.10.2015
 * @brief
 *
 *	The simple thruster implementation uses six inputs to control thrusters
 *	in the horizontal and vertical plane.
 */

#pragma once
#include "stdint.h"


void initThrusters(void);
void initThrustersFromEEPROM(void);
void setThrusterParamPwm(uint8_t inThruster,
						 int16_t inMin,
						 int16_t inMidNeg,
						 int16_t inMid,
						 int16_t inMidPos,
						 int16_t inMax);

void enableThrusters(void);
void disableThrusters(void);

void startEscSetup(void);
void stopEscSetup(void);


void setInput(uint8_t inX, uint8_t inY, uint8_t inZ, uint8_t inYaw, uint8_t inPitch, uint8_t inRoll);
void setInputDir(uint8_t inX, uint8_t inY, uint8_t inZ);
void setInputRot(uint8_t inYaw, uint8_t inPitch, uint8_t inRoll);

void updateThrusters();
