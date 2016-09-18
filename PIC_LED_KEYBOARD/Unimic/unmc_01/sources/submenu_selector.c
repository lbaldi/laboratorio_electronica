#include <submenu_selector.h>
#include <lcd_write.h>


void submenu_selector_0(void){
    
    lcd_write(9,1,">");
    submenu_selected = 0;
    lcd_write(9,2," ");
    
}

void submenu_selector_1(void){
    
    lcd_write(9,2,">");
    submenu_selected = 1;
    lcd_write(9,1," ");
    
}

void submenu_selector(void){
    
    switch(submenu_selected){
        
        case 0:
            submenu_selector_1();
            break;
            
        case 1:
            submenu_selector_0();
            break;
    }
    
}