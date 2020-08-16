#include "Buzzer.h"
#include "Arduino.h"

/**
 * The buzzer class that offers a few functions such as checking the current 
 * readings and taking actions based on them, sounding an audible alert 
 * by the buzzer connected to the specified pin as well as snoozing and testing the buzzer.
 * \author Maksymilian Kawula
 * \version 1.0
 * 
 * @param _buzzerPin - GPIO pin of a connected buzzer
 * @param buttonBuzzerPin - GPIO pin of a connected button that is responsible
 * for snoozing the buzzer
 */
Buzzer::Buzzer(byte _buzzerPin, byte buttonBuzzerPin) : buzzerPin(_buzzerPin)
{

	// Initialising the button
	button = new Button(buttonBuzzerPin);

	pinMode(buzzerPin, OUTPUT);
	lastTimeLog = millis();
	lastTimeBuzz = millis();
	btnCurrent = ButtonStateEnum::OFF;
	buzzerState = BuzzerStateEnum::NONE;
}

/**
 * The method that checks the status of the LED and changes the status of 
 * the buzzer together with checking the status of the button.
 * It also manages the button that potentially snoozes the buzzer.
 * 
 * @param _currentLed - The current status of the LED
 * @param _occupied - The current status of PIR sensor
 */
BuzzerStateEnum Buzzer::checkStatus(CurrentRangeStateEnum _currentLed, bool _occupied)
{
	currentLed = _currentLed;
	occupied = _occupied;

	if (currentLed == CurrentRangeStateEnum::AMBER && occupied)
	{
		checkIfSnoozed();
		if (currentLed != oldLed)
		{
			oldLed = currentLed;
			buzzerState = BuzzerStateEnum::AMBER_BUZZER;
		}
	}
	else if (currentLed == CurrentRangeStateEnum::RED && occupied)
	{
		checkIfSnoozed();
		if (currentLed != oldLed)
		{
			oldLed = currentLed;
			buzzerState = BuzzerStateEnum::RED_BUZZER;
		}
	}
	else
	{
		checkBuzzer();
		buzzerState = BuzzerStateEnum::NONE;
	}

	checkTheButton();

	return buzzerState;
}

/**
 * The method that checks whether the button has been pushed
 * to snooze the buzzer.
 */
void Buzzer::checkTheButton()
{
	ButtonStateEnum btnRead = button->checkState();
	if (btnRead != btnCurrent)
	{
		btnCurrent = btnRead;
		switch (btnCurrent)
		{
		case ButtonStateEnum::ON:
			buzzerState = BuzzerStateEnum::SNOOZED;
			checkBuzzer();
			lastTimeLog = millis();
			break;
		case ButtonStateEnum::OFF:
			break;
		}
	}
}

/**
* The method that checks whether the buzzer is snoozed; 
* if it's not, then an appropriate action is taken
 */
void Buzzer::checkIfSnoozed()
{
	if (buzzerState == BuzzerStateEnum::SNOOZED)
	{
		if (timeDiff(lastTimeLog, snoozeInterval))
		{
			buzzerState = BuzzerStateEnum::NONE;
		}
	}
	else
	{
		if (currentLed == CurrentRangeStateEnum::AMBER && occupied)
		{
			if (timeDiff(lastTimeLog, amberAlertInterval))
			{
				if (soundBuzzer() == false)
					lastTimeLog = millis();
			}
		}
		else if (currentLed == CurrentRangeStateEnum::RED && occupied)
		{
			if (timeDiff(lastTimeLog, redAlertInterval))
			{
				if (soundBuzzer() == false)
					lastTimeLog = millis();
			}
		}
	}
}

/**
 * This method ensure that buzzer is off.
 */
void Buzzer::checkBuzzer() {
	digitalWrite(buzzerPin, 0);
}

/**
 * The method that sounds an audible alarm with the connected buzzer every 1 second.
 * 
 * @return Boolean value to state whether the buzzer sounds an audible alert or not.
 */
bool Buzzer::soundBuzzer()
{
	byte buzzer = 1;
	if (timeDiff(lastTimeBuzz, breakInterval)) {
		buzzer = digitalRead(buzzerPin) ^ 1;
		digitalWrite(buzzerPin, buzzer);
		lastTimeBuzz = millis();
	}
	return buzzer;
}

/**
 * The method used for self-test to sound an audiable alert
 */
void Buzzer::buzzerTestON()
{
	digitalWrite(buzzerPin, HIGH);
}

/**
 * The method used for self-test to snooze the buzzer
 */
void Buzzer::buzzerTestOFF()
{
	digitalWrite(buzzerPin, LOW);
}

boolean Buzzer::timeDiff(unsigned long start, int specifiedDelay)
{
	return (millis() - start >= specifiedDelay);
}
