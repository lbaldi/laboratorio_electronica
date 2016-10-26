#include <xc.h>
#include<delays.h>
#include<stdio.h>

#include <unmc_lcd_216.h>
#include <unmc_config_01.h>
#include <unmc_inout_01.h>

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

void display_setup(void){
    
    lcd_init();
    lcd_comand(0b00001100);         //Display=on / Cursor=off / Blink=off
    //lcd_comand(0b00001110);       //Display=on / Cursor=on  / Blink=off
    //lcd_comand(0b00001111);       //Display=on / Cursor=on  / Blink=on

}

void lcd_write(int column, int row, const char* string){
    
    lcd_gotoxy(column, row);
    lcd_putrs(string);
    
}

void display_init(void){
    
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

int main(void){
    
    pic_setup();
    
    display_setup();
        
    while(1){
        
        display_init();
        
        for(int i = 0; i < 32; i++){
            
            __delay_ms(0.5);
            
            intensity_led(1, i, 31);
            intensity_led(2, i, 31);
            intensity_led(3, i, 31);
            
        }
  
    }  
}