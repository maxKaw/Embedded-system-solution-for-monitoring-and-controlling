#include "Temperature.h"
#include "Arduino.h"
#include "CurrentRangeStateEnum.h"

/**
 * The temperature class is capable of checking the current 
 * readings and checking them against the stated ranges.
 * 
 * \authors Maksymilian Kawula, Gabriele Conti, Matthew Fallon
 * \version 1.0
 */
Temperature::Temperature() {

}

/**
 * Checks the current readings and assigns the correct range state.
 * 
 * @return The range state is returned
 */
CurrentRangeStateEnum Temperature::checkRangeTemp(void) {
	if ((currentTemp >= tempGreenRanges[0])
			&& (currentTemp <= tempGreenRanges[1])) {
		return CurrentRangeStateEnum::GREEN;
	} else if ((currentTemp >= tempAmberRanges[0])
			&& (currentTemp <= tempAmberRanges[1])) {
		return CurrentRangeStateEnum::AMBER;
	}

	return CurrentRangeStateEnum::RED;
}

float Temperature::getCurrentTemp(void) {
	return currentTemp;
}

void Temperature::setCurrentTemp(float _currnetTemp) {
	currentTemp = _currnetTemp;
}

void Temperature::setGreenRanges(byte start, byte end){
	tempGreenRanges[0] = start;
	tempGreenRanges[1] = end;
}

void Temperature::setAmberRanges(byte start, byte end){
	tempGreenRanges[0] = start;
	tempGreenRanges[1] = end;
}


