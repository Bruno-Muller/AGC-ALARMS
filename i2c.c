/*
 * File:   i2c.c
 * Author: Bruno
 *
 * Created on 7 février 2016, 00:22
 */

#include <xc.h>

#include "alarms.h"
#include "bits.h"

#define I2C_FRAME_MASK  0x3F

#define I2C_FRAME_START 0
#define I2C_FRAME_END   2

#define I2C_DISP_ADDRESS    0x10

unsigned char i2c_buffer[3];

#define channel     i2c_buffer[0]
#define data_high   i2c_buffer[1]
#define data_low    i2c_buffer[2] 

void i2c_init() {
    TRISCbits.TRISC0 = 1;
    TRISCbits.TRISC1 = 1;

    SSP1ADD = I2C_DISP_ADDRESS << 1;
    //SSPMSK = 0xFF;

    SSP1STATbits.SMP = 0; // Slew rate control enabled for high speed mode (400 kHz)
    //SSPSTATbits.SMP = 1; // Slew Rate Control (limiting) disabled. Operating in I2C Standard mode (100 kHz and 1 MHz).
    SSP1STATbits.CKE = 0; // Disable SMBus specific inputs
    SSP1CON1bits.CKP = 1; // Release clock stretch

    SSP1CON1bits.SSPM = 0b0110; // I2C Slave mode, 7-bit address

    SSP1CON2bits.GCEN = 0; // General call address disabled

    SSP1CON1bits.SSPEN = 1; // Enables the serial port and configures the SDA and SCL pins as serial port pins 

    SSP1IE = 1;
}

void i2c_interrupt_handler() {
    static unsigned char i = 0;
    unsigned char temp;

    temp = SSPBUF; // Read SSPBUF, even for dummy data

    // ADDRESS
    if (SSPSTATbits.D_nA == 0) {
        i = I2C_FRAME_START;
    }        // DATA
    else {
        // READ (slave writes to master)
        if (SSPSTATbits.R_nW) {
        }
            // WRITE (slave reads from master)
        else {
            i2c_buffer[i] = temp;

            if (i == I2C_FRAME_END) {

                // Is it our channel ?
                if (channel == 0x08) {

                    // ***********************
                    // * Data pattern :      *
                    // * data_high  data_low *
                    // * xAAAABCC   CCCDDDDD *
                    // ***********************

                    // Compute digit pair number
                    temp = (data_high & 0b01111000) >> 3;
                    
                    if (temp == DIGIT_RESERVED) {
                        if (testbit(data_low, 3)) setbit(alarms_state[0], 3);
                        else clrbit(alarms_state[0], 3); // NO ATT
                        if (testbit(data_low, 5)) setbit(alarms_state[2], 2);
                        else clrbit(alarms_state[2], 2); // GIMBALL LOCK
                        if (testbit(data_low, 7)) setbit(alarms_state[2], 5);
                        else clrbit(alarms_state[2], 5); // TRACKER
                        if (testbit(data_high, 0)) setbit(alarms_state[2], 3);
                        else clrbit(alarms_state[2], 3); // PROG
                    }
                    
                } else if (channel == 0x09) {
                    if (testbit(data_low, 1)) setbit(alarms_state[3], 5);
                    else clrbit(alarms_state[3], 5); // COMP ACTY
                    if (testbit(data_low, 2)) setbit(alarms_state[0], 2);
                    else clrbit(alarms_state[0], 2); // UPLINK ACTY
                    if (testbit(data_low, 3)) setbit(alarms_state[1], 5);
                    else clrbit(alarms_state[1], 5); // TEMP
                    if (testbit(data_low, 4)) setbit(alarms_state[0], 5);
                    else clrbit(alarms_state[0], 5); // KEY REL
                    if (testbit(data_low, 6)) setbit(alarms_state[1], 2);
                    else clrbit(alarms_state[1], 2); // OPR ERR
                } else if (channel == 0x0B) {
                    if (testbit(data_high, 2)) setbit(alarms_state[0], 4);
                    else clrbit(alarms_state[0], 4); // STBY
                }
            }
        }
        i++;
    }
}