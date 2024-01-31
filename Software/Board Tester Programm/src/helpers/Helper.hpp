/*
 * Helper.hpp
 *
 *  Created on: 29.05.2023
 *      Author: Astra
 */

#ifndef HELPERS_HELPER_HPP_
#define HELPERS_HELPER_HPP_


#include <iomanip>
#include "wiringPi.h"
#include <fstream>

#include "CustomTypes.hpp"
#include "Strings.hpp"

//#include <fcntl.h>



#define USB_SERIAL_PORT_0_ACK 0
#define TERMINAL_BUFFER_SIZE 256
#define DATE_TIME_BUFFER_SIZE 20

namespace Helper{


	string convertGpiosStrinVectorToString(std::vector<string> textVector);
	string convertFloatToString(float value);
	string trimString(string text);
  	string getTestFileHeaderText(uint8_t mode, uint16_t pullType, uint16_t speed);
	string getConnectionToPinTextError(int error);
 	int getActivUsbSerialPort();
	string executeCommandAndGetOutput(string command);
 	std::string getCurrentDateTimeText(bool isFileName);
	GPIO_Initial_Struct getInitialStructure(uint8_t pin, uint8_t port, uint16_t pullType, uint8_t mode, uint16_t speed);
	string getSpeedText(uint16_t speed);
	string getPortText(uint8_t port);
	bool isModerRegisterInitialized(uint32_t moderRegisterState, GPIO_Initial_Struct * gpioInitialStructure);
	bool isPullTypeRegisterInitialized(uint32_t pupDrRegisterState, GPIO_Initial_Struct * gpioInitialStructure);
	bool isSpeedRegisterInitialized(uint32_t oSpeedRegisterState, GPIO_Initial_Struct * gpioInitialStructure);
	bool isOtypeRegisterInitialized(uint32_t oTypeRegisterState, GPIO_Initial_Struct * gpioInitialStructure);
	string getGpioInitialErrorText(GPIO_INITIAL_STATE initialState);
	string getHandleRegistersAckText(OUTPUT_HANDLE_STATE state);
	bool isGpioConnectedToPushButton(GPIO_Initial_Struct *gpioInitialStructure);
	GPIO_Registers_Struct getNucleoBoardRegisters(uint8_t * readBuffer);
	int getIoExpanderCommand(int muxNumber, uint8_t pinNumber, std::vector<IO_Expander_Multiplexer_Select_Commands> * mux_select_vector);
	int  initMultiplexerSelectPins(int muxNumber,std::vector<IO_Expander_Multiplexer_Select_Commands> *mux_select_vector);
	bool isGpioTestable(uint8_t port, uint8_t pin);
	string getTestCommandText(TestCommand command);
  	string addVectorStringToText(string text, std::vector<string> stringVector);

  };




#endif /* HELPERS_HELPER_HPP_ */
