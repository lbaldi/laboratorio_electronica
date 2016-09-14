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
#include <unmc_lcd_216.h>
#include <unmc_rtcc_01.h>
#include <unmc_config_01.h>
#include <unmc_inout_02.h>

#include <stdio.h>
#include <string.h>

#include "unmc_inout_01.h"

// Global Variables
int state;
int menu_selected;
int submenu_selected;
char pin[5];
char pin_input[5];
int home_callback;
int home_callback_delay;

/* ACTIVITIES
 * #0 HOME
 * #1 PIN ACCESS
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
    home_callback_delay = 50;
    menu_selected = 0;
    submenu_selected = 0;
    strcpy(pin, "1234");
}

void reset_home_callback(void){
    
    home_callback = home_callback_delay;
    
}

void home_clock_refresh(void){
    
    sprintf(buffer1,"%02u/%02u/%02u",dia,mes,anio);
    lcd_write(9,1,buffer1);
    sprintf(buffer1,"%02u:%02u:%02u",hora,minuto,segundo);
    lcd_write(9,2,buffer1);
    
}

void home_state_refresh(void){

    if(state == 1){
        sprintf(buffer1,"ACT:OK");
        LED_2_On;                
    }
    else{
        sprintf(buffer1,"ACT:NO");
        LED_2_Off;
    }
    lcd_write(1,2,buffer1);

}

void action_menu_selector_0(void){
    lcd_write(1,1,">");
    menu_selected = 0;
    lcd_write(9,2," ");
}

void action_menu_selector_1(void){
    lcd_write(1,2,">");
    menu_selected = 1;
    lcd_write(1,1," ");
}

void action_menu_selector_2(void){
    lcd_write(9,1,">");
    menu_selected = 2;
    lcd_write(1,2," ");
}

void action_menu_selector_3(void){
    lcd_write(9,2,">");
    menu_selected = 3;
    lcd_write(9,1," ");
}

void action_menu_selector(void){
    
    switch(menu_selected){
        
        case 0:
            action_menu_selector_1();
            break;
            
        case 1:
            action_menu_selector_2();
            break;
            
        case 2:
            action_menu_selector_3();
            break;
            
        case 3:
            action_menu_selector_0();
            break;
            
    }
    
}

void activity_home(void){
    
    activity = 0;
    lcd_init();
    lcd_write(1,1,"X28");
    
}

void activity_pin(void){
    
    activity = 1;
    strcpy(pin_input, "");
    lcd_init();
    lcd_write(1,1,"Ingrese su PIN:");
    lcd_gotoxy(1,2);
    lcd_comand(0b00001111);
}

void activity_menu(void){
    
    activity = 2;
    lcd_init();
    lcd_write(2,1,"Activar");
    lcd_write(2,2,"Reloj");
    lcd_write(10,1,"Pin");
    lcd_write(10,2,"Alarma");
    action_menu_selector_0();
    
}

void action_submenu_selector_0(void){
    lcd_write(9,1,">");
    submenu_selected = 0;
    lcd_write(9,2," ");
}

void action_submenu_selector_1(void){
    lcd_write(9,2,">");
    submenu_selected = 1;
    lcd_write(9,1," ");
}

void action_submenu_selector(void){
    
    switch(submenu_selected){
        
        case 0:
            action_submenu_selector_1();
            break;
            
        case 1:
            action_submenu_selector_0();
            break;
    }
    
}

void activity_submenu_activation(void){
    
    activity = 4;
    lcd_init();
    lcd_write(1,1,"Activar");
    lcd_write(10,1,"SI");
    lcd_write(10,2,"NO");
    action_submenu_selector_0();
}

void activity_submenu_clock(void){
    
    activity = 3;
    lcd_init();
    lcd_write(1,1,"Reloj");
    lcd_write(10,1,"Fecha");
    lcd_write(10,2,"Hora");
    action_submenu_selector_0();
    
}

void activity_submenu_pin(void){
    
    activity = 5;
    lcd_init();
    lcd_write(1,1,"Pin");
    lcd_write(10,1,"Cambiar");
    lcd_write(10,2,"Reset");
    action_submenu_selector_0();
}

void activity_submenu_alarm(void){
    
    activity = 6;
    lcd_init();
    lcd_write(1,1,"Alarma");
    lcd_write(10,1,"Volumen");
    lcd_write(10,2,"Tono");
    action_submenu_selector_0();
}

void pin_input_validator(void){
    
    int comparision = strcmp(pin, pin_input);
    
    if( comparision == 0 ){
        activity_menu();
    }
    else{
        activity_pin();
    }
}

void button_A(void){
    
    reset_home_callback();
    
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

void button_B_menu(void){
    
    switch(menu_selected){
            
        case 0:
            activity_submenu_activation();
            break;

        case 1:
            activity_submenu_clock();
            break;

        case 2:
            activity_submenu_pin();
            break;

        case 3:
            activity_submenu_alarm();
            break;

    }
}

void button_B_submenu_activation(void){
   
    switch(submenu_selected){
            
        case 0:
            state = 1;
            break;

        case 1:
            state = 0;
            break;

    }
    
    activity_home();
    
}

void button_B(void){
    
    reset_home_callback();
    
    switch(activity){
        
        case 2:
            button_B_menu();
            break;
            
        case 4:
            button_B_submenu_activation();
            break;
    
    }
    
}

void button_C(void){
    
    reset_home_callback();
    
    if(activity != 0 && activity != 1){
        activity_menu();
    }
    
}

void button_D(void){
    
    reset_home_callback();
    
    activity_home();
    
}

void button_asterisk(void){
    
    reset_home_callback();
    
    // lcd_putrs("*");
    
}

void button_hash(void){
    
    reset_home_callback();
    
    if(activity == 1){
        pin_input_validator();
    }
    
}

void button_number(void){
    
    reset_home_callback();
    
    if(activity == 1){
        
        lcd_putrs("*");        
        sprintf(buffer1, "%01u", key);
        strcat(pin_input, buffer1);
        
    } else {
        
        sprintf(buffer1,"%01u",key);
        // lcd_putrs(buffer1);
        
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

void check_home_callback(void){
    
    if(activity != 0){
        
        home_callback--;
        
        if(home_callback == 0){
            
            reset_home_callback();
            
            activity_home();
            
        }
    }
    
}

int main(void){
    
    setup();
    
    activity_home();
    
    while(1){
        
        check_home_callback();
        
        Read_RTC();
        
        keyboard_control();
        
        if(activity == 0){
            home_clock_refresh();
            home_state_refresh();
        }
        
    }
    
    return 0;
    
}