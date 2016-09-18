#include <siren.h>
#include <lcd_write.h>
#include <unmc_lcd_216.h>
#include <submenu_selector.h>


void activity_submenu_siren(void){
    
    activity = 7;
    lcd_init();
    lcd_write(1,1,"Sirena");
    lcd_write(10,1,"Volumen");
    lcd_write(10,2,"Tono");
    submenu_selector_0();
    
}

void activity_submenu_siren_volume(void){
    
    activity = 8;
    lcd_init();
    lcd_write(1,1,"Volumen");
    lcd_write(10,1,"Bajo");
    lcd_write(10,2,"Alto");
    submenu_selector_0();
    
}

void activity_submenu_siren_tone(void){
    
    activity = 9;
    lcd_init();
    lcd_write(1,1,"Tono");
    lcd_write(10,1,"Tono 1");
    lcd_write(10,2,"Tono 2");
    submenu_selector_0();
    
}
