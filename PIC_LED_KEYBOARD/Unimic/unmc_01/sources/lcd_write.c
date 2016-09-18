#include <lcd_write.h>
#include <unmc_lcd_216.h>

void lcd_write(int column, int row, const char* string){
    
    lcd_gotoxy(column, row);
    lcd_putrs(string);
    
}
