/*
 * UartHandler.hpp
 *
 *  Created on: 31.08.2023
 *      Author: Astra
 */

#ifndef INTERFACES_UARTHANDLER_HPP_
#define INTERFACES_UARTHANDLER_HPP_


#include <cstdint>
#include "../helpers/Helper.hpp"
#include <wiringSerial.h>
#include "../helpers/Strings.hpp"
#include <String.h>
#include <unistd.h>

#define BAUDRATE 115200
#define BAUDRATE 115200
#define INIT_CHAR 0

class UartHandler{

public:

	UartHandler();
 	virtual ~UartHandler();
 	int initialUart();
	int closeUart();
	uint8_t *readUartData(int bytes);
 	bool isUartDataAvailable();
	int sendUartData(uint16_t data);
	int getFileDescriptor();
private:

	int uartFd;

};



#endif /* INTERFACES_UARTHANDLER_HPP_ */
