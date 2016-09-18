/* 
 * File:   pin.h
 * Author: gonza
 *
 * Created on 18 de septiembre de 2016, 14:00
 */

#ifndef PIN_H
#define	PIN_H

#ifdef	__cplusplus
extern "C" {
#endif
    
    extern int activity;
    extern char pin[5];
    extern char pin_input[5];
    
    void activity_pin(void);
    void activity_submenu_pin(void);
    void activity_submenu_pin_set(void);
    void pin_set(void);
    void pin_input_validator(void);
    void pin_reset(void);
    
    
#ifdef	__cplusplus
}
#endif

#endif	/* PIN_H */

