// LCD module connections
sbit LCD_RS at RD0_bit;
sbit LCD_EN at RD1_bit;
sbit LCD_D4 at RA0_bit;
sbit LCD_D5 at RA1_bit;
sbit LCD_D6 at RA2_bit;
sbit LCD_D7 at RA3_bit;
sbit LCD_RS_Direction at TRISD0_bit;
sbit LCD_EN_Direction at TRISD1_bit;
sbit LCD_D4_Direction at TRISA0_bit;
sbit LCD_D5_Direction at TRISA1_bit;
sbit LCD_D6_Direction at TRISA2_bit;
sbit LCD_D7_Direction at TRISA3_bit;
// End LCD module connections


short int horas,segundos,minutos,dias,meses,years;
int matriz[16]={0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F,0x77,0x7C,0x39,0x5E,0x79,0x71};
char texto;
int interrupcion=0;
void main() {
OSCCON.B7=1;//seleccionamos modo idle/sleep  bit IDLEN
OSCCON.B6=1;//configuramos oscilador interno a 4 MHZ
OSCCON.B5=1;
OSCCON.B4=0;//termina configuracion de oscilador interno a 1Mhz
OSCCON.B1=1;//seleccionamos el oscilador interno
OSCCON.B0=0;
ADCON1=0X0F;//configuramos puertos analogicos como puertos digitales
ADCON0.B5=0;
ADCON0.B4=0;
ADCON0.B3=0;
ADCON0.B2=0;
ADCON0.B0=0;//apagamos convertidor a/d
LATB=0;
//LATD=0;
LATA=0;
LATC=0;
//-------------------aquui voy a apagar cosas que no vamos a usar
//T2CON.B2=0;//apagamos timer2 por si estuviera encendido
//T3CON.B0=0;
//T1CON.B0=0;

//-------------------aquui voy a apagar cosas que no vamos a usar
//TRISD=0X00;//todo el puerto d como salida
TRISB.B2=1;//puerto b0 como entrada para la int0
TRISA.B4=1;//entrada digital para el T0CKI
TRISC.B0=1;//entrada para dormir al pic
INTCON=0x80;//habilitamos interrupciones globales
INTCON2=0X80;//interrupcion externa 2 en falling edge
INTCON3=0X10;//habilitamos interrupcion externa int2
INTCON3.B1=0;//apagamos bandera de int2
T0CON=0b11101000;//configuramos timer0 con reloj externo y en modo de 8 bit, sin prescalamiento
TMR0L=0;
OSCTUNE.B7=0;//seleccionamos intrc como fuente del reloj de 31kHz


I2C1_Init(100000);
 Lcd_Init();                        // Initialize LCD
  Lcd_Cmd(_LCD_CLEAR);               // Clear display
  Lcd_Cmd(_LCD_CURSOR_OFF);          // Cursor off
  Lcd_out(1,1,"gotitas");

delay_ms(2000);

I2C1_Start();
   I2C1_Wr(0xD2);                // reinicio preventivo
   I2C1_Wr(0x6B);
   I2C1_Wr(0x80);
   I2C1_Stop();
   Delay_ms(2000);
    //inicia config acelerometro
        I2C1_Start();                // Configuring MPU6050 and interruption
       I2C1_Wr(0xD2);                          // 19 x(4F)
       I2C1_Wr(0x19);                           //D0
       I2C1_Wr(0x07);
       I2C1_Stop();
       
  I2C1_Start();
   I2C1_Wr(0xD2);                //    (0x6B) cycle=0 y sleep=0
   I2C1_Wr(0x6B);
   I2C1_Wr(0x00);
   I2C1_Stop();

    I2C1_Start();
   I2C1_Wr(0XD2);                //    (0x6C) deshabilitar STBY_XA, STBY_YA, STBY_ZA = 0
   I2C1_Wr(0x6C);
   I2C1_Wr(0x00);
   I2C1_Stop();

      I2C1_Start();
   I2C1_Wr(0xD2);                //(0x1C) High Pass Filer [2:0] = b000
   I2C1_Wr(0x1B);
   I2C1_Wr(0x00);
   I2C1_Stop();


    I2C1_Start();
   I2C1_Wr(0xD2);                //(0x1C) High Pass Filer [2:0] = b000
   I2C1_Wr(0x1C);
   I2C1_Wr(0x00);
   I2C1_Stop();


    I2C1_Start();
   I2C1_Wr(0xD2);                //   (0x38)= (0x40) (solo activa motion interrupt)
   I2C1_Wr(0x38);
   I2C1_Wr(0x40);
   I2C1_Stop();


    I2C1_Start();
   I2C1_Wr(0xD2);                // Establecer duración de motion interrupt (0x20) value=1
   I2C1_Wr(0x20);
   I2C1_Wr(0x01);
   I2C1_Stop();
   

    I2C1_Start();
   I2C1_Wr(0xD2);                //interrupt data ready (INTERRUPT) STATUS
   I2C1_Wr(0x3A);
   I2C1_Wr(0x01);
   I2C1_Stop();

     I2C1_Start();
   I2C1_Wr(0xD2);                //(0X1F) de 1-255 LSBs. Donde 1=32mg y 255=8.16g (valor recomendado =20 para pequeños     movimientos)
   I2C1_Wr(0x1F);
   I2C1_Wr(0x01);
   I2C1_Stop();


     I2C1_Start();
   I2C1_Wr(0xD2);                // 1A   (0b00000111)  MOtion HPF HOLD    (07)
   I2C1_Wr(0x1A);
   I2C1_Wr(0xFF);
   I2C1_Stop();

     I2C1_Start();
   I2C1_Wr(0xD2);                //Establecer la frecuencia de Wake-up (0x6C) bits 7 y 6 a frecuencia deseada
   I2C1_Wr(0x6C);
   I2C1_Wr(0x60);
   I2C1_Stop();

    I2C1_Start();
   I2C1_Wr(0xD2);                //Establecer la frecuencia de Wake-up (0x6C) bits 7 y 6 a frecuencia deseada
   I2C1_Wr(0x6B);
   I2C1_Wr(0x10);
   I2C1_Stop();
     //termina config acelerometro
     lcd_out(1,9,"A");//acelerometro listo
  //inicia config del reloj ds3231
  


   I2C1_Start();
I2C1_Wr(0xD0); //direccion ds1307 1101000 + 0 para escribir
I2C1_Wr(0X00);//direccion de los segundos
I2C1_Wr(0X00);//escribimos 0 segundos y bit ch 0
I2C1_Stop();
I2C1_Start();
I2C1_Wr(0xD0); //direccion ds1307 1101000 + 0 para escribir
I2C1_Wr(0X01);//direccion de los minutos
I2C1_Wr(0X20);//escribimos 32 minutos
I2C1_Stop();
I2C1_Start();
I2C1_Wr(0xD0); //direccion ds1307 1101000 + 0 para escribir
I2C1_Wr(0X02);//direccion de las horas
I2C1_Wr(0X12);//configuramos reloj 24 horas hora 12
I2C1_Stop();
I2C1_Start();
I2C1_Wr(0xD0); //direccion ds1307 1101000 + 0 para escribir
I2C1_Wr(0X03);//direccion del dia de la semana
I2C1_Wr(0X01);//escribimos 01 a 07, en este caso 06 para sabado
I2C1_Stop();
I2C1_Start();
I2C1_Wr(0xD0); //direccion ds1307 1101000 + 0 para escribir
I2C1_Wr(0X04);//direccion de la fecha del dia
I2C1_Wr(0X13);//escribimos 11 para el dia 11
I2C1_Stop();
I2C1_Start();
I2C1_Wr(0xD0); //direccion ds1307 1101000 + 0 para escribir
I2C1_Wr(0X05);//direccion del mes
I2C1_Wr(0X07);//escribimos 07 para el mes 07
I2C1_Stop();
I2C1_Start();
I2C1_Wr(0xD0); //direccion ds1307 1101000 + 0 para escribir
I2C1_Wr(0X06);//direccion del año
I2C1_Wr(0X20);//escribimos 20 para el año 20
I2C1_Stop();

 //configuracion de la alarma------------------------------------------------------------------
I2C1_Start();
I2C1_Wr(0xD0); //direccion de ds3132
I2C1_Wr(0X07);//direccion de la alarma 1 m1
I2C1_Wr(0X00);//escribimos 0 para a1m1=0 y segundos 15
I2C1_Stop();

I2C1_Start();
I2C1_Wr(0xD0); //direccion de ds3132
I2C1_Wr(0X08);//direccion de la alarma 1 m2
I2C1_Wr(0X80);//escribimos 80 para a1m2=1 y minutos 0000
I2C1_Stop();

I2C1_Start();
I2C1_Wr(0xD0); //direccion de ds3132
I2C1_Wr(0X09);//direccion de la alarma 1 m3
I2C1_Wr(0X80);//escribimos 80 para a1m3=1 y horas 0000
I2C1_Stop();

I2C1_Start();
I2C1_Wr(0xD0); //direccion de ds3132
I2C1_Wr(0X0A);//direccion de la alarma 1 m4
I2C1_Wr(0X80);//escribimos 80 para a1m4=1 y dias 00
I2C1_Stop();

I2C1_Start();
I2C1_Wr(0xD0); //direccion de ds3132
I2C1_Wr(0X0E);//direccion de registro de control
I2C1_Wr(0X05);//escribimos 05 para activarr la alarma por el pin int/sqw  y habilitamos la alarma 1
I2C1_Stop();//la alarma 1 se activa una vez por minuto
//configuracion de la alarma -----------------------------------------------------------


   lcd_out(1,10,"R");//reloj listo
//termina config del reloj ds3231

TMR0L=0;//borramos las gotitas
while(1)
        {
        texto=TMR0L/100;//divido entre 100 para sacar centenas

        lcd_chr(1,12,texto+48);//imprimo centenas de el ciclo de trabajo en su posicion correspondiente

        texto=(TMR0L/10)%10;//divido entre10 y hago modulo de 10 para sacar las decenas

        lcd_chr(1,13,texto+48);//imprimo decenas de el ciclo de trabajo en su posicion correspondiente

        texto=TMR0L%10;//hago modulo de 10 para sacar las unidades

        lcd_chr(1,14,texto+48);//imprimo unidades del ciclo de trabajo en su posicion correspondiente   7
        



         //termina impresion de la hora
        if(interrupcion==1)//aqui sabemos si ocurrio la interrupcion
        {
        //escribimos esto para limpiar la bandera de la alarma 1
        I2C1_Start();
        I2C1_Wr(0xD0); //direccion de ds3132
        I2C1_Wr(0X0F);//direccion de registro de control
        I2C1_Wr(0X00);//escribimos 00 para limpiar el bit de A1F para que la alarma pueda ocurrir otra ve
        I2C1_Stop();//la alarma 1 se activa una vez por minuto
        
        //comienza secuencia de leido de hora

         I2C1_Start();
         I2C1_Wr(0xD0);                // Address Device + Write
         I2C1_Wr(0x00);                // Address Pointer
         I2C1_Repeated_start();
         I2C1_Wr(0xD1);                // Address Device + Read
         segundos =  I2C1_Rd(0);
         I2C1_Stop();

         I2C1_Start();
         I2C1_Wr(0xD0);                // Address Device + Write
         I2C1_Wr(0x01);                // Address Pointer
         I2C1_Repeated_start();
         I2C1_Wr(0xD1);                // Address Device + Read
         minutos =  I2C1_Rd(0);
         I2C1_Stop();

          I2C1_Start();
         I2C1_Wr(0xD0);                // Address Device + Write
         I2C1_Wr(0x02);                // Address Pointer
         I2C1_Repeated_start();
         I2C1_Wr(0xD1);                // Address Device + Read
         horas =  I2C1_Rd(0);
         I2C1_Stop();
         //termina lecturad de la hora
         horas=((horas >> 5 )*20)+(((horas >> 4) & 0b0001)*10)+ (horas & 0b00001111);
         minutos= ((minutos >> 4)*10)+(minutos & 0b00001111);
         segundos=((segundos >> 4)*10)+(segundos & 0b00001111);
         //termina conversion de la hora -_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_
         
          texto=(horas/10);//divido entre10 y hago modulo de 10 para sacar las decenas

        lcd_chr(2,1,texto+48);//imprimo decenas de el ciclo de trabajo en su posicion correspondiente

        texto=horas%10;//hago modulo de 10 para sacar las unidades

        lcd_chr(2,2,texto+48);//imprimo unidades del ciclo de trabajo en su posicion correspondiente   7

        lcd_out(2,3,":");

         texto=(minutos/10);//divido entre10 y hago modulo de 10 para sacar las decenas

        lcd_chr(2,4,texto+48);//imprimo decenas de el ciclo de trabajo en su posicion correspondiente

        texto=minutos%10;//hago modulo de 10 para sacar las unidades

        lcd_chr(2,5,texto+48);//imprimo unidades del ciclo de trabajo en su posicion correspondiente   7

          lcd_out(2,6,":");

             texto=(segundos/10);//divido entre10 y hago modulo de 10 para sacar las decenas

        lcd_chr(2,7,texto+48);//imprimo decenas de el ciclo de trabajo en su posicion correspondiente

        texto=segundos%10;//hago modulo de 10 para sacar las unidades

        lcd_chr(2,8,texto+48);//imprimo unidades del ciclo de trabajo en su posicion correspondiente   7
        //lcd_out(2,1,"si ocurrio");
        //delay_ms(2000);
        //lcd_out(2,1,"           ");
        
        I2C1_Start();
         I2C1_Wr(0xD0);                // Address Device + Write
         I2C1_Wr(0x04);                // Address Pointer
         I2C1_Repeated_start();
         I2C1_Wr(0xD1);                // Address Device + Read
         dias =  I2C1_Rd(0); //de 1-31
         I2C1_Stop();
         //-_--____--_--____---__--__-__-___
          I2C1_Start();
         I2C1_Wr(0xD0);                // Address Device + Write
         I2C1_Wr(0x05);                // Address Pointer
         I2C1_Repeated_start();
         I2C1_Wr(0xD1);                // Address Device + Read
         meses =  I2C1_Rd(0); //de 1-12
         I2C1_Stop();

          I2C1_Start();
         I2C1_Wr(0xD0);                // Address Device + Write
         I2C1_Wr(0x06);                // Address Pointer
         I2C1_Repeated_start();
         I2C1_Wr(0xD1);                // Address Device + Read
         years =  I2C1_Rd(0); //0-99
         I2C1_Stop();
         
         years=((years >> 4 )*10)+(years & 0b00001111);
         meses= ((meses >> 4)*10)+(meses & 0b00001111);
         dias=((dias >> 4)*10)+(dias & 0b00001111);
         
           texto=(dias/10);//divido entre10 y hago modulo de 10 para sacar las decenas

        lcd_chr(2,9,texto+48);//imprimo decenas de el ciclo de trabajo en su posicion correspondiente

        texto=dias%10;//hago modulo de 10 para sacar las unidades

        lcd_chr(2,10,texto+48);//imprimo unidades del ciclo de trabajo en su posicion correspondiente   7

        lcd_out(2,11,"/");

         texto=(meses/10);//divido entre10 y hago modulo de 10 para sacar las decenas

        lcd_chr(2,12,texto+48);//imprimo decenas de el ciclo de trabajo en su posicion correspondiente

        texto=meses%10;//hago modulo de 10 para sacar las unidades

        lcd_chr(2,13,texto+48);//imprimo unidades del ciclo de trabajo en su posicion correspondiente   7

          lcd_out(2,14,"/");

             texto=(years/10);//divido entre10 y hago modulo de 10 para sacar las decenas

        lcd_chr(2,15,texto+48);//imprimo decenas de el ciclo de trabajo en su posicion correspondiente

        texto=years%10;//hago modulo de 10 para sacar las unidades

        lcd_chr(2,16,texto+48);//imprimo unidades del ciclo de trabajo en su posicion correspondiente   7
         
        interrupcion=0;
        }
        if(PORTC.B0==1)
        {

         
        LATD=0;
        TMR0L=0;
        OSCCON=OSCCON & 0b10000010;
        //OSCCON.B6=0;//configuramos oscilador interno a 31 KHz
        //OSCCON.B5=0;
        //OSCCON.B4=0;//termina configuracion de oscilador interno a 31 KHz
        asm sleep;
        }
        }

}
void interrupt()
  {
  OSCCON=OSCCON | 0b01000010;//configuramos oscilador a 1mhz otra vez

   INTCON.B7=0;
  if(INTCON3.B1==1)
  {
  interrupcion=1;
  TMR0L=0;
  INTCON3.B1=0;
  }
  else{}

  INTCON.B7=1;
  }