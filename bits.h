/* 
 * File:   bits.h
 * Author: Bruno
 *
 * Created on 7 février 2016, 11:32
 */

#ifndef BITS_H
#define	BITS_H

#define testbit(var, bit) ((var) & (1 <<(bit)))
#define setbit(var, bit) ((var) |= (1 << (bit)))
#define clrbit(var, bit) ((var) &= ~(1 << (bit)))

#endif	/* BITS_H */

