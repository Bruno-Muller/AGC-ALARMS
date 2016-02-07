/*
 * File:   io.c
 * Author: Bruno
 *
 * Created on 7 février 2016, 00:26
 */


#include <xc.h>

void io_init() {
    // Disable analog pins
    ANSELA = 0x00;
    ANSELC = 0x00;

    // LATCH
    LATA = 0b0000100;
    LATC = 0;

    // TRIS
    TRISA = 0;
    TRISC = 0x03;    
}
