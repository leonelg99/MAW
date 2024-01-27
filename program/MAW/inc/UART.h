/*
 * UART.h
 *
 *  Created on: 13 oct. 2023
 *      Author: lguer
 */

#ifndef PROGRAM_MAW_INC_UART_H_
#define PROGRAM_MAW_INC_UART_H_
/*
 * Libraries
 */
#include "sapi.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define UART UART_232	//Mask to avoid write UART_232

/*
 * Public functions
 * serialInit(): inicitialize uart.
 * receiveMsg(): check if there is any data to receive and if it's in the correct format.
 * If it's then save it and return 0.
 * sendMsg(): send one of the standard messages (see UART.c file) indicate by the parameter(index).
 */
void serialInit(void);
uint8_t receiveMsg(uint8_t *, uint8_t);
void sendMsg(uint8_t);

#endif /* PROGRAM_MAW_INC_UART_H_ */
