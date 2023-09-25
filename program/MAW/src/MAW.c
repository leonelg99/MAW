/*=============================================================================
 * Program: MAW
 * Date: 2023/09/07
 * Version: 1
 *===========================================================================*/

/*=====[Inclusions of function dependencies]=================================*/

#include "../inc/MAW.h"

//#include "../inc/MSMotorShield.h"
//#include "MSMotorShield.cpp"
#include "../inc/CxxWrapper.h"
#include "sapi.h"

/*=====[Definition macros of private constants]==============================*/

/*=====[Definitions of extern global variables]==============================*/

/*=====[Definitions of public global variables]==============================*/

/*=====[Definitions of private global variables]=============================*/

/*=====[Main function, program entry point after power on or reset]==========*/

int main( void )
{
   // ----- Setup -----------------------------------

	struct MS_DCMotor* M1=newMS_DCMotor(1);
	setSpeed(M1,200);
	run(M1,RELEASE);
   // ----- Repeat for ever -------------------------
   while( 1 ) {
	   run(M1,FORWARD);

   }

   // YOU NEVER REACH HERE, because this program runs directly or on a
   // microcontroller and is not called by any Operating System, as in the 
   // case of a PC program.
   return 0;
}
