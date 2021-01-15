
#include <xc.h> // library xc.h inclut tous les uC
#include "IO.h"
#include "PWM.h"
#include "robot.h"
#include "utilities.h"

#define PWMPER 40.0
float acceleration = 5;

void InitPWM(void) {
    PTCON2bits.PCLKDIV = 0b000; //Divide by 1
    PTPER = 100 * PWMPER; //P�riode en pourcentage

    //R�glage PWM moteur 1 sur hacheur 1
    IOCON1bits.POLH = 1; //High = 1 and active on low =0
    IOCON1bits.POLL = 1; //High = 1
    IOCON1bits.PMOD = 0b01; //Set PWM Mode to Redundant
    FCLCON1 = 0x0003; //D�sactive la gestion des faults

    //Reglage PWM moteur 2 sur hacheur 6
    IOCON6bits.POLH = 1; //High = 1
    IOCON6bits.POLL = 1; //High = 1
    IOCON6bits.PMOD = 0b01; //Set PWM Mode to Redundant
    FCLCON6 = 0x0003; //D�sactive la gestion des faults

    /* Enable PWM Module */
    PTCONbits.PTEN = 1;
}


void PWMSetSpeed(float vitesseEnPourcents, int numeroMoteur) {



    if (numeroMoteur == 1) {

        robotState.vitesseGaucheCommandeCourante = vitesseEnPourcents;

        if (vitesseEnPourcents > 0) {
            LEFT_MOTOR_ENH = 1; //Pilotage de la pin en mode IO
            LEFT_MOTOR_INL = 1; //Mise � 1 de la pin
            LEFT_MOTOR_ENL = 0; //Pilotage de la pin en mode PWM 
        } else {
            LEFT_MOTOR_ENL = 1; //Pilotage de la pin en mode IO
            LEFT_MOTOR_INH = 1; //Mise � 1 de la pin
            LEFT_MOTOR_ENH = 0; //Pilotage de la pin en mode PWM
        }
        LEFT_MOTOR_DUTY_CYCLE = Abs(robotState.vitesseGaucheCommandeCourante * PWMPER);
    } else if (numeroMoteur == 0) {

        robotState.vitesseDroiteCommandeCourante = vitesseEnPourcents;

        if (vitesseEnPourcents > 0) {
            RIGHT_MOTOR_ENL = 1; //Pilotage de la pin en mode IO
            RIGHT_MOTOR_INH = 1; //Mise � 1 de la pin
            RIGHT_MOTOR_ENH = 0; //Pilotage de la pin en mode PWM 
        } else {
            RIGHT_MOTOR_ENH = 1; //Pilotage de la pin en mode IO
            RIGHT_MOTOR_INL = 1; //Mise � 1 de la pin
            RIGHT_MOTOR_ENL = 0; //Pilotage de la pin en mode PWM
        }


        RIGHT_MOTOR_DUTY_CYCLE = Abs(robotState.vitesseDroiteCommandeCourante * PWMPER);
    }
}
 
void PWMUpdateSpeed() {
    // Cette fonction est appel�e sur timer et permet de suivre des rampes d?acc�l�ration
//    if (robotState.vitesseDroiteCommandeCourante == robotState.vitesseDroiteConsigne)
//        robotState.vitesseDroiteCommandeCourante = Min(
//            robotState.vitesseDroiteCommandeCourante + acceleration,
//            robotState.vitesseDroiteConsigne);
//    if (robotState.vitesseDroiteCommandeCourante > robotState.vitesseDroiteConsigne)
//        robotState.vitesseDroiteCommandeCourante = Max(robotState.vitesseDroiteCommandeCourante - acceleration, robotState.vitesseDroiteConsigne);
    
            
    if (robotState.vitesseDroiteCommandeCourante > 0) {
        RIGHT_MOTOR_ENH = 0; //pilotage de la pin en mode IO
        RIGHT_MOTOR_INH = 1; //Mise � 1 de la pin
        RIGHT_MOTOR_ENL = 1; //Pilotage de la pin en mode PWM
    } else {
        RIGHT_MOTOR_ENL = 0; //pilotage de la pin en mode IO
        RIGHT_MOTOR_INL = 1; //Mise � 1 de la pin
        RIGHT_MOTOR_ENH = 1; //Pilotage de la pin en mode PWM
    }
    //RIGHT_MOTOR_DUTY_CYCLE = Abs(robotState.vitesseDroiteCommandeCourante) * PWMPER;
    RIGHT_MOTOR_DUTY_CYCLE = Abs(robotState.vitesseDroiteConsigne) * PWMPER;
    
    
//    if (robotState.vitesseGaucheCommandeCourante < robotState.vitesseGaucheConsigne)
//        robotState.vitesseGaucheCommandeCourante = Min(
//            robotState.vitesseGaucheCommandeCourante + acceleration,
//            robotState.vitesseGaucheConsigne);
//    if (robotState.vitesseGaucheCommandeCourante > robotState.vitesseGaucheConsigne)
//        robotState.vitesseGaucheCommandeCourante = Max(
//            robotState.vitesseGaucheCommandeCourante - acceleration, robotState.vitesseGaucheConsigne);

    if (robotState.vitesseGaucheCommandeCourante > 0) {
        LEFT_MOTOR_ENL = 0; //pilotage de la pin en mode IO
        LEFT_MOTOR_INL = 1; //Mise � 1 de la pin
        LEFT_MOTOR_ENH = 1; //Pilotage de la pin en mode PWM
    } else {
        LEFT_MOTOR_ENH = 0; //pilotage de la pin en mode IO
        LEFT_MOTOR_INH = 1; //Mise � 1 de la pin
        LEFT_MOTOR_ENL = 1; //Pilotage de la pin en mode PWM
    }
    //LEFT_MOTOR_DUTY_CYCLE = Abs(robotState.vitesseGaucheCommandeCourante) * PWMPER;
    LEFT_MOTOR_DUTY_CYCLE = Abs(robotState.vitesseGaucheConsigne) * PWMPER;
}

void PWMSetSpeedConsigne(float vitesseEnPourcents, char moteur) 
{
    if (moteur == MOTEUR_DROIT)
        robotState.vitesseDroiteConsigne = vitesseEnPourcents;
    else if (moteur == MOTEUR_GAUCHE)
        robotState.vitesseGaucheConsigne = vitesseEnPourcents;
    robotState.vitesseAngulaireConsigne = (robotState.vitesseDroiteConsigne - robotState.vitesseDroiteConsigne)/DISTROUES;
    //PWMSetSpeedConsignePolaire();
}

void PWMSetSpeedConsignePolaire()
{
    // Calcul consigne en angulaire
    
    
    //Correction Angulaire 
    float erreurVitesseAngulaire =  robotState.vitesseAngulaireConsigne - robotState.vitesseAngulaireFromOdometry;
    float correctionVitesseAngulaire = erreurVitesseAngulaire*Kp;
    float correctionVitesseAngulairePourcent = correctionVitesseAngulaire * COEFF_VITESSE_ANGULAIRE_PERCENT ; 
    
    
    // Correction Lin�aire
    float erreurVitesseLineaire =  robotState.vitesseLineaireConsigne - robotState.vitesseLineaireFromOdometry; 
    float correctionVitesseLineaire = erreurVitesseLineaire*Kp;
    float correctionVitesseLineairePourcent = correctionVitesseLineaire * COEFF_VITESSE_LINEAIRE_PERCENT ;
    
    //G�n�ration des consignes droite et gauche
    robotState.vitesseDroiteConsigne = correctionVitesseLineairePourcent + correctionVitesseAngulairePourcent;
    robotState.vitesseDroiteConsigne = LimitToInterval(robotState.vitesseDroiteConsigne, -100, 100);
    robotState.vitesseGaucheConsigne = correctionVitesseLineairePourcent + correctionVitesseAngulairePourcent;
    robotState.vitesseGaucheConsigne = LimitToInterval(robotState.vitesseGaucheConsigne, -100, 100);
}