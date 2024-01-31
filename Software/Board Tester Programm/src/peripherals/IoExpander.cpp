/*
 * IoExpander.cpp
 *
 *  Created on: 29.05.2023
 *      Author: Astra
 */

#include "../peripherals/IoExpander.hpp"


IoExpander::IoExpander(){
 	logger = NULL;
 	i2cHandler = NULL;
}

IoExpander::~IoExpander() {
	i2cHandler->closeI2cConnection();
	delete (logger);
	delete (i2cHandler);
}


/* initialize I/O Expander
 * Parameter:
 * _logger: logger object
 */
void IoExpander::initialIoExpander(Logger *_logger, I2cHandler * _i2cHandler){
	logger = _logger;
	i2cHandler = _i2cHandler;
 }

/* reset I/O Expander
 */
void IoExpander::resetRegisters(){
	writeDataToRegister(IODIRA, 0x00);
	writeDataToRegister(IODIRB, 0x00);
	writeDataToRegister(OLATA, 0x00);
	writeDataToRegister(OLATB, 0x00);
}

/* Write Data to I/O Expander Register
 * Parameter:
 * registerIO: Register
 * data: Data
 */
int IoExpander::writeDataToRegister(int registerIO, int data){
	i2cHandler->write1ByteToI2cSlave(registerIO, data);
 	int registerValue = readRegister(registerIO);
  	if(registerValue != data){
 		int registerValue2 = -1;
 		logger->printToFileLogger("IoExpander:Error writeData register:" + to_string(registerIO) + " Command: " + to_string(data) + " to I/O Expander  read Value: " + to_string(registerValue) , false);
 		for (int i=0; i< MAX_WRITE_TRY; i++){
 	  		logger->printToFileLogger("IoExpander: writeData attempt number: " + to_string(i) + " to write Data" , false);
 	  		i2cHandler->write1ByteToI2cSlave(registerIO, data) ;
			registerValue2 = readRegister(registerIO);
			if(registerValue2 == data){
				break;
			}else{
				logger->printToFileLogger("IoExpander:writeData attempt number: "+ to_string(i) +" failed, read Value: " + to_string(registerValue2), false);
			}
		}
 		if(registerValue2 == data){
 			return 1;
		}else{
			logger->printToFileLogger("Error:IoExpander writeData register:" + to_string(registerIO) + " failed to send Command: " +to_string((int) data) +" to I/O Expander  read Value: " + to_string(registerValue2), false);
			return IO_EXPNADER_WRITE_ERROR;
		}
	}else{
 		return 1;
	}
}

/* read I/O Expander Register
 * Parameter:
 * registerAddress : Register Address
 */
int IoExpander::readRegister(int registerAddress){
	return i2cHandler->readI2c1ByteFromAddress(registerAddress);
}



