#ifndef IO_H
#define IO_H

// <editor-fold defaultstate="collapsed" desc="LEDS">
//Affectation des pins des LEDS
#define LED_ORANGE _LATC10 
#define LED_BLEUE _LATG7
#define LED_BLANCHE _LATG6
// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="Choppers">
#define MOTOR1_IN1 _LATB14
#define MOTOR1_IN2 _LATB15

#define MOTOR6_IN1 _LATC6
#define MOTOR6_IN2 _LATC7
// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="Motors">
// Left motor
#define LEFT_MOTOR_INH MOTOR1_IN1
#define LEFT_MOTOR_INL MOTOR1_IN2
#define LEFT_MOTOR_ENL IOCON1bits.PENL
#define LEFT_MOTOR_ENH IOCON1bits.PENH
#define LEFT_MOTOR_DUTY_CYCLE PDC1

// Right motor
#define RIGHT_MOTOR_INH MOTOR6_IN1
#define RIGHT_MOTOR_INL MOTOR6_IN2
#define RIGHT_MOTOR_ENL IOCON6bits.PENL
#define RIGHT_MOTOR_ENH IOCON6bits.PENH
#define RIGHT_MOTOR_DUTY_CYCLE PDC6
// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="JACK">
#define JACK _RA1
// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="Prototypes">
void InitIO();
// </editor-fold>

#endif /* IO_H */