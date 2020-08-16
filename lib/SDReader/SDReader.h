/**
 * The SD Reader class allows for the control of our SD reader. 
* This class is capable of testing the SD reader, finding if an SD card
* is inserted, or if the wanted file is available. We can then create the
* file if needed. The class will also connect to an online server to get an accurate
* date/time stamp for saving to the SD file.

 *  \authors Dawid Kocma, Ewan Porter
 *  \version 1.0
 * 
 * 
 * ref: https://randomnerdtutorials.com/esp32-data-logging-temperature-to-microsd-card/
 */

#ifndef SDReader_h
#define SDReader_h
#include "Arduino.h"
#include "FS.h"
#include "SD.h"
#include <SPI.h>

class SDReader {

	String dataMessage;

public:
	void SDSetup(int sdPin);
	void SDWrite(float hum, float temp, bool presence, String dayTimeStamp);
	void writeFile(fs::FS &fs, const char * path, const char * message);
	void appendFile(fs::FS &fs, const char * path, const char * message);
	String dayTimeCheck();
};

#endif
