/* 
 * File:   PWM.h
 * Author: TP-EO-5
 *
 * Created on 16 janvier 2020, 16:06
 */

#ifndef PWM_H
#define	PWM_H

#define MOTEUR_DROIT 0
#define MOTEUR_GAUCHE 1

#define COEFF_VITESSE_LINEAIRE_PERCENT 1/25
#define COEFF_VITESSE_ANGULAIRE_PERCENT 1/50
#define Kp 0.1
#define Ki 0.1
#define Kd 0.1
void InitPWM(void); 
void PWMSetSpeed(float vitesseEnPourcents, int numeroMoteur );
void PWMUpdateSpeed();
void PWMSetSpeedConsigne(float vitesseEnPourcent, char moteur) ;
void PWMSetSpeedConsignePolaire();
#endif