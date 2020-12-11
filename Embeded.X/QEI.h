/* 
 * File:   QEI.h
 * Author: TP-EO-1
 *
 * Created on 20 novembre 2020, 10:01
 */

#ifndef QEI_H
#define	QEI_H

#ifdef	__cplusplus
extern "C" {
#endif

    #define DISTROUES 0.221 //Valeur à surement redefinir 
    #define RESOLUTION_M 0.00001629854 //distance parcourue pour un point 
    #define FREQ_ECH_QEI 250 //frequence echantillonage
    #define POSITION_DATA 0x0061 

    void InitQEI1();
    void InitQEI2();
    void QEIUpdateData();
    void SendPositionData();
    
#ifdef	__cplusplus
}
#endif

#endif	/* QEI_H */

