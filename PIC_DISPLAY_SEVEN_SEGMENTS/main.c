/*******************************************************************************
 *
 *                Unimic 01 Version 1.1   18 october 2016
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
#include <unmc_config_01.h>
#include <unmc_inout_01.h>

int temp = 0;

void setup(void){  
    
    OSCTUNEbits.INTSRC=1;      
    OSCTUNEbits.PLLEN=0;       
    OSCCONbits.IRCF0=1;         
    OSCCONbits.IRCF1=1;
    OSCCONbits.IRCF2=1;
    OSCCONbits.SCS0=0;          
    OSCCONbits.SCS1=0;
    TRISA = 0b11110001;
    TRISB = 0;
    TRISC = 0b00100111;
    ANCON0=0b11111110;          
    ANCON1=0b11111111;          
    ADCON0=0b01000000;          
    ADCON1=0b00111000;          
    RTCCFGbits.RTCEN=1;
    RTCCFGbits.RTCWREN=1;
    T1CONbits.T1OSCEN=1;
    
}

void segment_a(void){
    
    DSS_A_On;
    __delay_ms(1);
    DSS_A_Off;
    
}

void segment_b(void){
    
    DSS_B_On;
    __delay_ms(1);
    DSS_B_Off;
    
}

void segment_c(void){
    
    DSS_C_On;
    __delay_ms(1);
    DSS_C_Off;
    
}

void segment_d(void){
    
    DSS_D_On;
    __delay_ms(1);
    DSS_D_Off;
    
}

void segment_e(void){
    
    DSS_E_On;
    __delay_ms(1);
    DSS_E_Off;
    
}

void segment_f(void){
    
    DSS_F_On;
    __delay_ms(1);
    DSS_F_Off;
    
}

void segment_g(void){
    
    DSS_G_On;
    __delay_ms(1);
    DSS_G_Off;
    
}

void number_0(void){
    
    segment_a();
    segment_b();
    segment_c();
    segment_d();
    segment_e();
    segment_f();
    
}

void number_1(void){
    
    segment_b();
    segment_c();
    
}

void number_2(void){
    
    segment_a();
    segment_b();
    segment_d();
    segment_e();
    segment_g();
    
}

void number_3(void){
    
    segment_a();
    segment_b();
    segment_c();
    segment_d();
    segment_g();
    
}

void number_4(void){
    
    segment_b();
    segment_c();
    segment_f();
    segment_g();
    
}

void number_5(void){
    
    segment_a();
    segment_c();
    segment_d();
    segment_f();
    segment_g();
    
}

void number_6(void){
    
    segment_a();
    segment_c();
    segment_d();
    segment_e();
    segment_f();
    segment_g();
    
}

void number_7(void){
    
    segment_a();
    segment_b();
    segment_c();
    
}

void number_8(void){
    
    segment_a();
    segment_b();
    segment_c();
    segment_d();
    segment_e();
    segment_f();
    segment_g();
    
}

void number_9(void){
    
    segment_a();
    segment_b();
    segment_c();
    segment_d();
    segment_f();
    segment_g();  
    
}

void number_to_segment(int number){
    
    switch(number){
        
        case 0:
            number_0();
            break;
        case 1:
            number_1();
            break;
        case 2:
            number_2();
            break;
        case 3:
            number_3();
            break;
        case 4:
            number_4();
            break;
        case 5:
            number_5();
            break;
        case 6:
            number_6();
            break;
        case 7:
            number_7();
            break;
        case 8:
            number_8();
            break;
        case 9:
            number_9();
            break;
            
    }
    
}

void read_temp(void){
    
    ADCON0bits.ADON = 1;    //Encendido de conversor analogica/digital
    ADCON0bits.GO = 1;      //Comienzo de conversion analogica/digital
    
    int voltage;
    
    while (ADCON0bits.GO);    
    __delay_ms(1);
    voltage = (int) ADRESH;       
    
    ADCON0bits.ADON = 0;    //Fin de conversi�n analogica/digital  
    
    //Caso de limite excedido (fuera del rango 0 a 100)
    if(voltage < 25 || voltage > 76){
        
        temp = -1;
        
    } else {
        
        temp = (int)(1.94 * voltage - 48);
        
    }
    
}

void write_temp(void){
    
    //Caso de limite excedido (fuera del rango 0 a 100)
    if(temp == -1){
        
        DSS_UNIT_On;
        segment_g();
        DSS_UNIT_Off;
        
        DSS_TEN_On;
        segment_g();
        DSS_TEN_Off;
        
    } else {
        
        DSS_UNIT_On;
        number_to_segment(temp % 10);
        DSS_UNIT_Off;
        
        DSS_TEN_On;
        number_to_segment(temp / 10);
        DSS_TEN_Off; 
        
    }
    
}

int main(void){
    
    setup();
    
    while(1){
        
        read_temp();
        write_temp();       
        
    }
    
    return 0;  
    
}