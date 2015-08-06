#include "fpalib.h"

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

