/**
 * Embedded system which operates as an environmental monitoring and control system.
 * It is capable of gathering readings such as temperature and humidity as well as detecting presence.
 * Various actions are performed based on those readings, such as sounding an audible alert, 
 * lighting up LED on different colours and spinning a fan at various speeds.  
 * 
 * \version 1.0
 * \authors Maksymilian Kawula, Gabriele Conti, Matthew Fallon, Dawid Kocma, Ewan Porter
 */


#include "DHT.h"
#include "CurrentRangeStateEnum.h"
#include <Humidity.h>
#include "Temperature.h"
#include "Buzzer.h"
#include "LCD.h"
#include "Fan.h"
#include "PIR_Presence.h"
#include "WiFi.h"
#include <HTTPClient.h>
#include "RGBLedChanger.h"
#include "SDReader.h"
#include "Logger.h"
#include "StoreButton.h"
#include <WiFi.h>
#include <HTTPClient.h>
#include <WiFiType.h>
#include <WString.h>
#include <IPAddress.h>
#include <esp32-hal.h>
#include <esp32-hal-gpio.h>
#include <HardwareSerial.h>

// WIFI credentials
char *SSID = "AndroidAP";
char *PASS = "password";

// --------------------------
// ALL THE PINS VARIABLES OF THE CONNECTED COMPONENTS ARE PLACED BELOW
// --------------------------

// Pin for DHT sensor
DHT dht(04, DHT11);

// Pin for PIR sensor
#define pirPin 23

// Pin for the buzzer
#define buzzerPin 19
// Pin for the buzzer buttona
#define buttonBuzzerPin 12

// Pin for the store button
#define storeButtonPin 25

// Pin for the DC Motor / Fan
const byte fan = 26;
// --------------------------
// --------------------------

// Initial buzzer state
BuzzerStateEnum oldBuzzerState = BuzzerStateEnum::NONE;
BuzzerStateEnum newBuzzerState = BuzzerStateEnum::NONE;

// Initial led state
CurrentRangeStateEnum currentLed = CurrentRangeStateEnum::GREEN;

// Initial PIR status
int pirStat = 0;

//Initialize Remote Logger
Logger logger = Logger();
unsigned long lastLogTime;
unsigned long lastTransmitDataTime;
unsigned long storeInterval = 5000;
unsigned long transmitDataInterval = 30000;
unsigned long oldOutputInterval = 5000;
unsigned long newOutputInterval;

// Initial ranges state
CurrentRangeStateEnum tempCurrentRange = CurrentRangeStateEnum::GREEN;
CurrentRangeStateEnum humCurrentRange = CurrentRangeStateEnum::GREEN;

float currentTemp;
float currentHum;
boolean newStatePresence = false;
boolean oldStatePresence = false;

unsigned long lastPresenceTime;
unsigned long vacantTime = 5000;

unsigned long lastPostTime;

long PIRwaitPeriod = 600000;

//for post requests
String Date;

Humidity humidity;
Temperature temperature;

PIR_Presence PIRsensor(pirPin);

Buzzer buzzer(buzzerPin, buttonBuzzerPin);

Fan fanDC(fan);

LCD lcdDisplay;

RGBLedChanger RGBChanger;

//SD inits
int sdPin = 5;
SDReader reader;
unsigned long lastSDTime;

StoreButton storeButton(storeButtonPin);
unsigned long lastStoreTime = 0;

boolean timeDiff(unsigned long start, unsigned long specifiedDelay)
{
	return (millis() - start >= specifiedDelay);
}
/**
 * This method prints out the current temperature, humidity, buzzer and PIR status every 5/10/60/120/300 seconds.
 * The interval is up to the user, but the default interval is set at 5 seconds.
 * This method also logs every interval change and set of readings sent to the server.
 */
void printStatus()
{
	// This piece of code informs the user about every interval change
	if (oldOutputInterval != newOutputInterval)
	{
		oldOutputInterval = newOutputInterval;
		Serial.print(F("Output interval is now "));
		if (oldOutputInterval == 60000) {
			Serial.println(F("1 minute "));
		} else if (oldOutputInterval > 60000) {
			Serial.printf("%d minutes \n", ((oldOutputInterval / 1000) / 60));
		} else {
			Serial.printf("%d seconds \n", ((oldOutputInterval / 1000)));
		}
	}
	
	if (timeDiff(lastLogTime, oldOutputInterval))
	{
		// This piece of code informs the user about readings sent to the server
		if (timeDiff(lastTransmitDataTime, transmitDataInterval))
		{
			logger.transmitData(temperature.getCurrentTemp(), humidity.getCurrentHum(), oldStatePresence);
			Serial.println(F("Set of readings sent to the server"));
			lastTransmitDataTime = millis();
		}

		Serial.println("--------------------");
		Serial.print(F("Temp is: "));
		Serial.println(String(temperature.getCurrentTemp()));
		Serial.print(F("Hum: "));
		Serial.println(String(humidity.getCurrentHum()));
		if (oldStatePresence)
		{
			Serial.println("Presence Detected");
		}
		else
		{
			Serial.println("Presence not detected");
		}
		Serial.print(F("Buzzer status: "));
		switch (oldBuzzerState)
		{
		case BuzzerStateEnum::SNOOZED:
			Serial.println(F("Snoozed"));
			break;
		case BuzzerStateEnum::AMBER_BUZZER:
			Serial.println(F("Amber buzzer"));
			break;
		case BuzzerStateEnum::RED_BUZZER:
			Serial.println(F("Red buzzer"));
			break;
		case BuzzerStateEnum::NONE:
			Serial.println(F("None"));
			break;
		}
		Serial.println("--------------------");
		lastLogTime = millis();
	}
}
/**
 * This method is responsible for changing the states of LED, Buzzer and PIR. It gathers the current state
 * and check it against the old state for each of the components. It also informs the user about 
 * every state change via the serial monitor.
 */
void logChanges()
{
	if (tempCurrentRange == CurrentRangeStateEnum::GREEN && humCurrentRange == CurrentRangeStateEnum::GREEN)
	{
		// Led set to green
		if (currentLed != CurrentRangeStateEnum::GREEN)
		{
			RGBChanger.setLEDGreen();
			currentLed = CurrentRangeStateEnum::GREEN;
			Serial.println(F("Changed to Green light"));
		}
	}
	else if ((tempCurrentRange == CurrentRangeStateEnum::AMBER && humCurrentRange == CurrentRangeStateEnum::AMBER) || (((tempCurrentRange == CurrentRangeStateEnum::AMBER) && humCurrentRange != CurrentRangeStateEnum::RED) || ((humCurrentRange == CurrentRangeStateEnum::AMBER) && tempCurrentRange != CurrentRangeStateEnum::RED)))
	{
		// Led set to amber
		if (currentLed != CurrentRangeStateEnum::AMBER)
		{
			RGBChanger.setLEDAmber();
			currentLed = CurrentRangeStateEnum::AMBER;
			Serial.println(F("Changed to Amber light"));
		}
	}
	else if ((tempCurrentRange == CurrentRangeStateEnum::RED && humCurrentRange == CurrentRangeStateEnum::RED) || (tempCurrentRange == CurrentRangeStateEnum::RED || humCurrentRange == CurrentRangeStateEnum::RED))
	{
		// Led set to red
		if (currentLed != CurrentRangeStateEnum::RED)
		{
			RGBChanger.setLEDRed();
			currentLed = CurrentRangeStateEnum::RED;
			Serial.println(F("Changed to Red light"));
		}
	}

	// This piece of code informs the user about the PIR status change
	if (oldStatePresence != newStatePresence)
	{
		oldStatePresence = newStatePresence;
		Serial.print(F("Presence state changed to: "));
		if (oldStatePresence)
		{
			Serial.println(F("detected"));
		}
		else
		{
			Serial.println(F("not detected"));
		}
	}

	// This piece of code informs the user about the Buzzer's status change
	if (oldBuzzerState != newBuzzerState)
	{
		oldBuzzerState = newBuzzerState;
		Serial.print(F("Buzzer state changed to: "));
		switch (oldBuzzerState)
		{
		case BuzzerStateEnum::SNOOZED:
			Serial.println(F("Snoozed"));
			break;
		case BuzzerStateEnum::AMBER_BUZZER:
			Serial.println(F("Amber buzzer"));
			break;
		case BuzzerStateEnum::RED_BUZZER:
			Serial.println(F("Red buzzer"));
			break;
		case BuzzerStateEnum::NONE:
			Serial.println(F("None"));
			break;
		}
	}
}

/**
 * This method is responsible for power-on self-tests, so various tests to ensure that
 * all the components are working correctly before the system is started.
 */
void selfTest()
{
	Button *button = new Button(buttonBuzzerPin);
	ButtonStateEnum btnCurrent = ButtonStateEnum::OFF;
	RGBChanger.setLEDRed();
	Serial.println("-- Tap the button if red LED does work.");
	while (true)
	{
		ButtonStateEnum btnRead = button->checkState();
		if (btnRead != btnCurrent)
		{
			btnCurrent = btnRead;
			if (btnCurrent == ButtonStateEnum::ON)
				break;
		}
	}

	RGBChanger.setLEDAmber();
	Serial.println("-- Tap the button if amber LED does work.");
	while (true)
	{
		ButtonStateEnum btnRead = button->checkState();
		if (btnRead != btnCurrent)
		{
			btnCurrent = btnRead;
			if (btnCurrent == ButtonStateEnum::ON)
				break;
		}
	}

	RGBChanger.setLEDGreen();
	Serial.println("-- Tap the button if green LED does work.");
	while (true)
	{
		ButtonStateEnum btnRead = button->checkState();
		if (btnRead != btnCurrent)
		{
			btnCurrent = btnRead;
			if (btnCurrent == ButtonStateEnum::ON)
				break;
		}
	}

	buzzer.buzzerTestON();
	Serial.println("-- Tap the button if the buzzer does work");
	while (true)
	{
		ButtonStateEnum btnRead = button->checkState();
		if (btnRead != btnCurrent)
		{
			btnCurrent = btnRead;
			if (btnCurrent == ButtonStateEnum::ON)
				break;
		}
	}
	buzzer.buzzerTestOFF();

	Serial.println("-- Tap the button if LCD displays 'LCD TEST'");
	lcdDisplay.lcdTest();
	while (true)
	{
		ButtonStateEnum btnRead = button->checkState();
		if (btnRead != btnCurrent)
		{
			btnCurrent = btnRead;
			if (btnCurrent == ButtonStateEnum::ON)
				break;
		}
	}

	Serial.println("-- Make a movement in front of the PIR sensor within 10 seconds");
	if (PIRsensor.testSensor())
	{
		Serial.println("-- PIR sensor is fully working");
	}
	else
	{
		Serial.println("-- PIR sensor is not working");
	}

	Serial.println(
		"-- Tap the button if the fan is spinning with the high speed");
	fanDC.fanTestHighSpeed();
	while (true)
	{
		ButtonStateEnum btnRead = button->checkState();
		if (btnRead != btnCurrent)
		{
			btnCurrent = btnRead;
			if (btnCurrent == ButtonStateEnum::ON)
				break;
		}
	}

	Serial.println(
		"-- Tap the button if the fan is spinning with the low speed");
	fanDC.fanTestLowSpeed();
	while (true)
	{
		ButtonStateEnum btnRead = button->checkState();
		if (btnRead != btnCurrent)
		{
			btnCurrent = btnRead;
			if (btnCurrent == ButtonStateEnum::ON)
				break;
		}
	}

	Serial.println("-- Tap the button if the fan is not spinning anymore");
	fanDC.fanTestStop();
	while (true)
	{
		ButtonStateEnum btnRead = button->checkState();
		if (btnRead != btnCurrent)
		{
			btnCurrent = btnRead;
			if (btnCurrent == ButtonStateEnum::ON)
				break;
		}
	}

	Serial.println("-- All the tests finished successfully!");
}

/**
 * The method that is run once at the very begginning 
 * and sets all the initialise connected components.
 */
void setup()
{
	// Initialising WIF connection
	Serial.print("Connecting to ");
	Serial.println(SSID);
	WiFi.begin(SSID, PASS);

	while (WiFi.status() != WL_CONNECTED)
	{
		delay(250);
		Serial.println(".");
	}

	Serial.println("Connected to: ");
	Serial.println(WiFi.localIP());

	// Initialising  Serial monitor
	Serial.begin(115200);
	Serial.println("");
	Serial.println(F("Serial monitor test"));

	// Initialising DHT sensor
	dht.begin();
	Serial.println(F("Temperature/humanity sensor is initialised"));

	// Checking DHT sensor
	if ((isnan(dht.readHumidity()) || isnan(dht.readTemperature())) || (isnan(dht.readHumidity()) && isnan(dht.readTemperature())))
	{
		Serial.println(F("Failed to read from DHT sensor!"));
	}
	else
	{
		Serial.println(F("DHT sensor is fully working"));
	}

	// Initialising PIR sensor
	pinMode(pirPin, INPUT);
	Serial.println(F("PIR sensor is initialised"));

	// Initialising LCD
	lcdDisplay.setLCD();
	Serial.println(F("LCD display is initialised"));

	// Initialising store button
	long intervals[6] = {5000, 10000, 30000, 60000, 120000, 300000};
	storeButton.setIntervals(intervals);
	Serial.println(F("Store button initialised"));


	//SD Setup
	reader.SDSetup(sdPin);

		WiFi.begin(SSID, PASS);

			while (WiFi.status() != WL_CONNECTED) {
				delay(250);
				Serial.println(".");
			}
	
			Serial.println("Connected to: ");
			Serial.println(WiFi.localIP());
	
	lastLogTime = millis();
	lastStoreTime = millis();
	lastSDTime = millis();

	// Runs the test of all the components connected to MCU
	selfTest();
}

/**
 * The method responsible for getting and storing the reading from the sensors
 * at the interval rate set by the user
 */
void storeReadings()
{
	if (timeDiff(lastStoreTime, storeInterval))
	{
		// Checks current humidity
		humidity.setCurrentHum(dht.readHumidity());
		// Checks the range for humidity and stores it
		humCurrentRange = humidity.checkRange();

		// Checks current temperature
		temperature.setCurrentTemp(dht.readTemperature());
		// Checks the range for temperature and stores it
		tempCurrentRange = temperature.checkRangeTemp();

		lastStoreTime = millis();
	}
}

/**
 * The method that is responsible for cyclic executive
 */
void loop()
{
	storeReadings();
	
	newBuzzerState = buzzer.checkStatus(currentLed, oldStatePresence);

	// Checks the PIR sensor
	newStatePresence = PIRsensor.checkSensor();

	// Sends the current LED status to the fan
	fanDC.checkStatus(currentLed);

	logChanges();

	printStatus();

	// Checks the interval button
	newOutputInterval = storeButton.checkInterval();

	// Sends readings to the LCD
	lcdDisplay.takeReadings(temperature, humidity, oldBuzzerState, currentLed);

	// Intrerval for SD Card reader
		if(timeDiff(lastSDTime, 12000)){
			reader.SDWrite(humidity.getCurrentHum(), temperature.getCurrentTemp(), newStatePresence, reader.dayTimeCheck());
			lastSDTime = millis();
		}
}