// MakeShields Motor shield library
// copyright MakeShields, 2010
// this code is public domain, enjoy!


//#include <avr/io.h>
//#include "Arduino.h"

#include "../inc/MSMotorShield.h"
#include "../inc/CxxWrapper.h"

static uint8_t latch_state;

#define MICROSTEPS 16  // 8, 16 & 32 are popular

//#define MOTORDEBUG 1

MSMotorController::MSMotorController(void) {
}

void MSMotorController::enable(void) {
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

  latch_tx();  // "reset"

  //ENABLE_PORT &= ~BV(ENABLE); // enable the chip outputs!
  //digitalWrite(MOTORENABLE, LOW);
  gpioWrite(MOTORENABLE,LOW);
}


void MSMotorController::latch_tx(void) {
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

static MSMotorController MC;


/******************************************
               MOTORS
******************************************/
inline void initPWM1() {
	pwmInit(PWM0, PWM_ENABLE);
}

inline void setPWM1(uint8_t percent) {
	pwmWrite(PWM0, percent);
	pwmInit(PWM0, PWM_ENABLE_OUTPUT);
}

inline void initPWM2() {
	pwmInit(PWM1, PWM_ENABLE);
}

inline void setPWM2(uint8_t percent) {
	pwmWrite(PWM1, percent);
	pwmInit(PWM1, PWM_ENABLE_OUTPUT);
}

inline void initPWM3() {
	pwmInit(PWM2, PWM_ENABLE);
}

inline void setPWM3(uint8_t percent) {
	pwmWrite(PWM2, percent);
	pwmInit(PWM2, PWM_ENABLE_OUTPUT);
}

inline void initPWM4() {
	pwmInit(PWM3, PWM_ENABLE);
}

inline void setPWM4(uint8_t percent) {
	pwmWrite(PWM3, percent);
	pwmInit(PWM3, PWM_ENABLE_OUTPUT);
}



MS_DCMotor::MS_DCMotor(uint8_t num) {
  motornum = num;

  MC.enable();

  switch (num) {
  case 1:
    latch_state &= ~BV(MOTOR1_A) & ~BV(MOTOR1_B); // set both motor pins to 0
    MC.latch_tx();
    initPWM1();
    break;
  case 2:
    latch_state &= ~BV(MOTOR2_A) & ~BV(MOTOR2_B); // set both motor pins to 0
    MC.latch_tx();
    initPWM2();
    break;
  case 3:
    latch_state &= ~BV(MOTOR3_A) & ~BV(MOTOR3_B); // set both motor pins to 0
    MC.latch_tx();
    initPWM3();
    break;
  case 4:
    latch_state &= ~BV(MOTOR4_A) & ~BV(MOTOR4_B); // set both motor pins to 0
    MC.latch_tx();
    initPWM4();
    break;
  }
}

void MS_DCMotor::run(uint8_t cmd) {
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
    MC.latch_tx();
    break;
  case BACKWARD:
    latch_state &= ~BV(a);
    latch_state |= BV(b);
    MC.latch_tx();
    break;
  case RELEASE:
    latch_state &= ~BV(a);
    latch_state &= ~BV(b);
    MC.latch_tx();
    break;
  }
}


void MS_DCMotor::setSpeed(uint8_t percent) {
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


