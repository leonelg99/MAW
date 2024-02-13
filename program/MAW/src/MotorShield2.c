/*
 * MotorShield2.c
 *
 *  Created on: 29 sep. 2023
 *      Author: lguer
 */


#include "../inc/MotorShield2.h"

//MACROS

#define SCALATE(numero) ((uint8_t)((((numero) > 100) ? 100 : (numero)) * 1.0 / 100 * 100 + 155))

//Function Prototypes
static void latchTx(void);
static void motorsEnable(void);
static inline uint8_t initPWM1(void);
static inline uint8_t initPWM2(void);
static inline uint8_t initPWM3(void);
static inline uint8_t initPWM4(void);
static inline uint8_t setPWM1(uint8_t);
static inline uint8_t setPWM2(uint8_t);
static inline uint8_t setPWM3(uint8_t);
static inline uint8_t setPWM4(uint8_t);
static void setSpeed(uint8_t, uint8_t);
static void run(uint8_t, uint8_t);
static uint8_t goForward(uint8_t);
static uint8_t goBackward(uint8_t);
static uint8_t goRelease(void);
static uint8_t goRotateRight(uint8_t);
static uint8_t goRotateLeft(uint8_t);
static uint8_t goTurnRight(uint8_t, uint8_t);
static uint8_t goTurnLeft(uint8_t, uint8_t);

/******************************************
           LATCH CONTROLLER
******************************************/
uint8_t latch_state=0;


/*
 * latchTx(): this function makes the bit sequence that will be sent to the latch within
 * the L293D driver for the control of the DC motors.
 * */
static void latchTx(void) {
  uint8_t i;

  //LATCH_PORT &= ~BV(LATCH);
  //digitalWrite(MOTORLATCH, LOW);
  gpioWrite(MOTORLATCH, LOW);

  //SER_PORT &= ~BV(SER);
  //digitalWrite(MOTORDATA, LOW);
  gpioWrite(MOTORDATA, LOW);

  for (i=0; i<8; i++) {
    //CLK_PORT &= ~BV(CLK);
    //digitalWrite(MOTORCLK, LOW);
	  gpioWrite(MOTORCLK, LOW);

    if (latch_state & BV(7-i)) {
      //SER_PORT |= BV(SER);
      //digitalWrite(MOTORDATA, HIGH);
    	gpioWrite(MOTORDATA, HIGH);
    } else {
      //SER_PORT &= ~BV(SER);
      //digitalWrite(MOTORDATA, LOW);
    	gpioWrite(MOTORDATA, LOW);
    }
    //CLK_PORT |= BV(CLK);
    //digitalWrite(MOTORCLK, HIGH);
    gpioWrite(MOTORCLK, HIGH);
  }
  //LATCH_PORT |= BV(LATCH);
  //digitalWrite(MOTORLATCH, HIGH);
  gpioWrite(MOTORLATCH, HIGH);
}


/*
 * motorsEnable(): this function starts the pin control of the driver.
 * */
static void motorsEnable(void) {
  // setup the latch
  /*
  LATCH_DDR |= BV(LATCH);
  ENABLE_DDR |= BV(ENABLE);
  CLK_DDR |= BV(CLK);
  SER_DDR |= BV(SER);
  */
  gpioInit(MOTORLATCH,GPIO_OUTPUT);
  gpioInit(MOTORENABLE,GPIO_OUTPUT);
  gpioInit(MOTORDATA,GPIO_OUTPUT);
  gpioInit(MOTORCLK,GPIO_OUTPUT);

  //pinMode(MOTORLATCH, OUTPUT);
  //pinMode(MOTORENABLE, OUTPUT);
  //pinMode(MOTORDATA, OUTPUT);
  //pinMode(MOTORCLK, OUTPUT);

  latch_state = 0;

  latchTx();  // "reset"

  //ENABLE_PORT &= ~BV(ENABLE); // enable the chip outputs!
  //digitalWrite(MOTORENABLE, LOW);
  gpioWrite(MOTORENABLE,LOW);
}


/******************************************
               MOTORS
******************************************/


/*
 * initPWM1(): starts the "PWM 1" signal that will be used for the corresponding motor.
 * */
static inline uint8_t initPWM1() {
	return(pwmInit(PWM0, PWM_ENABLE));
}


/*
 * setPWM1(WCvalue): sets the working cycle of the "PWM 1" signal with a "WCvalue" value.
 * */
static inline uint8_t setPWM1(uint8_t WCvalue) {
	uint8_t error= 1;

	if(!pwmInit(PWM0, PWM_ENABLE_OUTPUT)){
		error=0;
	}
	if(!pwmWrite(PWM0, WCvalue)){
		error=-1;
	}
	return(error);
}


/*
 * initPWM2(): starts the "PWM 2" signal that will be used for the corresponding motor.
 * */
static inline uint8_t initPWM2() {
	return(pwmInit(PWM1, PWM_ENABLE));
}


/*
 * setPWM2(WCvalue): sets the working cycle of the "PWM 2" signal with a "WCvalue" value.
 * */
static inline uint8_t setPWM2(uint8_t WCvalue) {
	uint8_t error= 1;
	if(!pwmInit(PWM1, PWM_ENABLE_OUTPUT)){
		error=0;
	}
	if(!pwmWrite(PWM1, WCvalue)){
		error=-1;
	}
	return (error);
}


/*
 * initPWM3(): starts the "PWM 3" signal that will be used for the corresponding motor.
 * */
static inline uint8_t initPWM3() {
	return(pwmInit(PWM2, PWM_ENABLE));
}


/*
 * setPWM3(WCvalue): sets the working cycle of the "PWM 3" signal with a "WCvalue" value.
 * */
static inline uint8_t setPWM3(uint8_t WCvalue) {
	uint8_t error= 1;
	if(!pwmInit(PWM2, PWM_ENABLE_OUTPUT)){
		error=0;
	}
	if(!pwmWrite(PWM2, WCvalue)){
		error=-1;
	}
	return (error);
}


/*
 * initPWM4(): starts the "PWM 4" signal that will be used for the corresponding motor.
 * */
static inline uint8_t initPWM4() {
	return(pwmInit(PWM3, PWM_ENABLE));
}


/*
 * setPWM4(WCvalue): sets the working cycle of the "PWM 4" signal with a "WCvalue" value.
 * */
static inline uint8_t setPWM4(uint8_t WCvalue) {
	uint8_t error= 1;
	if(!pwmInit(PWM3, PWM_ENABLE_OUTPUT)){
		error=0;
	}
	if(!pwmWrite(PWM1, WCvalue)){
		error=-1;
	}
	return (error);
}


/*
 * motorsInit(): forces an initial value upon the 4 motors of the robot. This is done so that
 * the motors have a known value during the start up of the system.
 * */
void motorsInit() {
  motorsEnable();
  for(int num=1; num<=4;num++){
	  switch (num) {
	    case 1:
	      latch_state &= ~BV(MOTOR1_A) & ~BV(MOTOR1_B); // set both motor pins to 0
	      latchTx();
	      initPWM1();
	      break;
	    case 2:
	      latch_state &= ~BV(MOTOR2_A) & ~BV(MOTOR2_B); // set both motor pins to 0
	      latchTx();
	      initPWM2();
	      break;
	    case 3:
	      latch_state &= ~BV(MOTOR3_A) & ~BV(MOTOR3_B); // set both motor pins to 0
	      latchTx();
	      initPWM3();
	      break;
	    case 4:
	      latch_state &= ~BV(MOTOR4_A) & ~BV(MOTOR4_B); // set both motor pins to 0
	      latchTx();
	      initPWM4();
	      break;
	  }
  }
}


/*
 * setSpeed(motornum, WCvalue): this functions sets the value "WCvalue" as the working cycle
 * value of the "motornum" PWM signal. For example, setSpeed(1, 255) would set the value of
 * the PWM 1 signal's working cycle at a value of 255.
 * */
static void setSpeed(uint8_t motornum, uint8_t WCvalue) {
  switch (motornum) {
  case 1:
    setPWM1(WCvalue); break;
  case 2:
    setPWM2(WCvalue); break;
  case 3:
    setPWM3(WCvalue); break;
  case 4:
    setPWM4(WCvalue); break;
  }
}


/*
 * run(motornum, cmd): executes the comand "cmd" on the "motornum" motor. The available
 * commands for the motors are:
 * 		- BRAKE: stops the wheels for moving
 * 		- FORWARD: spins the wheels forward
 * 		- BACKWARD: spins the wheels backward
 * 		- RELEASE: lets the wheels spin freely
 * */
static void run(uint8_t motornum, uint8_t cmd) {
  uint8_t a, b;
  switch (motornum) {
  case 1:
    a = MOTOR1_A; b = MOTOR1_B; break;
  case 2:
    a = MOTOR2_A; b = MOTOR2_B; break;
  case 3:
    a = MOTOR3_A; b = MOTOR3_B; break;
  case 4:
    a = MOTOR4_A; b = MOTOR4_B; break;
  default:
	  break;
  }

  switch (cmd) {

  case BRAKE:
  	latch_state |= BV(a);
  	latch_state |= BV(b);
  	latchTx();
  	break;
  case FORWARD:
    latch_state |= BV(a);
    latch_state &= ~BV(b);
    latchTx();
    break;
  case BACKWARD:
    latch_state &= ~BV(a);
    latch_state |= BV(b);
    latchTx();
    break;
  case RELEASE:
    latch_state &= ~BV(a);
    latch_state &= ~BV(b);
    latchTx();
    break;
  }
}
/*
 *
 * M1 |------| M4
 *       |
 * 	     |
 * M2 |------| M3
 *
 * */


/*
 * goForward(speed): makes the robot go forward at a "speed" velocity.
 * */
static uint8_t goForward(uint8_t speed){
	for(int i=1; i<=4;i++){
		if(i==2) setSpeed(i,((speed*80)/100));
		else if(i==1) setSpeed(i,((speed*120)/100));
		else setSpeed(i,speed);
	}
	for(int i=1;i<=4;i++){
		run(i,FORWARD);
	}
}


/*
 * goBackward(speed): makes the robot go backward at a "speed" velocity.
 * */
static uint8_t goBackward(uint8_t speed){
	for(int i=1; i<=4;i++){
		setSpeed(i,speed);
	}

	run(1,BACKWARD);
	delay(10);
	run(3,BACKWARD);
	run(4,BACKWARD);
	delay(30);
	run(2,BACKWARD);
}


/*
 * goRelease(): releases the wheels of the robot.
 * */
static uint8_t goRelease(){
	for(int i=1; i<=4;i++){
		setSpeed(i,0);
	}
	for(int i=1;i<=4;i++){
		run(i,RELEASE);
	}
	return 0;
}


/*
 * goRotateRight(speed): the robot makes a clockwise spin over its own axis at a "speed" velocity.
 * */
static uint8_t goRotateRight(uint8_t speed){
	for(int i=1; i<=4;i++){
		setSpeed(i,speed);
	}
	for(int i=1;i<=2;i++){
		run(i,FORWARD);
	}
	for(int i=3;i<=4;i++){
		run(i,BACKWARD);
	}

	return 0;
}


/*
 * goRotateLeft(speed): the robot makes a counter-clockwise spin over its own axis
 * at a "speed" velocity.
 * */
static uint8_t goRotateLeft(uint8_t speed){
	for(int i=1; i<=4;i++){
		setSpeed(i,speed);
	}
	for(int i=1;i<=2;i++){
		run(i,BACKWARD);
	}
	for(int i=3;i<=4;i++){
		run(i,FORWARD);
	}
	return 0;
}


/*
 * goBrake(): brakes the wheels of the robot.
 * */
static uint8_t goBrake(){
	for(int i=1; i<=4;i++){
		setSpeed(i,0);
	}
	for(int i=1;i<=4;i++){
		run(i,BRAKE);
	}
	return 0;
}


/*
 * forwardBasedOnRightSegment(speed, segmentNumber): makes the robot move forward while turning to
 * its right. The turning angle is determined by "segmentNumber" and the speed in which turns its
 * between "speed * 1/8" and "speed * 7/8".
 * */
static void forwardBasedOnRightSegment(uint8_t speed, uint8_t segmentNumber){
	uint8_t aux = speed * (segmentNumber/8);
	if(aux < VEL_MIN) aux = VEL_MIN;

	for(int i=1; i<=2;i++){
		setSpeed(i,speed);
	}
	for(int i=3; i<=4;i++){
		setSpeed(i,aux);
	}
	for(int i=1;i<=4;i++){
		run(i,FORWARD);
	}
}


/*
 * forwardBasedOnLeftSegment(speed, segmentNumber): makes the robot move forward while turning to
 * its left. The turning angle is determined by "segmentNumber" and the speed in which turns its
 * between "speed * 1/8" and "speed * 7/8".
 * */
static void forwardBasedOnLeftSegment(uint8_t speed, uint8_t segmentNumber){
	uint8_t aux = speed * (segmentNumber/8);
	if(aux < VEL_MIN) aux = VEL_MIN;

	for(int i=1; i<=2;i++){
		setSpeed(i,aux);
	}
	for(int i=3; i<=4;i++){
		setSpeed(i,speed);
	}
	for(int i=1;i<=4;i++){
		run(i,FORWARD);
	}
}


// AUX = 0 significa error
// AUX = 1 es que se ejecutó correctamente

/*
 * goTurnRight(angle, speed): determines the aforementioned "segmentNumber" that the
 * forwardBasedOnRightSegment function will use. The segmentations of the possible angles
 * is done by diving the range between 10° to 80° in 8 segments.
 * */
static uint8_t goTurnRight(uint8_t angle, uint8_t speed){
	uint8_t aux = 0;

	if(speed < VEL_MIN) speed = VEL_MIN;

	for(int i = 1; i < 8;i++){
		if(angle <= (i + 1)*10 ){ 			// Pregunta si el ángulo está entre 10 grados y 80 grados
			forwardBasedOnRightSegment(speed, i);
			aux = 1;
		}
	}
	return aux;
}


// AUX = 0 significa error
// AUX = 1 es que se ejecutó correctamente

/*
 * goTurnLeft(angle, speed): determines the aforementioned "segmentNumber" that the
 * forwardBasedOnLeftSegment function will use. The segmentations of the possible angles
 * is done by diving the range between 170° to 100° in 8 segments.
 * */
static uint8_t goTurnLeft(uint8_t angle, uint8_t speed){
	uint8_t aux = 0;

	if(speed < VEL_MIN) speed = VEL_MIN;

	for(int i = 1;i < 8;i++){
		if(angle <= (i + 1 + 9)*10 ){		// Pregunta si el ángulo está entre 100 y 170 grados
			forwardBasedOnLeftSegment(speed, i);
			aux = 1;
		}
	}
	return aux;
}


/*
 * vehicleCmd(cmd, angle, speed): this function calls the previously introduced functions based on
 * the requirements made by the "cmd" command. Although they're not always used, the "angle" and
 * "speed" variables are also included in the function call, so they're used when needed to.
 * */
uint8_t vehicleCmd(uint8_t cmd, uint16_t angle, uint8_t speed){

	uint8_t sp=SCALATE(speed);

	switch (cmd){
	case FORWARD:
		goForward(speed);
		break;
	case BACKWARD:
		goBackward(speed);
		break;
	case RELEASE:
		goRelease();
		break;
	case ROTATERIGHT:
		goRotateRight(speed);
		break;
	case ROTATELEFT:
		goRotateLeft(speed);
		break;
	case TURNRIGHT:
		goTurnRight(angle, speed);
		break;
	case TURNLEFT:
		goTurnLeft(angle, speed);
		break;
	case TURNRIGHTBACKWARD:
		break;
	case TURNLEFTBACKWARD:
		break;
	case BRAKE:
		goBrake();
		break;
	}
}
