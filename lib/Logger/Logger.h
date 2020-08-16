#include <Arduino.h>

#ifndef LIBRARIES_SERVERLOGGING_LOGGER_H_
#define LIBRARIES_SERVERLOGGING_LOGGER_H_


/**
 * The logger class that is capable of sending the current readings 
 * and statuses to the server
 * 
 * \authors Matthew Fallon, Gabriele Conti
 * \version 1.0
 */
class Logger {
private:
	unsigned long lastPostTime;
	String Date;
public:
	Logger();
	void transmitData(float, float, bool);
};

#endif /* LIBRARIES_SERVERLOGGING_LOGGER_H_ */
