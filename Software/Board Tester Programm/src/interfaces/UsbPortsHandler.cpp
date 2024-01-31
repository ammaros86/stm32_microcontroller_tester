/*
 * UsbPortHandler.cpp
 *
 *  Created on: 31.08.2023
 *      Author: Astra
 */

#include "UsbPortsHandler.hpp"


UsbPortsHandler::UsbPortsHandler(){
}

UsbPortsHandler::~UsbPortsHandler() {
}

/* restart USB Ports
 */
int UsbPortsHandler::restartUsbPort(){
	int ackPowerUsbOff = powerUsbPortOff();
	delay(_2S_DELAY);
	int ackPowerUsbOn = powerUsbPortOn();
	delay(_3S_DELAY);
 	if(ackPowerUsbOff && ackPowerUsbOn){
		return RESTART_USB_PORT_DONE;
	}else{
		return RESTART_USB_PORT_FAILED;
	}
}

/* turn USB Ports Off
 */
int UsbPortsHandler::powerUsbPortOff(){
 	string ack = Helper::executeCommandAndGetOutput(Strings::POWER_OFF_USB_COMMAND);
	ack = Helper::trimString(ack);
 	if (ack == Strings::USB_COMMAND_SET_ACK){
  		return 1;
 	}else{
		return TURN_USB_PORT_OFF_ERROR;
 	}
}

/* turn USB Ports On
 */
int UsbPortsHandler::powerUsbPortOn(){
	string ack = Helper::executeCommandAndGetOutput(Strings::POWER_ON_USB_COMMAND);
	ack = Helper::trimString(ack);
	if (ack == Strings::USB_COMMAND_SET_ACK){
  		return 1;
 	}else{
		return -1;
 	}
}

/* check if a Nucleo Board connected to a USB Port
 */
bool UsbPortsHandler::isMicorcontrollerConnectedToBoardTester(){
 	string isBoardConnectedAckText = Helper::executeCommandAndGetOutput(Strings::CHECK_NUCLEO_BOARD_USB_CONNECTED_COMMAND);
 	cout << "isBoardConnectedAckText: " << isBoardConnectedAckText <<endl;
 	isBoardConnectedAckText = Helper::trimString(isBoardConnectedAckText);
 	cout << "isBoardConnectedAckText: " << isBoardConnectedAckText <<endl;
 	if (isBoardConnectedAckText == Strings::USB_BOARD_CONNECTED_ACK) {
 		return true;
 	}else{
		return false;
	}
}
