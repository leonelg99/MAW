/*
 * CxxWrapper.h
 *
 *  Created on: 25 sep. 2023
 *      Author: lguer
 */

#ifndef PROGRAM_MAW_INC_CXXWRAPPER_H_
#define PROGRAM_MAW_INC_CXXWRAPPER_H_

#define FORWARD 1
#define BACKWARD 2
#define BRAKE 3
#define RELEASE 4

#ifdef __cplusplus
extern "C"{
#endif
typedef struct MS_DCMotor MS_DCMotor;
MS_DCMotor* newMS_DCMotor(uint8_t motornum);
void run(MS_DCMotor* motor,uint8_t cmd);
void setSpeed(MS_DCMotor* motor,uint8_t speed);
#ifdef __cplusplus
}
#endif

#endif /* PROGRAM_MAW_INC_CXXWRAPPER_H_ */
