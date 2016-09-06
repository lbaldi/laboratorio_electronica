/*******************************************************************************
 *
 *                Unimic 01 Version 1.1   6 September 2016
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
#include <delays.h>
#include <stdio.h>
#include <unmc_lcd_216.h>
#include <unmc_rtcc_01.h>
#include <unmc_config_01.h>
#include <unmc_inout_02.h>

// Global Variables
int state;
int locked;
int select;
int select_op;
int selection;
int activity_pin;
int activity_menu;
int activity_submenu;
char* pin;
char* pin_input;

void lcd_write(int column, int row, char* string){
    lcd_gotoxy(column, row);
    lcd_putrs(string);
}

void setup(void){

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
    locked = 1;
    select = 1;
    selection = 1;
    activity_pin = 0;
    activity_menu = 0;
    activity_submenu = 0;
    select_op = 1;
    pin = "1234";
}

void home_clock_refresh(void){

    sprintf(buffer1,"%02u/%02u/%02u",dia,mes,anio);
    lcd_write(9,1,buffer1);
    sprintf(buffer1,"%02u:%02u:%02u",hora,minuto,segundo);
    lcd_write(9,2,buffer1);

}

void home(void){

    lcd_init();
    lcd_write(1,1,"X28");

}

void menu_pin(void){

    lcd_init();
    lcd_write(1,1,"Ingrese su PIN:");
    lcd_gotoxy(1,2);
    lcd_comand(0b00001111);
}

void menu_submenu(void){

    lcd_init();
    lcd_write(2,1,"Activar");
    lcd_write(2,2,"Reloj");
    lcd_write(10,1,"Pin");
    lcd_write(10,2,"Alarma");

}

void menu_iterator(void){

    switch(selection){

        case 1:
            lcd_write(1,1,">");
            selection = 2;
            select = 1;
            lcd_write(9,2," ");
            break;

        case 2:
            lcd_write(1,2,">");
            selection = 3;
            select = 2;
            lcd_write(1,1," ");
            break;

        case 3:
            lcd_write(9,1,">");
            selection = 4;
            select = 3;
            lcd_write(1,2," ");
            break;

        case 4:
            lcd_write(9,2,">");
            selection = 1;
            select = 4;
            lcd_write(9,1," ");
            break;

    }

}

void menu_submenu_iterator(void){

    switch(select_op){

        case 1:
            lcd_write(9,1,">");
            select_op = 2;
            lcd_write(9,2," ");
            break;

        case 2:
            lcd_write(9,2,">");
            select_op = 1;
            lcd_write(9,1," ");
            break;
    }

}

void menu_submenu_activate(void){

    lcd_init();
    lcd_write(1,1,"Activar");
    lcd_write(10,1,"SI");
    lcd_write(10,2,"NO");

}

void menu_submenu_clock(void){

    lcd_init();
    lcd_write(1,1,"Reloj");
    lcd_write(10,1,"Fecha");
    lcd_write(10,2,"Hora");

}

void menu_submenu_pin(void){

    lcd_init();
    lcd_write(1,1,"Pin");
    lcd_write(10,1,"Cambiar");
    lcd_write(10,2,"Reset");

}

void menu_submenu_alarm(void){

    lcd_init();
    lcd_write(1,1,"Alarma");
    lcd_write(10,1,"Volumen");
    lcd_write(10,2,"Tono");

}

void pin_input_validator(void){
    //if(pin == pin_input){
    if(1){
       // @TODO REVISAR ESTADO
        locked = 0;
        activity_pin = 0;
        activity_submenu = 1;
        menu_submenu();
    }
}

void button_A(void){

    if(activity_pin == 0){

        menu_pin();
        activity_pin = 1;

    } else if (locked == 0){

        if (activity_menu == 0) {

            menu_submenu();
            activity_menu = 1;

        } else {

            if (activity_submenu == 0){
                menu_iterator();
            }

            if (activity_submenu == 1){
                menu_submenu_iterator();
            }
        }

    }


}

void button_B(void){

    if(locked == 0){

        switch(select){

            case 1:
                menu_submenu_activate();
                activity_submenu = 1;
                break;

            case 2:
                menu_submenu_clock();
                activity_submenu = 1;
                break;

            case 3:
                menu_submenu_pin();
                activity_submenu = 1;
                break;

            case 4:
                menu_submenu_alarm();
                activity_submenu = 1;
                break;
        }

    }

}

void button_C(void){

    if(locked == 0){
        menu_submenu();
        activity_submenu = 0;
    }

}

void button_D(void){

    home();
    activity_pin = 0;
    locked = 1;
    activity_menu = 0;
    activity_submenu = 0;

}

void button_asterisk(void){
    lcd_putrs("*");
}

void button_hash(void){

    if(activity_pin == 1){
        pin_input_validator();
    }

}

void button_number(void){

    if(activity_pin == 1){
        
        lcd_putrs("*");
        
    } else {
        
        sprintf(buffer1,"%01u",key);
        lcd_putrs(buffer1);
        
    }

}

void keyboard_control(void){

    row1=1;row2=0;row3=0;row4=0;
    {
        if (column1==1){key=1;button_number();while(column1==1){};}
        if (column2==1){key=2;button_number();while(column2==1){};}
        if (column3==1){key=3;button_number();while(column3==1){};}
        if (column4==1){key=11;button_A();while(column4==1){};}
    }

    row1=0;row2=1;row3=0;row4=0;
    {
        if (column1==1){key=4;button_number();while(column1==1){};}
        if (column2==1){key=5;button_number();while(column2==1){};}
        if (column3==1){key=6;button_number();while(column3==1){};}
        if (column4==1){key=12;button_B();while(column4==1){};}
    }

    row1=0;row2=0;row3=1;row4=0;
    {
        if (column1==1){key=7;button_number();while(column1==1){};}
        if (column2==1){key=8;button_number();while(column2==1){};}
        if (column3==1){key=9;button_number();while(column3==1){};}
        if (column4==1){key=13;button_C();while(column4==1){};}
    }

    row1=0;row2=0;row3=0;row4=1;
    {
        if (column1==1){key=14;button_asterisk();while(column1==1){};}
        if (column2==1){key=0;button_number();while(column2==1){};}
        if (column3==1){key=15;button_hash();while(column3==1){};}
        if (column4==1){key=16;button_D();while(column4==1){};}
    }

    __delay_ms(98);             // 98ms is the maximum admitted delay

}

int main(void){

    setup();
    home();
    while(1){
        Read_RTC();
        keyboard_control();
        if(activity_pin == 0 && activity_menu == 0 && activity_submenu == 0){home_clock_refresh();}
    }

    return 0;
}