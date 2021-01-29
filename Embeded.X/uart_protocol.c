#include <xc.h>
#include "uart.h"
#include "uart_protocol.h"
#include "utilities.h"
#include "CB_TX1.h"
#include "CB_RX1.h"
#include "IO.h"
#include "PWM.h"
#include "robot.h"

#define EMPTY_TRAME_SIZE 6

StateReception rcv_state = Waiting;
unsigned short int msg_decoded_func = 0;
unsigned short int msg_decoded_payload_length = 0;
unsigned char msg_decoded_payload[128];
unsigned char msg_decoded_payload_index = 0;

unsigned char calcChecksum(unsigned short int msg_func, unsigned short int msg_payload_length, unsigned char* msg_payload)
{
    int i = 0;
    unsigned char ret = 0xFE;
    
    ret = (unsigned char)(ret ^ msg_func);
    ret = (unsigned char)(ret ^ msg_payload_length);
    for (i = 0; i < msg_payload_length; i++)
        ret ^= msg_payload[i];
    
    return ret;
}

void uartEncodeAndSendMessage(unsigned short int msg_func, unsigned short int msg_payload_length, unsigned char* msg_payload)
{
    int i = 0, j = 0;
    unsigned char msg[EMPTY_TRAME_SIZE + msg_payload_length];
    
    // START OF TRAME
    msg[i++] = 0xFE;
    
    // COMMAND
    msg[i++] = msg_func >> 8;
    msg[i++] = msg_func; // 2octets
    
    // PAYLOAD LENGTH
    msg[i++] = msg_payload_length >> 8;
    msg[i++] = msg_payload_length; // 2 octets
    
    // PAYLOAD
    for (j = 0; j < msg_payload_length; j++)
        msg[i++] = msg_payload[j];
    
    // CHECKSUM
    msg[i++] = calcChecksum(msg_func, msg_payload_length, msg_payload);
    
    // SEND MSG
    SendMessage(msg, i);
}

void uartDecodeMessage(unsigned char c)
{
    switch (rcv_state)
    {
        case Waiting:
            if (c == 0xFE)
                rcv_state = FunctionMSB;
            break;
            
        case FunctionMSB:
            msg_decoded_func += c;
            msg_decoded_func <<= 8;
            rcv_state = FunctionLSB;
            break;
            
        case FunctionLSB:
            msg_decoded_func += c;
            rcv_state = PayloadLengthMSB;
            break;
            
        case PayloadLengthMSB:
            msg_decoded_payload_length += c;
            msg_decoded_payload_length <<= 8;
            rcv_state = PayloadLengthLSB;
            break;
            
        case PayloadLengthLSB:
            msg_decoded_payload_length += c;
            if (msg_decoded_payload_length == 0)
                rcv_state = Checksum;
            else
                rcv_state = Payload;
            break;
            
        case Payload:
            msg_decoded_payload[msg_decoded_payload_index++] = c;
            if (msg_decoded_payload_index == msg_decoded_payload_length)
                rcv_state = Checksum;
            break;
            
        case Checksum:
            if (calcChecksum(msg_decoded_func, msg_decoded_payload_length, msg_decoded_payload) == c)
                uartProcessDecodedMessage(msg_decoded_func, msg_decoded_payload_length, msg_decoded_payload);
                
            msg_decoded_func = 0;
            msg_decoded_payload_length = 0;
            msg_decoded_payload_index = 0;
            
            rcv_state = Waiting;              
            break;
            
        default:
            rcv_state = Waiting;
            break;
    }
}

void uartProcessDecodedMessage(unsigned short int func, unsigned short int payload_length, unsigned char* payload)
{
    switch (func)
    {
        case 0x20: // Led setup
            switch (payload[0])
            {
                case 1:
                    LED_BLEUE = payload[1];
                    break;
                case 2:
                    LED_ORANGE = payload[1];
                    break;
                case 3:
                    LED_BLANCHE = payload[1];
                    break;
            }
            break;
            
        case 0x50:
            robotState.vitesseLineaireConsigne = getFloat(payload, 0);
            PWMSetSpeedConsigne(getFloat(payload, 0), MOTEUR_DROIT);
            PWMSetSpeedConsigne(getFloat(payload, 0), MOTEUR_GAUCHE);
            break;

        default: // Unknow command
            break;
    }
}