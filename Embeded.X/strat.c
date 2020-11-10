#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include "ChipConfig.h"
#include "IO.h"
#include "timer.h"
#include "PWM.h"
#include "robot.h"
#include "adc.h"
#include "main.h"
#include "strat.h"


void OperatingSystemLoop(void) {
    switch (robotState.task) {
        case STATE_ATTENTE:
            PWMSetSpeedConsigne(0, MOTEUR_DROIT);
            PWMSetSpeedConsigne(0, MOTEUR_GAUCHE);
            if (robotState.mode == AUTO)
                robotState.task = STATE_ATTENTE_EN_COURS;
            break;

        case STATE_ATTENTE_EN_COURS:
            if (timestamp > 2000)
                robotState.task = STATE_AVANCE;
            break;

        case STATE_AVANCE:
            PWMSetSpeedConsigne(robotState.direction == FORWARD ? NORMAL_SPEED : -NORMAL_SPEED, MOTEUR_DROIT);
            PWMSetSpeedConsigne(robotState.direction == FORWARD ? NORMAL_SPEED : -NORMAL_SPEED, MOTEUR_GAUCHE);
            robotState.task = STATE_AVANCE_EN_COURS;
            break;
        case STATE_AVANCE_EN_COURS:
            if (robotState.mode == AUTO)
                SetNextRobotStateInAutomaticMode();
            break;

        case STATE_TOURNE_GAUCHE:
            PWMSetSpeedConsigne(55, MOTEUR_DROIT);
            PWMSetSpeedConsigne(30, MOTEUR_GAUCHE);
            robotState.task = STATE_TOURNE_GAUCHE_EN_COURS;
            break;
        case STATE_TOURNE_GAUCHE_EN_COURS:
            if (robotState.mode == AUTO)
                SetNextRobotStateInAutomaticMode();
            break;

        case STATE_TOURNE_DROITE:
            PWMSetSpeedConsigne(30, MOTEUR_DROIT);
            PWMSetSpeedConsigne(55, MOTEUR_GAUCHE);
            robotState.task = STATE_TOURNE_DROITE_EN_COURS;
            break;
        case STATE_TOURNE_DROITE_EN_COURS:
            if (robotState.mode == AUTO)
                SetNextRobotStateInAutomaticMode();
            break;

        case STATE_TOURNE_SUR_PLACE_GAUCHE:
            PWMSetSpeedConsigne(25, MOTEUR_DROIT);
            PWMSetSpeedConsigne(-25, MOTEUR_GAUCHE);
            robotState.task = STATE_TOURNE_SUR_PLACE_GAUCHE_EN_COURS;
            break;
        case STATE_TOURNE_SUR_PLACE_GAUCHE_EN_COURS:
            if (robotState.mode == AUTO)
                SetNextRobotStateInAutomaticMode();
            break;
            
        case STATE_TOURNE_SUR_PLACE_DROITE:
            PWMSetSpeedConsigne(-25, MOTEUR_DROIT);
            PWMSetSpeedConsigne(25, MOTEUR_GAUCHE);
            robotState.task = STATE_TOURNE_SUR_PLACE_DROITE_EN_COURS;
            break;
        case STATE_TOURNE_SUR_PLACE_DROITE_EN_COURS:
            if (robotState.mode == AUTO)
                SetNextRobotStateInAutomaticMode();
            break;
          
        case STATE_SLOW: 
            PWMSetSpeedConsigne(SLOW_SPEED, MOTEUR_DROIT);
            PWMSetSpeedConsigne(SLOW_SPEED, MOTEUR_GAUCHE);
            robotState.task = STATE_SLOW_EN_COURS;
            break;
        case STATE_SLOW_EN_COURS:
            if (robotState.mode == AUTO)
                SetNextRobotStateInAutomaticMode();
            break;
            
        case STATE_FAST: 
            PWMSetSpeedConsigne(FAST_SPEED, MOTEUR_DROIT);
            PWMSetSpeedConsigne(FAST_SPEED, MOTEUR_GAUCHE);
            robotState.task = STATE_FAST_EN_COURS;
            break;
        case STATE_FAST_EN_COURS:
            if (robotState.mode == AUTO)
                SetNextRobotStateInAutomaticMode();
            break; 

        default:
            robotState.task = STATE_ATTENTE;
            break;
    }
    PWMUpdateSpeed();
}

unsigned char nextStateRobot = 0;

void SetNextRobotStateInAutomaticMode() {
    unsigned char positionObstacle = PAS_D_OBSTACLE;

    //Détermination de la position des obstacles en fonction des télémètres
    if (robotState.distanceTelemetre2 < 40 || robotState.distanceTelemetre0 < 40 || robotState.distanceTelemetre4 < 20)
        positionObstacle = OBSTACLE_A_VENIR_EN_FACE;
    if (robotState.distanceTelemetre2 < 20) //Obstacle en face
        positionObstacle = OBSTACLE_EN_FACE;
    else if (robotState.distanceTelemetre0 < 15) //Obstacle à droite
        positionObstacle = OBSTACLE_A_DROITE;
    else if (robotState.distanceTelemetre4 < 15) //Obstacle en face
        positionObstacle = OBSTACLE_A_GAUCHE;
    else if (robotState.distanceTelemetre1 < 25) //Obstacle à droite
        positionObstacle = OBSTACLE_A_DROITE;
    else if (robotState.distanceTelemetre3 < 25) //Obstacle à gauche
        positionObstacle = OBSTACLE_A_GAUCHE;
    else if (robotState.distanceTelemetre2 > 40 && robotState.distanceTelemetre0 > 40 && robotState.distanceTelemetre4 > 40)
        positionObstacle = PAS_D_OBSTACLE;

    //Détermination de l'état à venir du robot
    if (positionObstacle == PAS_D_OBSTACLE)
        nextStateRobot = STATE_AVANCE;
    else if (positionObstacle == OBSTACLE_A_DROITE)
        nextStateRobot = STATE_TOURNE_GAUCHE;
    else if (positionObstacle == OBSTACLE_A_GAUCHE)
        nextStateRobot = STATE_TOURNE_DROITE;
    else if (positionObstacle == OBSTACLE_A_VENIR_EN_FACE)
        nextStateRobot = STATE_RALENTIR;
    else if (positionObstacle == OBSTACLE_EN_FACE)
    {
        if (robotState.distanceTelemetre0 < robotState.distanceTelemetre4)
            nextStateRobot = STATE_TOURNE_SUR_PLACE_GAUCHE;
        else
            nextStateRobot = STATE_TOURNE_SUR_PLACE_DROITE;
    }
    
    //Si l'on n'est pas dans la transition de l'étape en cours
    if (nextStateRobot != robotState.task - 1)
        robotState.task = nextStateRobot;
}



void setMode(char mode)
{
    robotState.mode = mode;
    
    // RESET
    LED_ORANGE = LED_BLEUE = LED_BLANCHE = OFF;
    PWMSetSpeedConsigne(0,MOTEUR_DROIT);
    PWMSetSpeedConsigne(0,MOTEUR_GAUCHE);
    PWMUpdateSpeed();
    robotState.task = STATE_ATTENTE;
    
    if (mode == OFF) // Restart timestamp
        timestamp = 0;
}