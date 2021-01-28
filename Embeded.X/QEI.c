// <editor-fold defaultstate="collapsed" desc="Includes">
#include <p33EP512GM706.h>
#include <math.h>
#include "QEI.h"
#include "robot.h"
#include "timer.h"
#include "utilities.h"
#include "uart_protocol.h"

// </editor-fold>

void InitQEI1()
{
    QEI1IOCbits.SWPAB = 1; // QEAx and QEBx are swapped
    QEI1GECL = 0xFFFF;
    QEI1GECH = 0xFFFF;
    QEI1CONbits.QEIEN = 1; // Enable QEI Module
}

void InitQEI2()
{
    QEI2IOCbits.SWPAB = 1; // not swapped
    QEI2GECL = 0xFFFF;
    QEI2GECH = 0xFFFF;
    QEI2CONbits.QEIEN = 1;
}

float QeiDroitPosition_T_1, QeiGauchePosition_T_1, QeiDroitPosition, QeiGauchePosition;
float delta_d, delta_g, delta_theta, dx;

void QEIUpdateData()
{
    // Save old data
    QeiDroitPosition_T_1 = QeiDroitPosition;
    QeiGauchePosition_T_1 = QeiGauchePosition;
    
    // Update 
    long QEI1RawValue = POS1CNTL ;
    QEI1RawValue +=((long)POS1HLD<<16);
    
    long QEI2RawValue = POS2CNTL ;
    QEI2RawValue +=((long)POS2HLD<<16);
    
    //Conversion m (regle pour la taille des roues codeuses)
    QeiDroitPosition = RESOLUTION_M*QEI1RawValue;
    QeiGauchePosition = -RESOLUTION_M*QEI2RawValue;
    
    //Calcul des deltas de positions
    delta_d = QeiDroitPosition - QeiDroitPosition_T_1;
    delta_g = QeiGauchePosition - QeiGauchePosition_T_1;
    //delta_theta = atan((delta_d - delta_g) /DISTROUE);
    delta_theta = (delta_d - delta_g) / DISTROUES ; 
    dx = (delta_d + delta_g)/2;
    
    //Calcul des vitesses 
    //attention a remultiplier par la frequence d echantillonage 
    robotState.vitesseDroitFromOdometry = delta_d*FREQ_ECH_QEI;
    robotState.vitesseGaucheFromOdometry = delta_g*FREQ_ECH_QEI;
    robotState.vitesseLineaireFromOdometry = (robotState.vitesseDroitFromOdometry + robotState.vitesseGaucheFromOdometry)/2;
    robotState.vitesseAngulaireFromOdometry = delta_theta * FREQ_ECH_QEI;
    
    // Mise à jour du positionnement terrain à t-1
    robotState.xPosFromOdometry_1 = robotState.xPosFromOdometry;
    robotState.yPosFromOdometry_1 = robotState.yPosFromOdometry;
    robotState.angleRadianFromOdometry_1 = robotState.angleRadianFromOdometry;
    
    // Calcul des positions dans le référentiel du terrain
    robotState.xPosFromOdometry += dx*cos(robotState.angleRadianFromOdometry);
    robotState.yPosFromOdometry += dx*sin(robotState.angleRadianFromOdometry);
    robotState.angleRadianFromOdometry += delta_theta;
    
    if (robotState.angleRadianFromOdometry > PI)
        robotState.angleRadianFromOdometry -= 2*PI;
    else if (robotState.angleRadianFromOdometry < -PI)
        robotState.angleRadianFromOdometry += 2*PI;
}

void SendPositionData()
{
    unsigned char positionPayload[28];
    getBytesFromInt32(positionPayload, 0, timestamp);
    getBytesFromFloat(positionPayload, 4, robotState.xPosFromOdometry);
    getBytesFromFloat(positionPayload, 8, robotState.yPosFromOdometry);
    getBytesFromFloat(positionPayload, 12, robotState.angleRadianFromOdometry);
    getBytesFromFloat(positionPayload, 16, robotState.vitesseLineaireFromOdometry);
    getBytesFromFloat(positionPayload, 20, robotState.vitesseAngulaireFromOdometry);
    getBytesFromFloat(positionPayload, 24, robotState.vitesseLineaireConsigne);
    uartEncodeAndSendMessage(POSITION_DATA, 28, positionPayload);
}