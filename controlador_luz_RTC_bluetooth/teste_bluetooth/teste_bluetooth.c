///////////////////////////////////////////////////////////////////////////////
//       Spider                                                              //
//       09/12/2017                                                          //
//       Vers�o: 2.1                                                         //
/////////////////////////////////////////////////////////////////////////////// 

//---CONFIGURANDO Uc---//

#include  <16f877a.h>
#use      delay(clock=8000000)
#use      I2C(master, sda=PIN_C4, scl=PIN_C3)
#use      rs232(baud=9600,parity=N,xmit=PIN_C6,rcv=PIN_C7,bits=8)
#include  "lcd_r.c"
#fuses    HS, NOLVP, NOPUT, NOBROWNOUT, NOWDT, NOPROTECT

//!//Outputs - vers�o proto
//!#define  C1      pin_b0
//!#define  C2      pin_b1
//!#define  C3      pin_b2 
//!#define  C4      pin_b3 
//!#define  C5      pin_b4 
//!#define  C6      pin_b5 
//!#define  C7      pin_b6 
//!#define  C8      pin_b7

//!//Outputs - Vers�o placa
#define  C1      pin_b7
#define  C2      pin_b6
#define  C3      pin_b5
#define  C4      pin_b4
#define  C5      pin_b3
#define  C6      pin_b2 
#define  C7      pin_b1 
#define  C8      pin_b0


//Inputs IHM - comando
#define  COR     pin_a3 
#define  MAQ     pin_a4 
#define  RET     pin_a5
#define  RDN     pin_e0

//Inputs IHM - bot�es
#define  OK      pin_a0  
#define  MAIS    pin_a1  
#define  MENOS   pin_a2

//EEPROM
#define GET_C1   0x00
#define GET_C2   0x01
#define GET_C3   0x02
#define GET_C4   0x03
#define GET_C5   0x04
#define GET_C6   0x05
#define GET_C7   0x06
#define GET_C8   0x07
#define RDN_INI  0x08
#define RDN_FIM  0x09 
#define RDN_A_D  0x0A
#define GET_SEN  0x0B
#define GET_VER  0x0C
#define GET_INV  0x0D

//Bluetooth
#define albt    pin_c5

//Vari�veis
unsigned short trava_mais       = 0,   
               trava_menos      = 0,
               trava_OK         = 0,
               trava_indiv      = 0,
               trava_setor      = 0,
               trava_config     = 0,
               trava_clock      = 0,
               trava_COR        = 0,
               trava_MQ         = 0,
               trava_RET        = 0,
               trava_RDN        = 0,
               trava_menus      = 0,
               trava_config_N   = 0,
               trava_seta       = 0,
               trava_ronda      = 0,
               trava_relogio    = 0,
               finalizado       = 0,
               trava1           = 0,
               ccto1            = 0, 
               ccto2            = 0,
               ccto3            = 0,
               ccto4            = 0,
               ccto5            = 0,
               ccto6            = 0,
               ccto7            = 0,
               ccto8            = 0,  
               trava_COR_C1     = 0,
               trava_MAQ_C2     = 0,
               trava_RET_C3     = 0,
               trava_RDN_C4     = 0,
               trava_COR_C5     = 0,
               trava_MAQ_C6     = 0,
               trava_RET_C7     = 0,
               trava_okay       = 0,
               trava_dsmin      = 0,
               trava_dsh        = 0,
               trava_dsd        = 0,
               trava_dsmes      = 0,
               trava_dsa        = 0,
               trava_dsdsem     = 0,
               trava_entra      = 0,
               trava_ativado    = 0,
               trava_verao      = 0,
               trava_inverno    = 0,
               trava_senai      = 0,
               trava_auto       = 0;
               
unsigned int   limite_seta      = 3,
               limite_menus     = 6,
               limite_config    = 3,
               config_esse      = 0,
               seta             = 1,
               pre_programada   = 1,
               muda_menus       = 1,
               config           = 1,
               conectado        = 0,
               info                ,
               seg                 ,
               min                 ,
               hrs                 ,
               dia                 ,
               mes                 ,
               ano                 ,
               sem                 ,
               divisor2         = 0,
               divisor          = 0,
               i                = 0,
               ativado   [8]={0b11110, 0b11110, 0b11110, 0b11110, 0b11110, 0b00000, 0b00000, 0b00000},              
               desativado[8]={0b11110, 0b10010, 0b10010, 0b10010, 0b11110, 0b00000, 0b00000, 0b00000},
               lugar_seta[8]={0b00000, 0b00100, 0b00010, 0b11111, 0b00010, 0b00100, 0b00000, 0b00000},
               inicio    [8]={0b11100, 0b01110, 0b00111, 0b01110, 0b11100, 0b00000, 0b00000, 0b00000};
               
short          trava_rondac     = 0,
               ativa            = 0;

char           sem_v[4];

//INICIALIZA LCD E BLUETOOTH
void inicializa(){

   lcd_ini();
   delay_ms(250);
   lcd_escreve("\f");
   delay_ms(250);
   lcd_escreve("\k");
   lcd_escreve("\z");
   output_high(pin_e1);
   
   output_low(albt);
   delay_ms(1000);
   output_high(pin_c5);
   conectado = 0;

}

//APRESENTA O PROGRAMA
void apresentacao(){

   int z;                                                                 
   
   lcd_pos_xy(1,1);                                                          
   printf(lcd_escreve, "|     Lugh     |");
   delay_ms(500);
   lcd_pos_xy(1,2); 
   printf(lcd_escreve, "|     Light    |");
   delay_ms(2000);
   
   for (z=0;z<=16;z++){                                                       
      lcd_pos_xy(z,1);
      cgram(0);
      delay_ms(50);
      lcd_pos_xy(z,2);
      cgram(0);                                         
      delay_ms(50);   
   }      
   
   lcd_pos_xy(5,1);                               
   printf(lcd_escreve, " SENAI ");                                           
   delay_ms(2000);
   lcd_escreve("\f");
}

//GRAVA DADOS NA MEM�RIA CGRAM
void ativa_CGRAM(){

   cgram_x(0);
   for(i=0;i<=7 ;i++) lcd_envia_byte(1,inicio[i]);
   
   /*
   inicio    [8]={0b11100, 
                  0b01110, 
                  0b00111, 
                  0b01110, 
                  0b11100, 
                  0b00000, 
                  0b00000, 
                  0b00000},
   */

   cgram_x(1);
   for(i=0;i<=7 ;i++) lcd_envia_byte(1,ativado[i]);
   
   /*
   ativado   [8]={0b11110, 
                  0b11110, 
                  0b11110, 
                  0b11110, 
                  0b11110, 
                  0b00000, 
                  0b00000, 
                  0b00000},
   */
   
   cgram_x(2);
   for(i=0;i<=7 ;i++) lcd_envia_byte(1,desativado[i]); 
   
   /*
   desativado[8]={0b11110, 
                  0b10010, 
                  0b10010, 
                  0b10010, 
                  0b11110, 
                  0b00000, 
                  0b00000, 
                  0b00000},
   */
   
   cgram_x(3);
   for(i=0;i<=7 ;i++) lcd_envia_byte(1,lugar_seta[i]); 
   
   /*
   lugar_seta[8]={0b00000, 
                  0b00100, 
                  0b00010, 
                  0b11111, 
                  0b00010, 
                  0b00100, 
                  0b00000, 
                  0b00000},
   */
}

//INICIALIZA ROTINA DE INTERRUP��O
void timers(){

   setup_timer_0(RTCC_INTERNAL|RTCC_DIV_64);
   set_timer0(131);
   
   setup_timer_1(T1_INTERNAL|T1_DIV_BY_8);
   set_timer1(131);
   
   enable_interrupts(INT_TIMER1);
   enable_interrupts(INT_RTCC);
   enable_interrupts(GLOBAL);
   
}

//GUARDA DADOS NA MEM�RIA EEPROM       
void escreve_EEPROM(){
   
   write_eeprom(GET_C1, ccto1);
   write_eeprom(GET_C2, ccto2);
   write_eeprom(GET_C3, ccto3);
   write_eeprom(GET_C4, ccto4);
   write_eeprom(GET_C5, ccto5);
   write_eeprom(GET_C6, ccto6);   
   write_eeprom(GET_C7, ccto7);
   write_eeprom(GET_C8, ccto8);

   write_eeprom(RDN_A_D, trava_ativado);

}

//L� DADOS ARMAZENADOS NA MEM�RIA EEPROM 
 void leitura_EEPROM(){

   ccto1 = read_EEPROM(GET_C1);
   ccto2 = read_EEPROM(GET_C2);
   ccto3 = read_EEPROM(GET_C3);
   ccto4 = read_EEPROM(GET_C4);
   ccto5 = read_EEPROM(GET_C5);
   ccto6 = read_EEPROM(GET_C6);
   ccto7 = read_EEPROM(GET_C7);
   ccto8 = read_EEPROM(GET_C8);

   trava_ativado = read_EEPROM(RDN_A_D); 
   
   
   output_bit(C1,ccto1); 
   output_bit(C2,ccto2);                              
   output_bit(C3,ccto3);
   output_bit(C4,ccto4);
   output_bit(C5,ccto5);                                             
   output_bit(C6,ccto6);
   output_bit(C7,ccto7);
   output_bit(C8,ccto8);
   

   
}

//ATIVA MODULO BLUETOOTH 
void LUGH_LIGHT2(){

      switch(info){

         //ALTERA C4
         case 'I':
         ccto4 = ~ccto4;
         break;

         //ALTERA C5
         case 'O':
         ccto5 = ~ccto5;
         break;

         //ALTERA C6
         case 'P':
         ccto6 = ~ccto6;
         break;

         //ALTERA C7
         case 'A': 
         ccto7 = ~ccto7;
         break;

         //ALTERA C8
         case 'S':
         ccto8 = ~ccto8;
         break;

         case 'C':
         conectado=1;
         break;
      }
      break;
}

//ATIVA MODULO BLUETOOTH
void LUGH_LIGHT(){

   while (1){
   
      if(kbhit()==1){
         info=getc();
         
      }
                    
      switch(info){
     
         //ALTERA M�QUINAS
         case 'Q':
         ccto2 = ~ccto2;
         ccto3 = ~ccto3;
         ccto4 = ~ccto4;
         break;
       
         //ALTERA RET�FICA
         case 'W': 
         ccto5 = ~ccto5;
         ccto6 = ~ccto6;
         break;
        
         //ALTERA CORREDOR
         case 'E':
         ccto1 = ~ccto1;
         ccto7 = ~ccto7;
         break;
        
         //ALTERA RONDA NOTURNA
         case 'R':
         ccto8 = ~ccto8;
         break;
        
         //ALTERA C1
         case 'T':
         ccto1 = ~ccto1;
         break;
        
         //ALTERA C2
         case 'Y':
         ccto2 = ~ccto2;
         break;
        
         //ALTERA C3
         case 'U': 
         ccto3 = ~ccto3;
         break;
        LUGH_LIGHT2();
      }
      break;
   }
}

//LEITURA DE DADOS RTC - HORAS/MINUTOS/SEGUNDOS
void le_hora_rtc(){
   i2c_start();                  // Inicia com. p/ escrita
   /* Pede o hor�rio */
   i2c_write(0b11010000);        // End:1101000 0(escrita)
   i2c_write(0x00);              // End:00h do RTC(seg)                                  // comanda segundos
   /* Recebe o hor�rio */
   i2c_start();                  // Inicia com. p/ leitura
   i2c_write(0b11010001);        // End:1101000 1(leitura)  
   seg = i2c_read(1);            // seg<-RTC(00h) - ACK
   min = i2c_read(1);            // min<-RTC(01h) - ACK
   hrs = i2c_read(0);            // hrs<-RTC(02h) - no ACK
   i2c_stop();
}   

//ESCRITA DE DADOS RTC - HORAS/MINUTOS/SEGUNDOS
void escreve_hora_rtc(){
   i2c_start();                  // Inicia com. p/ escrita
   /* Seleciona hor�rio */
   i2c_write(0b11010000);        // End:1101000 0(escrita)
   i2c_write(0x00);              // End:00h do RTC(seg)                                  // comanda segundos
   /* Escreve hor�rio */
   i2c_write(seg);               // End:00h do RTC(seg)
   i2c_write(min);               // End:01h do RTC(min)
   i2c_write(hrs);               // End:02h do RTC(hrs)
   i2c_stop();
}   

//LEITURA DE DADOS RTC - DIA/M�S/ANO
void le_data_rtc(){

   i2c_start();                  // Inicia com. p/ escrita
   /* Pede a data */
   i2c_write(0b11010000);        // End:1101000 0(escrita)
   i2c_write(0x04);              // End:04h do RTC(dia)                                  // comanda segundos
   /* Recebe a data */
   i2c_start();                  // Inicia com. p/ leitura
   i2c_write(0b11010001);        // End:1101000 1(leitura)  
   dia = i2c_read(1);            // dia<-RTC(04h) - ACK
   mes = i2c_read(1);            // mes<-RTC(05h) - ACK
   ano = i2c_read(0);            // ano<-RTC(06h) - no ACK
   i2c_stop();  
}  

//ESCRITA DE DADOS RTC - DIA/M�S/ANO
void escreve_data_rtc(){
   i2c_start();                  // Inicia com. p/ escrita
   /* Seleciona data */
   i2c_write(0b11010000);        // End:1101000 0(escrita)
   i2c_write(0x04);              // End:04h do RTC(dia)                                  // comanda segundos
   /* Escreve data */
   i2c_write(dia);               // End:04h do RTC(dia)
   i2c_write(mes);               // End:05h do RTC(mes)
   i2c_write(ano);               // End:06h do RTC(ano)
   i2c_stop();
}   

//ESCRITA DE DADOS RTC - DIAS DA SEMANA
void escreve_sem_rtc(){
   i2c_start();                  // Inicia com. p/ escrita
   /* Seleciona data */
   i2c_write(0b11010000);        // End:1101000 0(escrita)
   i2c_write(0x03);              // End:03h do RTC(sem)                                  // comanda segundos
   /* Escreve semana */
   i2c_write(sem);               // End:03h do RTC(sem)
   i2c_stop();
}  

//LEITURA DE DADOS RTC - DIAS DA SEMANA
void le_sem_rtc(){

   i2c_start();                  // Inicia com. p/ escrita
   /* Pede a semana */
   i2c_write(0b11010000);        // End:1101000 0(escrita)
   i2c_write(0x03);              // End:03h do RTC(sem)                                  // comanda segundos
   /* Recebe a semana */
   i2c_start();                  // Inicia com. p/ leitura
   i2c_write(0b11010001);        // End:1101000 1(leitura)  
   sem = i2c_read(0);            // sem<-RTC(03h) - no ACK
   i2c_stop();
  
   if(sem <= 0x07){
      switch(sem){
         case 1:sem_v="Dom"; break;
         case 2:sem_v="Seg"; break;
         case 3:sem_v="Ter"; break;
         case 4:sem_v="Qua"; break;
         case 5:sem_v="Qui"; break;
         case 6:sem_v="Sex"; break;
         case 7:sem_v="Sab"; break;
      }
   }
   else{
      sem = 0X00;
      escreve_sem_rtc();
   }
} 

//RONDA NOTURNA AUTOM�TICA
void ronda_noturna_auto(){

   if(trava_ativado){
   
      if(trava_inverno){
         if((hrs >= 0x18 && hrs <= 0x23) || (hrs >= 0x00 && hrs < 0x06)){
            ccto8 = 1;
         }
         else{
            ccto8 = 0;
         }
      }
      
      if(trava_verao){
         if((hrs >= 0x20 && hrs <= 0x23) || (hrs >= 0x00 && hrs < 0x07)){
            ccto8 = 1;
         }
         else{
            ccto8 = 0; 
         }
      }
      
      if(trava_senai){
         if((hrs == 0x23) || (hrs >= 0x00 && hrs < 0x07)){
            ccto8 = 1;
         }
         else{
            ccto8 = 0;
         }
      }  
   }
}

//MENU: SETORIZADO 
void corredor(){

   if(input(COR)){
      if(!trava_COR){
         trava_COR = 1;
         ccto1 = ~ccto1;
         ccto7 = ~ccto7;
      }
   }
   else{
      trava_COR = 0;
   }
}

//MENU: SETORIZADO
void maquinas(){

   if(input(MAQ)){
      if(!trava_MQ){
         trava_MQ = 1;
         ccto2 = ~ccto2;
         ccto3 = ~ccto3;
         ccto4 = ~ccto4;
      }
   }
   else{
      trava_MQ = 0;
   }
}

//MENU: SETORIZADO
void retifica(){

   if(input(RET)){
      if(!trava_RET){
         trava_RET = 1;
         ccto5 = ~ccto5;
         ccto6 = ~ccto6;
      }
   }
   else{
      trava_RET = 0;
   }
} 

//MENU: SETORIZADO
void ronda_noturna(){

   if(!trava_ativado){
      if(input(RDN)){
         if(!trava_RDN){
            trava_RDN = 1;
            ccto8 = ~ccto8;                                         
         }
      }
      else{
         trava_RDN = 0;
      }
   }
} 

//MENU: SETORIZADO
void imprime_setor(){

   if(ccto1 && ccto7){
      lcd_pos_xy(2,2);
      printf(lcd_escreve, " ON");
   }
   else{
      lcd_pos_xy(2,2);
      printf(lcd_escreve, "OFF");
   }
   if(ccto2 && ccto3 && ccto4){
      lcd_pos_xy(6,2);
      printf(lcd_escreve, " ON");
   }
   else{
      lcd_pos_xy(6,2);
      printf(lcd_escreve, "OFF");
   }
   if(ccto5 && ccto6){
      lcd_pos_xy(10,2);
      printf(lcd_escreve, " ON");
   }
   else{
      lcd_pos_xy(10,2);
      printf(lcd_escreve, "OFF");
   }
   if(ccto8){
      lcd_pos_xy(14,2);
      printf(lcd_escreve, " ON");
   }
   else{
      lcd_pos_xy(14,2);
      printf(lcd_escreve, "OFF");
   }
}

//PASSAGEM MENU INDIVIDUAL - SETORIZADO
void regula_setorizado(){

   if(ccto1 | ccto7){ 
      ccto1 = 1;
      ccto7 = 1;   
   }
   
   if(ccto2 | ccto3 | ccto4){
      ccto2 = 1;
      ccto3 = 1; 
      ccto4 = 1;
   }
   
   if(ccto5 | ccto6){
      ccto5 = 1;
      ccto6 = 1;   
   }  
}

//MENU: SETORIZADO
void comanda_setor(){

   corredor();
   maquinas();
   retifica();
   ronda_noturna();
   regula_setorizado();
   imprime_setor();
   
}

//MENU: SETORIZADO
void setorizado(){

   if(!trava_setor){
      
      trava_setor = 1;    
      lcd_escreve("\f");
    
      lcd_pos_xy(1,1);
      printf(lcd_escreve, " COR MAQ RET RDN");
      
   }
   
      trava_indiv     = 0;
      trava_config    = 0;
      trava_clock     = 0;
      
      comanda_setor();
}

//MENU: INDIVIDUAL - NAVEGADOR
void navegador_seta(){

   if(trava_seta){
   
      // configura bot�o OK
      if(input(OK)){                          
         if(!trava_OK){  
            seta ++;
            trava_OK = 1;    
         }  
      }
      else{ 
         trava_OK = 0;     
      }
        
   // configura��es de limites 
      if(seta >= limite_seta)   seta  = 1;
      
   }
}

//MENU: INDIVIDUAL
void circuito_1(){

   if(input(COR)){
      if(!trava_COR_C1){
         trava_COR_C1 = 1;
         ccto1 = ~ccto1;                                                
      }
   }
   else{
      trava_COR_C1 = 0;
   }
}

//MENU: INDIVIDUAL
void circuito_2(){

   if(input(MAQ)){
      if(!trava_MAQ_C2){
         trava_MAQ_C2 = 1;
         ccto2 = ~ccto2;                                           
      }
   }
   else{
      trava_MAQ_C2 = 0;
   }
}

//MENU: INDIVIDUAL
void circuito_3(){

   if(input(RET)){
      if(!trava_RET_C3){
         trava_RET_C3 = 1;
         ccto3 = ~ccto3;                                             
      }
   }
   else{
      trava_RET_C3 = 0;
   }
}

//MENU: INDIVIDUAL
void circuito_4(){

   if(input(RDN)){
      if(!trava_RDN_C4){
         trava_RDN_C4 = 1;
         ccto4 = ~ccto4;                                           
      }
   }
   else{
      trava_RDN_C4 = 0;
   }
}

//MENU: INDIVIDUAL
void circuito_5(){

   if(input(COR)){
      if(!trava_COR_C5){
         trava_COR_C5 = 1;
         ccto5 = ~ccto5;                                                
      }
   }
   else{
      trava_COR_C5 = 0;
   }
} 

//MENU: INDIVIDUAL
void circuito_6(){

   if(input(MAQ)){
      if(!trava_MAQ_C6){
         trava_MAQ_C6 = 1;
         ccto6 = ~ccto6;                                           
      }
   }
   else{
      trava_MAQ_C6 = 0;
   }
}

//MENU: INDIVIDUAL
void circuito_7(){

   if(input(RET)){
      if(!trava_RET_C7){
         trava_RET_C7 = 1;
         ccto7 = ~ccto7;                                                 
      }
   }
   else{
      trava_RET_C7 = 0;
   }
}

//MENU: INDIVIDUAL
void imprime_um_a_quatro(){

   if(ccto1==1){
      lcd_pos_xy(10,1);
      cgram(1);
   }
   else{
      lcd_pos_xy(10,1);
      cgram(2);
   }
   if(ccto2==1){
      lcd_pos_xy(12,1);
      cgram(1);
   }
   else{
      lcd_pos_xy(12,1);
      cgram(2);
   }
   if(ccto3==1){
      lcd_pos_xy(14,1);
      cgram(1);
   }
   else{
      lcd_pos_xy(14,1);
      cgram(2);
   }
   if(ccto4==1){
      lcd_pos_xy(16,1);
      cgram(1);
   }
   else{
      lcd_pos_xy(16,1);
      cgram(2);
   }
}

//MENU: INDIVIDUAL
void imprime_cinco_a_oito(){

   if(ccto5==1){
      lcd_pos_xy(10,2);
      cgram(1);
   }
   else{
      lcd_pos_xy(10,2);
      cgram(2);
   }
   if(ccto6==1){
      lcd_pos_xy(12,2);
      cgram(1);
   }
   else{
      lcd_pos_xy(12,2);
      cgram(2);
   }
   if(ccto7==1){
      lcd_pos_xy(14,2);
      cgram(1);
   }
   else{
      lcd_pos_xy(14,2);
      cgram(2);
   }
   if(ccto8==1){
      lcd_pos_xy(16,2);
      cgram(1);
   }
   else{
      lcd_pos_xy(16,2);
      cgram(2);
   }
}

//MENU: INDIVIDUAL
void um_a_quatro(){

   trava_seta     = 1;
   trava_menus    = 1;
   trava_config_N = 0;

   if(seta==1){
   
      lcd_pos_xy(7,2);
      printf(lcd_escreve " ");
      lcd_pos_xy(7,1);
      cgram(3);
   
      circuito_1();
      circuito_2();
      circuito_3();
      circuito_4();
      
      imprime_um_a_quatro();
      imprime_cinco_a_oito();
      escreve_EEPROM();

   }
}

//MENU: INDIVIDUAL
void cinco_a_oito(){

   trava_seta     = 1;
   trava_menus    = 1;
   trava_config_N = 0;

   if(seta==2){
   
      lcd_pos_xy(7,1);
      printf(lcd_escreve " ");
      lcd_pos_xy(7,2);
      cgram(3);

   
      circuito_5();
      circuito_6();
      circuito_7();
      ronda_noturna();
      
      imprime_um_a_quatro();
      imprime_cinco_a_oito();
      escreve_EEPROM();
      
   } 
}

//MENU: INDIVIDUAL
void comanda_individual(){

   trava_menus    = 1;
   trava_config_N = 0;
   trava_seta     = 1;
   
   navegador_seta();
   
   switch(seta){                            
   
      case 1: um_a_quatro();   break;
      case 2: cinco_a_oito();  break;
      
   }
}

//MENU: INDIVIDUAL
void individual(){

   if(!trava_indiv){
      
      trava_indiv = 1;
      lcd_escreve("\f");
    
      lcd_pos_xy(1,1);
      printf(lcd_escreve "Modo    1 2 3 4 ");
      lcd_pos_xy(1,2);
      printf(lcd_escreve "Manual  5 6 7 8 ");
      
   }
   
      trava_setor     = 0;
      trava_config    = 0;
      trava_clock     = 0;

      comanda_individual();
      
}

//MENU: CLOCK
void mostra_horario(){

   trava_menus    = 1;
   trava_config_N = 0;
   trava_seta     = 0;

   if(!input(MAIS) && !input(MENOS)){
   
      le_hora_rtc();
      le_data_rtc();
      le_sem_rtc();
                      
      lcd_pos_xy(1,1);
      printf(lcd_escreve,"%02X:%02X:%02X  %s        \n", hrs, min, seg, sem_v);
      printf(lcd_escreve,"%02X.%02X.%02X             ", dia, mes, ano);
      
      delay_ms(200);
      
   }
}

//MENU: CLOCK
void clock(){

   trava_menus    = 1;
   trava_config_N = 0;
   trava_seta     = 0;
   
 if(!trava_clock){                                                          // trava para escrever 1 vez
      
      trava_clock = 1;    
      lcd_escreve("\f");
      lcd_pos_xy(1,1);
      printf(lcd_escreve,"%02X:%02X:%02X  %s        \n", hrs, min, seg, sem_v);
      printf(lcd_escreve,"%02X.%02X.%02X             ", dia, mes, ano);
      
   }
   
      trava_indiv     = 0;
      trava_setor     = 0;
      trava_config    = 0;
      
      mostra_horario();
   
}

//MENU: BLUETOOTH
void estado_bluetooth(){
   
   trava_menus    = 1;
   trava_config_N = 0;
   trava_seta     = 0;
   
   lcd_pos_xy(20,3);
      
    switch(info){
        case 'C':
        lcd_pos_xy(20,3);
        conectado=1;
        break;
    }
     
    if(conectado==1){
    
       lcd_pos_xy(1,1);
       printf(lcd_escreve, "  Bluetooth:   ");
       lcd_pos_xy(1,2);
       printf(lcd_escreve, "  Conectado    ");
       
    }
    
    else if(conectado==0){
       
       lcd_pos_xy(1,1);
       printf(lcd_escreve, "  Bluetooth:     ");
       lcd_pos_xy(1,2);
       printf(lcd_escreve, "  Desconectado   ");
       
    }
}

//MENU: BLUETOOTH
void bluetooth(){

   trava_menus     = 1;
   trava_config_N  = 0;
   trava_seta      = 0;
   
   trava_indiv     = 0;
   trava_setor     = 0;
   trava_config    = 0;
   trava_clock     = 0;
      
   estado_bluetooth();
   
}

//MENU: CONFIGURA - NAVEGADOR
void navegador_config_inicial(){

   if(!input(OK)) trava1 = 0;
   if(trava_config_N){
      
      // configura bot�o MAIS
      if(input(MAIS)){
         if(!trava_mais){      
            config ++;
            trava_mais = 1;     
         }     
      }
      else{   
         trava_mais = 0;       
      }
        
      // configura bot�o MENOS
      if(input(MENOS)){
         if(!trava_menos){  
            config --;
            trava_menos = 1; 
         }   
      }
      else{ 
         trava_menos = 0;        
      }
   
      if(input(OK) && !trava1){
         if(!trava_OK){    
            config_esse = 1;
            trava_OK = 1;     
         }   
      }
        
      else{   
         trava_OK = 0;     
      }
        
      // configura��es de limites
      if(config >= limite_config)       config      = 1;
      if(config <= 0)                   config      = 2; 
      
   }
}

//MENU: CONFIGURA/REL�GIO
void soma_decrementa_minuto(){
    
    lcd_escreve("\f"); 
    lcd_pos_xy(1,1);
    printf (lcd_escreve, "     MINUTO :");
    lcd_pos_xy(7,2);
    printf(lcd_escreve, "%02u" ,min);
 
   if (trava_dsmin){
      while(!input(OK)){
         if (input(MAIS)){
            if(min < 60){
               min ++;
               lcd_pos_xy(7,2);
               printf(lcd_escreve, "%02u" ,min);
               delay_ms(300);
            }
            else{
               min = 00;
            }
         }   
      
         if (input(MENOS)){
            if(min >= 01){
               min --;
              
               lcd_pos_xy(7,2);
               printf(lcd_escreve, "%02u", min);
               delay_ms(300);
            }
            else{
               min = 59;
            }
         }
      }
   }
}

//MENU: CONFIGURA/REL�GIO
void soma_decrementa_hora(){

   lcd_escreve("\f");
   lcd_pos_xy(1,1);
   printf (lcd_escreve "     HORA :");
   lcd_pos_xy(7,2);
   printf(lcd_escreve "%02u", hrs);
   delay_ms(50);   
   
   if(trava_dsh){
   
      while(!input(OK)){
         
         if (input(MAIS)){
            if(hrs <= 22){
               hrs ++;
               lcd_pos_xy(7,2);
               printf(lcd_escreve "%02u", hrs);
               delay_ms(300);
            }
            else{
               hrs = 00;
            }
         }   
      
         if (input(MENOS)){
            if(hrs >= 01){
               hrs --;
               lcd_pos_xy(7,2);
               printf(lcd_escreve "%02u", hrs);
               delay_ms(300);
            }
            else{
               hrs = 23;
            }
         }
      }
   }  
}

//MENU: CONFIGURA/REL�GIO
void soma_decrementa_dia_semana(){

   lcd_escreve("\f");
   sem = 1;
   sem_v= "Dom";
   lcd_pos_xy(1,1);
   printf (lcd_escreve " DIA DA SEMANA:");
   lcd_pos_xy(7,2);
   printf(lcd_escreve "%02s", sem_v);
   delay_ms(50);
   
   if(trava_dsdsem){
      while(!input(OK)){
         if (input(MAIS)){
            if(sem <= 06){
               sem ++;
               switch(sem){
                  case 1: (sem_v="Dom"); break;
                  case 2: (sem_v="Seg"); break;
                  case 3: (sem_v="Ter"); break;
                  case 4: (sem_v="Qua"); break;
                  case 5: (sem_v="Qui"); break;
                  case 6: (sem_v="Sex"); break;
                  case 7: (sem_v="Sab"); break;
                  
               }
               lcd_pos_xy(7,2);
               printf(lcd_escreve "%02s", sem_v);
               delay_ms(300);
            }
            else{
               sem = 0;
            }
         }
         if (input(MENOS)){
            if(sem >= 01){
               sem --;
               switch(sem){
                  case 1: sem_v="Dom"; break;
                  case 2: sem_v="Seg"; break;
                  case 3: sem_v="Ter"; break;
                  case 4: sem_v="Qua"; break;
                  case 5: sem_v="Qui"; break;
                  case 6: sem_v="Sex"; break;
                  case 7: sem_v="Sab"; break;
                  
               }
               lcd_pos_xy(7,2);
               printf(lcd_escreve "%02s", sem_v);
               delay_ms(300);
            }
            
            else{
               sem = 8;
            }
         }
      }
   }
}   

//MENU: CONFIGURA/REL�GIO
void soma_decrementa_dia(){
     
   lcd_escreve("\f");
   lcd_pos_xy(1,1);
   printf (lcd_escreve "      DIA:");
   lcd_pos_xy(7,2);
   printf(lcd_escreve "%02u", dia);
   delay_ms(50);
   
   if(trava_dsd){
   
      while(!input(OK)){
      
         if (input(MAIS)){
            if(dia <= 30){
               dia ++;
               lcd_pos_xy(7,2);
               printf(lcd_escreve "%02u", dia);
               delay_ms(300);
            }
            else{
               dia = 01;
            }
         }   
         
         if (input(MENOS)){
            if(dia >= 01){
               dia --;
               lcd_pos_xy(7,2);
               printf(lcd_escreve "%02u", dia);
               delay_ms(300);
            }
            else{
               dia = 31;
            }
         }
      } 
   }        
}

//MENU: CONFIGURA/REL�IO
void soma_decrementa_mes(){
   
   lcd_escreve("\f");
   lcd_pos_xy(1,1);
   printf (lcd_escreve "      MES:");
   lcd_pos_xy(7,2);
   printf(lcd_escreve "%02u", mes);
   delay_ms(50);
   
   if(trava_dsmes){
      while(!input(OK)){
         if (input(MAIS)){
            if(mes <= 11){
               mes ++;
               lcd_pos_xy(7,2);
               printf(lcd_escreve "%02u", mes);
               delay_ms(300);
            }
            else{
               mes = 01;
            }
         }   
      
         if (input(MENOS)){
            if(mes >= 01){
               mes --;
               lcd_pos_xy(7,2);
               printf(lcd_escreve "%02u", mes);
               delay_ms(300);
            }
            else{
               mes = 12;
            }
         }
      } 
   }      
}  

//MENU: CONFIGURA/REL�GIO
void soma_decrementa_ano(){

   ano = 17;
   lcd_escreve("\f");
   lcd_pos_xy(1,1);
   printf (lcd_escreve "      ANO:");
   lcd_pos_xy(7,2);
   printf(lcd_escreve "20%02u", ano);
   
   if(trava_dsa){
   
      while(!input(OK)){
      
         if (input(MAIS)){
            if(ano <= 99){
               ano ++;
               lcd_pos_xy(9,2);
               printf(lcd_escreve "%02u", ano);
               delay_ms(300);
            }
            else{
               ano = 0;
            }
         }   
      
         if (input(MENOS)){
            if(ano >= 01){
               ano --;
               lcd_pos_xy(9,2);
               printf(lcd_escreve "%02u", ano);
               delay_ms(300);
            }
            else{
               ano = 99;
            }
         }
      } 
      trava_okay = 1;
   } 
}

//MENU: CONFIGURA/RONDA NOTURNA (ALTERADO)
void inverno(){

   if(!trava_inverno){
   
      lcd_escreve("\f");
      lcd_pos_xy(1,1);
      printf(lcd_escreve, " Modo Inverno   ");
      lcd_pos_xy(1,2);
      printf(lcd_escreve, " 18 as 06 horas ");
      
      trava_inverno = 1;
      
   }
   
   trava_verao   = 0;
   trava_senai   = 0;
   
}

//MENU: CONFIGURA/RONDA NOTURNA (ALTERADO)
void verao(){

   if(!trava_verao){

      lcd_escreve("\f");
      lcd_pos_xy(1,1);
      printf(lcd_escreve, " Modo Verao     ");
      lcd_pos_xy(1,2);
      printf(lcd_escreve, " 20 as 07 horas ");
   
      trava_verao   = 1;
      
   }
   
   trava_inverno = 0;
   trava_senai   = 0;
   
}
   
//MENU: CONFIGURA/RONDA NOTURNA (ALTERADO)
void senai(){

   if(!trava_senai){

      lcd_escreve("\f");
      lcd_pos_xy(1,1);
      printf(lcd_escreve, " Modo Padrao    ");
      lcd_pos_xy(1,2);
      printf(lcd_escreve, " 23 as 07 horas ");
   
      trava_senai   = 1;
      
   }
   
   trava_inverno = 0;
   trava_verao   = 0;

}

//MENU: CONFIGURA/RONDA NOTURNA (ALTERADO)
void ronda_pre_programada(){

   lcd_escreve("\f");
   delay_ms(50);
   pre_programada = 1;
   
   printf(lcd_escreve, " Modo Inverno ");
   printf(lcd_escreve, " 18 as 06 horas ");
   
   if(trava_auto){
   
      while(!input(OK)){
      
         switch(pre_programada){
         
            case 1: inverno ();  break;
            case 2: verao   ();  break;
            case 3: senai   ();  break;
      
         }
         
         if(input(MAIS)){    
            pre_programada ++;
            delay_ms(300);
         }
         
         if(input(MENOS)){
         
            pre_programada --;
            delay_ms(300);
            
         }
         
         //LIMITES
         if(pre_programada >= 4) pre_programada = 1;
         if(pre_programada <= 0) pre_programada = 3;
         
      }
   }
}

//MENU: CONFIGURA/RONDA NOTURNA
void liga_desliga(){

   lcd_escreve("\f");
   lcd_pos_xy(1,1);
   printf(lcd_escreve, " RONDA NOTURNA: ");
   delay_ms(50);
   
   if(trava_entra){
   
      while(!input(OK)){
      
         if (input(MAIS)){
               ativa= ~ativa;
               delay_ms(300);
         }

         if (input(MENOS)){

               ativa= ~ativa;
               delay_ms(300);
         }
         
         if(ativa){
         
            trava_ativado = 1;
            lcd_pos_xy(1,2);
            printf(lcd_escreve, "    LIGADO      ");
         }
         else{
            
            trava_ativado = 0;
            lcd_pos_xy(1,2);
            printf(lcd_escreve, "    DESLIGADO   ");
         }
      }  
   }
}   
      
//CONVERS�O DECIMAL - HEXADECIMAL
int d2h(int x){

      int x_alto, x_baixo;
      
      x_alto  = 0.1*x;
      x_baixo = x-(10*x_alto);
      
      x_alto = x_alto*0b00010000;
      
      x = x_alto | x_baixo;
       
      return x;
}

//MENU: CONFIGURA/REL�GIO
void comanda_relogio(){

   trava_menus    = 0;
   trava_config_N = 1;
   trava_seta     = 0;
   config         = 1;
   
   if(config==1 && config_esse==1){
   
      config_esse    = 0;
      trava_config_N = 0;
      trava_menus    = 0;
      trava_seta     = 0;
   
      if(!finalizado){
      
         do{
            
            if(!trava_okay){
               min = 0;
               hrs = 0;
               dia = 1;
               mes = 1;
               ano = 0;
       
               
               trava_okay   = 0;
               trava_dsmin  = 1;
               trava_dsh    = 0;
               trava_dsd    = 0;
               trava_dsmes  = 0;
               trava_dsa    = 0;
               trava_dsdsem = 0;
               
               delay_ms(500);
               
               soma_decrementa_minuto();
              
               trava_okay   = 0;
               trava_dsmin  = 0;
               trava_dsh    = 1;
               trava_dsd    = 0;
               trava_dsmes  = 0;
               trava_dsa    = 0;
               trava_dsdsem = 0;
               
               delay_ms(500);
                              
               soma_decrementa_hora();
              
               trava_okay   = 0;
               trava_dsmin  = 0;
               trava_dsh    = 0;
               trava_dsd    = 0;
               trava_dsmes  = 0;
               trava_dsa    = 0; 
               trava_dsdsem = 1;
               
               delay_ms(500);
               
               soma_decrementa_dia_semana();
               
               trava_okay   = 0;
               trava_dsmin  = 0;
               trava_dsh    = 0;
               trava_dsd    = 1;
               trava_dsmes  = 0;
               trava_dsa    = 0; 
               trava_dsdsem = 0;
               
               delay_ms(500);
               
               soma_decrementa_dia();
               
               trava_okay   = 0;
               trava_dsmin  = 0;
               trava_dsh    = 0;
               trava_dsd    = 0;
               trava_dsmes  = 1;
               trava_dsa    = 0; 
               trava_dsdsem = 0;
               
               delay_ms(500);
               
               soma_decrementa_mes();
             
               trava_okay   = 0;
               trava_dsmin  = 0;
               trava_dsh    = 0;
               trava_dsd    = 0;
               trava_dsmes  = 0;
               trava_dsa    = 1;
               trava_dsdsem = 0;
               
               delay_ms(500);
               
               soma_decrementa_ano();
               
               trava_okay   = 1;
               trava_dsmin  = 0;
               trava_dsh    = 0;
               trava_dsd    = 0;
               trava_dsmes  = 0;
               trava_dsa    = 0;
               trava_dsdsem = 0;
            }
            
            min=d2h(min);
            hrs=d2h(hrs);
            dia=d2h(dia);
            mes=d2h(mes);
            ano=d2h(ano);
            
            escreve_hora_rtc();
            escreve_data_rtc();
            escreve_sem_rtc();

            if(input(OK) && trava_okay) finalizado=1;                                        // esse � o ultimo OK 
            
         }while(!finalizado && !trava_okay);
         
         trava_okay = 0;
         config_esse = 0;
         
      }
      
      escreve_hora_rtc();
      escreve_data_rtc();
      escreve_sem_rtc();

      muda_menus     = 1;
      trava_menus    = 1;
      trava_config_N = 0;
      trava_seta     = 0;
      
   }
}

//MENU: CONFIGURA/REL�GIO
void relogio(){
      
   trava_menus     = 0;
   trava_config_N  = 1;
   trava_seta      = 0;
   navegador_config_inicial();
   
   if(!trava_relogio){                                                          // trava para escrever 1 vez
      
      trava_relogio = 1;    
      lcd_escreve("\f");    
      lcd_pos_xy(1,1);
      printf(lcd_escreve, " Configurar:");
      lcd_pos_xy(1,2);
      printf(lcd_escreve, " Relogio   ");
      
   }
   
      trava_ronda  = 0;

      comanda_relogio();
   
}

//MENU: CONFIGURA/RONDA NOTURNA
void comanda_ronda(){

   trava_menus    = 0;
   trava_config_N = 1;
   trava_seta     = 0;
   config         = 2;

   if(config==2 && config_esse==1){
   
      config_esse    = 0;
      trava_menus    = 0;
      trava_config_N = 0;
      trava_seta     = 0;
      
      if(!finalizado){
      
         do{ 
            
            if(!trava_rondac){      

               trava_rondac   = 0; // Trava para garantir quebra do if
               trava_auto     = 1; // Trava para garantir entrada na sub-rotina ronda programada
               trava_entra    = 0;
               delay_ms(500);
               
               ronda_pre_programada();
               
               trava_rondac   = 0;
               trava_auto     = 0;
               trava_entra    = 1;
               delay_ms(500);
               
               liga_desliga();
               
               trava_rondac  = 1;
               trava_entra   = 0;
               trava_auto    = 0;
     
            }
            
            le_hora_rtc();
            
            if(input(OK) && trava_rondac) finalizado=1;                                        // esse � o ultimo OK 
            
         }while(!finalizado && !trava_rondac);
         
         trava_rondac = 0;
         config_esse = 0;
         hrs=d2h(hrs);

      }
      
      escreve_hora_rtc();
      
      muda_menus     = 1;
      trava_menus    = 1;
      trava_config_N = 0;
      trava_seta     = 0;
    
   }
}

//MENU: CONFIGURA/RONDA NOTURNA
void config_noturna(){
      
   trava_menus    = 0;
   trava_config_N = 1;
   trava_seta     = 0;
   
   if(!trava_ronda){                                                          // trava para escrever 1 vez
      
      trava_ronda = 1;    
      lcd_escreve("\f");
      lcd_pos_xy(1,1);
      printf(lcd_escreve, " Configurar:");
      lcd_pos_xy(1,2);
      printf(lcd_escreve, " Ronda Noturna");
   }
   
      trava_relogio  = 0;

      comanda_ronda();
   
}

//MENU: CONFIGURA
void configuracao_ativa(){

   if(muda_menus==5 && input(OK)){ 
   
      config_esse   = 0;
      config        = 1;
      trava_relogio = 0;
      delay_ms(300);

      do{
         
         trava_menus            = 0;
         trava_seta             = 0;
         trava_config_N         = 1;
         trava1                 = 1;
         if(!input(OK)) trava1  = 0;
         
         navegador_config_inicial();
      
         switch(config){
         
            case 1: relogio       ();  break;           //criar navegador para o RTC
            case 2: config_noturna();  break;           //criar navegador para o RDN
            
         }
      }while(!finalizado);
   }
}

//MENU: CONFIGURA
void configura(){

   trava_menus    = 1;
   trava_config_N = 0;
   trava_seta     = 0;
   
   if(!trava_config){                                                          // trava para escrever 1 vez
      
      trava_config = 1;    
      lcd_escreve("\f");
      lcd_pos_xy(1,1);
      printf(lcd_escreve, " Configuracao ");
      
   }
   
      trava_indiv     = 0;
      trava_setor     = 0;
      trava_clock     = 0;
      finalizado      = 0;
      config_esse     = 0;
      
      configuracao_ativa();

}

//NAVEGADOR ENTRE MENUS  
void navegador_menus(){
  
   if(trava_menus){
  
   // configura bot�o MAIS
   
      if(input(MAIS)){
      
         if(!trava_mais){
           
            muda_menus ++;
            trava_mais = 1;
            
         }
        
      }
        
      else{
        
         trava_mais = 0;
           
      }
        
   // configura bot�o MENOS
   
      if(input(MENOS)){
      
         if(!trava_menos){
           
            muda_menus --;
            trava_menos = 1;
           
         }
        
      }
        
      else{
        
         trava_menos = 0;
           
      }
   
   // configura��es de limites 
   
      if(muda_menus >= limite_menus) muda_menus  = 1;
      if(muda_menus <= 0)            muda_menus  = 5;
      
   }
}

#int_TIMER1
void TIMER1_eeprom_rdn(void){

  //Executa a tarefa a cada 4 ms
  if(divisor2>=4){             
  
      LUGH_LIGHT();
      LUGH_LIGHT2();
      divisor2=0;
      
   }
   
   divisor2++;
   set_timer1(131);
}

#int_RTCC
void RTCC_isr(void){

   //Executa a tarefa a cada 80 ms
   if(divisor>=20){
  
      escreve_EEPROM();
      leitura_EEPROM();
      ronda_noturna_auto();
      divisor=0;
   
   }
   divisor++;
   set_timer0(131);
   
}
     
void main(){

   inicializa();
   ativa_CGRAM();
   apresentacao();
   leitura_EEPROM();
   timers();

   while(1){
   
   trava_menus    = 1;
   trava_seta     = 0;
   trava_config_N = 0;
   navegador_menus();
   
      switch(muda_menus){
      
         case 1: setorizado();  break;
         case 2: individual();  break;
         case 3: clock();       break;
         case 4: bluetooth();   break;
         case 5: configura();   break;
         
      }
   }
}
