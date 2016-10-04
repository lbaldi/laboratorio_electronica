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
#include <unmc_config_01.h>
#include <unmc_inout_01.h>

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
    
}

void number_0(void){
    DSS_A_On;
    DSS_B_On;
    DSS_C_On;
    DSS_D_On;
    DSS_E_On;
    DSS_F_On;
    DSS_G_Off;
}

void number_1(void){
    DSS_A_Off;
    DSS_B_On;
    DSS_C_On;
    DSS_D_Off;
    DSS_E_Off;
    DSS_F_Off;
    DSS_G_Off;
}

void number_2(void){
    DSS_A_On;
    DSS_B_On;
    DSS_C_Off;
    DSS_D_On;
    DSS_E_On;
    DSS_F_Off;
    DSS_G_On;
}

void number_3(void){
    DSS_A_On;
    DSS_B_On;
    DSS_C_On;
    DSS_D_On;
    DSS_E_Off;
    DSS_F_Off;
    DSS_G_On;
}

void number_4(void){
    DSS_A_Off;
    DSS_B_On;
    DSS_C_On;
    DSS_D_Off;
    DSS_E_Off;
    DSS_F_On;
    DSS_G_On;
}

void number_5(void){
    DSS_A_On;
    DSS_B_Off;
    DSS_C_On;
    DSS_D_On;
    DSS_E_Off;
    DSS_F_On;
    DSS_G_On;
}

void number_6(void){
    DSS_A_On;
    DSS_B_Off;
    DSS_C_On;
    DSS_D_On;
    DSS_E_On;
    DSS_F_On;
    DSS_G_On;
}

void number_7(void){
    DSS_A_On;
    DSS_B_On;
    DSS_C_On;
    DSS_D_Off;
    DSS_E_Off;
    DSS_F_Off;
    DSS_G_Off;
}

void number_8(void){
    DSS_A_On;
    DSS_B_On;
    DSS_C_On;
    DSS_D_On;
    DSS_E_On;
    DSS_F_On;
    DSS_G_On;
}

void number_9(void){
    DSS_A_On;
    DSS_B_On;
    DSS_C_On;
    DSS_D_On;
    DSS_E_Off;
    DSS_F_On;
    DSS_G_On;
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


int main(void){
    
    setup();
    
    int unit = 3;
    int ten = 2;
    
    DSS_UNIT_On;
    DSS_TEN_Off;
    
    while(1){
   
        if(DSS_UNIT == 0){
            number_to_segment(unit);
        }else{
            number_to_segment(ten);
        }
        
        DSS_UNIT_Toggle;
        DSS_TEN_Toggle;

        __delay_ms(98);
        
    }
    
    return 0;
    
}