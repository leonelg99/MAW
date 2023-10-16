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
static void rotate(uint8_t);
static void extend(uint8_t);

static void servosInits(){

	servoConfig( SERVO_NB, SERVO_ENABLE );
	servoConfig( SERVO_NA1, SERVO_ENABLE );
	servoConfig( SERVO_NA2, SERVO_ENABLE );
	servoConfig( SERVO_NG, SERVO_ENABLE );

}

void armInit(){
	servosInits();

}

static void rotate(uint8_t value){
	angleB = servoRead(SERVO_NB);
	if((value>=270-MARGIN) && (value<=270+MARGIN)){	//LEFT
		if(angleB<180)
			servoWrite(SERVO_NB,(angleB+ANGLE_GAP));
		else sendMSG(3);
	}else if(((value>=0) && (value<=10))||((value>=350)&&(value<360))){		//RIGHT
		if(angleB>0)
					servoWrite(SERVO_NB,(angleB-ANGLE_GAP));
		 else sendMSG(3);
	}
}
static void extend(uint8_t value){
	angleA1 = servoRead(SERVO_NA1);
	if((value>=90-MARGIN) && (value<=90+MARGIN)){
		if(angleA2<ALTITUD__MAX_ANGLE)
			servoWrite(SERVO_NA1,(angleA1+ANGLE_GAP));
		else sendMSG(3);
	}else if ((value>=270-MARGIN) && (value<=270+MARGIN)){
		if(angleA2>0)
			servoWrite(SERVO_NA1,(angleA1-ANGLE_GAP));
		else sendMSG(3);
	}
}

static void elevation(uint8_t value){
	angleA2 = servoRead(SERVO_NA2);
	if((value>=90-MARGIN) && (value<=90+MARGIN)){
		if(angleA2<ELEVATION_MAX_ANGLE)
			servoWrite(SERVO_NA2,(angleA2+ANGLE_GAP));
		else sendMSG(3);
	}else if ((value>=270-MARGIN) && (value<=270+MARGIN)){
		if(angleA2>ELEVATION_MIN_ANGLE)
			servoWrite(SERVO_NA2,(angleA2-ANGLE_GAP));
		else sendMSG(3);
	}
}
}

void armCmd(uint8_t cmd, uint8_t value){

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
	}

}
