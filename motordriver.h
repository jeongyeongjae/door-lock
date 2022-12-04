#ifndef _MOTORDROVER_H_
#define _MOTORDROVER_H_

#include "mbed.h"

class Motor{
	public:
		Motor(PinName pwm, PinName dir);
	void forward(double speed);
	void backward(double speed);
	void stop();
	void setdir(int dir);
	void incspeed(double step);
	void decspeed(double step);
	
	protected:
		PwmOut _pwm;
		DigitalOut _dir;
		double speedrate;
		int state; //1(forward),0(stop),-1(backward)
};

#endif