/*
 * Arm.c
 *
 *  Created on: 13 oct. 2023
 *      Author: lguer
 */

#include "../inc/Arm.h"

//Actual position of servo motors
static uint8_t angleB=0;
static uint8_t angleA1=0;
static uint8_t angleA2=0;
static uint8_t angleG=0;


static void servoInits(void);

static void servosInits(){

	servoConfig( SERVO_NB, SERVO_ENABLE );
	servoConfig( SERVO_NA1, SERVO_ENABLE );
	servoConfig( SERVO_NA2, SERVO_ENABLE );
	servoConfig( SERVO_NG, SERVO_ENABLE );

}

void armInit(){
	servosInits();

}


void executeCmd(uint8_t cmd, uint8_t value){

	switch(cmd){
	case ROTATE:
		break;
	case EXTENSION:
		break;
	case ALTITUDE:
		break;
	case GRIPPER:
		break;
	}

}
