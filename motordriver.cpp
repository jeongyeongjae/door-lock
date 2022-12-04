#include "motordriver.h"

Motor::Motor(PinName pwm, PinName dir)
:_pwm(pwm), _dir(dir)
{
	_pwm.period(0.001); //[eropd: lmse => freq.: 1000Hz
	_pwm.write(0); //pwm = 0
	
	_dir = 0;
	state = 0;
	speedrate = 0.0;
}
void Motor::forward(double speed){
	if (state < 0){ //current state = backward
		_pwm=0;
		wait(0.1);
	}
	_dir =1;
	_pwm = speed;
	state = 1;
	
}
void Motor::backward(double speed){
	if (state < 0){ //current state = backward
		_pwm=1;
		wait(0.1);
	}
	_dir = 0;
	_pwm = speed;
	state = -1;
	
}
void Motor::stop(void){
	_pwm = 0;
	state = 0;
}

void Motor::setdir(int dir){
	if(state != dir){
		_pwm = 0;
		wait(0.1);
		state= dir;
		if(dir > 0) _dir = 0;
		else _dir = 1;
		_pwm = speedrate;
	}
}

void Motor::incspeed(double step){
	speedrate += step;
	if(speedrate > 1) speedrate = 1.0;
	_pwm = speedrate;
}

void Motor::decspeed(double step){
	speedrate -= step;
	if(speedrate < 0) speedrate = 0.0;
	_pwm = speedrate;
}