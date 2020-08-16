#include <RGBLedChanger.h>
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

/**
 * This method will setup the RGB LED with the specified pins and attach them 
 * to specific channels to allow for frequency writing later.
 */
void RGBLedChanger::LEDSetup(int redPin, int greenPin, int bluePin){
	ledcSetup(channelRed, frequency, cycle);
	ledcAttachPin(redPin, channelRed);
	ledcSetup(channelGreen, frequency, cycle);
	ledcAttachPin(greenPin, channelGreen);
	ledcSetup(channelBlue, frequency, cycle);
	ledcAttachPin(bluePin, channelBlue);
}

/**
 * This method will write to the LED with the RGB values for each pin
 * to make the LED display as red.
 */
void RGBLedChanger::setLEDRed() {
	ledcWrite(channelRed, 255);
	ledcWrite(channelGreen, 1);
	ledcWrite(channelBlue, 2);
}

/**
 * This method will write to the LED with the RGB values for each pin
 * to make the LED display as green.
 */
void RGBLedChanger::setLEDGreen() {
	ledcWrite(channelRed, 1);
	ledcWrite(channelGreen, 255);
	ledcWrite(channelBlue, 10);
}

/**
 * This method will write to the LED with the RGB values for each pin
 * to make the LED display as amber.
 */
void RGBLedChanger::setLEDAmber() {
	ledcWrite(channelRed, 255);
	ledcWrite(channelGreen, 50);
	ledcWrite(channelBlue, 1);
}
