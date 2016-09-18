#include <pin.h>
#include <lcd_write.h>
#include <unmc_lcd_216.h>
#include <submenu_selector.h>
#include <string.h>
//#include <home.h>

void activity_pin(void){
    
    activity = 1;
    strcpy(pin_input, "");
    lcd_init();
    lcd_write(1,1,"Ingrese su PIN:");
    lcd_gotoxy(1,2);
    lcd_comand(0b00001111);
    
}

void activity_submenu_pin(void){
    
    activity = 5;
    lcd_init();
    lcd_write(1,1,"Pin");
    lcd_write(10,1,"Cambiar");
    lcd_write(10,2,"Reset");
    submenu_selector_0();
}

void activity_submenu_pin_set(void){
    
    activity = 6;
    strcpy(pin_input, "");
    lcd_init();
    lcd_write(1,1,"Nuevo PIN:");
    lcd_gotoxy(1,2);
    lcd_comand(0b00001111);
    
}

void pin_set(void){
    
    strcpy(pin, pin_input);
    activity_home();
    
}

void pin_input_validator(void){
    
    if( strcmp(pin, pin_input) == 0 ){       
        activity_menu();        
    } 
    else {                
        activity_pin();        
    }    
    
}

void pin_reset(void){
    
    strcpy(pin, "1234");
    
}
