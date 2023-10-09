/*=============================================================================
 * Program: MAW
 * Date: 2023/09/07
 * Version: 1
 *===========================================================================*/

/*=====[Inclusions of function dependencies]=================================*/

#include "../inc/MAW.h"
#include "sapi.h"
#include "../inc/MotorShield2.h"

/*=====[Definition macros of private constants]==============================*/

/*=====[Definitions of extern global variables]==============================*/

/*=====[Definitions of public global variables]==============================*/

/*=====[Definitions of private global variables]=============================*/

/*=====[Main function, program entry point after power on or reset]==========*/

#define SERVO_N   SERVO2
#define SERVO_N2  SERVO3
int main( void )
{
	 boardConfig();

	 bool_t valor = 0;

	   uint8_t servoAngle = 0; // 0 a 180 grados

	   // Configurar Servo
	   valor = servoConfig( 0, SERVO_ENABLE );
	   servoConfig( 2, SERVO_ENABLE );
	   valor = servoConfig( SERVO_N, SERVO_ENABLE_OUTPUT );
	   servoConfig( SERVO_N2, SERVO_ENABLE_OUTPUT );
	   // Usar Servo
	   valor = servoWrite( SERVO_N, servoAngle );
	   servoWrite( SERVO_N2, servoAngle );
	   gpioWrite( LEDB, 1 );

	   // ------------- REPETIR POR SIEMPRE -------------
	   while(1) {
	      servoWrite( SERVO_N, 0 );
	      servoWrite( SERVO_N2, 0 );
	      delay(500);

	      servoWrite( SERVO_N, 90 );
	      servoWrite( SERVO_N2, 90 );
	      delay(500);

	      servoWrite( SERVO_N, 180 );
	      servoWrite( SERVO_N2, 180 );
	      delay(500);
	   }

	   // NO DEBE LLEGAR NUNCA AQUI, debido a que a este programa no es llamado
	   // por ningun S.O.
	   return 0 ;
}
