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
int menu_selector;
int menu_selected;
int submenu_selector;
char* pin;
char* pin_input;

/* ACTIVIDADES
 * #0 INITIAL
 * #1 PIN
 * #2 MENU
 * #3 CLOCK
 * #4 ACTIVATION
 * #5 PIN CONFIG
 * #6 ALARM
 */
int activity;


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
    menu_selected = 1;
    menu_selector = 1;
    submenu_selector = 1;
    pin = "1234";
}

void home_clock_refresh(void){

    sprintf(buffer1,"%02u/%02u/%02u",dia,mes,anio);
    lcd_write(9,1,buffer1);
    sprintf(buffer1,"%02u:%02u:%02u",hora,minuto,segundo);
    lcd_write(9,2,buffer1);

}

//DONE
void activity_home(void){
    
    activity = 0;
    lcd_init();
    lcd_write(1,1,"X28");

}

//DONE
void activity_pin(void){

    activity = 1;
    lcd_init();
    lcd_write(1,1,"Ingrese su PIN:");
    lcd_gotoxy(1,2);
    lcd_comand(0b00001111);
}

//DONE
void activity_menu(void){

    activity = 2;
    lcd_init();
    lcd_write(2,1,"Activar");
    lcd_write(2,2,"Reloj");
    lcd_write(10,1,"Pin");
    lcd_write(10,2,"Alarma");

}

//DONE
void action_menu_selector(void){

    switch(menu_selector){

        case 1:
            lcd_write(1,1,">");
            menu_selector = 2;
            menu_selected = 1;
            lcd_write(9,2," ");
            break;

        case 2:
            lcd_write(1,2,">");
            menu_selector = 3;
            menu_selected = 2;
            lcd_write(1,1," ");
            break;

        case 3:
            lcd_write(9,1,">");
            menu_selector = 4;
            menu_selected = 3;
            lcd_write(1,2," ");
            break;

        case 4:
            lcd_write(9,2,">");
            menu_selector = 1;
            menu_selected = 4;
            lcd_write(9,1," ");
            break;

    }

}

//DONE
void action_submenu_selector(void){

    switch(submenu_selector){

        case 1:
            lcd_write(9,1,">");
            submenu_selector = 2;
            lcd_write(9,2," ");
            break;

        case 2:
            lcd_write(9,2,">");
            submenu_selector = 1;
            lcd_write(9,1," ");
            break;
    }

}

//DONE
void activity_submenu_activation(void){

    activity = 4;
    lcd_init();
    lcd_write(1,1,"Activar");
    lcd_write(10,1,"SI");
    lcd_write(10,2,"NO");

}

//DONE
void activity_submenu_clock(void){

    activity = 3;
    lcd_init();
    lcd_write(1,1,"Reloj");
    lcd_write(10,1,"Fecha");
    lcd_write(10,2,"Hora");

}

//DONE
void activity_submenu_pin(void){

    activity = 5;
    lcd_init();
    lcd_write(1,1,"Pin");
    lcd_write(10,1,"Cambiar");
    lcd_write(10,2,"Reset");

}

//DONE
void activity_submenu_alarm(void){

    activity = 6;
    lcd_init();
    lcd_write(1,1,"Alarma");
    lcd_write(10,1,"Volumen");
    lcd_write(10,2,"Tono");

}

//DONE
void pin_input_validator(void){
    // @TODO if(pin == pin_input){
    if(1){
        activity_menu();
    }
    else{
        activity_pin();
    }
}

//DONE
void button_A(void){
    
    switch(activity){

        case 0:
            activity_pin();
            break;

        case 1:
            break;

        case 2:
            action_menu_selector();
            break;

        default: 
            action_submenu_selector();
            break;
    }
    
}

//DONE
void button_B(void){

    if(activity == 2){

        switch(menu_selected){

            case 1:
                activity_submenu_activation();
                break;

            case 2:
                activity_submenu_clock();
                break;

            case 3:
                activity_submenu_pin();
                break;

            case 4:
                activity_submenu_alarm();
                break;
        }

    }
    
}

//DONE
void button_C(void){

    if(activity != 0 && activity != 1){
        activity_menu();
    }

}

//DONE
void button_D(void){

    activity_home();

}

// @TODO
void button_asterisk(void){
    lcd_putrs("*");
}

//DONE
void button_hash(void){

    if(activity == 1){
        pin_input_validator();
    }

}

//DONE
void button_number(void){

    if(activity == 1){
        
        lcd_putrs("*");
        
    } else {
        
        sprintf(buffer1,"%01u",key);
        lcd_putrs(buffer1);
        
    }

}

//DONE
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

//DONE
int main(void){

    setup();
    activity_home();
    while(1){
        Read_RTC();
        keyboard_control();
        if(activity == 0){home_clock_refresh();}
    }

    return 0;
}