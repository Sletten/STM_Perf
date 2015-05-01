/**
 * @file		thruster.h
 * @author		Sletten
 * @version		v0.0.1
 * @date		15.02.2015
 * @brief
 *
 *
 *
 *
 * @history
 * 		15.02.2015	- First implementation
 *
 */


#ifndef THRUSTER_H
#define THRUSTER_H


void setInputDir(short inX, short inY, short inZ);
void setInputX(short inX);
void setInputY(short inY);
void setInputZ(short inZ);

void setInputRot(short inYaw, short inPitch, short inRoll);
void setInputYaw(short inYaw);
// void setInputPitch(short inPitch);
// void setInputRoll(short inRoll);


void shutDownTimers(); // emergency shutdown of thrusters
void setThrustCurve(char inCurve);



#endif
