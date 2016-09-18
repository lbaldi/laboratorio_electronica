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

#include <stdio.h>
#include <string.h>
#include <xc.h>
#include <delays.h>
#include <unmc_lcd_216.h>
#include <unmc_rtcc_01.h>
#include <unmc_config_01.h>
#include <unmc_inout_01.h>
#include <unmc_inout_02.h>

#include <lcd_write.h>
#include <menu_selector.h>
#include <submenu_selector.h>
#include <pin.h>
#include <activation.h>
#include <siren.h>

// Global Variables
int cursor_horizontal_position;
int menu_selected;
int submenu_selected;
int home_callback;
int home_callback_delay;
int config_widget;
int config_widget_time = 0;
char pin[5];
char pin_input[5];


/*ALARM STATE
 * #0: DISABLED
 * #1: ENABLED
 * #2: ACTIVATED
 */
int state;

/* ACTIVITIES
 * #0 HOME
 * #1 PIN ACCESS
 * #2 MENU
 * #3 DATE AND TIME
 * #4 ACTIVATION
 * #5 PIN CONFIG
 * #7 PIN SET
 * #6 SIREN CONFIG
 * #8 SIREN VOLUME SET 
 * #9 SIREN TONE SET
 * #10 DATE CONFIG
 * #11 TIME CONFIG
 */
int activity;

/* VOLUME
 * #0 LOW
 * #1 HIGH
 */
int alarm_volume;

/* TONE
 * #0 BEEP 1
 * #1 BEEP 2
 */
int alarm_tone;

//Home and home callback

void activity_home(void){
    
    activity = 0;
    lcd_init();
    lcd_write(1,1,"X28");
    
}

void reset_home_callback(void){    
    
    home_callback = home_callback_delay;
    
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

//Entrance to configuration: menu activity

void activity_menu(void){
    
    activity = 2;
    lcd_init();
    lcd_write(2,1,"Activar");
    lcd_write(2,2,"Reloj");
    lcd_write(10,1,"Pin");
    lcd_write(10,2,"Sirena");
    menu_selector_0();
    
}

//Submenu date and time configuration

void activity_submenu_date_and_time(void){
    
    activity = 3;
    lcd_init();
    lcd_write(1,1,"Reloj");
    lcd_write(10,1,"Fecha");
    lcd_write(10,2,"Hora");
    submenu_selector_0();
    
}

void activity_submenu_date_config(void){
    
    activity = 10;
    lcd_init();
    lcd_write(1,1,"Ingrese la fecha");       
    sprintf(buffer1,"%02u/%02u/%02u",dia,mes,anio);
    lcd_write(1,2,buffer1);
    lcd_gotoxy(1, 2);
    lcd_comand(0b00001111);
    
}

void activity_submenu_time_config(void){
    
    activity = 11;
    lcd_init();
    lcd_write(1,1,"Ingrese la hora"); 
    sprintf(buffer1,"%02u:%02u:%02u",hora,minuto,segundo);
    lcd_write(1,2,buffer1); 
    lcd_gotoxy(1, 2);
    lcd_comand(0b00001111);
    
}

//Date and time set

void cursor_displacement(void){
    
    if(cursor_horizontal_position == 8){
        cursor_horizontal_position = 0;
    }
    else if (cursor_horizontal_position == 2 || cursor_horizontal_position == 5){
        cursor_horizontal_position++;
    }
    
    cursor_horizontal_position++;
    lcd_gotoxy(cursor_horizontal_position, 2);
    __delay_ms(98);
    
}

void date_set(void){     
     
    switch(cursor_horizontal_position){
        
        case 1:            
            dia = key*10;
            break;
            
        case 2:            
            dia += key;
            break;
            
        case 4:
            mes = key*10;
            break;
            
        case 5:
            mes += key;
            break;
            
        case 7:
            anio = key * 10;
            break;
            
        case 8:
            anio += key;
            break;
            
    }
    
    Write_RTC();
    __delay_ms(98);   
    
}

void time_set(void){
    
        switch(cursor_horizontal_position){
        
        case 1:            
            hora = key*10;
            break;
            
        case 2:            
            hora += key;
            break;
            
        case 4:
            minuto = key*10;
            break;
            
        case 5:
            minuto += key;
            break;
            
        case 7:
            segundo = key * 10;
            break;
            
        case 8:
            segundo += key;
            break;
            
    }
    
    Write_RTC();
    __delay_ms(98);
    
}

// Home information refresh

void home_clock_refresh(void){
    
    sprintf(buffer1,"%02u/%02u/%02u",dia,mes,anio);
    lcd_write(9,1,buffer1);
    sprintf(buffer1,"%02u:%02u:%02u",hora,minuto,segundo);
    lcd_write(9,2,buffer1);  
    
}

void home_state_refresh(void){  
    
    if(state == 1){
        sprintf(buffer1,"ACT:SI");
    }
    else{
        sprintf(buffer1,"ACT:NO");
    }
    lcd_write(1,2,buffer1);
    
}

void home_volume_refresh(void){  
    
    if(alarm_volume == 0){
        sprintf(buffer1,"VOL:BA");
    }
    else{
        sprintf(buffer1,"VOL:AL");
    }
    lcd_write(1,2,buffer1);
    
}

void home_tone_refresh(void){   
    
    if(alarm_tone == 0){
        sprintf(buffer1,"TONO:1");
    }
    else{
        sprintf(buffer1,"TONO:2");
    }
    lcd_write(1,2,buffer1);
    
}

void home_config_refresh(void){
    
    if(state == 1){LED_2_On;}
    if(state == 0){LED_2_Off;}
    
    config_widget_time++;
    
    switch(config_widget){
        
        case 0:
            home_state_refresh();
            if(config_widget_time == 5){
                config_widget_time = 0;
                config_widget = 1;
            }
            break;
            
        case 1:
            home_volume_refresh();
            if(config_widget_time == 5){
                config_widget_time = 0;
                config_widget = 2;
            }
            break;
            
        case 2:
            home_tone_refresh();
            if(config_widget_time == 5){
                config_widget_time = 0;
                config_widget = 0;
            }
            break;
            
    }
    
}

// Buttons behaviour

void button_A(void){
    
    reset_home_callback();
    
    switch(activity){
        
        case 0:
            activity_pin();
            break;
            
        case 1:
            break;
            
        case 2:
            menu_selector();
            break; 
            
        default: 
            submenu_selector();
            break;
    }
    
}

void button_B_menu(void){
    
    switch(menu_selected){
        
        case 0:
            activity_submenu_activation();
            break;
            
        case 1:
            activity_submenu_date_and_time();
            break;
            
        case 2:
            activity_submenu_pin();
            break;
            
        case 3:
            activity_submenu_siren();
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

void button_B_submenu_pin_config(void){
    
    switch(submenu_selected){
        
        case 0:
            activity_submenu_pin_set();
            break;
            
        case 1:
            pin_reset();
            activity_home();
            break;
            
    }
    
}

void button_B_submenu_siren_config(){
    
    switch(submenu_selected){
        
        case 0:
            activity_submenu_siren_volume();
            break;
            
        case 1:
            activity_submenu_siren_tone();
            break;
            
    }
    
}

void button_B_submenu_siren_volume(void){
    
    switch(submenu_selected){
        
        case 0:
            alarm_volume = 0;
            break;
            
        case 1:
            alarm_volume = 1;
            break;
            
    }
    
    activity_home();
    
}

void button_B_submenu_siren_tone(void){
    
    switch(submenu_selected){
        
        case 0:
            alarm_tone = 0;
            break;
            
        case 1:
            alarm_tone = 1;
            break;
            
    }
    
    activity_home();
    
}

void button_B_submenu_date_and_time_config(void){
    
    switch(submenu_selected){
        
        case 0:
            activity_submenu_date_config();
            break;
            
        case 1:
            activity_submenu_time_config();
            break;
            
    }
    
}

void button_B(void){
    
    reset_home_callback();
    
    switch(activity){
        
        case 2:
            button_B_menu();
            break;
            
        case 3:
            button_B_submenu_date_and_time_config();
            break;
            
        case 4:
            button_B_submenu_activation();
            break;
            
        case 5:
            button_B_submenu_pin_config();
            break;
            
        case 7:
            button_B_submenu_siren_config();
            break;
            
        case 8:
            button_B_submenu_siren_volume();
            break;
            
        case 9:
            button_B_submenu_siren_tone();
            break;      
            
    }
    
}

void button_C(void){
    
    cursor_horizontal_position == 0;
    reset_home_callback();
    
    if(activity != 0 && activity != 1){
        activity_menu();
    }
    
}

void button_D(void){
    
    cursor_horizontal_position == 0;
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
    else if(activity == 6){
        pin_set();
    }
    else if(activity == 10 || activity == 11){        
        activity_home();
    }
    
}

void button_number(void){
    
    reset_home_callback();
    sprintf(buffer1, "%01u", key);
    
    if(activity == 1 || activity == 6){        
        lcd_putrs("*");        
        strcat(pin_input, buffer1);        
    }  
    else if (activity == 10){
        date_set();
        cursor_displacement();
    }
    else if (activity == 11){
        time_set();
        cursor_displacement();
    }
    
}

// Keyboard, setup and Main

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
    config_widget = 0;
    menu_selected = 0;
    submenu_selected = 0;
    cursor_horizontal_position = 1;
    alarm_volume = 0;
    alarm_tone = 0;
    pin_reset();
    
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
            home_config_refresh();
        }       
        
    }
    
    return 0;
    
}