#ifndef LCD_h
#define LCD_h
#include "Arduino.h"
#include "CurrentRangeStateEnum.h"
#include "Humidity.h"
#include "Temperature.h"
#include <LiquidCrystal_I2C.h>
#include "Buzzer.h"

/**
 * Enum class that distinguishes the states of LCD
 */
enum LCDDisplayStateEnum {
	READINGS, SYSTEM_STATUS
};

/**
 * The LCD class is capable of displaying readings and status
 * of various components on connected LCD
 * 
 * \authors Maksymilian Kawula, Ewan Porter, Dawid Kocma
 * \version 1.0
 */
class LCD {
private:
	LiquidCrystal_I2C* lcd = NULL;
	unsigned long lastLogTime = 0;
	unsigned long lastLogTimeScreen = 0;
	unsigned long logInterval = 2000;
	unsigned long logIntervalScreen = 1000;
	LCDDisplayStateEnum currentState = LCDDisplayStateEnum::READINGS;
	Temperature temp;
	Humidity hum;
	BuzzerStateEnum buzzerState;
	CurrentRangeStateEnum systemState;
	boolean changeMilisAlert = true;
public:
	LCD();
	void setLCD();
	void takeReadings(Temperature temp, Humidity hum,
			BuzzerStateEnum buzzerState, CurrentRangeStateEnum systemState);
	void displayReadings(void);
	void lcdTest(void);
	boolean timeDiff(unsigned long start, int specifiedDelay);
};

#endif /* LCD_h */
