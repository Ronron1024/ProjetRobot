#ifndef UART_PROTOCOL_H
#define	UART_PROTOCOL_H

typedef enum
{
    Waiting,
    FunctionMSB,
    FunctionLSB,
    PayloadLengthMSB,
    PayloadLengthLSB,
    Payload,
    Checksum
} StateReception;

unsigned char calcChecksum(unsigned short int, unsigned short int, unsigned char*);
void uartEncodeAndSendMessage(unsigned short int, unsigned short int, unsigned char*);
void uartDecodeMessage(unsigned char);
void uartProcessDecodedMessage(unsigned short int, unsigned short int, unsigned char*);

#endif	/* UART_PROTOCOL_H */

