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

uint8_t executeCmd(uint8_t []);
void programInit(void);
void checkPower(void);
#endif /* PROGRAM_MAW_INC_COMANDS_H_ */
