#define LED_1           LATBbits.LATB2
#define LED_2           LATBbits.LATB1
#define LED_3           LATBbits.LATB0

#define LED_1_On        LED_1 = 1;
#define LED_2_On        LED_2 = 1;
#define LED_3_On        LED_3 = 1;

#define LED_1_Off       LED_1 = 0;
#define LED_2_Off       LED_2 = 0;
#define LED_3_Off       LED_3 = 0;

#define LED_1_Toggle    LED_1 = !LED_1;
#define LED_2_Toggle    LED_2 = !LED_2;
#define LED_3_Toggle    LED_3 = !LED_3;

#define switch2         PORTAbits.RA0
#define switch1         PORTAbits.RA1

