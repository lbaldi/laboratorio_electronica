/*********************************************************************
 *
 *                Unimic 01 Version 1.1   10 September 2015
 *
 ***********************************************************************
 * FileName:        main.c
 * Dependencies:
 * Processor:       PIC18F26J50
 * Compiler:        XC8 1.34
 * Company:         Microchip Technology, Inc.
 *
 * Software License Agreement
 *
 * THIS SOFTWARE IS PROVIDED IN AN �AS IS� CONDITION. NO WARRANTIES,
 * WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED
 * TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. THE COMPANY SHALL NOT,
 * IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL OR
 * CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 ***********************************************************************/
#include <xc.h>
#include<delays.h>
#include<stdio.h>
// Personal Libs ( Uninic folder in program files)
#include <unmc_lcd_216.h> // Display Lib
#include <unmc_rtcc_01.h> // Clock Lib
#include <unmc_config_01.h>
#include <unmc_inout_01.h>


//[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[
/// Funcion Caratula
/// Display presentation day hour
/// variable lecture diasem, anio, dia, hora, etc
//[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[
void caratula(void){
    
    lcd_comand(0b00001100);        //Enciende display sin cursor y sin blink  
    lcd_gotoxy(1,1);        
    lcd_putrs("UNTREF");
    sprintf(buffer2,"%02u/%02u/%02u",dia,mes,anio);
    lcd_gotoxy(9,1);
    lcd_putrs(buffer2);
    sprintf(buffer2,"%02u:%02u:%02u",hora,minuto,segundo);
    lcd_gotoxy(1,2);
    lcd_putrs(buffer2);
    b=b++;
    if(b<7)
    {
        lcd_gotoxy(1,1);
        lcd_putrs("UNTREF");
    }
    if(b>6)
    {
        if(b==11){b=0;}
        lcd_gotoxy(1,1);
        lcd_putrs("      ");
    }
}

//[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[
/// Funcion Setup
///
///
//[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[
void Setup(void){
    
    OSCTUNEbits.INTSRC=1;       //setea el oscilador de 32768 para el RTC
    OSCTUNEbits.PLLEN=0;        //desactiva PLL
    OSCCONbits.IRCF0=1;         //selecciona el clock en 8MHz
    OSCCONbits.IRCF1=1;
    OSCCONbits.IRCF2=1;
    OSCCONbits.SCS0=0;          //oscilator INTRC
    OSCCONbits.SCS1=0;
    TRISA = 0b11111111;
    TRISB = 0;
    TRISC = 0b00000111;         // C2, C1, C0 Entradas
    //TRISAbits.TRISA0=1;
    //TRISBbits.TRISB0=0;
    //TRISCbits.TRISC0=0;
    ANCON0=0b11111111;          // Config AN7 to AN0 Digital Ports
    ANCON0=0b10010111;          // Config AN11 Analog Port
    ADCON0=0b00101101;          // Control AN11 Analog Port
    ADCON1=0b00010000;          // Config Analog Port
    RTCCFGbits.RTCEN=1;
    RTCCFGbits.RTCWREN=1;
    T1CONbits.T1OSCEN=1;
    Write_RTC();
    lcd_init();
    //lcd_comand(0b00001100);     //Display=on / Cursor=off / Blink=off
    lcd_comand(0b00001110);       //Display=on / Cursor=on / Blink=off
    //lcd_comand(0b00001111);     //Display=on / Cursor=on / Blink=on

    LED_2_Off;
    LED_3_Off;
}

/*------------------------------------------------------------------------------
********************************************************************************
Funcion main
Funcion principal del programa
********************************************************************************
--------------------------------------------------------------------------------*/
int main(void){
    
    // Posiciones iniciales del cursor
    int cr_x = 1;
    int cr_y = 1;
    
    Setup();
    while(1){
        
        Read_RTC();
        caratula();
        
        // Mustra el cursor y parpadea
        lcd_setcursor_vb(1, 1);
        
        lcd_gotoxy(cr_x,cr_y); //posiciona caracter,linea

        if(switch1 == 0){
     
            // Si el cursor llego al final en el primer carril
            // me muevo al segundo carril desde la primer posicion
            // Si el cursor llego al final en el segundo carril
            // me muevo al primer carril desde la primer posicion
            if(cr_x == 16 && cr_y == 1){
                cr_x = 0;
                cr_y = 2;
            }
            else if(cr_x == 16 && cr_y == 2){
                cr_x = 0;
                cr_y = 1;
            }
            cr_x++;
            __delay_ms(98);
            LED_2_Toggle;
            
        }
        else LED_2_Off;

        if(switch2 == 0)
        {
            

            if(cr_x == 9 && cr_y == 1){
                // Cursor Dia Decena
                dia+=10;
            }
            else if(cr_x == 10 && cr_y == 1){
                // Cursor Dia Unidad
                dia++;
            }
            else if(cr_x == 12 && cr_y == 1){
                // Cursor Mes Decena
                mes+=10;
            }
            else if(cr_x == 13 && cr_y == 1){
                // Cursor Mes Unidad
                mes++;
            }
            else if(cr_x == 15 && cr_y == 1){
                // Cursor Anio Decena
                anio+=10;
            }
            else if(cr_x == 16 && cr_y == 1){
                // Cursor Anio Unidad
                anio++;
            }
            else if(cr_x == 1 && cr_y == 2){
                // Cursor Hora Decena
                hora+=10;
            }
            else if(cr_x == 2 && cr_y == 2){
                // Cursor Hora Unidad
                hora++;
            }
            else if(cr_x == 4 && cr_y == 2){
                // Cursor Minuto Decena
                minuto+=10;
            }
            else if(cr_x == 5 && cr_y == 2){
                // Cursor Minuto Unidad
                minuto++;
            }
            else if(cr_x == 7 && cr_y == 2){
                // Cursor Segundo Decena
                segundo+=10;
            }
            else if(cr_x == 8 && cr_y == 2){
                // Cursor Segundo Unidad
                segundo++;
            }
            
            Write_RTC();
            
            __delay_ms(98);
            LED_3_On;
        }
        else LED_3_Off;

        __delay_ms(98);            // 98ms retardo maximo para esta funcion  

    }
    return 0;
}