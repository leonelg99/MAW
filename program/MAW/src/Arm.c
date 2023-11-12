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
/*
 * This function moved the arm to righ or left based on
 * the angle in wich the comand has been send.
 * If it is 180 it moves to left, if it is 0 or 360 to the right, any other is ignored.
 * In addition, it consider a zone and not the exact value, so 180 acctualy is a
 * range from 180-MARGIN to 180+Margin, and the same for 360/0.
 * In the other hand the movement has been limited to avoid breaking the servomotor, so
 * when you try to move beyond the limit, the comand is ignored, and in it is place a message is send.
 */
static void rotate(uint8_t value){
	angleB = servoRead(SERVO_NB);
	if((value>=180-MARGIN) && (value<=180+MARGIN)){	//LEFT
		if(angleB<ROTATION_MAX_LEFT)
			servoWrite(SERVO_NB,(angleB+ANGLE_GAP));
		else sendMsg(3);
	}else if(((value>=0) && (value<=10))||((value>=350)&&(value<360))){		//RIGHT
		if(angleB>0)
					servoWrite(SERVO_NB,(angleB-ANGLE_GAP));
		 else sendMsg(3);
	}
}


static void extend(uint8_t value){
	angleA1 = servoRead(SERVO_NA1);
	if((value>=90-MARGIN) && (value<=90+MARGIN)){
		if(angleA2<EXTENSION_MAX_ANGLE)
			servoWrite(SERVO_NA1,(angleA1+ANGLE_GAP));
		else sendMsg(3);
	}else if ((value>=270-MARGIN) && (value<=270+MARGIN)){
		if(angleA2>0)
			servoWrite(SERVO_NA1,(angleA1-ANGLE_GAP));
		else sendMsg(3);
	}
}

static void elevation(uint8_t value){
	angleA2 = servoRead(SERVO_NA2);
	if((value>=90-MARGIN) && (value<=90+MARGIN)){
		if(angleA2<ELEVATION_MAX_ANGLE)
			servoWrite(SERVO_NA2,(angleA2+ANGLE_GAP));
		else sendMsg(3);
	}else if ((value>=270-MARGIN) && (value<=270+MARGIN)){
		if(angleA2>ELEVATION_MIN_ANGLE)
			servoWrite(SERVO_NA2,(angleA2-ANGLE_GAP));
		else sendMsg(3);
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
