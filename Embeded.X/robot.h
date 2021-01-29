/* 
 * File:   robot.h
 * Author: TP-EO-5
 *
 * Created on 16 janvier 2020, 15:43
 */

#ifndef ROBOT_H
#define ROBOT_H

#define OFF 0
#define ON 1
#define MANUAL 2
#define AUTO 3

typedef struct robotStateBITS {

    union {

        struct {
            unsigned char task;
            unsigned char direction;
            float vitesseGaucheConsigne;
            float vitesseGaucheCommande;
            float vitesseDroiteConsigne;
            float vitesseDroiteCommande;
            float vitesseAngulaireConsigne, vitesseLineaireConsigne;
            
            float distanceTelemetre0, distanceTelemetre1, distanceTelemetre2, distanceTelemetre3, distanceTelemetre4;
            unsigned char mode; 
            float vitesseDroitFromOdometry, vitesseGaucheFromOdometry;
            float vitesseLineaireFromOdometry, vitesseAngulaireFromOdometry;
            float xPosFromOdometry, xPosFromOdometry_1;
            float yPosFromOdometry, yPosFromOdometry_1;
            float angleRadianFromOdometry, angleRadianFromOdometry_1;
        };
    };
} ROBOT_STATE_BITS;

extern volatile ROBOT_STATE_BITS robotState;
#endif /* ROBOT_H */
