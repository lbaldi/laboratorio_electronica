#include <menu_selector.h>
#include <lcd_write.h>


void menu_selector_0(void){
    
    lcd_write(1,1,">");
    menu_selected = 0;
    lcd_write(9,2," ");
    
}

void menu_selector_1(void){
    
    lcd_write(1,2,">");
    menu_selected = 1;
    lcd_write(1,1," ");
    
}

void menu_selector_2(void){
    
    lcd_write(9,1,">");
    menu_selected = 2;
    lcd_write(1,2," ");
    
}

void menu_selector_3(void){
    
    lcd_write(9,2,">");
    menu_selected = 3;
    lcd_write(9,1," ");
    
}

void menu_selector(void){
    
    switch(menu_selected){
        
        case 0:
            menu_selector_1();
            break;
            
        case 1:
            menu_selector_2();
            break;
            
        case 2:
            menu_selector_3();
            break;
            
        case 3:
            menu_selector_0();
            break;
            
    }
    
}
