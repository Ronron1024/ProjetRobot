/* 
 * File:   utilities.h
 * Author: TP-EO-1
 *
 * Created on 20 novembre 2020, 12:39
 */

#ifndef UTILITIES_H
#define	UTILITIES_H

#ifdef	__cplusplus
extern "C" {
#endif

# define PI 3.141592653589793

double Abs(double value);
double Max(double value, double value2);
double Min(double value,double value2);
double LimitToInterval(double value, double min, double max);
double Modulo2PIAngleRadian(double angleRadian);

float RadianToDegree(float);
float DegreeToRadian(float);

float getFloat(unsigned char *p, int index);
double getDouble(unsigned char *p, int index);

void getBytesFromFloat(unsigned char*p, int index, float f);
void getBytesFromDouble(unsigned char *p, int index, double d);
void getBytesFromInt32(unsigned char *p, int index, long in);

void testADC(void);


#ifdef	__cplusplus
}
#endif

#endif	/* UTILITIES_H */

