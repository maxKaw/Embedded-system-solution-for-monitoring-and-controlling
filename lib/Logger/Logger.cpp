#include "../Logger/Logger.h"
#include <HTTPClient.h>
/**
 * The logger class that is capable of sending the current readings 
 * and statuses to the server
 * 
 * \authors Matthew Fallon, Gabriele Conti
 * \version 1.0
 */

Logger::Logger() {

}

/**
 * The method that establishes the connection with the server 
 * and transmits the data.
 * 
 * @param temp - Current temperature reading.
 * @param hum - Current humidity reading.
 * @param presence - Current presence reading from PIR sensor.
 */
void Logger::transmitData(float temp, float hum, bool presence) {
	if (WiFi.status() == WL_CONNECTED) {   //Check WiFi connection status
		HTTPClient http;
		String id1 = "hum";
		String id2 = "temp";
		String id3 = "pres";
		String id4 = "date";
		float val1 = hum;
		float val2 = temp;
		String val3 = String(presence);
		String val4 = Date;

		//	change server location
		String url = "https://simpl3daveftp.com/server.php?";

		url.concat(id1);
		url.concat("=");
		url.concat(val1);
		url.concat("&");
		url.concat(id2);
		url.concat("=");
		url.concat(val2);
		url.concat("&");
		url.concat(id3);
		url.concat("=");
		url.concat(val3);

		const char* headers[] = { "Date" };


		http.begin(url);  //Specify destination for HTTP request
		http.collectHeaders(headers, 1);
		http.setTimeout(10000);
		int httpResponseCode = http.GET();   // Sends the actual GET request

		if (httpResponseCode > 0) {
			Serial.println("Response Date = " + http.header("Date"));
			Date = http.header("Date");
			lastPostTime = millis();
		} else {
			Serial.print("Error on sending POST: ");
			Serial.println(httpResponseCode);
		}
		http.end();  //Free resources
	} else {
//		Serial.println("Error in WiFi connection");
	}
}
