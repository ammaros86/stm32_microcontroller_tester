/*
 * AnalogDigitalConverter.cpp
 *
 *  Created on: 29.05.2023
 *      Author: Astra
 */


#include "AnalogDigitalConverter.hpp"

AnalogDigitalConverter::AnalogDigitalConverter(){
 	adcRefernceVoltage = 0;
	spiHandler = NULL;
	adcDelayMicros = 0;
}

AnalogDigitalConverter::~AnalogDigitalConverter(){
	closeAdcSpiConnection();
}

/* initialize ADC
 * Parameter:
 * _spiHandler: SPI Interface for ADC
 *  _adcRefernceVoltageValue: ADC Reference Voltage
 */
void AnalogDigitalConverter::initialAdc(SpiHandler * _spiHandler, double _adcRefernceVoltageValue){
	spiHandler = _spiHandler;
	adcRefernceVoltage = _adcRefernceVoltageValue;
	double conversionTimeInMicros = (adcConversionClockCycles / adcSpeed) * 1000000;
	double sampleTimeInMicros = (adcAnalogInputSampleTimeCycles / adcSpeed) * 1000000;
	adcDelayMicros = static_cast<uint16_t>(conversionTimeInMicros + sampleTimeInMicros + adcDelay);
}

/* read ADC Value
 * Parameter:
 * channel : ADC Channel
 */
int AnalogDigitalConverter::readAdc(int channel){
    int adcValue = -1;
    int startBit = 1;
    int lastBufferValue = 0;
	uint8_t buffer[MCP3002_BUFFER_SIZE];

    buffer[0] = startBit;
    buffer[1] = channel;
    buffer[2] = lastBufferValue;

    if(spiHandler->writeReadSpiData(buffer, MCP3002_BUFFER_SIZE, adcDelayMicros) == -1){
    	return adcValue;
    }
    adcValue = (buffer[2] | ((buffer[1] & 0x0F) << 8));
 	return adcValue;
}

/* Measure and return Voltage
 */
float AnalogDigitalConverter::measureVoltage(){
	float voltage = 0;
	int adcValue  = readAdc(MCP3202_SINGLE_MODE|MCP3202_MSBF);
   	if(adcValue != -1){
		voltage = ((adcRefernceVoltage * adcValue) / ADC_RESOLUTION);
		return round(voltage * 100) / 100;
	}else{
 		return MEASURE_VOLTAGE_ERROR;
	}
}

/* close ADC SPI Connection
 */
void AnalogDigitalConverter::closeAdcSpiConnection(){
	spiHandler->closeSpiConnection();
	delete (spiHandler);
}

