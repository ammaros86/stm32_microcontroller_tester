/*
 * SpiHandler.hpp
 *
 *  Created on: 31.08.2023
 *      Author: Astra
 */

#ifndef INTERFACES_SPIHANDLER_HPP_
#define INTERFACES_SPIHANDLER_HPP_


#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>
#include <string.h>


#define SPI_Handler_ERROR -1


class SpiHandler{

public:

	SpiHandler();
 	virtual ~SpiHandler();
 	int initialSpiInterface(const char * channel, unsigned int speed);
 	void closeSpiConnection();
 	int writeReadSpiData(unsigned char* data, int length, uint16_t spi_delay);
 	int writeSpiData(unsigned char* data, int length, uint16_t spi_delay);

 	int readSpiData(uint8_t* data, int length);


private:
 	int fd;
 	int spiSpeedHz;

	const uint8_t spi_mode = 0;
	const uint8_t spi_word_length = 0;
	const uint16_t spi_delay = 0;

};

#endif /* INTERFACES_SPIHANDLER_HPP_ */
