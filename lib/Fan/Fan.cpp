#include "Fan.h"
#include "Arduino.h"
#include "CurrentRangeStateEnum.h"

/**
 * The fan class is capable of checking the current 
 * readings and taking actions based on them, such as turning 
 * on and off the fan connected to one of the pin.
 * On top of that, it offers testing the fan.
 * 
 * \authors Maksymilian Kawula, Ewan Porter, Dawid Kocma
 * \version 1.0
 */
Fan::Fan(byte _FanPin) : fanPin(_FanPin) {

	ledcSetup(ledChannel, freq, resolution);
	ledcAttachPin(fanPin, ledChannel);
	fanState = FanStateEnum::OFF_FAN;
}

/**
 * The method that changes the status of the Fan
 * based on the state of LED sent as an parameter
 * 
 * @param _currentLed - The current LED's state
 */
void Fan::checkStatus(CurrentRangeStateEnum _currentLed) {
	currentLed = _currentLed;

	if (currentLed == CurrentRangeStateEnum::AMBER) {
		ledcWrite(ledChannel, 100);

		if (currentLed != oldLed) {
			oldLed = currentLed;
			fanState = FanStateEnum::AMBER_SPEED;
		}
	} else if (currentLed == CurrentRangeStateEnum::RED) {
		ledcWrite(ledChannel, 255);

		if (currentLed != oldLed) {
			oldLed = currentLed;
			fanState = FanStateEnum::RED_SPEED;

		}
	} else if (currentLed == CurrentRangeStateEnum::GREEN) {
		ledcWrite(ledChannel, 0);

		if (currentLed != oldLed) {
			oldLed = currentLed;
			fanState = FanStateEnum::OFF_FAN;
		}
	}
}

/**
 * The method used for self-test to turn on the fan with the high speed of spinning
 */
void Fan::fanTestHighSpeed() {
	ledcWrite(ledChannel, 255);
}

/**
 * The method used for self-test to turn on the fan with the low speed of spinning
 */
void Fan::fanTestLowSpeed() {
	ledcWrite(ledChannel, 100);
}

/**
 * The method used for self-test to turn off the fan
 */
void Fan::fanTestStop() {
	ledcWrite(ledChannel, 0);
}
