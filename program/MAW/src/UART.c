/*
 * UART.c
 *
 *  Created on: 13 oct. 2023
 *      Author: lguer
 */


#include "../inc/UART.h"

/*
 * This messages are standards messages that EDU-CIAA could send to user at any moment.
 * Each one are based in differents situacions that can or must happend at some point.
 */
const uint8_t * messages[]={
		"EDU-CIAA READY\n",
		"ALERTA: BATERIA BAJA MOTORES!\n",
		"ALERTA: SE ESTA AGOTANDO LA BATERIA DE LA EDU-CIAA!\n",
		"WARNING: BATERIA BAJA DE LA EDU-CIAA!\n",
		"WARNING: BATERIA EN ESTADO CRITICO - APAGUE EL SISTEMA!!!\n",
		"ERROR 1: COMMAND NOT FOUND\n",
		"ERROR 2: COMMAND WRONG FORMAT\n",
		"ERROR 3: COMMAND TOO LONG\n",
		"ERROR 4: LIMITE ALCANZADO\n",
		"BRAZO ACTIVADO\n",
		"BRAZO DESACTIVADO\n",
		"ERROR: LIMITE ALCANZADO!\n"

};

/*
 * serialInit(): this function initialize and config the uart of the EDU-CIAA.
 * In this case, we use UART_232 as UART and set it to a speed of 115200 bps.
 */
void serialInit(){
	uartConfig( UART, 115200 );
}

/*
 * receiveMsg(): this function check if there is any data incoming in the uart channel.
 * And if there is, check that this data has the correct format. If the format is correct,
 * the function return 0 (true), if not return 1.
 * At the same time the data is saved in the memory direction pointed by the parameter msg.
 */
uint8_t receiveMsg(uint8_t * msg, uint8_t length){
	uint8_t dato  = 0,reading=1,index=0,ready=0;
	if(uartReadByte(UART,&dato)){
		if(dato == '\n'){
			sendMsg(6);
		} else{
			msg[index++]=dato;
			while(reading){
				if(index <= length && (dato != '\n')){
					if(uartReadByte(UART,&dato)){
						msg[index++]=dato;
					}
				}else {
					if(index > length) sendMsg(7);
					else ready=1;
					reading=0;

				}
			}
		}
	}
	return ready;
}

/*
 * sendMsg(): this function is used to send the standard messages in the upper part of this file.
 * It's mask the EDU-CIAA firmware_v3 function so only the position of the message is needed.
 */

void sendMsg(uint8_t numMsg){
	 uartWriteString( UART, messages[numMsg]);
}
