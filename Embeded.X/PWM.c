
#include <xc.h> // library xc.h inclut tous les uC
#include "IO.h"
#include "PWM.h"
#include "robot.h"
#include "utilities.h"
#include "QEI.h"

#define PWMPER 40.0
float acceleration = 5;

void InitPWM(void) {
    PTCON2bits.PCLKDIV = 0b000; //Divide by 1
    PTPER = 100 * PWMPER; //Période en pourcentage

    //Réglage PWM moteur 1 sur hacheur 1
    IOCON1bits.POLH = 1; //High = 1 and active on low =0
    IOCON1bits.POLL = 1; //High = 1
    IOCON1bits.PMOD = 0b01; //Set PWM Mode to Redundant
    FCLCON1 = 0x0003; //Désactive la gestion des faults

    //Reglage PWM moteur 2 sur hacheur 6
    IOCON6bits.POLH = 1; //High = 1
    IOCON6bits.POLL = 1; //High = 1
    IOCON6bits.PMOD = 0b01; //Set PWM Mode to Redundant
    FCLCON6 = 0x0003; //Désactive la gestion des faults

    /* Enable PWM Module */
    PTCONbits.PTEN = 1;
}

void PWMUpdateSpeed() {
    if (robotState.vitesseDroiteCommande > 0) {
        RIGHT_MOTOR_ENH = 0; //pilotage de la pin en mode IO
        RIGHT_MOTOR_INH = 1; //Mise à 1 de la pin
        RIGHT_MOTOR_ENL = 1; //Pilotage de la pin en mode PWM
    } else {
        RIGHT_MOTOR_ENL = 0; //pilotage de la pin en mode IO
        RIGHT_MOTOR_INL = 1; //Mise à 1 de la pin
        RIGHT_MOTOR_ENH = 1; //Pilotage de la pin en mode PWM
    }
    RIGHT_MOTOR_DUTY_CYCLE = Abs(robotState.vitesseDroiteCommande) * PWMPER;
    
    if (robotState.vitesseGaucheCommande > 0) {
        LEFT_MOTOR_ENL = 0; //pilotage de la pin en mode IO
        LEFT_MOTOR_INL = 1; //Mise à 1 de la pin
        LEFT_MOTOR_ENH = 1; //Pilotage de la pin en mode PWM
    } else {
        LEFT_MOTOR_ENH = 0; //pilotage de la pin en mode IO
        LEFT_MOTOR_INH = 1; //Mise à 1 de la pin
        LEFT_MOTOR_ENL = 1; //Pilotage de la pin en mode PWM
    }
    LEFT_MOTOR_DUTY_CYCLE = Abs(robotState.vitesseGaucheCommande) * PWMPER;
}

void PWMSetSpeedConsignePolaire()
{
    robotState.vitesseDroiteConsigne = robotState.vitesseLineaireConsigne + robotState.vitesseAngulaireConsigne * DISTROUES / 2;
    robotState.vitesseGaucheConsigne = robotState.vitesseLineaireConsigne - robotState.vitesseAngulaireConsigne * DISTROUES / 2;
    
////    //Correction Angulaire 
//    float erreurVitesseAngulaire =  robotState.vitesseAngulaireConsigne - robotState.vitesseAngulaireFromOdometry;
//    float correctionPVitesseAngulaire = erreurVitesseAngulaire*KpAng;
//    float correctionVitesseAngulairePourcent = correctionPVitesseAngulaire * COEFF_VITESSE_ANGULAIRE_PERCENT ; 
//    
//    
//    // Correction Linéaire
//    float erreurVitesseLineaire =  robotState.vitesseLineaireConsigne - robotState.vitesseLineaireFromOdometry; 
//    float correctionPVitesseLineaire = erreurVitesseLineaire*KpLin;
//    float correctionVitesseLineairePourcent = correctionPVitesseLineaire * COEFF_VITESSE_LINEAIRE_PERCENT ;
//    
//    //Génération des consignes droite et gauche
//    
//    correctionVitesseLineairePourcent= correctionPVitesseLineaire* 23;
//    correctionVitesseAngulairePourcent= correctionPVitesseAngulaire* 23;
//    robotState.vitesseDroiteCommande =correctionVitesseAngulairePourcent; //correctionVitesseLineairePourcent;+ correctionVitesseAngulairePourcent;
//    robotState.vitesseDroiteCommande = LimitToInterval(robotState.vitesseDroiteCommande, -100, 100);
//    robotState.vitesseGaucheCommande = correctionVitesseAngulairePourcent; //correctionVitesseLineairePourcent;//+ correctionVitesseAngulairePourcent;
//    robotState.vitesseGaucheCommande = LimitToInterval(robotState.vitesseGaucheCommande, -100, 100);
    
     
    float error_vit_ang = robotState.vitesseAngulaireConsigne - robotState.vitesseAngulaireFromOdometry;
    float out_corr_ang = error_vit_ang * KpAng;
    float corr_vit_ang = out_corr_ang * DISTROUES / 2 * COEFF_VITESSE_ANGULAIRE_PERCENT;
    
    float error_vit_lin = robotState.vitesseLineaireConsigne - robotState.vitesseLineaireFromOdometry;
    float out_corr_lin = error_vit_lin * KpLin;
    float corr_vit_lin = out_corr_lin * 1 / 2 * COEFF_VITESSE_LINEAIRE_PERCENT;
    
    robotState.vitesseDroiteCommande = corr_vit_lin + corr_vit_ang;
    robotState.vitesseDroiteCommande = LimitToInterval(robotState.vitesseDroiteCommande, -100, 100);
    robotState.vitesseGaucheCommande = corr_vit_lin - corr_vit_ang;
    robotState.vitesseGaucheCommande = LimitToInterval(robotState.vitesseGaucheCommande, -100, 100);
    
}