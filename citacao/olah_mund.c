/* Tabauda do 7
   kuma
   17.04.2017 */
#include <16f628a.h>
#use delay(clock=8000000)
#use RS232(baud=9600, xmit=pin_b2, rcv=pin_b1)
#fuses hs, nolvp, noprotect, nobrownout, nowdt
void main(){
   printf ("olah mundo");
}
