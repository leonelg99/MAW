/*
 * UART.c
 *
 *  Created on: 13 oct. 2023
 *      Author: lguer
 */


#include "../inc/UART.h"

const uint8_t * messages[]={
		"EDU-CIAA READY\n",
		"ALERTA: BATERIA BAJA MOTORES!\n",
		"ALERTA: BATERIA BAJA EDU-CIAA!\n"
		"ERROR 1: COMMAND NOT FOUND\n",
		"ERROR 2: COMMAND WRONG FORMAT\n",
		"ERROR 3: LIMITE ALCANZADO\n"

};


static volatile uint8_t msg[50]={};
static volatile uint8_t  received = 0;

void serialInit(){
	uartConfig( UART, 115200 );
}

uint8_t receiveMsg(uint8_t * msg, uint8_t leng){
	return received = waitForReceiveStringOrTimeoutBlocking(
						UART,
		                msg,
		                leng,
		                10000
		                );
}

void sendMsg(uint8_t numMsg){
	 uartWriteString( UART, messages[numMsg]);
}
