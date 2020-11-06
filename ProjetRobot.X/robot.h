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
            float vitesseGaucheCommandeCourante;
            float vitesseDroiteConsigne;
            float vitesseDroiteCommandeCourante;
            float distanceTelemetre0, distanceTelemetre1, distanceTelemetre2, distanceTelemetre3, distanceTelemetre4;
            unsigned char mode;            
        };
    };
} ROBOT_STATE_BITS;

extern volatile ROBOT_STATE_BITS robotState;
#endif /* ROBOT_H */
