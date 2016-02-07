/*
 * File:   alarms.c
 * Author: Bruno
 *
 * Created on 7 février 2016, 11:47
 */


#include "alarms.h"

#include "bits.h"

unsigned char alarms_state[] = {0x00,0x00,0x00,0x00};

unsigned char light;

void alarms_update(unsigned char channel, unsigned char data_high, unsigned char data_low) {
    
    // Is it our channel ?
    if (channel == 0x08) {

        // ***********************
        // * Data pattern :      *
        // * data_high  data_low *
        // * xAAAABCC   CCCDDDDD *
        // ***********************

        // Compute digit pair number
        unsigned char temp = (data_high & 0b01111000) >> 3;

        if (temp == DIGIT_RESERVED) {
            // NO ATT
            if (testbit(data_low, 3)) setbit(alarms_state[0], 3);
            else clrbit(alarms_state[0], 3); 
            
            // GIMBALL LOCK
            if (testbit(data_low, 5)) setbit(alarms_state[2], 2);
            else clrbit(alarms_state[2], 2); 
            
            // TRACKER
            if (testbit(data_low, 7)) setbit(alarms_state[2], 5);
            else clrbit(alarms_state[2], 5); 
            
            // PROG
            if (testbit(data_high, 0)) setbit(alarms_state[2], 3);
            else clrbit(alarms_state[2], 3); 
        }

    } else if (channel == 0x09) {
        // COMP ACTY
        if (testbit(data_low, 1)) setbit(alarms_state[3], 5);
        else clrbit(alarms_state[3], 5); 
        
        // UPLINK ACTY
        if (testbit(data_low, 2)) setbit(alarms_state[0], 2);
        else clrbit(alarms_state[0], 2); 
        
        // TEMP
        if (testbit(data_low, 3)) setbit(alarms_state[1], 5);
        else clrbit(alarms_state[1], 5); 
        
        // KEY REL
        if (testbit(data_low, 4)) setbit(alarms_state[0], 5);
        else clrbit(alarms_state[0], 5); 
        
        // OPR ERR
        if (testbit(data_low, 6)) setbit(alarms_state[1], 2);
        else clrbit(alarms_state[1], 2); 
        
        
    } else if (channel == 0x0B) {
        // STBY
        if (testbit(data_high, 2)) setbit(alarms_state[0], 4);
        else clrbit(alarms_state[0], 4); 
    }
}
