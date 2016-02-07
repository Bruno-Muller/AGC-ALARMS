/* 
 * File:   io.h
 * Author: Bruno
 *
 * Created on 7 février 2016, 00:26
 */

#ifndef IO_H
#define	IO_H

#define BLINK_SYNC  RA5

#define SELECT  RA2

#define select_assert()     SELECT = 0
#define select_disassert()  SELECT = 1

void io_init();

#endif	/* IO_H */

