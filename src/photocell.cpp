#include <Arduino.h>
#include <ArduinoJson.h>
#include <ATClient.h>
#include <SoftwareSerial.h>
#include <ESP8266ATClient.h>
#include "DebugUtils.h"

SoftwareSerial espSerial(2, 3);  // RX, TX

#define DEST_HOST "things.ubidots.com"
#define HOST_PORT "80"
#define AUTH_TOKEN "hw6av3k44D6BB6CfQtu4xVDZb8k3fJ2vNohTt4fWnBxN7oNMZw8bL6l7C9vI"

ATClient atclient(espSerial);
ESP8266ATClient esp8266(atclient);
int counter = 0;

void setup(void) {
	Serial.begin(38400);

	espSerial.begin(9600);

	esp8266.execATRST();
	esp8266.execATGMR();

	Serial.println( "Configure ESP8266 as Station mode with multiple connections");
	atclient.sendAT("AT+CWMODE=1", "OK", HALT);    // Station mode
	atclient.sendAT("AT+CIPMUX=1", "OK", HALT); // Allow multiple connections (we'll only use the first).

	//connect to the wifi
	boolean connectionEstablished = false;
	while (!connectionEstablished) {
		String cmd = "AT+CWJAP=\"";
		cmd += "SFR_1ED8";
		cmd += "\",\"";
		cmd += "morkbilemisc4athrocr";
		cmd += "\"";
		if (atclient.sendAT(cmd, "OK")) // Join Access Point
				{
			Serial.println("Connected to WiFi.");
			connectionEstablished = true;
		} else {
			Serial.println("Connection to WiFi failed.");
		}

		delay(1000);
	}

	delay(2000);

	atclient.sendAT("AT+CIPSTA_CUR?", "OK");

	delay(10000);
}

void loop(void) {
	  StaticJsonBuffer<200> jsonBuffer;

	  JsonObject& root = jsonBuffer.createObject();
	  root["value"] = counter++;

	  //root.prettyPrintTo(Serial);

	  //Serial.println("");
	  // Establish TCP connection
	  String cmd = "AT+CIPSTART=0,\"TCP\",\""; cmd += DEST_HOST; cmd += "\","; cmd += HOST_PORT;
	  if (!atclient.sendAT(cmd, "OK"))
	  {
	    if (!atclient.sendAT(cmd, "ALREADY CONNECTED"))
	    {
	      return;
	    }
	  }

	  // Get connection status
	  if (!atclient.sendAT("AT+CIPSTATUS", "OK", CONTINUE)) return;

	  // convert json object into string
	  char buffer[root.measureLength()+1];
	  root.printTo(buffer, sizeof(buffer));
	  String json(buffer);

	  // Build HTTP request.
	  cmd = "POST /api/v1.6/variables/568bc4b676254232a0ac2a9f/values HTTP/1.1\r\n";
	  cmd += "Host: "; cmd += DEST_HOST; cmd += ":"; cmd += HOST_PORT; cmd += "\r\n";
	  cmd += "X-Auth-Token: "; cmd += AUTH_TOKEN; cmd += "\r\n";
	  cmd += "Content-Length: "; cmd += root.measureLength(); cmd += "\r\n";
	  cmd += "Content-Type: application/json\r\n";
	  cmd += "\r\n";
	  cmd += json;


	  // Ready the module to receive raw data
	  if (!atclient.sendAT("AT+CIPSEND=0,"+String(cmd.length()), ">"))
	  {
		atclient.sendAT("AT+CIPCLOSE=0", "");
	    Serial.println("Connection timeout.");
	    return;
	  }
	  // Send the raw HTTP request
	  atclient.queryAT(cmd, "OK","ERROR");  // POST

	  // Loop forever echoing data received from destination server.
	  while (espSerial.available())
	  {
	    Serial.write(espSerial.read());
	  }
	  Serial.println();

	  atclient.sendAT("AT+CIPCLOSE=0", "OK");
}


