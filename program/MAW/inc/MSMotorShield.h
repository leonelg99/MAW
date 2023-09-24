// MakeShields Motor shield library
// copyright MakeShields, 2010
// this code is public domain, enjoy!
#ifndef _AFMotor_h_
#define _AFMotor_h_

#include <inttypes.h>
#include <stdio.h>
//#include <avr/io.h>
#include "sapi.h"
// comment out this line to remove microstepping support
// for smaller library. Be sure to delete the old library objects!
#define MICROSTEPPING 1

#ifdef MICROSTEPPING
#define MICROSTEP 8
#endif

#define BV(b) (1 << (b))

#define MOTOR1_A 2
#define MOTOR1_B 3
#define MOTOR2_A 1
#define MOTOR2_B 4
#define MOTOR4_A 0
#define MOTOR4_B 6
#define MOTOR3_A 5
#define MOTOR3_B 7

#define FORWARD 1
#define BACKWARD 2
#define BRAKE 3
#define RELEASE 4

#define SINGLE 1
#define DOUBLE 2
#define INTERLEAVE 3


/*
#define LATCH 4
#define LATCH_DDR DDRB
#define LATCH_PORT PORTB

#define CLK_PORT PORTD
#define CLK_DDR DDRD
#define CLK 4

#define ENABLE_PORT PORTD
#define ENABLE_DDR DDRD
#define ENABLE 7

#define SER 0
#define SER_DDR DDRB
#define SER_PORT PORTB
*/

// EDU_CIAA_NXP pin names
#define MOTORLATCH GPIO1
#define MOTORCLK GPIO7
#define MOTORENABLE GPIO5
#define MOTORDATA GPIO3

uint8_t getlatchstate(void);

#endif
