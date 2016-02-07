/*
 * File:   interrupt.c
 * Author: Bruno
 *
 * Created on 7 février 2016, 11:40
 */

#include "i2c.h"
#include "timer.h"

#include <xc.h>

void interrupt interrupt_handler() {
    if (TMR2IF) {
        TMR2IF = 0;
        timer_interrupt_handler();
    }
    
    if (SSP1IF == 1) {
        SSP1IF = 0; // Clear interrupt flag
        i2c_interrupt_handler();
    }
}
