/*
 * TemperatureProcess.hpp
 *
 *  Created on: 29.05.2023
 *      Author: Astra
 */

#ifndef TEST_PROCESSES_TEMPERATUREPROCESS_HPP_
#define TEST_PROCESSES_TEMPERATUREPROCESS_HPP_

#include <cmath>

#include "../controllers/PeripheralsInterfacesController.hpp"

#define Avg_Slope 0.0043
#define V25 1.43
#define VSENSE 3.3/4095  // 12 bit res
#define TEMPERATURE_REFERENCE_VOLTAGE 3.3
#define TEMPERATURE_SENSOR_RESOLUTION 4095

class TemperatureMeasurementProcess{

public:

	TemperatureMeasurementProcess();
	TemperatureMeasurementProcess(PeripheralsInterfacesController *_controller): controller(_controller){};
	virtual ~TemperatureMeasurementProcess();

	void setController(PeripheralsInterfacesController *_controller);
	float readBoardTemperatureSensor();

private:

	PeripheralsInterfacesController *controller;

};

#endif /* TEST_PROCESSES_TEMPERATUREPROCESS_HPP_ */
