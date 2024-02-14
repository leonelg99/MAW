/*
 * Arm.c
 *
 *  Created on: 13 oct. 2023
 *      Author: lguer
 */

#include "../inc/Arm.h"

//Actual position of servo motors
static uint8_t angleR=0;
static uint8_t angleEX=0;
static uint8_t angleE=0;
static uint8_t angleG=0;


static void servoInits(void);
static void rotate(uint8_t);
static void extend(uint8_t);

static void servosInits(){
	servoConfig( 0, SERVO_ENABLE );
	servoConfig( SERVO_R,  SERVO_ENABLE_OUTPUT);
	servoConfig( SERVO_EX, SERVO_ENABLE_OUTPUT);
	servoConfig( SERVO_E,  SERVO_ENABLE_OUTPUT);
	servoConfig( SERVO_G,  SERVO_ENABLE_OUTPUT);

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
	angleEX = servoRead(SERVO_EX);
	if((value>=90-MARGIN) && (value<=90+MARGIN)){
		if(angleEX<EXTENSION_MAX_ANGLE)
			servoWrite(SERVO_EX,(angleEX+ANGLE_GAP));
		else sendMsg(3);
	}else if ((value>=270-MARGIN) && (value<=270+MARGIN)){
		if(angleEX>0)
			servoWrite(SERVO_EX,(angleEX-ANGLE_GAP));
		else sendMsg(3);
	}
}

static void elevation(uint8_t value){
	angleE = servoRead(SERVO_E);
	if((value>=90-MARGIN) && (value<=90+MARGIN)){
		if(angleE<ELEVATION_MAX_ANGLE)
			servoWrite(SERVO_E,(angleE+ANGLE_GAP));
		else sendMsg(3);
	}else if ((value>=270-MARGIN) && (value<=270+MARGIN)){
		if(angleE>ELEVATION_MIN_ANGLE)
			servoWrite(SERVO_E,(angleE-ANGLE_GAP));
		else sendMsg(3);
	}

}

static void gripper(uint16_t value){
	if(value < 2){
		angleG = servoRead(SERVO_G);
		if(value == 0){
			if(angleG>0)
				servoWrite(SERVO_G,angleG+ANGLE_GAP);
			else sendMsg(12);
		}else{
			if(angleG<90)
				servoWrite(SERVO_G,angleG-ANGLE_GAP);
			else sendMsg(13);
		}
	}else {
		if(value == 2){
			if(angleG < 90)
				servoWrite(SERVO_G,90);
			else sendMsg(13);
		}else{
			if(angleG > 0)
				servoWrite(SERVO_G,0);
			else sendMsg(12);
		}
	}
}

static void goHome(){
	angleG = servoRead(SERVO_G);
	angleR = servoRead(SERVO_R);
	angleE = servoRead(SERVO_E);
	angleEX = servoRead(SERVO_EX);
	if(angleG<=90) gripper(2); //Open full gripper

	//Elevation depends of extension grade
	//
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
		gripper(value);
		break;
	case HOME:
		goHome();
		break;
	}

}
