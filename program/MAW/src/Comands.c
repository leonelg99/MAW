/*
 * Comands.c
 *
 *  Created on: 13 oct. 2023
 *      Author: lguer
 */
#include "../inc/Comands.h"
/*
 * Private Definitions and Macros
 * LEDSON/LEDSOFF: as their name says are used to turn on/off a couple of leds by
 * turning ON/OFF ENET_TXD0.
 * MARGIN: is literally a margin, a value later used to deal with angles operations.
 * LOWBATERY: the limit from which the battery charge level of the EDU-CIAA is considered low.
 * WARNINGBATERY: the limit from which the battery charge level of the EDU-CIAA is considered very low.
 * DEADBATERY: the limit from which the battery charge level of the EDU-CIAA is considered
 * extremely low, and can cause bad functions and damage.
 */
#define LEDSON gpioWrite( ENET_TXD0 , ON );
#define LEDSOFF gpioWrite( ENET_TXD0 , OFF );
#define MARGIN 30
#define LOWBATERY 		765
#define WARNINGBATERY 	644
#define DEADBATERY 		564
#define MESSAGE_LONG 30

/*
 * Private Variables
 * MODE: indicate in which mode the system is operating. 0 indicates vehicle mode,
 * in this mode we operate the motors to move the vehicle. Mean while with 1, is arm mode
 * in which case, we manage the robotic arm.
 * leds: is a variable used as flag to indicate if the leds are ON or OFF.
 */
static volatile uint8_t MODE=0, leds=0; //VEHICLE MODE
const char m [] = "Execute\n";

/*
 * Private Prototype Functions
 * decodeMessage(): this function decode the comand/message/data received by uart
 * motorAction(): This function activates the motors according to the command received
 * and the configuration parameters
 * armAction(): like the previous function, this operates the robotic arm according to
 * the command received and the configuration parameters.
 */
static void decodeMessage(uint8_t [], uint8_t [], uint16_t *, uint8_t *);
static void motorAction(uint8_t [], uint16_t, uint8_t);
static void armAction(uint8_t [], uint16_t, uint8_t);
static void stickAction (uint8_t [], uint16_t , uint8_t);
/*
 * programInit(): this function config and initialize ADC, UART, MotoroShield and system
 * tick.
 */
void programInit(){

	adcConfig( ADC_ENABLE ); /* ADC */
	motorsInit();
	armInit();
	serialInit();
	tickWrite(0);
}

/*
 * checkPower(): this function reads channels 1 and 2 of the ADC, and determinate if the
 * EDU-CIAA and/or Motors battery is charge level is low.
 * Depending of the battery level, a message could be send according how low it is.
 * The charge levels are 4, normal, in which case nothing happened, LOW which mean that the charge
 * level has decrease enough to be consider. WARNING which mean that charge levels are very low,
 * and it necessary to charge the battery, lastly, DEAD, in this case the charge level is so low
 * that could imply a not only malfunction but even a damage to the system.
 */
void checkPower(){
	uint16_t MotorBatery,CIAABatery;
    char x[20];
	MotorBatery = adcRead(CH2);
	CIAABatery = adcRead(CH1);

	if(MotorBatery <= LOWBATERY)
		sendMsg(1);
	if((CIAABatery <= LOWBATERY) && (CIAABatery > WARNINGBATERY))
		sendMsg(2);
	else if((CIAABatery <= WARNINGBATERY) && (CIAABatery > DEADBATERY))
		sendMsg(3);
	else if(CIAABatery <= DEADBATERY)
		sendMsg(4);

}

/*
 *executeCmd(): this function receive the message previously gotten by UART and process
 *and execute it.
 *The message is processed to obtain 3 values, the command (cmd), value1 and value2
 *(usually angle and speed). Then depending on cmd, it will execute different actions,
 *basically, change of MODE (arm or vehicle), turn off/on the leds, or execute so execute
 *basically, some action through motors or the arm (depending of the MODE) in which cases it will
 *call an specific function for it.
 */
uint8_t executeCmd(uint8_t msg[]){
	uint8_t cmd[7]="";
	uint16_t value1=0;
	uint8_t value2=0;

	gpioWrite( LED2, ON );
	decodeMessage(msg,cmd,&value1,&value2);
	if(strcmp(cmd,"")==0) {
		gpioWrite(LED2,ON);
		delay(300);
		gpioWrite(LED2,OFF);
	}

	if(strcmp(cmd,"Select")==0){
		MODE=!MODE;
		if (MODE == 1)
			sendMsg(9);
		else sendMsg(10);
		return 0;
	}
	if(strcmp(cmd,"Start")==0){
		sendMsg(0);
		if(leds==0) {
			LEDSON;
			leds=!leds;
		} else{
			LEDSOFF;
			leds=!leds;
		}
		return 0;
	}

	if(!MODE){
		motorAction(cmd,value1,value2);
	}else{
		armAction(cmd,value1,value2);
	}
    return 0;
}

/*
 * decodeMessage(): this function is used to split the message (msg) into 3 variables,
 * cmd, v1, and v2.
 */
static void decodeMessage(uint8_t cadena[], uint8_t palabra[], uint16_t *valor1, uint8_t *valor2) {
    // Copiamos la cadena para no modificar la original
    char copia[strlen(cadena) + 1];
    strcpy(copia, cadena);
    float aux=0;

    // Utilizamos strtok para dividir la cadena en tokens utilizando el delimitador ":"
    char *token = strtok(copia, ":");

    // La primera palabra
    if (token != NULL) {
        strcpy(palabra, token);
    }

    // El primer valor
    token = strtok(NULL, ":");
    if (token != NULL) {
        sscanf(token, "%hu", valor1);
    }

    // El segundo valor
    token = strtok(NULL, ":");
    if (token != NULL) {
    	 unsigned int temp_value;
    	 sscanf(token, "%u", &temp_value);
    	 *valor2 = (uint8_t)temp_value;
    }
}
/*
 * motorAction(): this function order the motors to execute an action depending cmd,
 * and it according to value1 and value2 (angle and speed).
 */
static void motorAction(uint8_t cmd[], uint16_t value1, uint8_t value2){

	if((strcmp(cmd,"SR")==0) && (value2!=0)){ //SR STICK RIGHT

		if ((value1>=80) && (value1<=100)){
			vehicleCmd(FORWARD,value1,value2);
		}else if((value1>=170) && (value1<=190)){
			vehicleCmd(ROTATELEFT,value1,value2);
		}else if((value1>=260) && (value1<=280)){
			vehicleCmd(BACKWARD,value1,value2);
		}else if(((value1>=0) && (value1<=10))||((value1>=350)&&(value1<360))){
			vehicleCmd(ROTATERIGHT,value1,value2);
		}else if ((value1<80) && (value1>10)){
			vehicleCmd(TURNRIGHT,value1,value2);
		}else if((value1>100) && (value1<170)){
			vehicleCmd(TURNLEFT,value1,value2);
		}else if((value1>190) && (value1<260)){
			vehicleCmd(TURNLEFTBACKWARD,value1,value2);
		}else if(((value1<370) && (value1>280))){
				vehicleCmd(TURNRIGHTBACKWARD,value1,value2);
		}
	}else{
		if(((value1==0) && (value2==0)) || (strcmp(cmd,"R2")==0)) vehicleCmd(BRAKE,0,0);
	}

}

//------------------------------------------------------------------------------------------------
/*
static void motorAction(uint8_t cmd[], uint16_t value1, uint8_t value2){
	if(strcmp(cmd,"R2")==0){
		vehicleCmd(BRAKE,value1,value2);
	}
	else if ((value1==0) && (value2==0)){
		vehicleCmd(RELEASE,0,0);
	}
	else{
		if((strcmp(cmd,"SR")==0) && (value2!=0)){ //SR STICK RIGHT
			if ((value1>=80) && (value1<=100)){
				vehicleCmd(FORWARD,value1,value2);

			}else if((value1>=170) && (value1<=190)){
				vehicleCmd(ROTATELEFT,value1,value2);

			}else if((value1>=260) && (value1<=280)){
				vehicleCmd(BACKWARD,value1,value2);

			}else if(((value1>=0) && (value1<=10))||((value1>=350)&&(value1<360))){
				vehicleCmd(ROTATERIGHT,value1,value2);

			}else if ((value1<80) && (value1>10)){
				vehicleCmd(TURNRIGHT,value1,value2);

			}else if((value1>100) && (value1<170)){
				vehicleCmd(TURNLEFT,value1,value2);

			}else if((value1>190) && (value1<260)){
				vehicleCmd(TURNLEFTBACKWARD,value1,value2);

			}else if(((value1<370) && (value1>280))){
				vehicleCmd(TURNRIGHTBACKWARD,value1,value2);
			}
		}
	}
}
*/
//------------------------------------------------------------------------------------------------

/*
 * armAction(): this function order the arm to execute an action depending cmd,
 * and it according to value1 (angle).
 * Value 2 is not used since normally is speed value, which has no sense for the arm.
 */
static void armAction(uint8_t cmd[], uint16_t value1, uint8_t value2){

	uint8_t aux=0,stop=0;
	uint8_t cmd2[7]="";
	uint16_t value10=0;
	uint8_t value20=0;
	uint8_t msg[MESSAGE_LONG]={};

	if(((strcmp(cmd,"SR")==0) || (strcmp(cmd,"SL")==0)) && (value2 != 0) ){
		while(!stop){
				 stickAction(cmd,value1,value2);
				 delay(500);
				 if(receiveMsg(msg,MESSAGE_LONG)){
					 decodeMessage(msg,cmd2,&value10,&value20);
					 if((strcmp(cmd2,"SR") || (strcmp(cmd2,"SL"))) && (value10==0) && (value20==0))
						 stop=!stop;
					 memset(msg,'\0',sizeof(msg));
				 }
			}
		aux=1;
		}

	if(!aux && strcmp(cmd,"R2")==0){
		//Close
		armCmd(GRIPPER,0);
		aux=1;
	}
	if(!aux && strcmp(cmd,"R1")==0){
		//OPEN
		armCmd(GRIPPER,1);
		aux=1;
	}

	if(!aux && strcmp(cmd,"L1")==0){
		//OPEN FULL
		armCmd(GRIPPER,2);
		aux=1;
	}
	if(!aux && strcmp(cmd,"L2")==0){
		//CLOSE FULL only if full open otherwise servo could be damage
		armCmd(GRIPPER,3);
		aux=1;
	}

	if(!aux && strcmp(cmd,"SLL")==0){
		//GO HOME FUNCTION
		aux=1;
	}
	delay(500);

}

static void stickAction (uint8_t cmd[], uint16_t value1, uint8_t value2){
	if(strcmp(cmd,"SR")==0){
		if ((value1>=(90-MARGIN)) && (value1<=(90+MARGIN))){
				armCmd(EXTENSION,value1);
		}else if((value1>=(270-MARGIN)) && (value1<=(270+MARGIN))){
				armCmd(EXTENSION,value1);
		}else if((value1>=(180-MARGIN)) && (value1<=(180+MARGIN))){
				armCmd(ROTATE,value1);
		}else if(((value1>=0) && (value1<=MARGIN))||((value1>=(360-MARGIN))&&(value1<360))){
				armCmd(ROTATE,value1);
		}
	 }else
		if(strcmp(cmd,"SL")==0){
			if(((value1>=(90-MARGIN)) && (value1<=(90+MARGIN))) || ((value1>=(270-MARGIN)) && (value1<=(270+MARGIN))))
				armCmd(ALTITUDE,value1);
			}
}
