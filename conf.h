/* 
 * File:   conf.d
 * Author: Bruno
 *
 * Created on 28 novembre 2015, 19:35
 */

#ifndef CONF_H
#define	CONF_H

#define _XTAL_FREQ 16000000

/******************************
 * #pragma config Settings    *
 * Register: CONFIG1 @ 0x8007 *
 ******************************/

/*
 * CPD = Data Memory Code Protection
 * OFF	Data memory code protection is disabled
 * ON	Data memory code protection is enabled
 */
#pragma config CPD = OFF

/*
 * BOREN = Brown-out Reset Enable
 * OFF      Brown-out Reset disabled
 * ON       Brown-out Reset enabled
 * NSLEEP   Brown-out Reset enabled while running and disabled in Sleep
 * SBODEN   Brown-out Reset controlled by the SBOREN bit in the BORCON register
 */
#pragma config BOREN = ON

/*
 * IESO = Internal/External Switchover
 * OFF      Internal/External Switchover mode is disabled
 * ON       Internal/External Switchover mode is enabled
 */
#pragma config IESO = OFF

/*
 * FOSC = Oscillator Selection
 * ECM      ECM, External Clock, Medium Power Mode (0.5-4 MHz): device clock supplied to CLKIN pin
 * XT       XT Oscillator, Crystal/resonator connected between OSC1 and OSC2 pins
 * LP       LP Oscillator, Low-power crystal connected between OSC1 and OSC2 pins
 * EXTRC    EXTRC oscillator: External RC circuit connected to CLKIN pin
 * ECH      ECH, External Clock, High Power Mode (4-32 MHz): device clock supplied to CLKIN pin
 * ECL      ECL, External Clock, Low Power Mode (0-0.5 MHz): device clock supplied to CLKIN pin
 * INTOSC   INTOSC oscillator: I/O function on CLKIN pin
 * HS       HS Oscillator, High-speed crystal/resonator connected between OSC1 and OSC2 pins
 */
#pragma config FOSC = INTOSC

/*
 * FCMEN = Fail-Safe Clock Monitor Enable
 * OFF      Fail-Safe Clock Monitor is disabled
 * ON       Fail-Safe Clock Monitor is enabled
 */
#pragma config FCMEN = OFF

/*
 * MCLRE = MCLR Pin Function Select
 * OFF      MCLR/VPP pin function is digital input
 * ON       MCLR/VPP pin function is MCLR
 */
#pragma config MCLRE = ON

/*
 * WDTE = Watchdog Timer Enable
 * OFF      WDT disabled
 * ON       WDT enabled
 * NSLEEP   WDT enabled while running and disabled in Sleep
 * SWDTEN   WDT controlled by the SWDTEN bit in the WDTCON register
 */
#pragma config WDTE = OFF

/*
 * CP =	Flash Program Memory Code Protection
 * OFF	Program memory code protection is disabled
 * ON	Program memory code protection is enabled
 */
#pragma config CP = OFF

/*
 * PWRTE =	Power-up Timer Enable
 * OFF	PWRT disabled
 * ON	PWRT enabled
 */
#pragma config PWRTE = ON

/*
 * CLKOUTEN =	Clock Out Enable
 * OFF	CLKOUT function is disabled. I/O or oscillator function on the CLKOUT pin
 * ON	CLKOUT function is enabled on the CLKOUT pin
 */
#pragma config CLKOUTEN = OFF

/******************************
 * #pragma config Settings    *
 * Register: CONFIG2 @ 0x8008 *
 ******************************/

/*
 * PLLEN =	PLL Enable
 * OFF	4x PLL disabled
 * ON	4x PLL enabled
 */
#pragma config PLLEN = OFF

/*
 * WRT =	Flash Memory Self-Write Protection
 * OFF	Write protection off
 * BOOT	000h to 1FFh write protected, 200h to 7FFh may be modified by EECON control
 * HALF	000h to 3FFh write protected, 400h to 7FFh may be modified by EECON control
 * ALL	000h to 7FFh write protected, no addresses may be modified by EECON control
*/
#pragma config WRT = OFF

/*
 * STVREN =	Stack Overflow/Underflow Reset Enable
 * OFF	Stack Overflow or Underflow will not cause a Reset
 * ON	Stack Overflow or Underflow will cause a Reset
*/
#pragma config STVREN = ON

/*
 * BORV =	Brown-out Reset Voltage Selection
 * LO	Brown-out Reset Voltage (Vbor), low trip point selected.
 * HI	Brown-out Reset Voltage (Vbor), high trip point selected.
*/
#pragma config BORV = HI

/*
 * LVP =	Low-Voltage Programming Enable
 * OFF	High-voltage on MCLR/VPP must be used for programming
 * ON	Low-voltage programming enabled
 */
//#pragma CONFIG LVP =

/*****************************
 * Register: IDLOC0 @ 0x8000 *
 * Register: IDLOC1 @ 0x8001 *
 * Register: IDLOC2 @ 0x8002 *
 * Register: IDLOC3 @ 0x8003 *
 *****************************/

#endif	/* CONF_H */



