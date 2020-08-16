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


//Library import 
#include <SDReader.h>
#include "Arduino.h"
#include "FS.h"
#include "SD.h"
#include <SPI.h>
#include <WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

/**
 * This method will setup the SD Reader, it will attempt to connect to the SD reader, 
 * then connect to the SD card, and finally open the specified txt file. If the first two fail, 
 * it will display that it has failed. If it could not find the specified txt file, it will create one 
 * with the name provided. 
 */
void SDReader::SDSetup(int sdPin){
	SD.begin(sdPin);
	if(!SD.begin(sdPin)) {
	  Serial.println("Card Mount Failed");
	  return;
	}
	uint8_t cardType = SD.cardType();
	if(cardType == CARD_NONE) {
	  Serial.println("No SD card attached");
	  return;
	}
	Serial.println("Initializing SD card...");
	if (!SD.begin(sdPin)) {
	  Serial.println("ERROR - SD card initialization failed!");
	  return; // init failed
	}
	//checks for existance of log file
	File file = SD.open("/data.txt");
	if(!file){
		Serial.println("File doens't exist");
		Serial.println("Creating file...");
		writeFile(SD, "/data.txt", "----Date---|--Hour--|-----Humidity----|----Temperature----|-Presence-  \r\n");
	} else {
		Serial.println("File already exists");
	}
	file.close();
}
/**
 * This method will create our data message by taking in the variables in the main class
 * and will then write it to the specified txt file. 
 * concatenates all the information in the dataMessage String variable.
 */
void SDReader::SDWrite(float hum, float temp, bool presence, String dayTimeStamp){
	dataMessage = String(dayTimeStamp) + ", Humidity: " + String(hum) + ", Temperature: " + String(temp) + ", Presence: " + String(presence) + "\r\n";
	appendFile(SD, "/data.txt", dataMessage.c_str());
}

/**
 * This method will check if data.txt file exist on the microSD card.
 * If it doesn't exist it will create new file.
 * 
 * 
 */
void SDReader::writeFile(fs::FS &fs, const char * path, const char * message) {
  Serial.printf("Writing file: %s\n", path);

  File file = fs.open(path, FILE_WRITE);
  if(!file) {
    Serial.println("Failed to open file for writing");
    return;
  }
  if(file.print(message)) {
    Serial.println("File written");
  } else {
    Serial.println("Write failed");
  }
  file.close();
}

//Appending record to file
void SDReader::appendFile(fs::FS &fs, const char * path, const char * message) {
  Serial.printf("Appending to file: %s\n", path);
	
  File file = fs.open(path, FILE_APPEND);
  if(!file) {
    Serial.println("Failed to open file for appending");
    return;
  }
  if(file.print(message)) {
    Serial.println("Message appended");
  } else {
    Serial.println("Append failed");
  }
  file.close();
}

/**
 * This method connects to NTP client server and downloads current time and date
 * 
 * 
 * 
 */
String SDReader::dayTimeCheck(){
	const char* ssid     = "VM4962733";
	const char* password = "sn7Wvdzychyw";


	// Define NTP Client to get time
	WiFiUDP ntpUDP;
	NTPClient timeClient(ntpUDP);

	// Variables to save date and time
	String formattedDate;
	String dayStamp;
	String timeStamp;
	String dayTimeStamp;

	 // Connect to Wi-Fi network with SSID and password
	  Serial.print("Connecting to ");
	  Serial.println(ssid);
	  WiFi.begin(ssid, password);
	  while (WiFi.status() != WL_CONNECTED) {
	    delay(500);
	    Serial.print(".");
	  }
	  Serial.println("");
	  Serial.println("WiFi connected.");

	  // Initialize a NTPClient to get time
	  timeClient.begin();

	  timeClient.setTimeOffset(3600);

	  while(!timeClient.update()) {
	     timeClient.forceUpdate();
	   }
	   // Format Date
	   formattedDate = timeClient.getFormattedDate();
	   Serial.println(formattedDate);

	   // Extract date
	   int splitT = formattedDate.indexOf("T");
	   dayStamp = formattedDate.substring(0, splitT);
	   Serial.println(dayStamp);
	   // Extract time
	   timeStamp = formattedDate.substring(splitT+1, formattedDate.length()-1);
	   Serial.println(timeStamp);

	   dayTimeStamp = dayStamp +", " +timeStamp;

	   return dayTimeStamp;
}
