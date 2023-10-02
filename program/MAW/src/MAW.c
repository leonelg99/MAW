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
#include "../inc/MotorShield2.h"

/*=====[Definition macros of private constants]==============================*/

/*=====[Definitions of extern global variables]==============================*/

/*=====[Definitions of public global variables]==============================*/

/*=====[Definitions of private global variables]=============================*/

/*=====[Main function, program entry point after power on or reset]==========*/

#define SERVO_N   SERVO2

int main( void )
{
 	boardInit();
   // ----- Setup -----------------------------------

	/*struct MS_DCMotor* M1=newMS_DCMotor(1);
	setSpeed(M1,200);
	run(M1,RELEASE);*/
	//motorsInit();
	servoConfig (0, SERVO_ENABLE );

	servoConfig( SERVO_N, SERVO_ENABLE_OUTPUT );

	servoWrite( SERVO_N, 0 );
	servoRead( SERVO_N );
   // ----- Repeat for ever -------------------------
   while( 1 ) {
	   //run(M1,FORWARD);
	  //executeCmd(FORWARD,200);

	   servoWrite( SERVO_N, 0 );
	         delay(500);

	         servoWrite( SERVO_N, 90 );
	         delay(500);

	         servoWrite( SERVO_N, 180 );
	         delay(500);

   }

   // YOU NEVER REACH HERE, because this program runs directly or on a
   // microcontroller and is not called by any Operating System, as in the 
   // case of a PC program.
   return 0;
}
