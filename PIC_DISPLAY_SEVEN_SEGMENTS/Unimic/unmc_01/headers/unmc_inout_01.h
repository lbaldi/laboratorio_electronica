/*********************************************************************
 *
 *                Unimic XC8  Version 1.1   02 September 2015
 *
 *********************************************************************
 * FileName:        unmc_config_01.h
 * Dependencies:
 * Processor:       PIC18F26J50
 * Compiler:        XC8 1.34
 * Company:         Microchip Technology, Inc.
 *
 *********************************************************************/

///** S E V E N G - S E G M E N T*************************************/

#define DSS_G          PORTBbits.RB1
#define DSS_G_On       DSS_G = 1;
#define DSS_G_Off      DSS_G = 0;
#define DSS_G_Toggle   DSS_G = !DSS_G;

#define DSS_F          PORTBbits.RB2
#define DSS_F_On       DSS_F = 1;
#define DSS_F_Off      DSS_F = 0;

#define DSS_E          PORTBbits.RB3
#define DSS_E_On       DSS_E = 1;
#define DSS_E_Off      DSS_E = 0;

#define DSS_D          PORTBbits.RB4
#define DSS_D_On       DSS_D = 1;
#define DSS_D_Off      DSS_D = 0;

#define DSS_C          PORTBbits.RB5
#define DSS_C_On       DSS_C = 1;
#define DSS_C_Off      DSS_C = 0;

#define DSS_B          PORTBbits.RB6
#define DSS_B_On       DSS_B = 1;
#define DSS_B_Off      DSS_B = 0;

#define DSS_A          PORTBbits.RB7
#define DSS_A_On       DSS_A = 1;
#define DSS_A_Off      DSS_A = 0;

#define DSS_TEN           LATCbits.LATC7
#define DSS_TEN_On        DSS_TEN = 0;
#define DSS_TEN_Off       DSS_TEN = 1;
#define DSS_TEN_Toggle    DSS_TEN = !DSS_TEN;

#define DSS_UNIT          LATCbits.LATC6
#define DSS_UNIT_On       DSS_UNIT = 0;
#define DSS_UNIT_Off      DSS_UNIT = 1;
#define DSS_UNIT_Toggle   DSS_UNIT = !DSS_UNIT;