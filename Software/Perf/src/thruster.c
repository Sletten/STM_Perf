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


#define THRUSTER_COUNT 	8


#define INPUT_MIN_VALUE 0
#define INPUT_MID_VALUE 125
#define INPUT_MAX_VALUE 250



#define	MAX_THRUST			200
#define MID_THRUST			100
#define MIN_THRUST			0

#define LINEAR_MULTIPLIER 	MID_THRUST

#define MAX_PWM				4800
#define MID_PWM				3600
#define MIN_PWM				2400




// Function declaration
void initThrusters(void);
void setThrusterValue(int thruster,short x, short y, short z, short yaw, short roll, short pitch);

void vectorizeInput(void);
void calculateThrust(void);
void scaleSpeed(void);

void setThrustValue(short inVal, short inThruster);

void setPwmOutput(short inSpeed, short inThruster);

void scaleThrust(float scale, short x, short y, short z, short yaw, short roll, short pitch);




char thrusterActive = FALSE;			// disable all thrusters in set to FALSE

char thrustCurve = QUADRATIC;



short axisInput[6];					// values set inn, 6 axes.
short axisValue[6];

short inputValue[6];
short inputVector[6];

short thrustValue[THRUSTER_COUNT];
float thrustVector[THRUSTER_COUNT][6];// thruster speed value
short thrustValueScaled[THRUSTER_COUNT];		// thruster speed value

short thrustLimit[THRUSTER_COUNT];		// limitations set to thruster in percentage



short pwmStop[THRUSTER_COUNT];			// pwm value for stop

short pwmFwdStart[THRUSTER_COUNT];		// pwm value for forward start

short pwmRevStart[THRUSTER_COUNT];		// pwm value for reverse start



void initThrusters(void)
{
	setThrusterValue(1,  1,  1, 0,  1, 0, 0);
	setThrusterValue(2,  1, -1, 0, -1, 0, 0);
	setThrusterValue(3, -1,  1, 0, -1, 0, 0);
	setThrusterValue(4, -1, -1, 0,  1, 0, 0);
	setThrusterValue(5,  0,  0, 1,  0, 0, 0);
	setThrusterValue(6,  0,  0, 1,  0, 0, 0);
	setThrusterValue(7,  0,  0, 1,  0, 0, 0);
	setThrusterValue(8,  0,  0, 1,  0, 0, 0);
}
void setThrusterValue(int thruster,short x, short y, short z, short yaw, short roll, short pitch)
{
	thrustVector[thruster][1] = x;
	thrustVector[thruster][2] = y;
	thrustVector[thruster][3] = z;
	thrustVector[thruster][4] = yaw;
	thrustVector[thruster][5] = roll;
	thrustVector[thruster][6] = pitch;
}


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

	for(i = 0; i < 6; i++)
	{
		short temp = inputValue[i] - INPUT_MID_VALUE;

		switch(thrustCurve)
		{
		case QUADRATIC:
			temp = temp*temp;
			if(inputValue[i] < 0)			// scale to fit varable
											// number range
				inputVector[i] = -temp/10;
			else
				inputVector[i] = temp/10;
			break;

		case LINEAR:
			inputVector[i] = temp*LINEAR_MULTIPLIER;
			break;
		}
	}


	calculateThrust();

	scaleSpeed();
}

void calculateThrust(void)
{
	int i = 0; 	// thrusterp
	int j = 0;	// axis

	for (i = 0 ; i < THRUSTER_COUNT; i++)
	{
		thrustValue[i] = 0;

		for(j = 0; j<6; j++ )
			thrustValue[i] += thrustVector[i][j]*inputVector[j];
	}
}

void scaleSpeed(void)
{
	int i = 0;

	short minScale;

	for(i = 0; i < THRUSTER_COUNT; i++)
	{
		int temp = thrustValue[i] - thrustLimit[i];

		if (temp > 0) // check thrust value for out of range value
		{
			int scale = temp/thrustLimit[i];
			scaleThrust(scale, 	thrustVector[i][1]*scale,
								thrustVector[i][2]*scale,
								thrustVector[i][3]*scale,
								thrustVector[i][4]*scale,
								thrustVector[i][5]*scale,
								thrustVector[i][6]*scale);

		}
	}
}

void scaleThrust(float scale, short x, short y, short z, short yaw, short roll, short pitch)
{
	int i = 0;	//thruster

	for (i = 0 ; i < THRUSTER_COUNT; i++)
	{
		thrustValue[i] -= thrustValue[i] * thrustVector[i][1]*scale*x;
		thrustValue[i] -= thrustValue[i] * thrustVector[i][2]*scale*y;
		thrustValue[i] -= thrustValue[i] * thrustVector[i][3]*scale*z;
		thrustValue[i] -= thrustValue[i] * thrustVector[i][4]*scale*yaw;
		thrustValue[i] -= thrustValue[i] * thrustVector[i][5]*scale*roll;
		thrustValue[i] -= thrustValue[i] * thrustVector[i][6]*scale*pitch;
	}
}



void setThrustValue(short inVal, short inThruster)
{

}


void setPwmOutput(short inSpeed, short inThruster);
