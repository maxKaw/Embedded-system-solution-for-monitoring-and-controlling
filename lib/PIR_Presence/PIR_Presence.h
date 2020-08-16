#ifndef PIR_Presence_h
#define PIR_Presence_h
#include "Arduino.h"

/**
 * The PIR class that is capable of gathering the signals from the sensor
 * and is capable of changing the states related based on those signals
 * 
 * \authors Maksymilian Kawula, Gabriele Conti
 * \version 1.0
 */
class PIR_Presence {
private:
    byte pirPin;
    byte pirStat;
    unsigned long lastPresenceTime;
    long presenceInterval;
public:
    explicit PIR_Presence(byte pirPin);
	bool checkSensor(void);
    boolean timeDiff(unsigned long start, int specifiedDelay);
    bool testSensor(void);
};

#endif
