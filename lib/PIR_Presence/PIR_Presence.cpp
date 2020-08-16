#include "Arduino.h"
#include "PIR_Presence.h"

/**
 * The PIR class that is capable of gathering the signals from the sensor
 * and is capable of changing the states related based on those signals.
 * 
 * \authors Maksymilian Kawula, Gabriele Conti
 * \version 1.0
 */
PIR_Presence::PIR_Presence(byte _pirPin) : pirPin(_pirPin) {
    pinMode(pirPin, INPUT);
    pirStat = 0;
    lastPresenceTime = 0;
    presenceInterval = 100000;
}

/**
 * The method that checks the PIR sensor and returns the states of it.
 * Once a presence is detected, it is considered as occupied for the next 10 minutes.
 * 
 * @return Boolean value representing states - occupied or vacant.
 */
bool PIR_Presence::checkSensor() {
    // Reads sensor value HIGH or LOW
	pirStat = digitalRead(pirPin);

	// Checks whether motion is detected (HIGH)
	if (pirStat == HIGH) {
		// Resets the presence time
		lastPresenceTime = millis();
        return true;
	} else {
		if (timeDiff(lastPresenceTime, presenceInterval)) {
			return false;
		}
	}
    return true;
}

/**
 * The method that is used for self-test which checks whether within 10 seconds
 * a presence is detected or not. If none presence is detected within 10 seconds,
 * then false value is returned. On the other hand, as soon as a presence is detected, 
 * true value is returned.
 * 
 * @return Boolean value representing the fact, whether presence is detected or not.
 */
bool PIR_Presence::testSensor() {
    long testTime = millis();

    while (true) {
        pirStat = digitalRead(pirPin);
        if (pirStat == HIGH) {
            return true;
        } else if (timeDiff(testTime, 10000)) {
            return false;
        }
    }
}

boolean PIR_Presence::timeDiff(unsigned long start, int specifiedDelay) {
	return (millis() - start >= specifiedDelay);
}
