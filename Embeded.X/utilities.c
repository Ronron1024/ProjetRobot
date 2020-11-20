// <editor-fold defaultstate="collapsed" desc="include">
#include "utilities.h"
#include "robot.h"
#include "IO.h"
#include "math.h"
#include <xc.h>
// </editor-fold>


double Abs(double value)
{
    if (value>=0)
        return value;
    else return -value;      
}

double Max(double value, double value2)
{
    if(value>value2)
        return value;
    else return value2;
}

double Min(double value,double value2)
{
    if (value < value2)
        return value;
    else
        return value2;    
}

double LimitToInterval(double value, double min, double max)
{
    if(value < min)
        return min;
    else if(value > max)
        return max;
    else
        return value;
}

float RadianToDegree(float value) {
    return value / PI * 180.0;
}

float DegreeToRadian(float value) {
    return value * PI / 180.0;
}

double Modulo2PIAngleRadian(double angleRadian) {
    double angleTemp = fmod(angleRadian - PI, 2 * PI) + PI;
    return fmod(angleTemp + PI, 2 * PI) - PI;
}

float getFloat(unsigned char *p, int index)
{
    float *result_ptr = (float*)(p + index);
    float result = *result_ptr;
    return result;
}

double getDouble(unsigned char *p, int index)
{
    double *result_ptr = (double*)(p + index);
    return *result_ptr;
}

void getBytesFromFloat(unsigned char *p, int index, float f)
{
    int i;
    unsigned char *f_ptr = (unsigned char*)&f;
    for (i = 0; i < 4; i++)
        p[index + i] = f_ptr[i];
}

void getBytesFromInt32(unsigned char *p, int index, long in)
{
    int i;
    unsigned char *f_ptr = (unsigned char*)&in;
    for (i = 0; i < 4; i++)
        p[index + i] = f_ptr[3-i];
}

void getBytesFromDouble(unsigned char *p, int index, double d)
{
    int i;
    unsigned char *f_ptr = (unsigned char*)&d;
    for (i = 0; i < 8; i++)
        p[index + i] = f_ptr[i];
}

void testADC(void)
{
    if (robotState.distanceTelemetre0 < 30 || robotState.distanceTelemetre1 < 30 || robotState.distanceTelemetre2 < 30 || robotState.distanceTelemetre3 < 30 || robotState.distanceTelemetre4 < 30)
    {
        LED_ORANGE = 1;
        if (robotState.distanceTelemetre0 < 20 || robotState.distanceTelemetre1 < 20 || robotState.distanceTelemetre2 < 20 || robotState.distanceTelemetre3 < 20 || robotState.distanceTelemetre4 < 20)
        {
            LED_BLEUE = 1;
            if (robotState.distanceTelemetre0 < 10 || robotState.distanceTelemetre1 < 10 || robotState.distanceTelemetre2 < 10 || robotState.distanceTelemetre3 < 10 || robotState.distanceTelemetre4 < 10)
                LED_BLANCHE = 1;
            else
                LED_BLANCHE = 0;
        }
        else
            LED_BLANCHE = LED_BLEUE = 0;
    }
    else
    {
        LED_ORANGE = LED_BLEUE = LED_BLANCHE = 0;
    }
}