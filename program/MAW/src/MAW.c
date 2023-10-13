/*=============================================================================
 * Program: MAW
 * Date: 2023/09/07
 * Version: 1
 *===========================================================================*/

/*=====[Inclusions of function dependencies]=================================*/

#include "../inc/MAW.h"
#include "sapi.h"
#include "../inc/Comands.h"

/*=====[Definition macros of private constants]==============================*/

/*=====[Definitions of extern global variables]==============================*/

/*=====[Definitions of public global variables]==============================*/

/*=====[Definitions of private global variables]=============================*/

/*=====[Main function, program entry point after power on or reset]==========*/

static uint8_t msg[50]={};

int main( void )
{
	 boardConfig();



	 while(1);
	 	 executeCmd(msg);
	 return 0 ;
}
