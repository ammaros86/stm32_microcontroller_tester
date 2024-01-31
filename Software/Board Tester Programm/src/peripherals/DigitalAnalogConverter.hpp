/*
 * DigitalAnalogConverter.hpp
 *
 *  Created on: 29.05.2023
 *      Author: Astra
 */

#ifndef PERIPHERALS_DIGITALANALOGCONVERTER_HPP_
#define PERIPHERALS_DIGITALANALOGCONVERTER_HPP_

#include "../helpers/Constants.hpp"
#include "../interfaces/SpiHandler.hpp"

#define CONTROL_BYTES 0x03

class DigitalAnalogConverter {

public:

	DigitalAnalogConverter();
	virtual ~DigitalAnalogConverter();

	void initialDac(SpiHandler * _spiHandler,double _dacRefernceVoltageValue);
 	int setDacVoltage(double voltage);

private:

	SpiHandler * spiHandler;
	double dacRefernceVoltage;

  	int configureDacNewValue(uint16_t value);
  	void closeDacSpiConnection();

  	const uint16_t configBits = CONTROL_BYTES << 12;
	const int dacResolution =  4095;
 };

#endif /* PERIPHERALS_DIGITALANALOGCONVERTER_HPP_ */
