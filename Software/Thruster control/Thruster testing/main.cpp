/*
Debug console application for testing out thruster software used on PERF module.

*/

#include <iostream>


#define q16m 7
#define q16f 8
#define q16One 0x0100

typedef int q16_t;

q16_t intToQ(int inInt);
int qToInt(q16_t inQ);
q16_t qAdd(q16_t a, q16_t b);
q16_t qSub(q16_t a, q16_t b);
q16_t qMlt(q16_t a, q16_t b);
q16_t qDiv(q16_t a, q16_t b);



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


void setInput(short inX, short inY, short inZ);
void setInputRot(short inYaw, short inPitch, short inRoll);


void vectorizeInput(void);
void calculateThrust(void);
void scaleInput(void);

void setThrustValue(short inVal, short inThruster);

void setPwmOutput(short inSpeed, short inThruster);

void scaleThrust(int scale1, int scale2, short x, short y, short z, short yaw, short roll, short pitch);

int abs(int in);



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




int main()
{

    std::cout << "test" << std::endl;


    initThrusters();

    setInput(100, 160, 180);
    setInputRot(-50, 100, 100);

    /*
    int temp = 327;
    int temp2 = 100;
    std::cout << temp << std::endl;
    q16_t qtemp = intToQ(temp);
    q16_t qtemp2 = intToQ(temp2);
    qtemp = qMlt(qtemp, qtemp2);
    std::cout << qtemp << std::endl;
    qtemp = -qtemp;
    temp = qToInt(qtemp);
    std::cout << temp << std::endl;
    */

    vectorizeInput();


    return 0;
}


q16_t intToQ(int inInt)
{
    return (inInt * q16One);
}
int qToInt(q16_t inQ)
{
    //return inQ / q16One;
    return inQ >> 8;
}
q16_t qAdd(q16_t a, q16_t b)
{
    return a + b;
}
q16_t qSub(q16_t a, q16_t b)
{
    return a - b;
}
q16_t qMlt(q16_t a, q16_t b)
{

    return (a*b) >> 8;

}
q16_t qDiv(q16_t a, q16_t b)
{
    return (a<<8) / b;

}




int abs(int in)
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

	thrustLimit[0] = 500;
	thrustLimit[1] = 500;
	thrustLimit[2] = 500;
	thrustLimit[3] = 500;
	thrustLimit[4] = 500;
	thrustLimit[5] = 500;
	thrustLimit[6] = 500;
	thrustLimit[7] = 500;
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


void setInput(short inX, short inY, short inZ)
{
    std::cout << "set input" << std::endl;

	inputValue[0] = inX;
	inputValue[1] = inY;
	inputValue[2] = inZ;
}
void setInputX(short inX)
{
	inputValue[0] = inX;

	vectorizeInput();
}
void setInputY(short inY)
{
	inputValue[1] = inY;

	vectorizeInput();
}
void setInputZ(short inZ)
{
	inputValue[1] = inZ;

	vectorizeInput();
}

void setInputRot(short inYaw, short inPitch, short inRoll)
{
	inputValue[3] = inYaw;
	inputValue[4] = inPitch;
	inputValue[5] = inRoll;
}
void setInputYaw(short inYaw)
{
	inputValue[3] = inYaw;

	vectorizeInput();
}
// void setInputPitch(short inPitch);
// void setInputRoll(short inRoll);


void vectorizeInput(void)
{
    std::cout << "vectorize input" << std::endl;

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

	std::cout << inputVector[0] << std::endl;
	std::cout << inputVector[1] << std::endl;
	std::cout << inputVector[2] << std::endl;
	std::cout << inputVector[3] << std::endl;
	std::cout << inputVector[4] << std::endl;
	std::cout << inputVector[5] << std::endl;

	calculateThrust();

	scaleInput();

	std::cout << thrustValue[0] << std::endl;
	std::cout << thrustValue[1] << std::endl;
	std::cout << thrustValue[2] << std::endl;
	std::cout << thrustValue[3] << std::endl;
	std::cout << thrustValue[4] << std::endl;
	std::cout << thrustValue[5] << std::endl;
	std::cout << thrustValue[6] << std::endl;
	std::cout << thrustValue[7] << std::endl;
}

void calculateThrust(void)
{
    std::cout << "calculate thrust" << std::endl;

	int i = 0; 	// thrusterp
	int j = 0;	// axis

	for (i = 0 ; i < THRUSTER_COUNT; i++)
	{
		thrustValue[i] = 0;

		for(j = 0; j<6; j++ )
			thrustValue[i] += thrustVector[i][j]*inputVector[j];
	}
	std::cout << thrustValue[0] << std::endl;
	std::cout << thrustValue[1] << std::endl;
	std::cout << thrustValue[2] << std::endl;
	std::cout << thrustValue[3] << std::endl;
	std::cout << thrustValue[4] << std::endl;
	std::cout << thrustValue[5] << std::endl;
	std::cout << thrustValue[6] << std::endl;
	std::cout << thrustValue[7] << std::endl;
}

// scale function
// scales each thruster one by one.
// not optimalization.


void scaleInput(void)
{
    std::cout << "scale speed" << std::endl;
	int i = 0;

	short minScale;

	for(i = 0; i < THRUSTER_COUNT; i++)
	{
	    std::cout << "scale speed loop "  << i << std::endl;
		int temp = abs(thrustValue[i]) - thrustLimit[i];
		std::cout << thrustValue[i]  << std::endl;

		if (temp > 0) // check thrust value for out of range value
		{
		    std::cout << "scale speed loop if" << std::endl;
		    std::cout << temp << std::endl;
		    std::cout << thrustLimit[i] << std::endl;

			scaleThrust(thrustLimit[i],abs(thrustValue[i]),
                                thrustVector[i][0],
								thrustVector[i][1],
								thrustVector[i][2],
								thrustVector[i][3],
								thrustVector[i][4],
								thrustVector[i][5]);

            calculateThrust();  //recalculate thrust
            i = 0;              //check every thruster again //TODO: should be obsolete if working
		}
	}
}



// second scale function
// scales thrust based on largest error ratio

void scaleInput2(void)
{
    std::cout << "scale speed" << std::endl;
	int i = 0;


	q16_t maxScale = 0;



    // check for maximum scaling need
    for(i = 0; i < THRUSTER_COUNT; i++)
    {
        q16_t scale = qDiv(intToQ(thrustLimit[i]), intToQ(thrustValue[i]));

        if (scale > maxScale)
        {
            maxScale = scale;
        }
    }

    for(i = 0; i < THRUSTER_COUNT; i++)
    {

    }
}



void scaleThrust(int scale1, int scale2, short x, short y, short z, short yaw, short roll, short pitch)
{
    std::cout << "scale thrust" << std::endl;


    std::cout << inputVector[0] << std::endl;
    std::cout << inputVector[1] << std::endl;
    std::cout << inputVector[2] << std::endl;
    std::cout << inputVector[3] << std::endl;
    std::cout << inputVector[4] << std::endl;
    std::cout << inputVector[5] << std::endl;
    std::cout << std::endl;


    inputVector[0] -= inputVector[0]*abs(x)*scale1/scale2;
    inputVector[1] -= inputVector[1]*abs(y)*scale1/scale2;
    inputVector[2] -= inputVector[2]*abs(z)*scale1/scale2;
    inputVector[3] -= inputVector[3]*abs(yaw)*scale1/scale2;
    inputVector[4] -= inputVector[4]*abs(roll)*scale1/scale2;
    inputVector[5] -= inputVector[5]*abs(pitch)*scale1/scale2;

    std::cout << inputVector[0] << std::endl;
    std::cout << inputVector[1] << std::endl;
    std::cout << inputVector[2] << std::endl;
    std::cout << inputVector[3] << std::endl;
    std::cout << inputVector[4] << std::endl;
    std::cout << inputVector[5] << std::endl;

    std::cout << std::endl;

    /*

    int i = 0;	//thruster

	for (i = 0 ; i < THRUSTER_COUNT; i++)
	{

	    std::cout << thrustValue[i] << std::endl;
	    std::cout << thrustVector[i][0] << std::endl;
	    std::cout << x << std::endl;
        std::cout << thrustVector[i][0]*x *scale1/scale2 << std::endl;
	    std::cout << thrustValue[i] * thrustVector[i][0]*x *scale1/scale2 << std::endl;
		thrustValue[i] -= thrustValue[i] * thrustVector[i][0]*x *scale1/scale2;
		thrustValue[i] -= thrustValue[i] * thrustVector[i][1]*y *scale1/scale2;
		thrustValue[i] -= thrustValue[i] * thrustVector[i][2]*z *scale1/scale2;
		thrustValue[i] -= thrustValue[i] * thrustVector[i][3]*yaw *scale1/scale2;
		thrustValue[i] -= thrustValue[i] * thrustVector[i][4]*roll *scale1/scale2;
		thrustValue[i] -= thrustValue[i] * thrustVector[i][5]*pitch *scale1/scale2;
		std::cout << thrustValue[i] << std::endl;
		std::cout << std::endl;
	}
    */
}



void setThrustValue(short inVal, short inThruster)
{

}


void setPwmOutput(short inSpeed, short inThruster);
