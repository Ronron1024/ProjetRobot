// <editor-fold defaultstate="collapsed" desc="Includes">
#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include <libpic30.h>
#include "ChipConfig.h"
#include "IO.h"
#include "timer.h"
#include "PWM.h"
#include "robot.h"
#include "adc.h"
#include "uart.h"
#include "uart_protocol.h"
#include "CB_TX1.h"
#include "CB_RX1.h"
#include "strat.h"
#include "QEI.h"
#include "main.h"
#include <stdlib.h>
// </editor-fold>

int main(void) {
    
    // <editor-fold defaultstate="collapsed" desc="Variables">
    // Telemeters [Must be replaced by the SICK Lidar]
    /*unsigned int* result;
    unsigned char ir_data[5] = {0};
    float volts = 0;*/
    
    // Misc
    int i;
    // </editor-fold>
    
    // <editor-fold defaultstate="collapsed" desc="Initialization">
    InitOscillator();
    InitIO();
    InitQEI1();
    InitQEI2();
    InitPWM();
    InitADC1();
    InitUART();
    //InitTimer23(); Void
    InitTimer1(); // QEI
    InitTimer4(); // timestamp
    
    // Init Random [May be done otherwise ?]
//    ADC1StartConversionSequence();
//    while (!ADCIsConversionFinished())
//    {
//        
//    }
    // </editor-fold>

    
    
    
    
    // FIX
    setMode(MANUAL);
    robotState.direction = STOP;
    
    
    
    // <editor-fold defaultstate="collapsed" desc="Main Loop">
    while (1) {
        
        // <editor-fold defaultstate="collapsed" desc="Telemeters conversion [must be replaced by SICK Lidar]">
        /*if (ADCIsConversionFinished()) 
        {
            ADCClearConversionFinishedFlag();

            result = ADCGetResult();
            volts = ((float) result[0]) * 3.3 / 4096 * 3.2;
            robotState.distanceTelemetre0 = 34 / volts - 5;
            volts = ((float) result[1]) * 3.3 / 4096 * 3.2;
            robotState.distanceTelemetre1 = 34 / volts - 5;
            volts = ((float) result[2]) * 3.3 / 4096 * 3.2;
            robotState.distanceTelemetre2 = 34 / volts - 5;
            volts = ((float) result[3]) * 3.3 / 4096 * 3.2;
            robotState.distanceTelemetre3 = 34 / volts - 5;
            volts = ((float) result[4]) * 3.3 / 4096 * 3.2;
            robotState.distanceTelemetre4 = 34 / volts - 5;
            
            ir_data[0] = (unsigned char) robotState.distanceTelemetre0;
            ir_data[1] = (unsigned char) robotState.distanceTelemetre1;
            ir_data[2] = (unsigned char) robotState.distanceTelemetre2;
            ir_data[3] = (unsigned char) robotState.distanceTelemetre3;
            ir_data[4] = (unsigned char) robotState.distanceTelemetre4;
            
            uartEncodeAndSendMessage(0x30, 5, ir_data);
        }*/
        // </editor-fold>
        
        for (i = 0; i < CB_RX1_GetDataSize(); i++)
        {
            uartDecodeMessage(CB_RX1_Get());
        }
    }
    // </editor-fold>

    
    
    return 0;
}