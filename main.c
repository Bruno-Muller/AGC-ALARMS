/* 
 * File:   main.c
 * Author: Bruno
 *
 * Created on 28 novembre 2015, 19:38
 */


#include <xc.h>

#include "conf.h"

#define SELECT  RA2

#define select_assert()     SELECT = 0
#define select_disassert()  SELECT = 1

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

void timer_init() {
    // Init Timer
    TMR2 = 0;
    T2CONbits.T2CKPS = 0b10; // Prescaler is 16
    PR2 = 250;               // 1 tick = 1ms at 16MHz
    T2CONbits.T2OUTPS = 0b1001; // 1:10 Postscaler ; interrupt each 10ms at 16 MHz
    TMR2IF = 0;
    TMR2IE = 1;
    T2CONbits.TMR2ON = 1;    
}

void i2c_init() {
    // Init I2C
    #define DISP_I2C_ADDRESS    0x10

    TRISCbits.TRISC0 = 1;
    TRISCbits.TRISC1 = 1;

    SSP1ADD = DISP_I2C_ADDRESS<<1;
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

void init() {
    // CLOCK 16 MHz
    OSCCON = 0b01111000; // 4x PLL is disabled; 16 MHz HF; Clock determined by FOSC<2:0> in Configuration Word 1

    io_init();
    timer_init();
    i2c_init();
    
    PEIE = 1;
}

#define DIGIT_RESERVED  0b00001100

#define I2C_FRAME_MASK  0x3F

#define I2C_FRAME_START 0
#define I2C_FRAME_END   2

#define CHANNEL_DSKY    010

#define BLINK_SYNC  RA5

#define testbit(var, bit) ((var) & (1 <<(bit)))
#define setbit(var, bit) ((var) |= (1 << (bit)))
#define clrbit(var, bit) ((var) &= ~(1 << (bit)))

unsigned char alarms[] = {0x00,0x00,0x00,0x00};

unsigned char light;

void interrupt interrupt_handler() {
    static unsigned char i = 0;
    static unsigned char buffer[3];
    unsigned char temp;

#define channel     buffer[0]
#define data_high   buffer[1]
#define data_low    buffer[2]

    if (TMR2IF) {
        static unsigned counter = 0;
        TMR2IF = 0;

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
    
    if (SSP1IF == 1) {
        SSP1IF = 0; // Clear interrupt flag
        temp = SSPBUF; // Read SSPBUF, even for dummy data

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

                    // Is it our channel ?
                    if (channel == 0x08) {

                        /***********************
                         * Data pattern :      *
                         * data_high  data_low *
                         * xAAAABCC   CCCDDDDD *
                         ***********************/

                        // Compute digit pair number
                        temp = (data_high & 0b01111000) >> 3;
                        if (temp == DIGIT_RESERVED) {
                            if (testbit(data_low, 3)) setbit(alarms[0], 3);
                            else clrbit(alarms[0], 3); // NO ATT
                            if (testbit(data_low, 5)) setbit(alarms[2], 2);
                            else clrbit(alarms[2], 2); // GIMBALL LOCK
                            if (testbit(data_low, 7)) setbit(alarms[2], 5);
                            else clrbit(alarms[2], 5); // TRACKER
                            if (testbit(data_high, 0)) setbit(alarms[2], 3);
                            else clrbit(alarms[2], 3); // PROG
                        }
                    } else if (channel == 0x09) {
                        if (testbit(data_low, 1)) setbit(alarms[3], 5);
                        else clrbit(alarms[3], 5); // COMP ACTY
                        if (testbit(data_low, 2)) setbit(alarms[0], 2);
                        else clrbit(alarms[0], 2); // UPLINK ACTY
                        if (testbit(data_low, 3)) setbit(alarms[1], 5);
                        else clrbit(alarms[1], 5); // TEMP
                        if (testbit(data_low, 4)) setbit(alarms[0], 5);
                        else clrbit(alarms[0], 5); // KEY REL
                        if (testbit(data_low, 6)) setbit(alarms[1], 2);
                        else clrbit(alarms[1], 2); // OPR ERR
                    } else if (channel == 0x0B) {
                        if (testbit(data_high, 2)) setbit(alarms[0], 4);
                        else clrbit(alarms[0], 4); // STBY
                    }
                }
            }
            i++;
        }
    }
}

int main(void) {
    
    init();

    ei();
    
    unsigned char temp1, temp2;
    

    while(1) {
        unsigned char i;
    
        for (i=0; i<4; i++) {

            temp1 = (PORTA & ~0x03) | (i & 0x03);
            temp2 = (PORTC & ~0x3C) | (alarms[i] & 0x3C);
            
            if (light == 0) {
                if (i == 0) temp2 &= 0b00011100;
                if (i == 1) temp2 &= 0b00111000;
            }

            select_disassert();

            PORTA = temp1;
            PORTC = temp2;

            select_assert();

            __delay_ms(2);
           
        }
    }
}
