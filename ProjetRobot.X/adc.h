/* 
 * File:   adc.h
 * Author: TP-EO-5
 *
 * Created on 6 février 2020, 18:41
 */

#ifndef ADC_H
#define	ADC_H

    void InitADC1(void);
    unsigned int * ADCGetResult(void);
    unsigned char ADCIsConversionFinished(void);
    void ADCClearConversionFinishedFlag(void);
    void ADC1StartConversionSequence(void);

#endif	/* ADC_H */

