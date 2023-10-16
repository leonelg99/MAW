/*
 * MotorShield2.c
 *
 *  Created on: 29 sep. 2023
 *      Author: lguer
 */


#include "../inc/MotorShield2.h"


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

static inline uint8_t initPWM1() {
	return(pwmInit(PWM0, PWM_ENABLE));
}
static inline uint8_t setPWM1(uint8_t percent) {
	uint8_t error= 1;

	if(!pwmInit(PWM0, PWM_ENABLE_OUTPUT)){
		error=0;
	}
	if(!pwmWrite(PWM0, percent)){
		error=-1;
	}

	return(error);
}


static inline uint8_t initPWM2() {
	return(pwmInit(PWM1, PWM_ENABLE));
}
static inline uint8_t setPWM2(uint8_t percent) {
	uint8_t error= 1;
	if(!pwmInit(PWM1, PWM_ENABLE_OUTPUT)){
		error=0;
	}
	if(!pwmWrite(PWM1, percent)){
		error=-1;
	}

	return (error);
}


static inline uint8_t initPWM3() {
	return(pwmInit(PWM2, PWM_ENABLE));
}
static inline uint8_t setPWM3(uint8_t percent) {
	uint8_t error= 1;
	if(!pwmInit(PWM2, PWM_ENABLE_OUTPUT)){
		error=0;
	}
	if(!pwmWrite(PWM2, percent)){
		error=-1;
	}

	return (error);
}


static inline uint8_t initPWM4() {
	return(pwmInit(PWM3, PWM_ENABLE));
}
static inline uint8_t setPWM4(uint8_t percent) {
	uint8_t error= 1;
	if(!pwmInit(PWM3, PWM_ENABLE_OUTPUT)){
		error=0;
	}
	if(!pwmWrite(PWM1, percent)){
		error=-1;
	}

	return (error);
}


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

static void setSpeed(uint8_t motornum, uint8_t percent) {
  switch (motornum) {
  case 1:
    setPWM1(percent); break;
  case 2:
    setPWM2(percent); break;
  case 3:
    setPWM3(percent); break;
  case 4:
    setPWM4(percent); break;
  }
}

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


static uint8_t goForward(uint8_t speed){
	for(int i=1; i<=4;i++){
		setSpeed(i,speed);
	}
	for(int i=1;i<=4;i++){
		run(i,FORWARD);
	}
}

static uint8_t goBackward(uint8_t speed){
	for(int i=1; i<=4;i++){
		setSpeed(i,speed);
	}
	for(int i=1;i<=4;i++){
		run(i,BACKWARD);
	}
}

static uint8_t goRelease(){
	for(int i=1; i<=4;i++){
		setSpeed(i,0);
	}
	for(int i=1;i<=4;i++){
		run(i,RELEASE);
	}
	return 0;
}

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

static uint8_t goBrake(){
	for(int i=1; i<=4;i++){
		setSpeed(i,0);
	}
	for(int i=1;i<=4;i++){
		run(i,BRAKE);
	}
	return 0;
}



static void forwardBasedOnRightSegment(uint8_t speed, uint8_t segmentNumber){
	for(int i=1; i<=2;i++){
		setSpeed(i,speed);
	}
	for(int i=3; i<=4;i++){
		setSpeed(i,(speed * (segmentNumber/8)));
	}
	for(int i=1;i<=4;i++){
		run(i,FORWARD);
	}
}

static void forwardBasedOnLeftSegment(uint8_t speed, uint8_t segmentNumber){
	for(int i=1; i<=2;i++){
		setSpeed(i,(speed * (segmentNumber/8)));
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
static uint8_t goTurnRight(uint8_t angle, uint8_t speed){
	uint8_t aux = 0;

	if(speed < VEL_MIN) speed = VEL_MIN;

	for(int i = 1; i++; i < 8){
		if(angle <= (i + 1)*10 ){ 						// Pregunta si el ángulo está entre 10 grados y 80 grados
			forwardBasedOnRightSegment(speed, i);
			aux = 1;
		}
	}
	return aux;
}


// AUX = 0 significa error
// AUX = 1 es que se ejecutó correctamente
static uint8_t goTurnLeft(uint8_t angle, uint8_t speed){
	uint8_t aux = 0;

	if(speed < VEL_MIN) speed = VEL_MIN;

	for(int i = 1; i++; i < 8){
		if(angle <= (i + 1 + 9)*10 ){					// Pregunta si el ángulo está entre 100 y 170 grados
			forwardBasedOnLeftSegment(speed, i);
			aux = 1;
		}
	}
	return aux;
}

uint8_t vehicleCmd(uint8_t cmd, uint8_t angle, uint8_t speed){
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
