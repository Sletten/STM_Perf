// Simplified thruster control
//


#include "thrusterSimple.h"

#include "fixpointLib.h"



#define FALSE			0
#define TRUE			1


#define QUADRATIC		1
#define LINEAR			0


#define THRUSTER_COUNT 	8


#define INPUT_MIN_VALUE 1
#define INPUT_MID_VALUE 128
#define INPUT_MAX_VALUE 255

#define LINEAR_MULTIPLIER 	INPUT_MID_VALUE


//127*127 = 16129
#define	MAX_THRUST			16129
#define MID_THRUST			0
#define MIN_THRUST			-16129

#define MAX_PWM				4800
#define MID_PWM				3600
#define MIN_PWM				2400



int8_t thrustCurve = QUADRATIC;

int16_t thrustLimit[THRUSTER_COUNT];		// limitations set to thruster in percentage

uint8_t inputValue[6];
int16_t inputVector[6];

int32_t thrustValue[THRUSTER_COUNT];
int32_t thrustVector[THRUSTER_COUNT][6];	// thruster speed value



void setThrusterValue(int thruster,short x, short y, short z, short yaw, short roll, short pitch);

void simpleThrust(void);



int32_t abs(int32_t in)
{
    if(in < 0 )
        return -in;

    return in;
}


void initThrusters(void)
{
	setThrusterValue(0,  1,  1, 0,  1, 0, 0);
	setThrusterValue(1,  1, -1, 0, -1, 0, 0);
	setThrusterValue(2, -1,  1, 0, -1, 0, 0);
	setThrusterValue(3, -1, -1, 0,  1, 0, 0);
	setThrusterValue(4,  0,  0, 1,  0, 0, 0);
	setThrusterValue(5,  0,  0, 1,  0, 0, 0);
	setThrusterValue(6,  0,  0, 1,  0, 0, 0);
	setThrusterValue(7,  0,  0, 1,  0, 0, 0);

    //127*127 = 16129


	thrustLimit[0] = MAX_THRUST;
	thrustLimit[1] = MAX_THRUST;
	thrustLimit[2] = MAX_THRUST;
	thrustLimit[3] = MAX_THRUST;
	thrustLimit[4] = MAX_THRUST;
	thrustLimit[5] = MAX_THRUST;
	thrustLimit[6] = MAX_THRUST;
	thrustLimit[7] = MAX_THRUST;
}

void setThrusterValue(int thruster,short x, short y, short z, short yaw, short roll, short pitch)
{
	thrustVector[thruster][0] = x;
	thrustVector[thruster][1] = y;
	thrustVector[thruster][2] = z;
	thrustVector[thruster][3] = yaw;
	thrustVector[thruster][4] = roll;
	thrustVector[thruster][5] = pitch;
}


void setInput(int8_t inX, int8_t inY, int8_t inZ, int8_t inYaw, int8_t inPitch, int8_t inRoll)
{
	inputValue[0] = inX;
	inputValue[1] = inY;
	inputValue[2] = inZ;
	inputValue[3] = inYaw;
	inputValue[4] = inPitch;
	inputValue[5] = inRoll;

	simpleThrust();
}

void setInputDir(int8_t inX, int8_t inY, int8_t inZ)
{
	inputValue[0] = inX;
	inputValue[1] = inY;
	inputValue[2] = inZ;

	simpleThrust();
}

void setInputRot(int8_t inYaw, int8_t inPitch, int8_t inRoll)
{
	inputValue[3] = inYaw;
	inputValue[4] = inPitch;
	inputValue[5] = inRoll;

	simpleThrust();
}

void simpleThrust(void)
{
	// vectorize input
	int8_t i = 0;
	int8_t j = 0;

	// resetting the thrustValues to 0
	for(i = 0; i < THRUSTER_COUNT; i++)
	{
		thrustValue[i] = 0;
	}

	for(i = 0; i < 6; i++)
	{
		// make input value into a signed value
		//TODO(Sletten): should be implemented such that inputValue is signed in the first place.
		int8_t temp = inputValue[i] - INPUT_MID_VALUE;

		int16_t tempQuad = 0;

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


	// Calcuklate thrust

	i = 0; 	// thruster
	j = 0;	// axis

	for (i = 0 ; i < THRUSTER_COUNT; i++)
	{
		thrustValue[i] = 0;

		for(j = 0; j<6; j++ )
		{
			thrustValue[i] += thrustVector[i][j]*inputVector[j];
		}
	}

	//TODO(Sletten): find a way to use set parameters to determine what thruster to scale
	//scale Horizontal


	q32_t q32_maxScaleHorizontal 	= q32One;
	q32_t q32_maxScaleVertical 		= q32One;

	// check for maximum scaling need
	for(i = 0; i < THRUSTER_COUNT; i++)
	{
		if(thrustVector[i][0] || thrustVector[i][1] || thrustVector[i][3]) // if horizontal thruster, x, y, yaw
		{
			q32_t q32_scale = qDiv(intToQ(thrustLimit[i]), intToQ(abs(thrustValue[i])));

			if (q32_scale < q32_maxScaleHorizontal) // the smaller scale counts
			{
				q32_maxScaleHorizontal = q32_scale;
			}
		}
		if(thrustVector[i][2] || thrustVector[i][4] || thrustVector[i][5]) // if vertical thruster, z, roll, pitch
		{
			int32_t temp = abs(thrustValue[i]);
			q32_t q32_scale = qDiv(intToQ(thrustLimit[i]), intToQ(temp));

			if (q32_scale < q32_maxScaleVertical) // the smaller scale counts
			{
				q32_maxScaleVertical = q32_scale;
			}
		}
	}

	for(i = 0; i < THRUSTER_COUNT; i++)
	{
		// only scale if thrustLimit for any one thruster is exceeded
		if((qToInt(q32_maxScaleHorizontal) < 1 )&& (thrustVector[i][0] || thrustVector[i][1] || thrustVector[i][3]))
		{
			thrustValue[i] = qToInt(qMlt(intToQ(thrustValue[i]), q32_maxScaleHorizontal));
		}
		if((qToInt(q32_maxScaleVertical) < 1 ) && (thrustVector[i][2] || thrustVector[i][4] || thrustVector[i][5]))
		{
			thrustValue[i] = qToInt(qMlt(intToQ(thrustValue[i]), q32_maxScaleVertical));
		}
	}

	//TODO(Sletten): set thruster pwm outputs
}





