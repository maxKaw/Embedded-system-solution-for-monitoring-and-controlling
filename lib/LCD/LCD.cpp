#include "LCD.h"
#include "Arduino.h"

/**
 * The LCD class is capable of displaying readings and statuses
 * of various components on connected LCD
 * 
 * \authors Maksymilian Kawula, Ewan Porter, Dawid Kocma
 * \version 1.0
 */
LCD::LCD() {

}

/**
 * The method that initializes LCD
 */
void LCD::setLCD() {
	lcd = new LiquidCrystal_I2C(0x27, 16, 2);
	lcd->init();
	lcd->backlight();
	// Runs the display
}

/**
 * The method that is capable of setting the readings sent as parameters
 * for LCD
 */
void LCD::takeReadings(Temperature _temp, Humidity _hum,
		BuzzerStateEnum _buzzerState, CurrentRangeStateEnum _systemState) {
	temp = _temp;
	hum = _hum;
	buzzerState = _buzzerState;
	systemState = _systemState;
	displayReadings();
}

/**
 * The method that is capable of displaying the current readings 
 * and statuses on LCD swapping those sets of information every 2 seconds. 
 * On top of that, it is capable of showing Buzzer related interrupting messages.
 */
void LCD::displayReadings() {
	// Buzzer status - Priority !
	if (timeDiff(lastLogTimeScreen, logIntervalScreen)) {
		if (buzzerState != BuzzerStateEnum::NONE) {
			lcd->clear();
			lcd->setCursor(0, 0);
			lcd->print(F("Buzzer status: "));
			lcd->setCursor(0, 1);
			switch (buzzerState) {
			case BuzzerStateEnum::SNOOZED:
				lcd->print(F("Snoozed"));
				break;
			case BuzzerStateEnum::AMBER_BUZZER:
				lcd->print(F("Amber buzzer"));
				break;
			case BuzzerStateEnum::RED_BUZZER:
				lcd->print(F("Red buzzer"));
				break;
			case BuzzerStateEnum::NONE:
				break;
			}
			if (changeMilisAlert) {
				lastLogTime = millis();
				changeMilisAlert = false;
			}
		} else if (currentState == LCDDisplayStateEnum::READINGS) {
			lcd->clear();
			lcd->setCursor(0, 0);
			lcd->print("Temp: " + String(temp.getCurrentTemp()));
			lcd->setCursor(0, 1);
			lcd->print("Hum: " + String(hum.getCurrentHum()));
		} else if (currentState == LCDDisplayStateEnum::SYSTEM_STATUS) {
			lcd->clear();
			lcd->setCursor(0, 0);
			lcd->print(F("System status: "));
			lcd->setCursor(0, 1);
			switch (systemState) {
			case CurrentRangeStateEnum::GREEN:
				lcd->print(F("Green range"));
				break;
			case CurrentRangeStateEnum::AMBER:
				lcd->print(F("Amber range"));
				break;
			case CurrentRangeStateEnum::RED:
				lcd->print(F("Red range"));
				break;
			}
		}

		// Mechanisms responsible for swapping reading and status messages
		if (timeDiff(lastLogTime, logInterval)) {
			currentState =
					currentState == LCDDisplayStateEnum::READINGS ?
							LCDDisplayStateEnum::SYSTEM_STATUS :
							LCDDisplayStateEnum::READINGS;
			if (buzzerState != BuzzerStateEnum::NONE) {
				buzzerState = BuzzerStateEnum::NONE;
				changeMilisAlert = true;
			}
			lastLogTime = millis();
		}
		lastLogTimeScreen = millis();
	}

}

/**
 * The method used for self-test to show the testing message on connected LCD
 */
void LCD::lcdTest() {
	lcd->clear();
	lcd->setCursor(0, 0);
	lcd->print(F("LCD TEST"));
}

boolean LCD::timeDiff(unsigned long start, int specifiedDelay) {
	return (millis() - start >= specifiedDelay);
}

