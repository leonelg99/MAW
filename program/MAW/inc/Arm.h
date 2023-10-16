/*
 * Arm.h
 *
 *  Created on: 13 oct. 2023
 *      Author: lguer
 */

#ifndef PROGRAM_MAW_INC_ARM_H_
#define PROGRAM_MAW_INC_ARM_H_

#include "sapi.h"
#include "UART.h"

#define SERVO_NB 	7
#define SERVO_NA1	2
#define SERVO_NA2	3
#define SERVO_NG	6

#define SERVO_B 	SERVO7
#define SERVO_A1	SERVO2
#define SERVO_A2	SERVO3
#define SERVO_G		SERVO6

#define ROTATE 	  	1
#define EXTENSION 	2
#define ALTITUDE  	3
#define GRIPPER	  	4

#define ANGLE_GAP 2
#define MARGIN 10
#define EXTENSION_MAX_ANGLE 180
#define ELEVATION_MAX_ANGLE 180
#define ELEVATION_MIN_ANGLE 0



void armCmd(uint8_t, uint8_t);

#endif /* PROGRAM_MAW_INC_ARM_H_ */
