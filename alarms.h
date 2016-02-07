/* 
 * File:   alarms.h
 * Author: Bruno
 *
 * Created on 7 février 2016, 11:46
 */

#ifndef ALARMS_H
#define	ALARMS_H

#define DIGIT_RESERVED  0b00001100

extern unsigned char alarms_state[];
extern unsigned char light;

void alarms_update(unsigned char channel, unsigned char data_high, unsigned char data_low);

#endif	/* ALARMS_H */

