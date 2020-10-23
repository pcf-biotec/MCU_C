/*      
        Programadores: Pedro C. e Marcos
        Nome do programa: Painel
        Data: 20/05/17
*/


//---Configurando MCU---//
#include "config_pic.c"
#include "lcd_r.c"

//---Defines---//

#define k1 pin_b1
#define k2 pin_b0
#define k3 pin_a0
#define bot1 pin_a1 
#define bot2 pin_a2
#define bot3 pin_a3

//---Variáveis---//
short tbot1=0;
short tbot2=0;
short tbot3=0;
short eb1=0;
short eb2=0;
short eb3=0;

//---Sub Rotinas---//

//Incicia LCD
void inicia_lcd(){
        lcd_ini();
        delay_ms(250);
        lcd_escreve("\f");
        lcd_pos_xy(1,1);
        printf(lcd_escreve,"IHM: K1  K2  K3 ");
}
// Trata botão1//
void tb1(){
        if(input(bot1)==1){
                if(tbot1==0){
                        tbot1=1;
                        eb1=~eb1;
                        output_bit(k1,eb1);
                }
        }
        else{
                tbot1=0;
        }
}
//Trata botão2//
void tb2(){
        if(input(bot2)==1){
                if(tbot2==0){
                        tbot2=1;
                        eb2=~eb2;
                        output_bit(k2,eb2);
                }
        }
        else{
                tbot2=0;
        }
}
//Trata botão3//
void tb3(){
        if(input(bot3)==1){
                if(tbot2==0){
                        tbot3=1;
                        eb3=~eb3;
                        output_bit(k3,eb3);
                }
        }
        else{
                tbot3=0;
        }
}
//Imprime//
void imprime(){
        if(eb1==1){
                lcd_pos_xy(5,2);
                printf(lcd_escreve," ON");
        }
        else{
                lcd_pos_xy(5,2);
                printf(lcd_escreve,"OFF");
        }
        if(eb2==1){
                lcd_pos_xy(9,2);
                printf(lcd_escreve," ON");
        }
        else{
                lcd_pos_xy(9,2);
                printf(lcd_escreve,"OFF");
        }
        if(eb3==1){
                lcd_pos_xy(13,2);
                printf(lcd_escreve," ON");
        }
        else{
                lcd_pos_xy(13,2);
                printf(lcd_escreve,"OFF");
        }
}        

//---ROTINA PRINCIPAL---//
void main(){
        inicia_lcd();
        while(1){
                tb1();
                tb2();
                tb3();
                imprime();
        }
}
         
                
