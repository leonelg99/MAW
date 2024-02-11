/*
 * Arm.c
 *
 *  Created on: 13 oct. 2023
 *      Author: lguer
 */

#include "../inc/Arm.h"

//Actual position of servo motors
static uint8_t angleR=0;
static uint8_t angleA1=0;
static uint8_t angleA2=0;
static uint8_t angleG=0;


static void servoInits(void);
static void rotate(uint8_t);
static void extend(uint8_t);

static void servosInits(){

	servoConfig( SERVO_R, SERVO_ENABLE );
	servoConfig( SERVO_EX, SERVO_ENABLE );
	servoConfig( SERVO_E, SERVO_ENABLE );
	servoConfig( SERVO_G, SERVO_ENABLE );

}

void armInit(){
	servosInits();

}


static void rotate(uint8_t value){
	angleR = servoRead(SERVO_R);
	if((value>=180-MARGIN) && (value<=180+MARGIN)){	//LEFT
		if(angleR<ROTATION_MAX_LEFT)
			servoWrite(SERVO_R,(angleR+ANGLE_GAP));
		else sendMsg(11);
	}else if(((value>=0) && (value<=10))||((value>=350)&&(value<360))){		//RIGHT
		if(angleR>ROTATION_MAX_RIGHT)
					servoWrite(SERVO_R,(angleR-ANGLE_GAP));
		 else sendMsg(11);
	}

}


static void extend(uint8_t value){
	angleA1 = servoRead(SERVO_EX);
	if((value>=90-MARGIN) && (value<=90+MARGIN)){
		if(angleA2<EXTENSION_MAX_ANGLE)
			servoWrite(SERVO_EX,(angleA1+ANGLE_GAP));
		else sendMsg(3);
	}else if ((value>=270-MARGIN) && (value<=270+MARGIN)){
		if(angleA2>0)
			servoWrite(SERVO_EX,(angleA1-ANGLE_GAP));
		else sendMsg(3);
	}
}

static void elevation(uint8_t value){
	angleA2 = servoRead(SERVO_E);
	if((value>=90-MARGIN) && (value<=90+MARGIN)){
		if(angleA2<ELEVATION_MAX_ANGLE)
			servoWrite(SERVO_E,(angleA2+ANGLE_GAP));
		else sendMsg(3);
	}else if ((value>=270-MARGIN) && (value<=270+MARGIN)){
		if(angleA2>ELEVATION_MIN_ANGLE)
			servoWrite(SERVO_E,(angleA2-ANGLE_GAP));
		else sendMsg(3);
	}

}

void armCmd(uint8_t cmd, uint16_t value){

	switch(cmd){
	case ROTATE:
		rotate(value);
		break;
	case EXTENSION:
		extend(value);
		break;
	case ALTITUDE:
		elevation(value);
		break;
	case GRIPPER:
		break;
	case HOME:
		break;
	}

}
