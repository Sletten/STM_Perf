/**
 * @file		fixpointLig.c
 * @author		Sletten
 * @version		v0.0.1
 * @date		30.08.2015
 * @brief
 *
 * Simple library to handle fixed point arithmetic
 *
 *
 * @history
 * 		30.08.2015	- First implementation
 *
 */


#include "fixpointLib.h"

q32_t intToQ(int32_t inInt)
{
    return (inInt * q32One);
}

int32_t qToInt(q32_t inQ)
{
	//Rounding
    inQ += q32Half;
    return inQ >> 8;
}

int32_t qToIntNoRound(q32_t inQ)
{
    return inQ >> 8;
}
q32_t qAdd(q32_t a, q32_t b)
{
    return a + b;
}
q32_t qSub(q32_t a, q32_t b)
{
    return a - b;
}
q32_t qMlt(q32_t a, q32_t b)
{

    return (a*b) >> 8;

}
q32_t qDiv(q32_t a, q32_t b)
{
    return (a<<8) / b;
}
