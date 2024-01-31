/*
 * AnalogDigitalConverter.hpp
 *
 *  Created on: 29.05.2023
 *      Author: Astra
 */

#ifndef PERIPHERALS_ANALOGDIGITALCONVERTER_HPP_
#define PERIPHERALS_ANALOGDIGITALCONVERTER_HPP_

#include "../helpers/Constants.hpp"
#include "../interfaces/SpiHandler.hpp"
#include <cmath>
#include <cstdint>
#include "wiringPi.h"

#define MCP3202_SPI_CHANNEL 1
#define MCP3202_SINGLE_MODE SHIFT_LEFT_BY(7)
#define MCP3202_MSBF SHIFT_LEFT_BY(5)
#define MCP3002_BUFFER_SIZE 3



class AnalogDigitalConverter{

public:

 	AnalogDigitalConverter();
	virtual ~AnalogDigitalConverter();
	void initialAdc(SpiHandler * _spiHandler, double _adcRefernceVoltageValue);
  	float measureVoltage();
	void closeAdcSpiConnection();

private:

	int readAdc(int channel);
	SpiHandler * spiHandler;
 	double adcRefernceVoltage;
	uint16_t adcDelayMicros;

	double const adcSpeed = 100000;  // 100 KHZ
	double const adcConversionClockCycles  = 12;
	double const adcAnalogInputSampleTimeCycles = 1.5;
	double const adcDelay = 500;

};



#endif /* PERIPHERALS_ANALOGDIGITALCONVERTER_HPP_ */
