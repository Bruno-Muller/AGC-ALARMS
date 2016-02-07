/* 
 * File:   main.c
 * Author: Bruno
 *
 * Created on 28 novembre 2015, 19:38
 */

#include "alarms.h"
#include "conf.h"
#include "i2c.h"
#include "io.h"
#include "timer.h"

#include <xc.h>

void init() {
    // CLOCK 16 MHz
    OSCCON = 0b01111000; // 4x PLL is disabled; 16 MHz HF; Clock determined by FOSC<2:0> in Configuration Word 1

    io_init();
    timer_init();
    i2c_init();
    
    PEIE = 1;
}

int main(void) {
    
    init();

    ei();
    
    unsigned char decoder_address, alarms;
    
    while(1) {
        unsigned char i;
    
        for (i=0; i<4; i++) {

            decoder_address = (PORTA & ~0x03) | (i & 0x03);
            alarms = (PORTC & ~0x3C) | (alarms_state[i] & 0x3C);
            
            // If blinking and lights must be turned off
            if (light == 0) {
                if (i == 0) alarms &= 0b00011100;
                if (i == 1) alarms &= 0b00111000;
            }

            select_disassert();

            PORTA = decoder_address;
            PORTC = alarms;

            select_assert();

            __delay_ms(2);
           
        }
    }
}
