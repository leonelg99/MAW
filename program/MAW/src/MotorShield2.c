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
static void setSpeed(uint8_t, uint8_t);
static inline void initPWM1(void);
static inline void initPWM2(void);
static inline void initPWM3(void);
static inline void initPWM4(void);
static inline void setPWM1(uint8_t);
static inline void setPWM2(uint8_t);
static inline void setPWM3(uint8_t);
static inline void setPWM4(uint8_t);
static void setSpeed(uint8_t, uint8_t);
static void run(uint8_t, uint8_t);
static void goForward(uint8_t);
static void goBackward(uint8_t);
static void goRelease(uint8_t);
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
static inline void initPWM1() {
	pwmInit(PWM0, PWM_ENABLE);
}

static inline void setPWM1(uint8_t percent) {
	pwmInit(PWM0, PWM_ENABLE_OUTPUT);
	pwmWrite(PWM0, percent);
}

static inline void initPWM2() {
	pwmInit(PWM1, PWM_ENABLE);
}

static inline void setPWM2(uint8_t percent) {
	pwmInit(PWM1, PWM_ENABLE_OUTPUT);
	pwmWrite(PWM1, percent);

}

static inline void initPWM3() {
	pwmInit(PWM2, PWM_ENABLE);
}

static inline void setPWM3(uint8_t percent) {
	pwmInit(PWM2, PWM_ENABLE_OUTPUT);
	pwmWrite(PWM2, percent);

}

static inline void initPWM4() {
	pwmInit(PWM3, PWM_ENABLE);
}

static inline void setPWM4(uint8_t percent) {
	pwmInit(PWM3, PWM_ENABLE_OUTPUT);
	pwmWrite(PWM3, percent);

}



uint8_t motorsInit() {


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
    return;
  }

  switch (cmd) {
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


static void goForward(uint8_t speed){
	for(int i=1; i<=4;i++){
		setSpeed(i,speed);
	}
	for(int i=1;i<=4;i++){
		run(i,FORWARD);
	}
}

static void goBackward(uint8_t speed){
	for(int i=1; i<=4;i++){
		setSpeed(i,speed);
	}
	for(int i=1;i<=4;i++){
		run(i,BACKWARD);
	}
}

static void goRelease(uint8_t speed){
	for(int i=1; i<=4;i++){
		setSpeed(i,speed);
	}
	for(int i=1;i<=4;i++){
		run(i,RELEASE);
	}
}

uint8_t executeCmd(uint8_t cmd, uint8_t speed){
	switch (cmd){
	case FORWARD:
		goForward(speed);
		break;
	case BACKWARD:
		goBackward(speed);
		break;
	case RELEASE:
		goRelease(speed);
		break;
	case RIGHT:
		break;
	case LEFT:
		break;
	case TURNRIGHT:
		break;
	case TURNLEFT:
		break;
	}
}
