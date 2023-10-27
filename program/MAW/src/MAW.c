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

	 //serialInit();
	 motorsInit();
	 //uint8_t volatile msg[50]={};
	 //sendMsg(0);
	 vehicleCmd(RELEASE, 0, 0);
	 while(1){
	 	 vehicleCmd(FORWARD, 0, 200);
	 	 delay(1000);
	 	 vehicleCmd(BRAKE, 0, 0);
	 	 delay(1000);
	 	 delay(1000);
	 	 vehicleCmd(RELEASE, 0, 0);
	 	 delay(1000);
	 	 delay(1000);

	 	/* if(receiveMsg(msg,strlen(msg))){
	 		executeCmd(msg);
	 	 }
	 	 memset(msg,0,sizeof(msg));*/
	 }

	 return 0 ;
}
