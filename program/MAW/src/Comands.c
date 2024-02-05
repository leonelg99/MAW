/*
 * Comands.c
 *
 *  Created on: 13 oct. 2023
 *      Author: lguer
 */
#include "../inc/Comands.h"

#define LEDSON gpioWrite( ENET_TXD0 , ON );
#define LEDSOFF gpioWrite( ENET_TXD0 , OFF );
#define MARGIN 10

static volatile uint8_t MODE=0, leds=0; //VEHICLE MODE
const char m [] = "Execute\n";

static void decodeMessage(uint8_t [], uint8_t [], uint8_t *, uint8_t *);
static void motorAction(uint8_t [], uint8_t, uint8_t);
static void armAction(uint8_t [], uint8_t);


void programInit(){
    gpioInit(ENET_TXD0,GPIO_ENABLE);
    gpioInit(ENET_TXD0,GPIO_OUTPUT);
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
	uint8_t cmd[7]="";
	uint8_t value1=0,value2=0;

	gpioWrite( LED2, ON );
	decodeMessage(msg,cmd,&value1,&value2);
	if(strcmp(cmd,"")==0) {
		gpioWrite(LED2,ON);
		delay(300);
		gpioWrite(LED2,OFF);
	}

	if(strcmp(cmd,"SELECT")==0){
		MODE=!MODE;
		return 0;
	}
	if(strcmp(cmd,"Start")==0){
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

}

static void decodeMessage(uint8_t cadena[], uint8_t palabra[], uint8_t *valor1, uint8_t *valor2) {
    // Copiamos la cadena para no modificar la original
    char copia[strlen(cadena) + 1];
    strcpy(copia, cadena);

    // Utilizamos strtok para dividir la cadena en tokens utilizando el delimitador ":"
    char *token = strtok(copia, ":");

    // La primera palabra
    if (token != NULL) {
        strcpy(palabra, token);
        uartWriteString( UART, token);
        uartWriteString( UART, "--");
        uartWriteString( UART, palabra);
        uartWriteString( UART, "--");
    }

    // El primer valor
    token = strtok(NULL, ":");
    if (token != NULL) {
        sscanf(token, "%hhu", valor1);
    }

    // El segundo valor
    token = strtok(NULL, ":");
    if (token != NULL) {
        sscanf(token, "%hhu", valor2);
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
