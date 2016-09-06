/*******************************************************************************
 *
 *                Unimic 01 Version 1.1   10 September 2015
 *
 *******************************************************************************
 * FileName:        main.c
 * Dependencies:
 * Processor:       PIC18F26J50
 * Compiler:        XC8 1.34
 * Company:         Microchip Technology, Inc.
 *
 * Software License Agreement
 *
 * THIS SOFTWARE IS PROVIDED IN AN �AS IS� CONDITION. NO WARRANTIES, WHETHER
 * EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED 
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO 
 * THIS SOFTWARE. THE COMPANY SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR 
 * SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 ******************************************************************************/

#include <xc.h>
#include<delays.h>
#include<stdio.h>
#include <unmc_lcd_216.h>
#include <unmc_rtcc_01.h>
#include <unmc_config_01.h>
#include <unmc_inout_02.h>

// Global Variables
int select;
int select_op;
int selection;
int submenu;

void Setup(void){
    
    OSCTUNEbits.INTSRC=1;       //setea el oscilador de 32768 para el RTC
    OSCTUNEbits.PLLEN=0;        //desactiva PLL
    OSCCONbits.IRCF0=1;         //selecciona el clock en 8MHz
    OSCCONbits.IRCF1=1;
    OSCCONbits.IRCF2=1;
    OSCCONbits.SCS0=0;          //oscilator INTRC
    OSCCONbits.SCS1=0;
    TRISA = 0b11110000;
    TRISB = 0;
    TRISC = 0b00000111;
    //TRISAbits.TRISA0=1;
    //TRISBbits.TRISB0=0;
    //TRISCbits.TRISC0=0;
    ANCON0=0b11111111;          // Config AN7 to AN0 Digital Ports
    //ANCON1=0b10010111;        // Config AN11 Analog Port
    ANCON1=0b11111111;          // Config AN12 to AN8 Digital Ports
    ADCON0=0b00101101;          // Control AN11 Analog Port
    ADCON1=0b00010000;          // Config Analog Port
    RTCCFGbits.RTCEN=1;
    RTCCFGbits.RTCWREN=1;
    T1CONbits.T1OSCEN=1;
    Write_RTC();
    lcd_init();
    lcd_comand(0b00001100);     //Display=on / Cursor=off / Blink=off
    LED_2_On;
    LED_3_On;
    
    // Setup Global Variables
    select = 1;
    selection = 1;
    submenu = 0;
    select_op = 1;

}

void Menu_Home(void){
    
    lcd_init();
    lcd_gotoxy(2,1);
    lcd_putrs("Menu1");
    lcd_gotoxy(2,2);
    lcd_putrs("Menu2");
    lcd_gotoxy(10,1);
    lcd_putrs("Menu3");
    lcd_gotoxy(10,2);
    lcd_putrs("Menu4");
}

void Menu_Iterator(void){
    
    switch(selection){
        case 1:
            lcd_gotoxy(1,1);
            lcd_putrs(">");
            selection = 2;
            select = 1;
            lcd_gotoxy(9,2);
            lcd_putrs(" ");
            break;
        case 2:
            lcd_gotoxy(1,2);
            lcd_putrs(">");
            selection = 3;
            select = 2;
            lcd_gotoxy(1,1);
            lcd_putrs(" ");
            break;
        case 3:
            lcd_gotoxy(9,1);
            lcd_putrs(">");
            selection = 4;
            select = 3;
            lcd_gotoxy(1,2);
            lcd_putrs(" ");
            break;
        case 4:
            lcd_gotoxy(9,2);
            lcd_putrs(">");
            selection = 1;
            select = 4;
            lcd_gotoxy(9,1);
            lcd_putrs(" ");
            break;
    }

}

void Menu_Submenu_Iterator(void){
    
    switch(select_op){
        case 1:
            lcd_gotoxy(9,1);
            lcd_putrs(">");
            select_op = 2;
            lcd_gotoxy(9,2);
            lcd_putrs(" ");
            break;
        case 2:
            lcd_gotoxy(9,2);
            lcd_putrs(">");
            select_op = 1;
            lcd_gotoxy(9,1);
            lcd_putrs(" ");
            break;
    }
    
}

void Button_A(void){
    
    if (submenu == 0){   
        Menu_Iterator();
    }
    
    if (submenu == 1){
        Menu_Submenu_Iterator();
    }

}

void Button_B(void){
    
    switch(select)
        {
        case 1:
            lcd_init();
            lcd_gotoxy(1,1);
            lcd_putrs("Menu 1");
            lcd_gotoxy(10,1);
            lcd_putrs("Option1");
            lcd_gotoxy(10,2);
            lcd_putrs("Option2");
            submenu = 1;
       break;
       case 2:
            lcd_init();
            lcd_gotoxy(1,1);
            lcd_putrs("Menu 2");
            lcd_gotoxy(10,1);
            lcd_putrs("Option1");
            lcd_gotoxy(10,2);
            lcd_putrs("Option2");
            submenu = 1;
       break;
       case 3:
            lcd_init();
            lcd_gotoxy(1,1);
            lcd_putrs("Menu 3");
            lcd_gotoxy(10,1);
            lcd_putrs("Option1");
            lcd_gotoxy(10,2);
            lcd_putrs("Option2");
            submenu = 1;
       break;
       case 4:
            lcd_init();
            lcd_gotoxy(1,1);
            lcd_putrs("Menu 4");
            lcd_gotoxy(10,1);
            lcd_putrs("Option1");
            lcd_gotoxy(10,2);
            lcd_putrs("Option2");
            submenu = 1;
       break;
    }

}

void Button_C(void){
    lcd_putrs("C");
}

void Button_D(void){
      
    Menu_Home();
    submenu = 0;
  
}

void Button_Star(void){
    lcd_putrs("*");
}

void Button_Hash(void){
    lcd_putrs("#");
}

void Keyboard_Control(void){
    
    row1=1;row2=0;row3=0;row4=0;
    {
        if (column1==1){key=1;sprintf(buffer2,"%01u",key);lcd_putrs(buffer2);while(column1==1){};}
        if (column2==1){key=2;sprintf(buffer2,"%01u",key);lcd_putrs(buffer2);while(column2==1){};}
        if (column3==1){key=3;sprintf(buffer2,"%01u",key);lcd_putrs(buffer2);while(column3==1){};}
        if (column4==1){key=11;Button_A();while(column4==1){};}
    }

    row1=0;row2=1;row3=0;row4=0;
    {
        if (column1==1){key=4;sprintf(buffer2,"%01u",key);lcd_putrs(buffer2);while(column1==1){};}
        if (column2==1){key=5;sprintf(buffer2,"%01u",key);lcd_putrs(buffer2);while(column2==1){};}
        if (column3==1){key=6;sprintf(buffer2,"%01u",key);lcd_putrs(buffer2);while(column3==1){};}
        if (column4==1){key=12;Button_B();while(column4==1){};}
    }

    row1=0;row2=0;row3=1;row4=0;
    {
        if (column1==1){key=7;sprintf(buffer2,"%01u",key);lcd_putrs(buffer2);while(column1==1){};}
        if (column2==1){key=8;sprintf(buffer2,"%01u",key);lcd_putrs(buffer2);while(column2==1){};}
        if (column3==1){key=9;sprintf(buffer2,"%01u",key);lcd_putrs(buffer2);while(column3==1){};}
        if (column4==1){key=13;Button_C();while(column4==1){};}
    }

    row1=0;row2=0;row3=0;row4=1;
    {
        if (column1==1){key=14;Button_Star();while(column1==1){};}
        if (column2==1){key=0;sprintf(buffer2,"%01u",key);lcd_putrs(buffer2);while(column2==1){};}
        if (column3==1){key=15;Button_Hash();while(column3==1){};}
        if (column4==1){key=16;Button_D();while(column4==1){};}
    }

    __delay_ms(98);             // 98ms retardo maximo para esta funcion

}

int main(void){
    
    Setup();
    Menu_Home();
    while(1){Keyboard_Control();}
    return 0;

}