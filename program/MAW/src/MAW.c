/*=============================================================================
 * Program: MAW
 * Date: 2023/09/07
 * Version: 1
 *===========================================================================*/

/*=====[Inclusions of function dependencies]=================================*/

#include "../inc/MAW.h"
#include "sapi.h"
#include "../inc/Comands.h"
#include "../inc/UART.h"
#include "../inc/MotorShield2.h"
/*
 * Private Definitions
 */
#define MESSAGE_LONG 50
/*=====[Definition macros of private constants]==============================*/

/*=====[Definitions of extern global variables]==============================*/

/*=====[Definitions of public global variables]==============================*/

/*=====[Definitions of private global variables]=============================*/

/*=====[Main function, program entry point after power on or reset]==========*/
/*
 * The main function of the program start by calling the functions for setup the system and
 * initializing the needed variables, and lastly execute a loop (while(true)) where all happen.
 * The loop essentially do 2 things, execute a command if there is one incoming by uart and every
 * one minute check the batteries levels.
 */
int main( void )
{
	 boardConfig();
	 programInit();
	 gpioWrite( LED1, OFF );
	 gpioWrite( LED2, OFF );
	 gpioWrite( LED3, OFF );
	 uint8_t volatile msg[MESSAGE_LONG]={};
	 tick_t tiempoEnTicks = 0;
	 sendMsg(0);
	 uint8_t x=0;

	 while(1){
		gpioWrite(LED1,OFF);
	 	if(receiveMsg(msg,MESSAGE_LONG)){
	 		gpioWrite(LED1,ON);
	 		//executeCmd(msg);
	 		uartWriteString( UART, msg);
	 	 }

		 /*uartWriteString(UART,"LPTM!");
		 delay(1000);*/
	 	 tiempoEnTicks = tickRead();

	 	 if( tiempoEnTicks >= 60 ){
	 	   	 checkPower();
	 	     tickWrite(0);
	 	 }
	 	memset(msg,0,sizeof(msg));
	 }


	 return 0 ;
}
