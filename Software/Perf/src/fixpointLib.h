/**
 * @file		fixpointLig.h
 * @author		Sletten
 * @version		v1.0
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

#pragma once


#include "stdint.h"


#define q32m 23
#define q32f 8
#define q32One  0x00000100
#define q32Half 0x00000080

typedef int32_t q32_t;

q32_t intToQ(int32_t inInt);
int32_t qToInt(q32_t inQ);
int32_t qToIntNoRound(q32_t inQ);
q32_t qAdd(q32_t a, q32_t b);
q32_t qSub(q32_t a, q32_t b);
q32_t qMlt(q32_t a, q32_t b);
q32_t qDiv(q32_t a, q32_t b);
