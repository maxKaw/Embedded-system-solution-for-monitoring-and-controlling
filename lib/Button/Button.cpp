#include "Button.h"
#include <Arduino.h>

/**
 * The button class that is capable of checking whether the button 
 * has been pressed and stores its status together with GPIO pin connected to
 * \author Maksymilian Kawula
 * \version 1.0
 * 
 * @param pin - GPIO pin of a connected button
 */
Button::Button(byte pin) {
	this->pin = pin;
	pinMode(this->pin, INPUT);
	this->lastChange = 0;
}

/**
 * Method that checks state of the button and manages bouncing
 */
ButtonStateEnum Button::checkState() {
	ButtonStateEnum now = ButtonStateEnum::OFF;
	if (digitalRead(pin))
		now = ButtonStateEnum::ON;

	if (now != bouncedState) {
		lastChange = millis();
		bouncedState = now;
	}

	if ((millis() - lastChange) >= BOUNCE_DELAY_MS)
		debouncedState = now;

	return debouncedState;
}

/**
 * Method that checks state of the button, manages bouncing and ensures it is held down for 1 second
 */
ButtonStateEnum Button::checkStateHeld1Second() {
	ButtonStateEnum now = ButtonStateEnum::OFF;
	if (digitalRead(pin)){
		now = ButtonStateEnum::ON;
	}

	if (now != bouncedState) {
		lastChange = millis();
		bouncedState = now;
	}

	if ((millis() - lastChange) >= BOUNCE_DELAY_MS) {
		if(now == ButtonStateEnum::OFF) {
			debouncedState = now;
		} else {
			if ((millis() - lastChange) >= BOUNCE_DELAY_MS_HELD_DOWN)
				debouncedState = now;
		}
	}
		

	return debouncedState;
}
