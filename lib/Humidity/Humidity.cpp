#include <Humidity.h>
#include "Arduino.h"

/**
 * The humidity class is capable of checking the current 
 * readings and checking them against the stated ranges.
 * 
 * \authors Maksymilian Kawula, Gabriele Conti, Matthew Fallon
 * \version 1.0
 */
Humidity::Humidity() {
	currentHum = 0;
}

/**
 * Checks the current readings and assigns the correct range state.
 * 
 * @return The range state is returned
 */
CurrentRangeStateEnum Humidity::checkRange() {
	if ((currentHum >= humGreenRanges[0])
			&& (currentHum <= humGreenRanges[1])) {
		return CurrentRangeStateEnum::GREEN;
	} else if ((currentHum >= humAmberRanges[0])
			&& (currentHum <= humAmberRanges[1])) {
		return CurrentRangeStateEnum::AMBER;
	}

	return CurrentRangeStateEnum::RED;
}

void Humidity::setCurrentHum(float _currentHum) {
	currentHum = _currentHum;
}

float Humidity::getCurrentHum() {
	return currentHum;
}

void Humidity::setGreenRanges(byte start, byte end){
	humGreenRanges[0] = start;
	humGreenRanges[1] = end;
}

void Humidity::setAmberRanges(byte start, byte end){
	humGreenRanges[0] = start;
	humGreenRanges[1] = end;
}


