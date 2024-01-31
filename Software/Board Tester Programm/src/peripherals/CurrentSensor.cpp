/*
 * CurrentSensor.cpp
 *
 *  Created on: 03.08.2023
 *      Author: Astra
 */


#include "CurrentSensor.hpp"

CurrentSensor::CurrentSensor(){
 	logger = NULL;
 	i2cHandler = NULL;
	currentLsb = 0;
}

CurrentSensor::~CurrentSensor() {
	i2cHandler->closeI2cConnection();
	delete (logger);
	delete (i2cHandler);
}

/* initialize Current Sensor
  * Parameter:
 * _logger: to print LOGs
 * _i2cController: I2C Interface
 */
void CurrentSensor::initialCurrentSensor(Logger *_logger, I2cHandler * _i2cHandler){
	logger = _logger;
	i2cHandler = _i2cHandler;
}

/* measure Nucleo Board Current Consumption
 */
float CurrentSensor::measureCurrentConsumption(){
	powerUpCurrentSensor();
	delay(_20MS_DELAY);
	resetCurrentSensor();
	delay(_20MS_DELAY);
    calibrateCurrentSensor();
	delay(_20MS_DELAY);
    configureCurrentSensor();
	delay(_20MS_DELAY);
	uint16_t measuredValue = i2cHandler->read2BytesFromAddress(CURRENT_REGISTER);
	delay(_20MS_DELAY);
	setCurrentSensorPowerDown();
	if(measuredValue < 0){
		logger->printToFileLogger("CurrentSensor readCurrent failed to read current ", false);
		return MEASURE_CURRENT_ERROR;
	}
	int16_t signedValue = (int16_t)measuredValue;
	if (signedValue > MAX_16BIT_POSITIV_VALUE) signedValue -= MAX_16_BIT_VALUE;
	float currentValue = signedValue * currentLsb * 1000;
 	if(currentValue < 0)
    	currentValue = currentValue * -1;

    return floor(currentValue * 100) / 100;
}

/* configure Current Sensor
 */
void CurrentSensor::configureCurrentSensor(){
    uint16_t configValue = ( VOLTAGE_RANGE_CURRENT_SENSOR << BRNG | MAXIMUM_SHUNT_VOLTAGE_320 << PG0 |
    		CURRENT_SENSOR_ADC_12_RESOLUTION << BADC1 | CURRENT_SENSOR_ADC_12_RESOLUTION << SDAC1  | SHUNT_BUS_TRIGGERED_MODE);
 	if(i2cHandler->write2BytesToI2cSlave(INA219_CONFIG_REGISTER, configValue) < 0){
 			logger->printToFileLogger("CurrentSensor configureCurrentSensor failed to write configuration value", false);
	}
}

/* calibrate Current Sensor
 */
void CurrentSensor::calibrateCurrentSensor(){
	currentLsb = maxExcpetedCurrent / FACTOR_CURRENT_LSB;
 	uint16_t calibrationValue = (uint16_t) trunc(FACTOR_CALIBRATION / (currentLsb * shuntResistance));
 	if(i2cHandler->write2BytesToI2cSlave(CALIBRATION_RESGITER, calibrationValue) < 0){
 			logger->printToFileLogger("CurrentSensor calibrateCurrentSensor failed to write calibration value", false);
	}
}

/* reset Current Sensor
 */
void CurrentSensor::resetCurrentSensor(){
	if(i2cHandler->write2BytesToI2cSlave(INA219_CONFIG_REGISTER, RESET_INA219_COMMAND) < 0){
		logger->printToFileLogger("CurrentSensor resetCurrentSensor failed to reset", false);
	}
}

/* set Current Sensor Power-Down Mode
 */
void CurrentSensor::setCurrentSensorPowerDown(){
	uint16_t configValue = i2cHandler->read2BytesFromAddress(INA219_CONFIG_REGISTER);
	if(configValue < 0){
		logger->printToFileLogger("CurrentSensor setCurrentSensorPowerDown failed to read configuration Register", false);
	}else{
		if(i2cHandler->write2BytesToI2cSlave(INA219_CONFIG_REGISTER, configValue | POWER_DOWN_COMMAND) < 0){
			logger->printToFileLogger("CurrentSensor setCurrentSensorPowerDown failed to write new configuration value", false);
		}
	}
}

/* Power Current Sensor Up after set in Power-Down Mode
 */
void CurrentSensor::powerUpCurrentSensor(){
	uint16_t configValue = i2cHandler->read2BytesFromAddress(INA219_CONFIG_REGISTER);
	if(configValue < 0){
		logger->printToFileLogger("CurrentSensor powerUpCurrentSensor failed to read configuration Register", false);
	}else{
		if(i2cHandler->write2BytesToI2cSlave(INA219_CONFIG_REGISTER, configValue | SHUNT_BUS_TRIGGERED_MODE) < 0){
			logger->printToFileLogger("CurrentSensor powerUpCurrentSensor failed to write new configuration value", false);
		}
	}
}
