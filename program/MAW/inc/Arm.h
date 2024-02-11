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

#define SERVO_R 	SERVO2	// SERVO2 <---> T_FIL2 de EDU-CIAA-NXP
#define SERVO_EX	SERVO6  // SERVO6 <---> LCD2 de EDU-CIAA-NXP
#define SERVO_E		SERVO7  // SERVO7 <---> LCD3 de EDU-CIAA-NXP
#define SERVO_G		SERVO3  // SERVO3 <---> T_FIL3 de EDU-CIAA-NXP

#define ROTATE 	  	1
#define EXTENSION 	2
#define ALTITUDE  	3
#define GRIPPER	  	4
#define HOME		5

#define ANGLE_GAP 			2
#define MARGIN 				30
#define EXTENSION_MAX_ANGLE 180
#define ELEVATION_MAX_ANGLE 180
#define ELEVATION_MIN_ANGLE 0
#define ROTATION_MAX_LEFT	180
#define ROTATION_MAX_RIGHT	0


void armInit(void);
void armCmd(uint8_t, uint16_t);

#endif /* PROGRAM_MAW_INC_ARM_H_ */
