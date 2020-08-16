#ifndef Humidity_h
#define Humidity_h
#include "Arduino.h"
#include "CurrentRangeStateEnum.h"


/**
 * The humidity class is capable of checking the current 
 * readings and checking them against the stated ranges.
 * 
 * \authors Maksymilian Kawula, Gabriele Conti, Matthew Fallon
 * \version 1.0
 */
class Humidity {

	// Humidity green ranges
	byte humGreenRanges[2] = { 35, 45 };
	// Humidity amber ranges
	byte humAmberRanges[2] = { 25, 75 };
	float currentHum;
public:
	Humidity();
	CurrentRangeStateEnum checkRange(void);
	void setCurrentHum(float);
	float getCurrentHum(void);
	void setGreenRanges(byte, byte);
	void setAmberRanges(byte, byte);
};

#endif  /* Humidity_h */
