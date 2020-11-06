/* 
 * File:   CB_TX1.h
 * Author: Table2
 *
 * Created on 7 septembre 2020, 15:12
 */

#ifndef CB_TX1_H
#define	CB_TX1_H

void SendMessage(unsigned char*, int);
void SendOne(void);
int CB_TX1_RemainingSize(void);
unsigned char CB_TX1_Get(void);
void CB_TX1_Add(unsigned char);
unsigned char CB_TX1_IsTransmitting(void);


#endif	/* CB_TX1_H */

