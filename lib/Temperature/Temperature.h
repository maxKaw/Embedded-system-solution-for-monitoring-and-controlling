#ifndef Temperature_h
#define Temperature_h

#include "CurrentRangeStateEnum.h"
#include "Arduino.h"

/**
 * The temperature class is capable of checking the current 
 * readings and checking them against the stated ranges.
 * 
 * \authors Maksymilian Kawula, Gabriele Conti, Matthew Fallon
 * \version 1.0
 */
class Temperature {

	// Temperature green ranges
	byte tempGreenRanges[2] = { 18, 24 };

	// Temperature amber ranges
	byte tempAmberRanges[2] = { 16, 27 };
	float currentTemp = 0;

public:
	Temperature();
	CurrentRangeStateEnum checkRangeTemp(void);
	void setCurrentTemp(float);
	float getCurrentTemp(void);
	void setGreenRanges(byte, byte);
	void setAmberRanges(byte, byte);
};

#endif /* Temperature_h */

