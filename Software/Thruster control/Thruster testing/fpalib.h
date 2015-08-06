#ifndef FPALIB_H
#define FPALIB_H

/*
Fixed-point arithmetic
*/



/*
#define fpa32
#define q32
*/

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



#endif // FPALIB_H_INCLUDED
