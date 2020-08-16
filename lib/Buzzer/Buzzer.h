#include "Arduino.h"
#include "CurrentRangeStateEnum.h"
#include "Button.h"

#ifndef Buzzer_h
#define Buzzer_h

/**
 * Enum class used to distinguish the current buzzer's state
 */
enum BuzzerStateEnum {
	AMBER_BUZZER, RED_BUZZER, SNOOZED, NONE
};

/**
 * The buzzer class that offers a few functions such as checking the current 
 * readings and taking actions based on them, sounding an audible alert 
 * by the buzzer connected to the specified pin as well as snoozing and testing the buzzer.
 * \author Maksymilian Kawula
 * \version 1.0
 */
class Buzzer {
private:
	unsigned long snoozeInterval = 120000;
	long alertInterval = 1000;
	BuzzerStateEnum buzzerState;
	long amberAlertInterval = 30000;
	long redAlertInterval = 5000;
	long breakInterval = 1000;
	unsigned long lastTimeLog = 0;
	unsigned long lastTimeBuzz = 0;
	CurrentRangeStateEnum currentLed = CurrentRangeStateEnum::GREEN;
	CurrentRangeStateEnum oldLed = CurrentRangeStateEnum::GREEN;
	bool occupied = false;
	byte buzzerPin;
	unsigned long logInterval = 5000;
	Button* button = NULL;
	ButtonStateEnum btnCurrent;
public:
	Buzzer(byte buzzer, byte buttonBuzzerPin);
	BuzzerStateEnum checkStatus(CurrentRangeStateEnum currentLed, bool occupied);
	bool soundBuzzer(void);
	boolean timeDiff(unsigned long start, int specifiedDelay);
	void buzzerTestON(void);
	void buzzerTestOFF(void);
	void checkTheButton(void);
	void checkIfSnoozed(void);
	void checkBuzzer(void);
};

#endif /* Buzzer_h */
