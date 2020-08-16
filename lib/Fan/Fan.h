#ifndef Fan_h
#define Fan_h
#include "Arduino.h"
#include "CurrentRangeStateEnum.h"

/**
 * Enum class used to distinguish the current fan's state
 */
enum FanStateEnum {
	AMBER_SPEED, RED_SPEED, OFF_FAN
};

/**
 * The fan class is capable of checking the current 
 * readings and taking actions based on them, such as turning 
 * on and off the fan connected to one of the pin.
 * On top of that, it offers testing the fan.
 * 
 * \authors Maksymilian Kawula, Ewan Porter, Dawid Kocma
 * \version 1.0
 */
class Fan {
private:
	FanStateEnum fanState;
	CurrentRangeStateEnum currentLed = CurrentRangeStateEnum::GREEN;
	CurrentRangeStateEnum oldLed = CurrentRangeStateEnum::GREEN;
	byte fanPin;
	const int freq = 2000;
	const int ledChannel = 0;
	const int resolution = 8;

public:
	explicit Fan(byte _FanPin);
	void checkStatus(CurrentRangeStateEnum currentLed);
	void fanStart(void);
	void fanTestHighSpeed(void);
	void fanTestLowSpeed(void);
	void fanTestStop(void);
};

#endif  /* Fan_h */
