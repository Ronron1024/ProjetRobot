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

#define COEFF_VITESSE_LINEAIRE_PERCENT 23
#define COEFF_VITESSE_ANGULAIRE_PERCENT 3.5
#define KpLin 0
#define KpAng 7
#define KiAng 0

void InitPWM(void); 
void PWMUpdateSpeed();
void PWMSetSpeedConsignePolaire();
#endif