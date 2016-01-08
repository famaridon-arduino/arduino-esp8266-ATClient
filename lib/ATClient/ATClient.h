/*
 * ATClient.h
 *
 *  Created on: 5 janv. 2016
 *      Author: famaridon
 */

#ifndef LIB_ATCLIENT_ATCLIENT_H_
#define LIB_ATCLIENT_ATCLIENT_H_

#include "Stream.h"
#include <Arduino.h>

enum ErrorHandel {
	HALT, CONTINUE
};

class ATClient {

public:

	/**
	 * Constuctor.
	 *
	 * @param uart - an reference of Stream object.
	 * @param atend - at command end chars
	 * @param timeout - how many time we wait for ACK
	 *
	 * @warning parameter baud depends on the AT firmware. 9600 is an common value.
	 */
	ATClient(Stream &uart, String atend = "\n\r", int timeout = 10000);

	/**
	 * send an AT command and wait for ack
	 *
	 * @param cmd command to execute
	 * @param ack to wait (null if you don't need to wait for ack)
	 * @param haltOnFail if ack is not found CONTINUE simply return false HALT block on infinit loop.
	 */
	boolean sendAT(String cmd, String ack, ErrorHandel haltOnFail = CONTINUE);

	/**
		 * send an AT command and read result while ack or errorCode is not found
		 *
		 * @param cmd command to execute
		 * @param read while ack is not found
		 * @param errorCode read while errorCode is not found
		 *
		 * @return result if ack is found. errorCode if errorCode is found. "TIMEOUT" if query is too long
		 */
	String queryAT(const String &cmd, String ack, String errorCode);

private:
	boolean findAck(String ack);
	String readToAck(String ack, String errorCode);
	void fatalError(String msg);

	Stream *stream;
	const String AT_END;
	const int TIMEOUT;
};

#endif /* LIB_ATCLIENT_ATCLIENT_H_ */
