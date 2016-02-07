/*
 * File:   i2c.c
 * Author: Bruno
 *
 * Created on 7 février 2016, 00:22
 */

#include "i2c.h"

#include "alarms.h"

#include <xc.h>

#define I2C_FRAME_MASK  0x3F
#define I2C_FRAME_START 0
#define I2C_FRAME_END   2

#define I2C_DISP_ADDRESS    0x10

#define channel     buffer[0]
#define data_high   buffer[1]
#define data_low    buffer[2]

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
    static unsigned char buffer[3];
    static unsigned char i = 0;
    unsigned char temp = SSPBUF; // Read SSPBUF, even for dummy data

    // ADDRESS
    if (SSPSTATbits.D_nA == 0) {
        i = I2C_FRAME_START;
    }
    // DATA
    else {
        // READ (slave writes to master)
        if (SSPSTATbits.R_nW) {
        }
        // WRITE (slave reads from master)
        else {
            buffer[i] = temp;

            if (i == I2C_FRAME_END) {
                alarms_update(channel, data_high, data_low);
            }
        }
        i++;
    }
}