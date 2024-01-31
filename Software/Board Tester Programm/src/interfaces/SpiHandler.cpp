/*
 * SpiHandler.cpp
 *
 *  Created on: 31.08.2023
 *      Author: Astra
 */

#include "SpiHandler.hpp"



SpiHandler::SpiHandler(){
 	fd = 0;
 	spiSpeedHz = 0;
}

SpiHandler::~SpiHandler(){
	closeSpiConnection();
}

int SpiHandler::initialSpiInterface(const char * channel, unsigned int speed){
	spiSpeedHz = speed ;
	if((fd = open (channel, O_RDWR)) < 0)
	  return SPI_Handler_ERROR;

	if(ioctl (fd, SPI_IOC_WR_MODE, &spi_mode) < 0)
		return SPI_Handler_ERROR;

	if(ioctl (fd, SPI_IOC_WR_BITS_PER_WORD, &spi_word_length) < 0)
		return SPI_Handler_ERROR;

	if(ioctl (fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed)   < 0)
		return SPI_Handler_ERROR;

	return fd ;
}

/* close I2C File Descriptor
 */
void SpiHandler::closeSpiConnection(){
	if(fd >= 0){
		close(fd);
		fd = -1;
	}
}

/* write and read SPI
 * Parameter:
 * data: contain Data
 * length : data length
 * spi_delay : SPI Delay
 */
int SpiHandler::writeReadSpiData(unsigned char* data, int length, uint16_t spi_delay){
	struct spi_ioc_transfer transferStruct ;
	memset (&transferStruct, 0, sizeof (transferStruct)) ;
	transferStruct.rx_buf = (unsigned long)data;
	transferStruct.tx_buf = (unsigned long)data;
 	transferStruct.len = length;
	transferStruct.delay_usecs = spi_delay;
	transferStruct.speed_hz = spiSpeedHz;
	transferStruct.bits_per_word = spi_word_length;
	return ioctl (fd, SPI_IOC_MESSAGE(1), &transferStruct);
}

/* write SPI
 * Parameter:
 * data: write Data
 * length : data length
 * spi_delay : SPI Delay
 */
int SpiHandler::writeSpiData(unsigned char* data, int length, uint16_t spi_delay){
	struct spi_ioc_transfer transferStruct ;
	memset (&transferStruct, 0, sizeof (transferStruct)) ;
 	transferStruct.tx_buf = (unsigned long)data;
 	transferStruct.len = length;
	transferStruct.delay_usecs = spi_delay;
	transferStruct.speed_hz = spiSpeedHz;
	transferStruct.bits_per_word = spi_word_length;
	return ioctl (fd, SPI_IOC_MESSAGE(1), &transferStruct);
}

/* read SPI
 * Parameter:
 * data: read Data
 * length : data length
 * spi_delay : SPI Delay
 */
int SpiHandler::readSpiData(uint8_t* data, int length){
	struct spi_ioc_transfer transferStruct ;
	memset (&transferStruct, 0, sizeof (transferStruct)) ;
 	transferStruct.rx_buf = (unsigned long)data;
	transferStruct.len = length;
	transferStruct.delay_usecs = spi_delay;
	transferStruct.speed_hz = spiSpeedHz;
	transferStruct.bits_per_word = spi_word_length;
	return ioctl (fd, SPI_IOC_MESSAGE(1), &transferStruct);
}
