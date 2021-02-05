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

#define COEFF_VITESSE_LINEAIRE_PERCENT 20
#define COEFF_VITESSE_ANGULAIRE_PERCENT 23

#define KpLin (7/2)
#define KiLin 15*0.7

#define KpAng (6/2)
#define KiAng (40*0.7)

void InitPWM(void); 
void PWMUpdateSpeed();
void UpdateAsservissement();
#endif