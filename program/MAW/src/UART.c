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
		"ERROR 3: COMMAND TOO LONG\n",
		"ERROR 4: LIMITE ALCANZADO\n"

};



static volatile uint8_t  received = 0;

void serialInit(){
	uartConfig( UART, 115200 );
}

uint8_t receiveMsg(uint8_t * msg, uint8_t length){
	uint8_t dato  = 0,reading=0,index=0;
	if(uartReadByte(UART,&dato)){
		if(strcmp((char*)&dato,"\n")){
			sendMsg(4);
			reading = 0;
		} else{
			msg[index++]=dato;
			while(reading){
				if(index <= length){
					if(uartReadByte(UART,&dato)){
						msg[index++]=dato;
					}else reading=0;
				}else {
					reading=0;
					sendMsg(5);
				}
			}
		}
	}
	// Devolver 1 si se obtuvieron todos los bytes, 0 en caso contrario
	return (reading == 0) ? 1 : 0;
}

void sendMsg(uint8_t numMsg){
	 uartWriteString( UART, messages[numMsg]);
}
