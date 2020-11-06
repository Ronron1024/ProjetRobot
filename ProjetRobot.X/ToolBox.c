#include <xc.h>
#include "robot.h"
#include "ToolBox.h"
#include "IO.h"
#include "PWM.h"

float Abs(float value) {
    if (value >= 0)
        return value;
    else return -value;
}

float Max(float value, float value2) {
    if (value > value2)
        return value;
    else
        return value2;
}

float Min(float value, float value2) {
    if (value < value2)
        return value;
    else
        return value2;
}

float LimitToInterval(float value, float lowLimit, float highLimit) {
    if (value > highLimit)
        value = highLimit;
    else if (value < lowLimit)
        value = lowLimit;

    return value;
}

float RadianToDegree(float value) {
    return value / PI * 180.0;
}

float DegreeToRadian(float value) {
    return value * PI / 180.0;
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