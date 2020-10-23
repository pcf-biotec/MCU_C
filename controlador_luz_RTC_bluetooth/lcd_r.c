/* 

   Biblioteca: Manipula��o de LCD � 4 bits
   Autor     : Rahul Martim Juliato
   Compilador: PICC-CCS
   
   Data: 10.12.2012
   Rev.:    03.2017

*/
   
   
/*
Exemplo de utiliza��o:
   
void main(void){

   lcd_ini();          //Inicializa o LCD  
   delay_ms(250);
   lcd_escreve('\f');
   delay_ms(250);

   
   lcd_pos_xy(2,1);    //Utiliza as rotinas de escrita
   printf(lcd_escreve,"LCD");
   lcd_pos_xy(2,2);
   printf(lcd_escreve,"Teste");
   delay_ms(2000);
      
   while(true){
   }
}

*/


/* Original   
#define lcd_enable     pin_D1      // pino enable do LCD
#define lcd_rs         pin_D0      // pino rs do LCD

#define lcd_d4         pin_D2      // pino de dados d4 do LCD
#define lcd_d5         pin_D3      // pino de dados d5 do LCD
#define lcd_d6         pin_D4      // pino de dados d6 do LCD
#define lcd_d7         pin_D5      // pino de dados d7 do LCD
*/

#define lcd_enable     pin_d3      // pino enable do LCD
#define lcd_rs         pin_d2      // pino rs do LCD

#define lcd_d4         pin_d4      // pino de dados d4 do LCD
#define lcd_d5         pin_d5      // pino de dados d5 do LCD
#define lcd_d6         pin_d6      // pino de dados d6 do LCD
#define lcd_d7         pin_d7     // pino de dados d7 do LCD



#define lcd_type 2           // 0=5x7, 1=5x10, 2=2 linhas
#define lcd_seg_lin 0x40    // Endere�o da segunda linha na RAM do LCD

// a constante abaixo define a seq��ncia de inicializa��o do m�dulo LCD
byte CONST INI_LCD[4] = {0x20 | (lcd_type << 2), 0xf, 1, 6};

//envia um comando para o pino d7 at� d4
void send_d7_d4(int8 address,int8 nibble)
{  
   

  if((nibble & 1)==1) output_high(pin_b4); else output_low(pin_b4);
  if((nibble & 2)==2) output_high(pin_b5); else output_low(pin_b5);
  if((nibble & 4)==4) output_high(pin_b6); else output_low(pin_b6);
  if((nibble & 8)==8) output_high(pin_b7); else output_low(pin_b7);
   delay_us(60);
   if(address)
     output_high(pin_b2);
      
   else
     output_low(pin_b2); 
      

 output_high(pin_b3);
 delay_us(2);
 output_low(pin_b3);
}

void lcd_envia_nibble( byte dado )
// envia um dado de quatro bits para o display
{
   // coloca os quatro bits nas saidas
   output_bit(lcd_d4,bit_test(dado,0));
   output_bit(lcd_d5,bit_test(dado,1));
   output_bit(lcd_d6,bit_test(dado,2));
   output_bit(lcd_d7,bit_test(dado,3));
   // d� um pulso na linha enable
   output_high(lcd_enable);
   output_low(lcd_enable);
}


void lcd_envia_byte( boolean endereco, byte dado )
{
   // coloca a linha rs em 0
   output_low(lcd_rs);
   // aguarda o display ficar desocupado
   //while ( bit_test(lcd_le_byte(),7) ) ;
   // configura a linha rs dependendo do modo selecionado
   output_bit(lcd_rs,endereco);
   delay_us(100);   // aguarda 100 us
   // caso a linha rw esteja definida, coloca em 0

   // desativa linha enable
   output_low(lcd_enable);
   // envia a primeira parte do byte
   lcd_envia_nibble(dado >> 4);
   // envia a segunda parte do byte
   lcd_envia_nibble(dado & 0x0f);
}


void lcd_ini()
// rotina de inicializa��o do display
{
   byte conta;
   output_low(lcd_d4);
   output_low(lcd_d5);
   output_low(lcd_d6);
   output_low(lcd_d7);
   output_low(lcd_rs);

   output_low(lcd_enable);
   delay_ms(15);
   // envia uma seq��ncia de 3 vezes 0x03
   // e depois 0x02 para configurar o m�dulo
   // para modo de 4 bits
   for(conta=1;conta<=3;++conta)
   {
      lcd_envia_nibble(3);
      delay_ms(5);
   }
   lcd_envia_nibble(2);
   // envia string de inicializa��o do display
   for(conta=0;conta<=3;++conta) lcd_envia_byte(0,INI_LCD[conta]);
}

void lcd_pos_xy( byte x, byte y)
{
   byte endereco;
   if(y!=1)                      //se y � diferente de 1, muda para segunda linha
      endereco = lcd_seg_lin;
   else
      endereco = 0;
   endereco += x-1;
   lcd_envia_byte(0,0x80|endereco);
}

//cgram(0); - mostra o car�ter especial armazenado na posi��o 0;
void cgram(int8 n){
   lcd_envia_byte(1,n);
}
//cgram_x(n); - n � a posi��o onde os caracteres s�o escritos, vai de 0 a 7
void cgram_x(int p){
lcd_envia_byte(0,0x40+p*8);
}

//for(i=0;i<=7 ;i++) lcd_envia_byte(1,aleatorio[i]); - respons�vel por enviar 8 bytes
void envia_bytes(void){

int8 i;
for(i=0 ;i < 3; i++)
   {
    send_d7_d4(0,0x03);
    delay_ms(5);
   }
    send_d7_d4(0,0x02);
    lcd_envia_byte(0, 0x28);
    delay_ms(5);
    lcd_envia_byte(0, 0x08);
    delay_ms(5);
    lcd_envia_byte(0, 0x01);
    delay_ms(5);
    lcd_envia_byte(0, 0x06);
    delay_ms(5);
    lcd_envia_byte(0, 0x0C);
}

// exemplo de caracter char - const char aleatorio[8]={0b01011,0b11011,0b01100,0b10011,0b11001,0b10101,0b00110,0b11100};

void lcd_escreve( char c)
// envia caractere para o display
{
   switch (c)
   {
     case '\f'    :   lcd_envia_byte(0,1);
              delay_ms(2);
            break;
     case '\n'   :
     case '\r'    :   lcd_pos_xy(1,2);
              break;
     case '\b'    :   lcd_envia_byte(0,0x10);
              break;
     case '\k' : lcd_envia_byte(0,14);
              delay_us(37);
               break;
     case '\z' : lcd_envia_byte(0,0x0c);
              delay_us(37);
               break;          
     default   :   lcd_envia_byte(1,c);
              break;
   }
}
