/*
 * TemperatureProcess..cpp
 *
 *  Created on: 29.05.2023
 *      Author: Astra
 */

#include "TemperatureProcess.hpp"


TemperatureMeasurementProcess::TemperatureMeasurementProcess() {
	controller =  NULL;
}

void TemperatureMeasurementProcess::setController(PeripheralsInterfacesController *_controller){
	controller = _controller;
}


TemperatureMeasurementProcess::~TemperatureMeasurementProcess(){

}

/* return measured Nucleo-Board Temperature
 */
float TemperatureMeasurementProcess::readBoardTemperatureSensor(){
	uint32_t uartCommand = 0xFFFF;
	controller->logger->printToFileLogger("\n Read Board Temperature Sensor UART Command: " +to_string(uartCommand), false);
	int ack = controller->uartHandler.sendUartData(uartCommand);
	if(ack == 2){
	 	uint8_t * readBuffer =  controller->uartHandler.readUartData(2);
	 	if(readBuffer == nullptr){
	 		controller->logger->printToFileLogger("Error:TemperatureMeasurementProcess readBoardTemperatureSensor failed to measure Nucleo-Board Temperature cause: NULL UART Data", false);
			return -1;
	 	}else{
	 		uint16_t sensorValue = 0x0;
	 		sensorValue = readBuffer[0] | (readBuffer[1] << 8);
	 		delete[] readBuffer;
	 	 	if(sensorValue != 0){
	 	 	 	float temperature =  ((V25 - (TEMPERATURE_REFERENCE_VOLTAGE / TEMPERATURE_SENSOR_RESOLUTION * sensorValue)) / Avg_Slope) + 25;
	 	 		return round(temperature * 100) / 100;
	 	   	}else{
	 	   		controller->logger->printToFileLogger("Error:TemperatureMeasurementProcess readBoardTemperatureSensor failed to measure Nucleo-Board Temperature", false);
	 	 		return -1;
	 	 	}
	 	}
	}else{
		controller->logger->printToFileLogger("Error:TemperatureMeasurementProcess readBoardTemperatureSensor failed to send UART Temperature command", false);
		return -1;
	}
}

