/* programa: Pisca_Pisca_8Bits
   Autor: Pedro C. ( KUMA )
   14.04.2017*/
#include <16f628a.h>
#use delay(clock=4000000)
#fuses xt, nolvp, nowdt, nobrownout, noprotect
void main(void){
   while(true){
      output_b(0x55);
      delay_ms(1000);
      output_b(0xAA);
      delay_ms(1000);
   }
}

