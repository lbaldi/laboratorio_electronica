#include <xc.h>
#include<delays.h>
#include<stdio.h>

#include <unmc_lcd_216.h>
#include <unmc_config_01.h>
#include <unmc_inout_01.h>

char led_1_intensity[2];
char led_2_intensity[2];
char led_3_intensity[2];
int intensity_1;
int intensity_2;
int intensity_3;
int current_led;


void pic_setup(void){
    
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
    ANCON0=0b11111111;          // Config AN7 to AN0 Digital Ports
    ANCON0=0b10010111;          // Config AN11 Analog Port
    ADCON0=0b00101101;          // Control AN11 Analog Port
    ADCON1=0b00010000;          // Config Analog Port
    T1CONbits.T1OSCEN=1;
    
    LED_1_Off;
    LED_2_Off;
    LED_3_Off;
}

void lcd_write(int column, int row, const char* string){
    
    lcd_gotoxy(column, row);
    lcd_putrs(string);
    
}
void display_init(void){
    
    lcd_init();
    lcd_comand(0b00001100);         //Display=on / Cursor=off / Blink=off
    
}

void display_home(void){
    
    lcd_write(1, 1, "| L1 | L2 | L3 |");
    lcd_write(1, 2, "|    |    |    |");
    
}

void intensity_led(int led, int current, int off){  
    
    switch(led){
        case 1:
            if(current == 0){
                LED_1_On;
            }
            if(current == off){
                LED_1_Off;
            }
            break;
        case 2:
            if(current == 0){
                LED_2_On;
            }
            if(current == off){
                LED_2_Off;
            }
            break;
        case 3:
            if(current == 0){
                LED_3_On;
            }
            if(current == off){
                LED_3_Off;
            }
            break;
    }
}

void leds_row_on(int led_1, int led_2, int led_3){
    
    for(int i = 0; i < 24; i++){
        
        lcd_gotoxy(16,2);        
        __delay_ms(0.5);
        
        intensity_led(1, i, led_1);
        intensity_led(2, i, led_2);
        intensity_led(3, i, led_3);
        
    }  
    
}

void switch_control(void){
    
    if(switch2 == 1){
        while(switch2 == 1){};
        intensity_2++;
    }
    
    if(switch1 == 0){
        while(switch1 == 0){};
        switch(current_led){
            case 1:
                intensity_1++;
                break;
            case 2:
                intensity_2++;
                break;
            case 3:
                intensity_3++;
                break;
        }
        
    }
  
}

int main(void){
    
    pic_setup();    
    display_init();
    display_home();
    
    intensity_1 = 1;
    intensity_2 = 8;
    intensity_3 = 15;
    current_led = 1;
    
    while(1){

        switch_control();
        
        leds_row_on(intensity_1,intensity_2,intensity_3);
        
        sprintf(led_1_intensity, "%d", intensity_1);
        lcd_write(3, 2, led_1_intensity);        
        
        sprintf(led_2_intensity, "%d", intensity_2);
        lcd_write(8, 2, led_2_intensity);        
        
        sprintf(led_3_intensity, "%d", intensity_3);
        lcd_write(13, 2, led_3_intensity);      
        
    }  
    
}