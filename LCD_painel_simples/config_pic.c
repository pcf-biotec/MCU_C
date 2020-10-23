/* 
        Configurações básicas do Microcontrolador PIC16F628A:
        Clock de 8000000Hz
        Sem brownout, low voltage power, watch dog timer, power up timer e sem proteção(ip)
*/

#include <16f628a.h>
#use delay (clock=8000000)
#fuses HS, nobrownout,nolvp,noput,noprotect,nowdt
        
