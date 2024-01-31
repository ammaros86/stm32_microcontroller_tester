/*
 * UsbPortHandler.hpp
 *
 *  Created on: 31.08.2023
 *      Author: Astra
 */

#ifndef INTERFACES_USBPORSTHANDLER_HPP_
#define INTERFACES_USBPORTSHANDLER_HPP_



#include "wiringPi.h"
#include "../helpers/Strings.hpp"
#include "../helpers/Constants.hpp"
#include "../helpers/Helper.hpp"

class UsbPortsHandler{

public:

	UsbPortsHandler();
 	virtual ~UsbPortsHandler();

  	int restartUsbPort();
 	int powerUsbPortOff();
	int powerUsbPortOn();
	bool isMicorcontrollerConnectedToBoardTester();

private:
	const char* getFlashCommandInterface1 = "sudo /home/BoardTester/stm32flash/stm32flash -b 115200 -v -w /home/BoardTester/stm32flash/TestProgramm.hex /dev/ttyACM0";
};


#endif /* INTERFACES_USBPORTHANDLER_HPP_ */
