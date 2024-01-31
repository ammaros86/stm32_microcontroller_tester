/*
 * CurrentSensor.hpp
 *
 *  Created on: 03.08.2023
 *      Author: Astra
 */

#ifndef INTERFACES_CURRENTSENSOR_HPP_
#define INTERFACES_CURRENTSENSOR_HPP_

#include "../interfaces/I2cHandler.hpp"
#include "../helpers/Logger.hpp"
#include <cmath>

#define INA219_CONFIG_REGISTER 0x00
#define RESET_INA219_COMMAND 0x0F
#define POWER_DOWN_COMMAND 0xFFF8
#define MAX_16BIT_POSITIV_VALUE 32767
#define MAX_16_BIT_VALUE 65536
#define CURRENT_REGISTER 0x04
#define CALIBRATION_RESGITER 0x05
#define VOLTAGE_RANGE_CURRENT_SENSOR 0
#define MAXIMUM_SHUNT_VOLTAGE_320 3
#define CURRENT_SENSOR_ADC_12_RESOLUTION 3
#define SHUNT_BUS_TRIGGERED_MODE 0x07
#define FACTOR_CURRENT_LSB 32769
#define FACTOR_POWER_LSB 20
#define FACTOR_CALIBRATION 0.04096
#define CURRENT_SENSOR_ADDRESS  0x40
#define BRNG 13
#define PG0 11
#define BADC1 7
#define SDAC1 3

class CurrentSensor{

public:

	CurrentSensor();
	virtual ~CurrentSensor();
 	void initialCurrentSensor(Logger *logger, I2cHandler * _i2cHandler);
	float measureCurrentConsumption();
 	void configureCurrentSensor();
	void calibrateCurrentSensor();
	void resetCurrentSensor();
	void setCurrentSensorPowerDown();

private:

	Logger *logger;
	I2cHandler *i2cHandler;
 	float currentLsb;
 	float maxExcpetedCurrent = 1;
	float shuntResistance = 0.1;

	void powerUpCurrentSensor();

};


#endif /* INTERFACES_CURRENTSENSOR_HPP_ */
