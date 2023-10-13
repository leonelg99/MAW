/*
 * Comands.c
 *
 *  Created on: 13 oct. 2023
 *      Author: lguer
 */
#include "../inc/Comands.h"


static uint8_t MODE=0; //VEHICLE MODE

static void motorAction(uint8_t [], uint8_t, uint8_t);
static void armAction(uint8_t, uint8_t, uint8_t);

uint8_t execuetAction(uint8_t msg[]){
	uint8_t cmd[5]={}, value1=0,value2=0;

	//decodeMessage(&cmd,&value,&value2);  a implmentar

	if(strcmp(cmd,"MODE")==0){
		MODE=!MODE;
		return 0;
	}

	if(!MODE){
		motorAction(cmd,value1,value2);
	}else{
		//armAction();
	}

	return 0;


}

static void motorAction(uint8_t cmd[], uint8_t value1, uint8_t value2){

	if(strcmp(cmd,"SR")==0){

		if ((value1>=80) && (value1<=100)){
			vehicleCmd(FORWARD,value1,value2);
		}else if((value1>=170) && (value1<=190)){
			vehicleCmd(ROTATELEFT,value1,value2);
		}else if((value1>=260) && (value1<=280)){
			vehicleCmd(BACKWARD,value1,value2);
		}else if(((value1>=0) && (value1<=10))||((value1>=350)&&(value1<360))){
			vehicleCmd(ROTATERIGHT,value1,value2);
		}else if ((value1<80) && (value1>10)){
			vehicleCmd(TURNRIGHT,value1,value2);
		}else if((value1>100) && (value1<170)){
			vehicleCmd(TURNLEFT,value1,value2);
		}else if((value1>190) && (value1<260)){
			vehicleCmd(TURNLEFTBACKWARD,value1,value2);
		}else if(((value1<370) && (value1>280)))
				vehicleCmd(TURNRIGHTBACKWARD,value1,value2);
	}else{
		if(strcmp(cmd,"R2")==0)
				vehicleCmd(BRAKE,value1,value2);
	}

}

/*static void armAction(uint8_t cmd[], uint8_t value1, uint8_t value2){
	switch(cmd){
		case "SR":
			break;
		case "SL"
			break;
		case "R2"
			break;
	}
}*/
