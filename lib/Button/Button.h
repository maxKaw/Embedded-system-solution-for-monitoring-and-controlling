#include <Arduino.h>

#ifndef Button_h
#define Button_h

enum class ButtonStateEnum {
	ON, OFF
};

/**
 * The button class that is capable of checking whether the button 
 * has been pressed and stores its status together with GPIO pin connected to
 * \author Maksymilian Kawula
 * \version 1.0
 */
class Button {
	const int BOUNCE_DELAY_MS = 5;
	const int BOUNCE_DELAY_MS_HELD_DOWN = 1000;
	int pin;
	ButtonStateEnum debouncedState = ButtonStateEnum::OFF;
	ButtonStateEnum bouncedState = ButtonStateEnum::OFF;
	long lastChange;
public:
	Button(byte pin);
	ButtonStateEnum checkState();
	ButtonStateEnum checkStateHeld1Second();
};

#endif /* Button_h */
