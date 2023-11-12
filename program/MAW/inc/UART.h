/*
 * UART.h
 *
 *  Created on: 13 oct. 2023
 *      Author: lguer
 */

#ifndef PROGRAM_MAW_INC_UART_H_
#define PROGRAM_MAW_INC_UART_H_

#include "sapi.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#define UART UART_232

void serialInit(void);
uint8_t receiveMsg(uint8_t *, uint8_t);
void sendMsg(uint8_t);

#endif /* PROGRAM_MAW_INC_UART_H_ */
