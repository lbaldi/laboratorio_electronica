#include <activation.h>
#include <unmc_lcd_216.h>
#include <lcd_write.h>
#include <submenu_selector.h>

void activity_submenu_activation(void){
    
    activity = 4;
    lcd_init();
    lcd_write(1,1,"Activar");
    lcd_write(10,1,"Si");
    lcd_write(10,2,"No");
    submenu_selector_0();
    
}