/**
 * @file		thrusterSimple.c
 * @author		Sletten
 * @version		v0.1.1
 * @date		30.08.2015
 * @brief
 *
 *	The simple thruster implementation uses six inputs to control thrusters
 *	in the horizontal and vertical plane.
 */


/*
 *
 *	Standard setup for thruster
 *	Front horizontal (0, and 1) is pointing forward, and aft horizontal, (2, and 3) is pointing backwards
 *	All four vertical is pointing upwards.
 *
 *	Changes to this setup can be changed in the initThrusters() method.
 *	Other setup can be us if desired, but the vectorization(values set insetThrusterValue() ) need to be recalculated.
 *
 * 		 /			   	   \
 * 		/ 0 /			\ 1 \
 * 		   / 			 \
 * 			(4)	 	  (5)
 *
 *
 *
 *
 * 			(6)		  (7)
 *		   \			 /
 * 		\ 2 \			/ 3 /
 * 		 \			 	   /
 *
 */
#include "thrusterSimple.h"

#include "stm32f10x_tim.h"
#include "stm32f10x_usart.h"


#include "fixpointLib.h"



#define FALSE			0
#define TRUE			1

#define QUADRATIC		1
#define LINEAR			0

#define THRUSTER_COUNT 	8

#define INPUT_MIN_VALUE 1
#define INPUT_MID_VALUE 128
#define INPUT_MAX_VALUE 255

#define LINEAR_MULTIPLIER 	INPUT_MID_VALUE - INPUT_MIN_VALUE


/* TODO(Sletten): Find a better way to use limiting. Theese max and min thrust values, should in theory not change.
 *	should limiting work as set a limit to the thurst that can be reached even if the stick input is not at 100%.
 * 	or should the limit set a new output limit for max stick input?
 *
 * 	could be set in two ways, change the maximum pwm-output, and change the thrust limit
 *
 */
#define	MAX_THRUST			16129	//127*127 = 16129
#define MID_THRUST			0
#define MIN_THRUST			-16129

#define MAX_PWM				5600
#define MID_PWM				3600
#define MIN_PWM				1800


int8_t thrustCurve = QUADRATIC;

int32_t escSetup = FALSE;

uint8_t inputValue[6];
int16_t inputVector[6];

/*
int32_t thrustValue[THRUSTER_COUNT];
int32_t thrustVector[THRUSTER_COUNT][6];	// thruster speed value

// TODO(Sletten): Does not take min thrust limit into account
int16_t thrustLimit[THRUSTER_COUNT];		// limitations set to thruster
*/

struct Thruster
{
	uint8_t enabled;	// bolean to define if the thruster is enabled or not

	int16_t thrustMax;	// these might not be used, and only referred to as the defined values above.
	int16_t thrustMid;
	int16_t thrustMin;

	int16_t pwmMax;		// Maximum pwm period for thruster foreward
	int16_t pwmMidPos;	// pwm period where thruster start in positive direction
	int16_t pwmMid;		// pwm period for mid posision, thruster not running
	int16_t pwmMidNeg;	// pwm period where thruster start in negative direction
	int16_t pwmMin;		// minimum pwm period for thruster backward

	int16_t thrustLimit;
	int32_t thrustValue;
	int32_t thrustVector[6];	// thruster speed value [x y z yaw roll pitch]

	volatile uint16_t *timerOutput;		// register for the timer output eg: TIM1->CCR1
};

struct Thruster thruster[THRUSTER_COUNT];


void setThrusterValue(uint8_t inThruster,short x, short y, short z, short yaw, short roll, short pitch);
void setThrusterTimer(uint8_t inThruster, volatile uint16_t *inReg);
void setThrust(uint8_t inThruster, int16_t);


int32_t abs(int32_t in)
{
    if(in < 0 )
        return -in;

    return in;
}


void initThrusters(void)
{
	int i = 0;
	for(i = 0; i < 6; i++)
	{
		inputValue[i] = 0;
	}

	// set thruster direction in reference to the axes.
	//					 x	 y	z  yaw roll pitch
	setThrusterValue(0,  1,  1, 0,  1, 0, 0);
	setThrusterValue(1,  1, -1, 0, -1, 0, 0);
	setThrusterValue(2, -1,  1, 0, -1, 0, 0);
	setThrusterValue(3, -1, -1, 0,  1, 0, 0);
	setThrusterValue(4,  0,  0, 1,  0, 0, 0); //  1, -1 // roll and pitch, not implemented yet, need to be tested
	setThrusterValue(5,  0,  0, 1,  0, 0, 0); // -1, -1
	setThrusterValue(6,  0,  0, 1,  0, 0, 0); //  1,  1
	setThrusterValue(7,  0,  0, 1,  0, 0, 0); // -1,  1

	// Set the registers for the timer outputs
	setThrusterTimer(0, &TIM4->CCR1);
	setThrusterTimer(1, &TIM4->CCR2);
	setThrusterTimer(2, &TIM4->CCR3);
	setThrusterTimer(3, &TIM4->CCR4);
	setThrusterTimer(4, &TIM3->CCR1);
	setThrusterTimer(5, &TIM3->CCR2);
	setThrusterTimer(6, &TIM3->CCR3);
	setThrusterTimer(7, &TIM3->CCR4);

	for (i = 0; i < THRUSTER_COUNT; i++)
	{
		thruster[i].enabled = TRUE;

		thruster[i].thrustMax = MAX_THRUST;
		thruster[i].thrustMid = MID_THRUST;
		thruster[i].thrustMin = MIN_THRUST;

		thruster[i].pwmMax		= 5600;
		thruster[i].pwmMidPos	= 3600;
		thruster[i].pwmMid		= 3600;
		thruster[i].pwmMidNeg	= 3600;
		thruster[i].pwmMin		= 1800;

		thruster[i].thrustLimit	= MAX_THRUST;
		thruster[i].thrustValue	= 0;
	}

	/*	// old
	thrustLimit[0] = MAX_THRUST;
	thrustLimit[1] = MAX_THRUST;
	thrustLimit[2] = MAX_THRUST;
	thrustLimit[3] = MAX_THRUST;
	thrustLimit[4] = MAX_THRUST;
	thrustLimit[5] = MAX_THRUST;
	thrustLimit[6] = MAX_THRUST;
	thrustLimit[7] = MAX_THRUST;
	*/
}

void setThrusterParamPwm(uint8_t inThruster, int16_t inMin, int16_t inMidNeg, int16_t inMid, int16_t inMidPos, int16_t inMax)
{
	thruster[inThruster].pwmMax		= inMax;
	thruster[inThruster].pwmMidPos	= inMidPos;
	thruster[inThruster].pwmMid		= inMid;
	thruster[inThruster].pwmMidNeg	= inMidNeg;
	thruster[inThruster].pwmMin		= inMin;
}


//TODO: implement this stuff
void initThrustersFromEEPROM(void)
{

}

void enableThrusters(void)
{
	int i = 0;
	for(i = 0; i < THRUSTER_COUNT ; i++)
	{
		thruster[i].enabled = TRUE;
	}
}
void disableThrusters(void)
{
	int i = 0;
	for(i = 0; i < THRUSTER_COUNT ; i++)
	{
		setThrust(i, thruster[i].pwmMid);
		thruster[i].enabled = FALSE;
	}
}


// start procedure for setting up endpoints for motor controllers
void startEscSetup(void)
{

}
void stopEscSetup(void)
{

}

/*	// old
void setThrusterValue(int thrusterNr,short x, short y, short z, short yaw, short roll, short pitch)
{
	thrustVector[thrusterNr][0] = x;
	thrustVector[thrusterNr][1] = y;
	thrustVector[thrusterNr][2] = z;
	thrustVector[thrusterNr][3] = yaw;
	thrustVector[thrusterNr][4] = roll;
	thrustVector[thrusterNr][5] = pitch;
}
*/

// set thruster direction in reference to the axes.
void setThrusterValue(uint8_t inThruster,short x, short y, short z, short yaw, short roll, short pitch)
{
	thruster[inThruster].thrustVector[0] = x;
	thruster[inThruster].thrustVector[1] = y;
	thruster[inThruster].thrustVector[2] = z;
	thruster[inThruster].thrustVector[3] = yaw;
	thruster[inThruster].thrustVector[4] = roll;
	thruster[inThruster].thrustVector[5] = pitch;
}

// set the register addess for the desired timer output for a given thruster
void setThrusterTimer(uint8_t inThruster, volatile uint16_t *inReg)
{
	thruster[inThruster].timerOutput = inReg;
}

// sets the pwm period for a thrusters timer output
void setThrust(uint8_t inThruster, int16_t inPeriod)
{
	// untested, but think it should work
	*thruster[inThruster].timerOutput = inPeriod;

	/*
	switch(inThruster)
	{
	case 0:
		TIM4->CCR1 = inPeriod;
		break;
	case 1:
		TIM4->CCR2 = inPeriod;
		break;
	case 2:
		TIM4->CCR3 = inPeriod;
		break;
	case 3:
		TIM4->CCR4 = inPeriod;
		break;
	case 4:
		TIM3->CCR1 = inPeriod;
		break;
	case 5:
		TIM3->CCR2 = inPeriod;
		break;
	case 6:
		TIM3->CCR3 = inPeriod;
		break;
	case 7:
		TIM3->CCR4 = inPeriod;
		break;
	}
	*/
}

// set control input, does not gets calculated and set before updateThrusters is called.
void setInput(uint8_t inX, uint8_t inY, uint8_t inZ, uint8_t inYaw, uint8_t inPitch, uint8_t inRoll)
{
	inputValue[0] = inX;
	inputValue[1] = inY;
	inputValue[2] = inZ;
	inputValue[3] = inYaw;
	inputValue[4] = inPitch;
	inputValue[5] = inRoll;
}

void setInputDir(uint8_t inX, uint8_t inY, uint8_t inZ)
{
	inputValue[0] = inX;
	inputValue[1] = inY;
	inputValue[2] = inZ;
}

void setInputRot(uint8_t inYaw, uint8_t inPitch, uint8_t inRoll)
{
	inputValue[3] = inYaw;
	inputValue[4] = inPitch;
	inputValue[5] = inRoll;
}

// calculates and updates the pwm outputs for the thrusters
void updateThrusters()
{
	if(escSetup > 0)
	{
		int8_t i = 0;
		if (escSetup == 1) // step 1
		{
			// turn off speed controllers
			for(i = 0; i < THRUSTER_COUNT; i++)
			{
				setThrust(i, thruster[i].pwmMax);
			}
			// Turn on speed controllers
		}
		else if(escSetup < 550) // 11 seconds // step 2
		{
			for(i = 0; i < THRUSTER_COUNT; i++)
			{
				setThrust(i, thruster[i].pwmMax);
			}
		}
		else if (escSetup < 650) // 13 seconds // step 3.1
		{
			for(i = 0; i < THRUSTER_COUNT; i++)
			{
				setThrust(i, thruster[i].pwmMid);
			}
		}
		else if (escSetup < 750) // 15 seconds // step 3.2
		{
			for(i = 0; i < THRUSTER_COUNT; i++)
			{
				setThrust(i, thruster[i].pwmMin);
			}
		}
		else // step 4
		{
			// turn speed controllers off
			for(i = 0; i < THRUSTER_COUNT; i++)
			{
				setThrust(i, thruster[i].pwmMid);
			}
			escSetup = 0;
		}
	}
	else
	{
		int8_t i = 0;
		int8_t j = 0;

		// resetting the thrustValues to 0
		for(i = 0; i < THRUSTER_COUNT; i++)
		{
			thruster[i].thrustValue = 0;
		}

		// vectorize input -ish, calculates the scaled input
		for(i = 0; i < 6; i++)
		{
			if(inputValue[i] == 0) // input not set or error, don't set any thrust.
			{
				inputVector[i] = 0;
			}
			else
			{
				// make input value into a signed value
				//TODO(Sletten): should be implemented such that inputValue is signed in the first place.
				int8_t temp = inputValue[i] - INPUT_MID_VALUE;

				int16_t tempQuad = 0;

				// scales the input
				switch(thrustCurve)
				{
				case QUADRATIC:
					tempQuad = temp*temp;


					if(temp < 0)			// scale to fit variable
													// number range
						inputVector[i] = -tempQuad;
					else
						inputVector[i] = tempQuad;
					break;

				case LINEAR:
					inputVector[i] = temp*LINEAR_MULTIPLIER;
					break;
				}
			}
		}

		// Calculate thrust for each thrusters from the input

		i = 0; 	// thruster
		j = 0;	// axis

		for (i = 0 ; i < THRUSTER_COUNT; i++)
		{
			thruster[i].thrustValue = 0;

			for(j = 0; j < 6; j++ )
			{
				thruster[i].thrustValue += thruster[i].thrustVector[j]*inputVector[j];
			}
		}


		//TODO(Sletten): find a way to use set parameters to determine what thruster to scale

		q32_t q32_maxScaleHorizontal 	= q32One;
		q32_t q32_maxScaleVertical 		= q32One;

		// check for maximum scaling need
		for(i = 0; i < THRUSTER_COUNT; i++)
		{
			// Horizontal thrusters
			if(thruster[i].thrustVector[0] || thruster[i].thrustVector[1] || thruster[i].thrustVector[3]) // if horizontal thruster, x, y, yaw
			{
				int32_t temp = abs(thruster[i].thrustValue);

				q32_t q32_scale = q32One;
				if (temp != 0) //avoid division by zero
				{
					q32_scale = qDiv(intToQ(thruster[i].thrustLimit), intToQ(temp));
				}

				if (q32_scale < q32_maxScaleHorizontal) // the smaller scale counts
				{
					q32_maxScaleHorizontal = q32_scale;
				}
			}

			// Vertical thrusters
			if(thruster[i].thrustVector[2] || thruster[i].thrustVector[4] || thruster[i].thrustVector[5]) // if vertical thruster, z, roll, pitch
			{
				int32_t temp = abs(thruster[i].thrustValue);

				q32_t q32_scale = q32One;
				if (temp != 0) //avoid division by zero
				{
					q32_scale = qDiv(intToQ(thruster[i].thrustLimit), intToQ(temp));
				}

				if (q32_scale < q32_maxScaleVertical) // the smaller scale counts
				{
					q32_maxScaleVertical = q32_scale;
				}
			}
		}

		for(i = 0; i < THRUSTER_COUNT; i++)
		{
			// only scale if thrustLimit for any one thruster is exceeded
			if((qToIntNoRound(q32_maxScaleHorizontal) < 1 ) &&
				(thruster[i].thrustVector[0] || thruster[i].thrustVector[1] || thruster[i].thrustVector[3]))
			{
				thruster[i].thrustValue = qToInt(qMlt(intToQ(thruster[i].thrustValue), q32_maxScaleHorizontal));
			}
			if((qToIntNoRound(q32_maxScaleVertical) < 1 ) &&
				(thruster[i].thrustVector[2] || thruster[i].thrustVector[4] || thruster[i].thrustVector[5]))
			{
				thruster[i].thrustValue = qToInt(qMlt(intToQ(thruster[i].thrustValue), q32_maxScaleVertical));
			}
		}

		// set the pwm outputs
		for(i = 0; i < THRUSTER_COUNT; i++)
		{
			if (thruster[i].enabled)
			{
				/*
				if(thruster[i].thrustValue > 0)
				{
					setThrust(i, MID_PWM + qToInt(qMlt(qDiv(intToQ(thruster[i].thrustValue),intToQ(MAX_THRUST)), intToQ(MAX_PWM - MID_PWM))));
				}
				else
				{
					setThrust(i, MID_PWM - qToInt(qMlt(qDiv(intToQ(thruster[i].thrustValue),intToQ(MIN_THRUST)), intToQ(MID_PWM - MIN_PWM))));
				}
				*/
				if(thruster[i].thrustValue == 0)
				{
					setThrust(i, thruster[i].pwmMid);
				}
				else if(thruster[i].thrustValue > 0) // a thrustvalue of 1 gives the lowest pwm-output where the thruster runs
				{
					/*
					 * pwm value = ((thrustvalue-1/maxThrust-1)*(pwmMax-pwmMidPos))
					 */
					setThrust(i, thruster[i].pwmMidPos + qToInt(qMlt(qDiv(intToQ(thruster[i].thrustValue-1),intToQ(MAX_THRUST-1)), intToQ(thruster[i].pwmMax - thruster[i].pwmMidPos))));
				}
				else
				{
					setThrust(i, thruster[i].pwmMidNeg - qToInt(qMlt(qDiv(intToQ(thruster[i].thrustValue+1),intToQ(MIN_THRUST+1)), intToQ(thruster[i].pwmMidNeg - thruster[i].pwmMin))));
				}
			}
		}
	}
}






