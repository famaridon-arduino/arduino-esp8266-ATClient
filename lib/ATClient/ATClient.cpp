/*
 * ATClient.cpp
 *
 *  Created on: 5 janv. 2016
 *      Author: famaridon
 */
#include "ATClient.h"

#define DEBUG
#define DEBUG_TRACE
#include "DebugUtils.h"


ATClient::ATClient(Stream &uart, const String atend, const int timeout): stream(&uart), AT_END(atend), TIMEOUT(timeout)
{
}

boolean ATClient::sendAT(String cmd, String ack, ErrorHandel haltOnFail)
{

	stream->println(cmd);
	#ifdef DEBUG_TRACE
	String message = "sendAT( "; message += cmd; message += " , "; message += ack; message += " , "; message += haltOnFail; message += " )";
	DEBUG_PRINTLN(message);
	#endif

	// If no ack response specified, skip all available module output.
	if (ack && !findAck(ack))
		if (haltOnFail == HALT)
			fatalError(cmd + " failed");          // Critical failure halt.
		else
			return false;            // Let the caller handle it.
	return true;                   // ack blank or ack found
}

String ATClient::queryAT(const String &cmd, String ack,String errorCode)
{
	stream->println(cmd);
	#ifdef DEBUG_TRACE
	String message = "queryAT( "; message += cmd; message += " , "; message += ack;message += " , "; message += errorCode; message += " )";
	DEBUG_PRINTLN(message);
	#endif

	if(!ack || ack == "")
	{
		return errorCode;
	}

	return readToAck(ack, errorCode);                   // ack blank or ack found
}

// Send a command to the module and wait for acknowledgement string
// (or flush module output if no ack specified).
// Echoes all data received to the serial monitor.

// Print error message and loop stop.
void ATClient::fatalError(String msg) {
	DEBUG_PRINT("HALT " + msg);
	while (true) {
	};
}

String ATClient::readToAck(String ack, String errorCode){
	String result = "";
	byte current_char = 0;
	byte keyword_length = ack.length();

	// Fail if the target string has not been sent by deadline.
	long deadline = millis() + TIMEOUT;
	while (millis() < deadline) {
		if (stream->available()) {
			char ch = stream->read();
			result += ch;
			DEBUG_PRINT(ch);
			if (result.endsWith(ack))
			{
				return result.substring(0,result.length()-ack.length());
			}
			else if (result.endsWith(errorCode))
			{
				return errorCode;
			}
		}
	}
	DEBUG_PRINTLN("TIMEOUT");
	return "TIMEOUT";  // Timed out
}

// Read characters from WiFi module and echo to serial until keyword occurs or timeout.
boolean ATClient::findAck(String ack) {
	byte current_char = 0;
	byte keyword_length = ack.length();

	// Fail if the target string has not been sent by deadline.
	long deadline = millis() + TIMEOUT;
	while (millis() < deadline) {
		if (stream->available()) {
			char ch = stream->read();
			DEBUG_PRINT(ch);
			if (ch == ack[current_char])
				if (++current_char == keyword_length) {
					return true;
				}
		}
	}
	DEBUG_PRINTLN("TIMEOUT");
	return false;  // Timed out
}
