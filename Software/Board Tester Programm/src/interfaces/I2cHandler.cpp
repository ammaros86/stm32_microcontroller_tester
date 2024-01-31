/*
 * I2cHandler.cpp
 *
 *  Created on: 31.08.2023
 *      Author: Astra
 */



#include "I2cHandler.hpp"


I2cHandler::I2cHandler(){
 	fd = 0;
}

I2cHandler::~I2cHandler() {
	closeI2cConnection();
}

/* initial I2C Device
 * Parameter:
 * deviceAddress: Device Address
 */
int I2cHandler::initialI2cInterface(uint8_t deviceAddress){
	const char * filePath = Strings::I2C_FILE_PATH.c_str();
 	if ((fd = open(filePath, O_RDWR)) < 0){
		return I2C_CONTROLLER_ERROR;
	}
	if (ioctl(fd, I2C_SLAVE, deviceAddress) < 0){
		return I2C_CONTROLLER_ERROR;
 	}
	return fd;
}

/* read 1 Byte from I2C Device
 * Parameter:
 * deviceAddress: Device Address
 */
uint8_t I2cHandler::readI2c1ByteFromAddress(uint8_t registerAddress){
    if (write(fd, &registerAddress, I2C_CONTROLLER_1_BYTE) != I2C_CONTROLLER_1_BYTE) {
        return I2C_CONTROLLER_ERROR;
    }
    uint8_t data;
    if (read(fd, &data, I2C_CONTROLLER_1_BYTE) != I2C_CONTROLLER_1_BYTE) {
        return I2C_CONTROLLER_ERROR;
    }
    return data;
}

/* read 2 Bytes from I2C Device
 * Parameter:
 * deviceAddress: Device Address
 */
uint16_t I2cHandler::read2BytesFromAddress(uint8_t registerAddress){
	uint8_t i2cBuffer[I2C_CONTROLLER_3_BYTES];
	i2cBuffer[0] = registerAddress;
	if (write(fd, i2cBuffer, I2C_CONTROLLER_1_BYTE) != I2C_CONTROLLER_1_BYTE) {
		return I2C_CONTROLLER_ERROR;
	}
	if (read(fd, i2cBuffer, I2C_CONTROLLER_2_BYTES) != I2C_CONTROLLER_2_BYTES) {
		return I2C_CONTROLLER_ERROR;
	}
	return (i2cBuffer[0] << 8) | i2cBuffer[1];
}

/* read 2 Bytes from I2C Device
 * Parameter:
 * deviceAddress: Device Address
 * value : value to write
 */
int I2cHandler::write2BytesToI2cSlave(uint8_t registerAddress, uint16_t value){
	uint8_t i2cBuffer[I2C_CONTROLLER_3_BYTES];
	i2cBuffer[0] = registerAddress;
	i2cBuffer[1] = value >> 8;
	i2cBuffer[2] = value & 0xFF;
	if (write(fd, i2cBuffer, I2C_CONTROLLER_3_BYTES) != I2C_CONTROLLER_3_BYTES){
		return I2C_CONTROLLER_ERROR;
 	}
	return 1;
}

/* write 1 Byte to I2C Device
 * Parameter:
 * deviceAddress: Device Address
 * value : value to write
 */
int I2cHandler::write1ByteToI2cSlave(uint8_t registerAddress, uint8_t value){
	uint8_t i2cBuffer[I2C_CONTROLLER_2_BYTES] = {registerAddress, value};
	if (write(fd, i2cBuffer, I2C_CONTROLLER_2_BYTES) != I2C_CONTROLLER_2_BYTES) {
		return I2C_CONTROLLER_ERROR;
	}
	return 1;
}

/* write 1 Byte direct to I2C Device
 * Parameter:
 * value : value to write
 */
int I2cHandler::writeToI2cSlave(uint8_t value){
	uint8_t i2cBuffer[I2C_CONTROLLER_1_BYTE] = {value};
	if (write(fd, i2cBuffer, I2C_CONTROLLER_1_BYTE) != I2C_CONTROLLER_1_BYTE) {
		return I2C_CONTROLLER_ERROR;
	}
	return 1;
}

/* close I2C File Descriptor
 */
int I2cHandler::closeI2cConnection(){
	if(close(fd) != 0){
		return I2C_CONTROLLER_ERROR;
	}
	return 1;
}

