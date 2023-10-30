/*
 * Comands.h
 *
 *  Created on: 13 oct. 2023
 *      Author: lguer
 */

#ifndef PROGRAM_MAW_INC_COMANDS_H_
#define PROGRAM_MAW_INC_COMANDS_H_

#include "../inc/MotorShield2.h"
#include "../inc/Arm.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define LOWBATERY 1000
#define TASK_PERIODICITY 60000

uint8_t executeCmd(uint8_t []);
void programInit(void);
#endif /* PROGRAM_MAW_INC_COMANDS_H_ */
