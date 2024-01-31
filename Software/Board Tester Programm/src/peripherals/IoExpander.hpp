/*
 * IoExpander.hpp
 *
 *  Created on: 29.05.2023
 *      Author: Astra
 */

#ifndef PERIPHERALS_IOEXPANDER_HPP_
#define PERIPHERALS_IOEXPANDER_HPP_

#include "../interfaces/I2cHandler.hpp"
#include "../helpers/Logger.hpp"

#define IODIRA 0x00
#define IODIRB 0x01
#define OLATA 0x012
#define OLATB 0x013
#define MAX_WRITE_TRY 10

class IoExpander{

public:

	IoExpander();
	virtual ~IoExpander();

 	void initialIoExpander(Logger *_logger, I2cHandler * _i2cHandler);
	void resetRegisters();
  	int writeDataToRegister(int registerIO, int data);
 	int readRegister(int registerAddress);

private:

 	Logger * logger;
 	I2cHandler *i2cHandler;
};



#endif /* PERIPHERALS_IOEXPANDER_HPP_ */
