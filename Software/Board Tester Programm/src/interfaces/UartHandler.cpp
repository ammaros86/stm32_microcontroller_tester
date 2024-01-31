/*
 * UartHandler.cpp
 *
 *  Created on: 31.08.2023
 *      Author: Astra
 */

#include "UartHandler.hpp"



UartHandler::UartHandler(){
	uartFd = 0;
}

UartHandler::~UartHandler() {
	closeUart();
}

/* initialize UART communication with the NUCLEO Board over USB
 */
int UartHandler::initialUart(){
 	uartFd = 0;
	int activUsbInterface = Helper::getActivUsbSerialPort();
	if(activUsbInterface == USB_SERIAL_PORT_TTYACM0_ACK){
 		uartFd = serialOpen(Strings::USB_SERIAL_PORT_TTYACM0.c_str(), BAUDRATE);
 	}else if(activUsbInterface == USB_SERIAL_PORT_TTYACM1_ACK){
		uartFd = serialOpen(Strings::USB_SERIAL_PORT_TTYACM1.c_str(), BAUDRATE);
	}
     return uartFd;
}

/* close UART communication
 */
int UartHandler::closeUart(){
	serialFlush(uartFd);
	serialClose(uartFd);
	uartFd = -1;
	return 1;
}

/* read UART Data from Nucleo Board
 * Parameter:
 * bytes: number of bytes to read
 */
uint8_t * UartHandler::readUartData(int bytes){
	uint8_t *readBuffer = new uint8_t[bytes];
 	memset(readBuffer, INIT_CHAR, bytes);
  	if(isUartDataAvailable()){
  		int rx_length = read(uartFd, (void *)readBuffer, bytes);
 		if (rx_length <= 0){
			 return nullptr;
		}
 	}else{
 		 return nullptr;
	}
	return readBuffer;
}

/* check if UART available to read
 */
bool UartHandler::isUartDataAvailable(){
	bool uartDataAvailable = false;
	for(int i=0; i<100; i++){
		if(serialDataAvail(uartFd) > 0){
			uartDataAvailable = true;
			break;
		}else{
			delay(_100MS_DELAY);
		}
	}
	return uartDataAvailable;
}

/* send UART Data to Nucleo Board
 * Parameter:
 * data: data to send over UART
 */
int UartHandler::sendUartData(uint16_t data){
 	if(uartFd <= 0){
 		return -1;
	}else{
 		uint8_t sendBuffer[2];
 		sendBuffer[0] = (data >> 8) & 0xFF;
 		sendBuffer[1] = data & 0xFF;
		int bytesSend = write(uartFd, sendBuffer, sizeof(sendBuffer));
		delay(_10MS_DELAY);
		return bytesSend;
	}
}

/* return UART File Descriptor
 */
int UartHandler::getFileDescriptor(){
	return uartFd;
}
