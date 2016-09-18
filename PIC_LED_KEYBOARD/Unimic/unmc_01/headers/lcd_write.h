/* 
 * File:   lcd_write.h
 * Author: gonzalo alejandro cozzi
 *
 * Created on 18 de septiembre de 2016, 12:19
 */

#ifndef LCD_WRITE_H
#define	LCD_WRITE_H

#ifdef	__cplusplus
extern "C" {
#endif
    
    void lcd_write(int column, int row, const char* string);
    
    
#ifdef	__cplusplus
}
#endif

#endif	/* LCD_WRITE_H */


