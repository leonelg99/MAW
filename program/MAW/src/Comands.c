/*
 * Comands.c
 *
 *  Created on: 13 oct. 2023
 *      Author: lguer
 */
#include "../inc/Comands.h"
/*
 * Private Definitions and Macros
 * LEDSON/LEDSOFF as their name says are used to turn on/off a couple of leds by
 * turning ON/OFF ENET_TXD0.
 * Margin is literaly a margin
 */
#define LEDSON gpioWrite( ENET_TXD0 , ON );
#define LEDSOFF gpioWrite( ENET_TXD0 , OFF );
#define MARGIN 10

static volatile uint8_t MODE=0, leds=0; //VEHICLE MODE
const char m [] = "entre";

static void decodeMessage(uint8_t [], uint8_t *, uint8_t *, uint8_t *);
static void motorAction(uint8_t [], uint8_t, uint8_t);
static void armAction(uint8_t [], uint8_t);


void programInit(){
	adcConfig( ADC_ENABLE ); /* ADC */
	motorsInit();
	serialInit();
	tickWrite(0);
}

void checkPower(){
	uint16_t MotorBatery,CIAABatery;
    char x[20];
	MotorBatery = adcRead(CH2);
	CIAABatery = adcRead(CH1);
	//sprintf(x,"%d",CIAABatery);
    //uartWriteString( UART,x);

	if(MotorBatery <= LOWBATERY)
		sendMsg(1);
	if((CIAABatery <= LOWBATERY) && (CIAABatery > WARNINGBATERY))
		sendMsg(2);
	else if((CIAABatery <= WARNINGBATERY) && (CIAABatery > DEADBATERY))
		sendMsg(3);
	else if(CIAABatery <= DEADBATERY)
		sendMsg(4);

}

uint8_t executeCmd(uint8_t msg[]){
	uint8_t cmd[7]={}, value1=0,value2=0;

	uartWriteString( UART, m);
	gpioWrite( LED2, ON );
	decodeMessage(msg,cmd,&value1,&value2);
	uartWriteString( UART, cmd);

	if(strcmp(cmd,"SELECT")==0){
		MODE=!MODE;
		return 0;
	}
	if(strcmp(cmd,"START")==0){
		sendMsg(0);
		if(leds==0) {
			LEDSON;
			leds=!leds;
		} else{
			LEDSOFF;
			leds=!leds;
		}
		return 0;
	}

	if(!MODE){
		motorAction(cmd,value1,value2);
	}else{
		armAction(cmd,value1);
	}

	return 0;
}


static void decodeMessage(uint8_t msg[], uint8_t * cmd, uint8_t * v1, uint8_t * v2){
	// Usamos strtok para dividir la cadena en tokens usando ':'
	    uartWriteString( UART,"Decode\n");
	    char *token = strtok((char *)msg, ":");
	    uartWriteString( UART,"----\n");
	    // El primer token es la palabra
	    if (token != NULL) {
	        strcpy(cmd, token);
	    } else {
	    	uartWriteString( UART,"ERROR\n");
	    }

	    // El segundo token es valor1
	    token = strtok(NULL, ":");
	    if (token != NULL) {
	        *v1 = atoi(token);
	    } else {
	        // Manejar un error si es necesario
	    	uartWriteString( UART,"ERROR 2\n");
	    }

	    // El tercer token es valor2
	    token = strtok(NULL, "\n");
	    if (token != NULL) {
	        *v2 = atoi(token);
	    } else {
	        // Manejar un error si es necesario
	    	uartWriteString( UART,"ERROR 3\n");
	    }
}

static void motorAction(uint8_t cmd[], uint8_t value1, uint8_t value2){

	if(strcmp(cmd,"SR")==0){ //SR STICK RIGHT

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

static void armAction(uint8_t cmd[], uint8_t value1){
	if(strcmp(cmd,"SR")==0){
		if ((value1>=90-MARGIN) && (value1<=90+MARGIN)){
			armCmd(EXTENSION,value1);
		}else if((value1>=180-MARGIN) && (value1<=180+MARGIN)){
			armCmd(EXTENSION,value1);
		}else if((value1>=270-MARGIN) && (value1<=270+MARGIN)){
			armCmd(ROTATE,value1);
		}else if(((value1>=0) && (value1<=MARGIN))||((value1>=360-MARGIN)&&(value1<360))){
			armCmd(ROTATE,value1);
		}
	}
	if(strcmp(cmd,"SL")==0){
		int a=0;
	}
	if(strcmp(cmd,"R2")==0){
		int b=0;
	}
	if(strcmp(cmd,"R1")==0){
		int c=0;
	}
}
