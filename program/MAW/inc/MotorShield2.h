/*
 * MotorShield2.h
 *
 *  Created on: 29 sep. 2023
 *      Author: lguer
 */

#ifndef PROGRAM_MAW_INC_MOTORSHIELD2_H_
#define PROGRAM_MAW_INC_MOTORSHIELD2_H_

#include "sapi.h"
#include <stdio.h>

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
#define	RIGHT 5
#define	LEFT 6
#define	TURNRIGHT 7
#define TURNLEFT 8

// EDU_CIAA_NXP pin names
#define MOTORLATCH GPIO1
#define MOTORCLK GPIO7
#define MOTORENABLE GPIO5
#define MOTORDATA GPIO3

void motorsInit(void);
//uint8_t setSpeed(uint8_t motornum, uint8_t speed);
uint8_t executeCmd(uint8_t cmd, uint8_t speed);


#endif /* PROGRAM_MAW_INC_MOTORSHIELD2_H_ */
