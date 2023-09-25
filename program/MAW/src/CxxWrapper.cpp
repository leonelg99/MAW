#include "../inc/MSMotorShield.h"
#include "../inc/CxxWrapper.h"

extern "C" {
	MS_DCMotor* newMS_DCMotor(uint8_t motornum){
		return new MS_DCMotor(motornum);
	}
	void run(MS_DCMotor* motor,uint8_t cmd){
		motor->run(cmd);
	}

	void setSpeed(MS_DCMotor* motor,uint8_t speed){
		 motor->setSpeed(speed);
	}
}
