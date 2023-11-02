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


/*=====[Definition macros of private constants]==============================*/

/*=====[Definitions of extern global variables]==============================*/

/*=====[Definitions of public global variables]==============================*/

/*=====[Definitions of private global variables]=============================*/

/*=====[Main function, program entry point after power on or reset]==========*/

int main( void )
{
	 boardConfig();
	 programInit();

	 uint8_t volatile msg[50]={};
	 tick_t tiempoEnTicks = 0;
	 sendMsg(0);
	 while(1){
		 sendMsg(0);
		 delay(100);

	 	/*if(receiveMsg(msg,strlen(msg))){
	 		executeCmd(msg);
	 	 }
	 	 tiempoEnTicks = tickRead();
	 	 if( tiempoEnTicks >= 60000 ){
	 	   	 //checkPower();
	 	     tickWrite(0);
	 	 }

	 	memset(msg,0,sizeof(msg));*/

	 }
	 return 0 ;
}
