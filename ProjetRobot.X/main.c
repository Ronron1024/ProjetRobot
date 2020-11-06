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
#include "main.h"
#include <stdlib.h>


int main(void) {
    /***************************************************************************************************/
    //Initialisations
    /****************************************************************************************************/
    InitOscillator();
    InitIO();
    InitTimer23();
    InitTimer1();
    InitTimer4();
    InitPWM();
    InitADC1();
    InitUART();

    // VARIABLES
    char initRand = 1;
    unsigned int* result;
    float volts = 0;
    int i;
    
    unsigned char ir_data[5] = {0};

    /****************************************************************************************************/
    // Boucle Principale
    /****************************************************************************************************/
    setMode(MANUAL);
    robotState.direction = STOP;
    while (1) {
        
        if (ADCIsConversionFinished()) 
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
            
            if (initRand)
            {
                srand(result[2]);
                initRand=0;
                
            }
        }
        
//        uartEncodeAndSendMessage(0x80, 5, (unsigned char*) "Miaou");
//        __delay32(40000000);
//        
//        for (i = 0; i < CB_RX1_GetDataSize(); i++)
//        {
//            unsigned char c = CB_RX1_Get();
//            SendMessage(&c, 1);
//        }
//        __delay32(1000);
        
        for (i = 0; i < CB_RX1_GetDataSize(); i++)
        {
            uartDecodeMessage(CB_RX1_Get());
        }
    } 
    return 0;
}