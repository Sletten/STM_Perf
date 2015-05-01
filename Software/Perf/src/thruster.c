/**
 * @file		thruster.c
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

#include "thruster.h"


#define FALSE			0
#define TRUE			1


#define QUADRATIC		1
#define LINEAR			0


#define THRUSTER_COUNT 8



#define	MAX_THRUST			200
#define MID_THRUST			100
#define MIN_THRUST			0

#define LINEAR_MULTIPLIER 	MID_THRUST

#define MAX_PWM				4800
#define MID_PWM				3600
#define MIN_PWM				2400




// Function declaration

void vectorizeInput(void);
void scaleSpeed(void);

void setThrustValue(short inVal, short inThruster);

void setPwmOutput(short inSpeed, short inThruster);





char thrusterActive = FALSE;			// disable all thrusters in set to FALSE

char thrustCurve = QUADRATIC;



short axisInput[6];					// values set inn, 6 axes.
short axisValue[6];


short thrustValue[THRUSTER_COUNT];		// thruster speed value

short thrustLimit[THRUSTER_COUNT];		// limitations set to thruster in percentage



short pwmStop[THRUSTER_COUNT];			// pwm value for stop

short pwmFwdStart[THRUSTER_COUNT];		// pwm value for forward start

short pwmRevStart[THRUSTER_COUNT];		// pwm value for reverse start




void setInput(short inX, short inY, short inZ)
{
	axisInput[0] = inX;
	axisInput[1] = inY;
	axisInput[2] = inZ;

	vectorizeInput();
}
void setInputX(short inX)
{
	axisInput[0] = inX;

	vectorizeInput();
}
void setInputY(short inY)
{
	axisInput[1] = inY;

	vectorizeInput();
}
void setInputZ(short inZ)
{
	axisInput[1] = inZ;

	vectorizeInput();
}

void setInputRot(short inYaw, short inPitch, short inRoll)
{
	axisInput[3] = inYaw;
	axisInput[4] = inPitch;
	axisInput[5] = inRoll;

	vectorizeInput();
}
void setInputYaw(short inYaw)
{
	axisInput[3] = inYaw;

	vectorizeInput();
}
// void setInputPitch(short inPitch);
// void setInputRoll(short inRoll);


void vectorizeInput(void)
{
	int i = 0;

	for(i = 0; i < THRUSTER_COUNT; i++)
	{
		thrustValue[i] = 0;
	}

	for(i = 0; i< 6; i++)
	{
		short temp = axisInput[i] - 125;

		switch(thrustCurve)
		{
		case QUADRATIC:
			temp = temp*temp;
			break;

		case LINEAR:
			temp = temp*LINEAR_MULTIPLIER;
			break;
		}

		if(axisInput[i] < 0)
			axisValue[i] = -temp/10;
		else
			axisValue[i] = temp/10;
	}



	thrustValue[0] =  axisValue[1] + axisValue[2] + axisValue[3];
	thrustValue[1] =  axisValue[1] - axisValue[2] - axisValue[3];
	thrustValue[2] = -axisValue[1] + axisValue[2] - axisValue[3];
	thrustValue[3] = -axisValue[1] - axisValue[2] + axisValue[3];

	thrustValue[4] = axisValue[4];
	thrustValue[5] = axisValue[4];
	thrustValue[6] = axisValue[4];
	thrustValue[7] = axisValue[4];


	scaleSpeed();
}

void scaleSpeed(void)
{
	int i = 0;

	short minScale;

	for(i = 0; i < 4; i++)
	{
		short temp = thrustLimit[i] > 0 ? thrustLimit[i] : -thrustLimit[i];

		temp -= LINEAR_MULTIPLIER*LINEAR_MULTIPLIER/10;

		if(temp >= 0)
		{

		}
	}
}



void setThrustValue(short inVal, short inThruster)
{

}


void setPwmOutput(short inSpeed, short inThruster);
