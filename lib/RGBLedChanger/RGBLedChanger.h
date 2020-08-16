#ifndef RGBLedChanger_h
#define RGBLedChanger_h
#include "Arduino.h"

/**
* The RGB Led Changer class allows for the control of a 
* RGB LED, taking certain pins from the LED and setting
* their frequency to allow for differing colours to 
* represent the colours we want.
*
* \authors Ewan Porter, Dawid Kocma
* \version 1.0
*/
class RGBLedChanger {

	double frequency = 5000;
	char cycle = 8;
	char channelRed = 0;
	char channelGreen = 1;
	char channelBlue = 2;

public:
	void LEDSetup(int redPin, int greenPin, int bluePin);
	void setLEDRed();
	void setLEDGreen();
	void setLEDAmber();
};

#endif /* RGBLedChanger_h */
