/*
 * ESP8266ATClient.h
 *
 *  Created on: 6 janv. 2016
 *      Author: famaridon
 */

#include "ESP8266ATClient.h"

/*
 * Constuctor.
 *
 * @param atClient - an reference of ATClient object.
 */
ESP8266ATClient::ESP8266ATClient(ATClient &atClient) :
		atClient(&atClient) {
}

// BASIC

bool ESP8266ATClient::execAT() {
	return atClient->sendAT("AT", "OK");
}

bool ESP8266ATClient::execATRST() {
	return atClient->sendAT("AT+RST", "ready");
}

String ESP8266ATClient::execATGMR() {
	return atClient->queryAT("AT+GMR", "OK", "ERROR");
}

String ESP8266ATClient::execATCWLAP() {
	return atClient->queryAT("AT+CWLAP", "OK", "ERROR");
}

/*
 bool setATGSLP(int time);
 bool setATE(Echo mode);
 bool execATRESTORE();
 bool setATUART(int baudeRate, int dataBits, int stopBits, Parity parity, FlowControl flowControl, FlashMode flash = CUR);
 SleepMode queryATSLEEP();
 bool setATSLEEP(SleepMode sleepMode);
 bool setATRFPOWER(int power);
 String queryRFVDD();
 bool setRFVDD(int powerVoltage);
 bool execRFVDD();


 WiFiMode queryATCWMODE(FlashMode flash = CUR);
 bool setATCWMODE(WiFiMode mode, FlashMode flash = CUR);
 String queryATCWJAP(FlashMode flash = CUR);
 bool setATCWJAP(String ssid, String pwd,FlashMode flash = CUR);
 bool setATLAP();
 String execATLAP();

 bool sATCWSAP(String ssid, String pwd, uint8_t chl, uint8_t ecn);
 bool eATCWLIF(String &list);

 bool eATCIPSTATUS(String &list);
 bool sATCIPSTARTSingle(String type, String addr, uint32_t port);
 bool sATCIPSTARTMultiple(uint8_t mux_id, String type, String addr, uint32_t port);
 bool sATCIPSENDSingle(const uint8_t *buffer, uint32_t len);
 bool sATCIPSENDMultiple(uint8_t mux_id, const uint8_t *buffer, uint32_t len);
 bool sATCIPCLOSEMulitple(uint8_t mux_id);
 bool eATCIPCLOSESingle(void);
 bool eATCIFSR(String &list);
 bool sATCIPMUX(uint8_t mode);
 bool sATCIPSERVER(uint8_t mode, uint32_t port = 333);
 bool sATCIPSTO(uint32_t timeout);
 */
