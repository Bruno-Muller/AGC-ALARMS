/*
 * File:   timer.c
 * Author: Bruno
 *
 * Created on 7 février 2016, 11:37
 */

#include "timer.h"

#include "alarms.h"
#include "io.h"

#include <xc.h>

void timer_init() {
    TMR2 = 0;
    T2CONbits.T2CKPS = 0b10; // Prescaler is 16
    PR2 = 250;               // 1 tick = 1ms at 16MHz
    T2CONbits.T2OUTPS = 0b1001; // 1:10 Postscaler ; interrupt each 10ms at 16 MHz
    TMR2IF = 0;
    TMR2IE = 1;
    T2CONbits.TMR2ON = 1;    
}

void timer_interrupt_handler() {
    static unsigned counter = 0;
    counter++;

    if (counter == 44) {
        light = 0;
        BLINK_SYNC = 0;
    } else if (counter == 66) {
        light = 1;
        BLINK_SYNC = 1;
        counter = 0;
    }
}
