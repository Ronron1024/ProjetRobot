// <editor-fold defaultstate="collapsed" desc="Includes">
#include <xc.h>
#include "IO.h"
#include "main.h"
// </editor-fold>

void InitIO() {
    
    // <editor-fold defaultstate="collapsed" desc="Analog In">
    // IMPORTANT : désactiver les entrées analogiques, sinon on perd les entrées numériques
    ANSELA = 0; // 0 desactive
    ANSELB = 0;
    ANSELC = 0;
    ANSELD = 0;
    ANSELE = 0;
    ANSELF = 0;
    ANSELG = 0;
    // </editor-fold>
    
    // <editor-fold defaultstate="collapsed" desc="Leds">
    // LED
    _TRISC10 = 0; // LED Orange
    _TRISG6 = 0; //LED Blanche
    _TRISG7 = 0; // LED Bleue
    // </editor-fold>
    
    // <editor-fold defaultstate="collapsed" desc="Motors">
    _TRISB14 = 0;
    _TRISB15 = 0;
    _TRISC6 = 0;
    _TRISC7 = 0;
    // </editor-fold>

    // <editor-fold defaultstate="collapsed" desc="JACK">
    _TRISA1 = 1;
    // </editor-fold>

    // <editor-fold defaultstate="collapsed" desc="UART">
    _U1RXR = 24;
    _RP36R = 1; 
    // </editor-fold>
 
}
