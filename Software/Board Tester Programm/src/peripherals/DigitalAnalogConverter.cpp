/*
 * DigitalAnalogConverter.cpp
 *
 *  Created on: 29.05.2023
 *      Author: Astra
 */

#include "../peripherals/DigitalAnalogConverter.hpp"

DigitalAnalogConverter::DigitalAnalogConverter() {
	spiHandler = NULL;
	dacRefernceVoltage = 0;
}

DigitalAnalogConverter::~DigitalAnalogConverter() {
	closeDacSpiConnection();
}

/* initialize DAC
 * Parameter:
 * _spiHandler: SPI Interface for DAC
 *  _dacRefernceVoltageValue: DAC Reference Voltage
 */
void DigitalAnalogConverter::initialDac(SpiHandler * _spiHandler, double _dacRefernceVoltageValue){
	spiHandler = _spiHandler;
	dacRefernceVoltage = _dacRefernceVoltageValue;

}


/* Set DAC Voltage
 * Parameter:
 * voltage : Voltage value
 */
int DigitalAnalogConverter::setDacVoltage(double voltage) {
	double dacVoltage = voltage * dacResolution  / dacRefernceVoltage;
	return configureDacNewValue(dacVoltage);
}

/* Configure DAC New Value
 * Parameter:
 * value :: DAC Value
 */
int DigitalAnalogConverter::configureDacNewValue(uint16_t value) {
 	uint16_t command = configBits;
	command |= value;
	unsigned char commandArray[2];
	commandArray[0] = command >> 8;
	commandArray[1] = command & 0xFF;
//	int ack = wiringPiSPIDataRW(MCP4921_CH, commandArray, sizeof(commandArray));
	int ack = spiHandler->writeSpiData(commandArray, sizeof(commandArray), 0);
  	if(ack < -1){
   		return DAC_SET_VALUE_ERROR;
	}else{
		return 1;
	}
}

/* close DAC SPI  Connection
 */
void DigitalAnalogConverter::closeDacSpiConnection(){
	spiHandler->closeSpiConnection();
	delete (spiHandler);
}
