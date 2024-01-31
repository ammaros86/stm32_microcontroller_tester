/*
 * I2cHandler.hpp
 *
 *  Created on: 31.08.2023
 *      Author: Astra
 */

#ifndef INTERFACES_I2CHANDLER_HPP_
#define INTERFACES_I2CHANDLER_HPP_



#include <cstdint>
#include <unistd.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <iostream>
#include <sys/ioctl.h>
#include "../helpers/Strings.hpp"
#include <fcntl.h>

#define I2C_CONTROLLER_ERROR -1
#define I2C_CONTROLLER_1_BYTE 1
#define I2C_CONTROLLER_2_BYTES 2
#define I2C_CONTROLLER_3_BYTES 3

class I2cHandler{

public:

	I2cHandler();
 	virtual ~I2cHandler();
 	int initialI2cInterface(uint8_t deviceAddress);
 	uint8_t readI2c1ByteFromAddress(uint8_t registerAddress);
 	uint16_t read2BytesFromAddress(uint8_t registerAddress);
 	int write2BytesToI2cSlave(uint8_t registerAddress, uint16_t value);
 	int write1ByteToI2cSlave(uint8_t registerAddress, uint8_t value);
 	int writeToI2cSlave(uint8_t value);
	int closeI2cConnection();

private:

	int fd;

};


#endif /* INTERFACES_I2CHANDLER_HPP_ */
