/*
 * AnalogInputTestProcess.cpp
 *
 *  Created on: 24.06.2023
 *      Author: Astra
 */

#include "AnalogInputTestProcess.hpp"

AnalogInputTestProcess::AnalogInputTestProcess() {
	controller =  NULL;
}

AnalogInputTestProcess::~AnalogInputTestProcess() {

}


/*  set Peripheral Controller
 * Parameter:
 *_controller: Peripheral Controller
 */
void AnalogInputTestProcess::setController(PeripheralsInterfacesController *_controller){
	controller = _controller;
}

/*  main method to test all GPIO Input
 * Test Input with No Pull, Pull Down and Pull Up configuration
 * Parameter:
 *isAppCommand: to update Database
 */
void AnalogInputTestProcess::initialAndTestAllAnalogInput(bool isAppCommand) {
	analogInputTestResultVector.clear();
 	analogInputError.clear();
	analogInputFail.clear();
	controller->measureVddAndGetGpioInputVoltagesLevels();
	for(uint8_t positionAnalogPinVector = 0; positionAnalogPinVector < controller->board_analog_pins_vector.size(); positionAnalogPinVector++){
		GPIO_Initial_Struct gpioInitialStructure = getAnalogInputInitialStructure(positionAnalogPinVector);
		controller->logger->printTestReportGpioNumberText(positionAnalogPinVector, &gpioInitialStructure);
		int multiplexersConfigError = controller->openMultiplexerConnectionToAnalog(positionAnalogPinVector);
 		if(multiplexersConfigError < 0){
			saveFailedErrorAnalogInput(gpioInitialStructure.pinText, false);
			handleMultiplexersFailure(gpioInitialStructure.pinText, isAppCommand, multiplexersConfigError);
		}else{
			testSingleAnalogInput(isAppCommand, &gpioInitialStructure);
		}
	}
	controller->logger->printSectionSeperator(false);
	checkAnalogInputTestResults();
 }


/* return a structure to initial GPIO Input
 * Parameter:
 *positionPullVector: to get the Pull Type
 *positionGpioPinVector: to get the Pin and Port of the GPIO Input
 */
GPIO_Initial_Struct AnalogInputTestProcess::getAnalogInputInitialStructure(uint8_t positionAnalogPinVector){
  	uint16_t pin = static_cast<uint16_t>(controller->board_analog_pins_vector.at(positionAnalogPinVector));
	uint8_t port = static_cast<uint8_t>(controller->analog_ports_vector.at(positionAnalogPinVector));
	return Helper::getInitialStructure(pin, port, COMMAND_Init_NO_PULL, COMMAND_Init_ANALOGE, COMMAND_Init_GPIO_SPEED_FREQ_LOW);
}

/* return a structure to initial GPIO Input
 * Parameter:
 *positionPullVector: to get the Pull Type
 *positionGpioPinVector: to get the Pin and Port of the GPIO Input
 */
void AnalogInputTestProcess::testSingleAnalogInput(bool isAppCommand, GPIO_Initial_Struct * gpioInitialStructure){
	GPIO_INITIAL_STATE  gpioInitialState =  controller->initialGpioAndCheckGpiosAckRegisters(gpioInitialStructure);
	Analog_Input_Test_Result_Struct testResult = {};
	testResult.pin = gpioInitialStructure->pinText;
	if(gpioInitialState == GPIO_INITIAL_STATE::initialised){
  		if(controller->mulutiplexerController.setMultiplexer7Conenction(Mulitplexer7_Config::DacConnection) == MUX7_CONTROLL_ERROR){
  			saveFailedErrorAnalogInput(gpioInitialStructure->pinText, false);
  		}

		controller->logger->printToFileLogger("DAC SET Voltage: " + to_string(controller->voltageRefernces.ANALOG_NULL_VOLTAGE_VALUE), false);
  		if(controller->dacHandler.setDacVoltage(controller->voltageRefernces.ANALOG_NULL_VOLTAGE_VALUE) == DAC_SET_VALUE_ERROR){
  			saveFailedErrorAnalogInput(gpioInitialStructure->pinText, false);
  		}
		testResult.measuredVotalge1 = readAnalogValue(gpioInitialStructure);
		controller->logger->printToFileLogger("-> NUCLEO ADC Measured Voltage: "+ to_string(testResult.measuredVotalge1), false);

		controller->logger->printToFileLogger("DAC SET Voltage: " + to_string(controller->voltageRefernces.LOW_INPUT_VOLTAGE_MAX_VALUE), false);
   		if(controller->dacHandler.setDacVoltage(controller->voltageRefernces.LOW_INPUT_VOLTAGE_MAX_VALUE) == DAC_SET_VALUE_ERROR){
   			saveFailedErrorAnalogInput(gpioInitialStructure->pinText, false);
  		}
		testResult.measuredVotalge2 = readAnalogValue(gpioInitialStructure);
		controller->logger->printToFileLogger("-> NUCLEO ADC Measured Voltage: "+ to_string(testResult.measuredVotalge2), false);

	 	controller->logger->printToFileLogger("DAC SET Voltage: "+ to_string(controller->voltageRefernces.HIGH_INPUT_VOLTAGE_MIN_VALUE), false);
   		if(controller->dacHandler.setDacVoltage(controller->voltageRefernces.HIGH_INPUT_VOLTAGE_MIN_VALUE) == DAC_SET_VALUE_ERROR){
   			saveFailedErrorAnalogInput(gpioInitialStructure->pinText, false);
  		}
	 	testResult.measuredVotalge3 = readAnalogValue(gpioInitialStructure);
		controller->logger->printToFileLogger("-> NUCLEO ADC Measured Voltage: "+ to_string(testResult.measuredVotalge3), false);

		controller->logger->printToFileLogger("DAC SET Voltage: "+ to_string(controller->voltageRefernces.ANALOG_MAX_VOLTAGE_VALUE), false);
		if(controller->dacHandler.setDacVoltage(controller->voltageRefernces.ANALOG_MAX_VOLTAGE_VALUE) == DAC_SET_VALUE_ERROR){
			saveFailedErrorAnalogInput(gpioInitialStructure->pinText, false);
  		}
		testResult.measuredVotalge4 = readAnalogValue(gpioInitialStructure);
		controller->logger->printToFileLogger("-> NUCLEO ADC Measured Voltage: "+ to_string(testResult.measuredVotalge4), false);


		if(isAnalogInputOk(&testResult)){
			testResult.comment = "OK";
			controller->logger->printToFileLogger("=> ANALOG PIN OK", false);
 		}else{
 			saveFailedErrorAnalogInput(testResult.pin, true);
			controller->logger->printToFileLogger("=> ANALOG PIN NOT OK", false);
 		}
//		if(isAppCommand)
//			controller->database.updateBoardAnalogInputData(&testResult);

		analogInputTestResultVector.push_back(testResult);
	 	controller->mulutiplexerController.setMultiplexer7Conenction(Mulitplexer7_Config::free);
	}else{
		saveFailedErrorAnalogInput(gpioInitialStructure->pinText, false);
		handleInitialFailureAnalog(testResult.pin, isAppCommand);
	}
}

/*
 * get the GPIO Input State HIGH/LOW
 * Parameter:
 * pinNumber: GPIO Pin Number
 * port:  GPIO Port
 */
double AnalogInputTestProcess::readAnalogValue(GPIO_Initial_Struct * gpioInitialStructure){
	double measuredVoltage = 0;
	if(controller->handleGpio(gpioInitialStructure->Pin, gpioInitialStructure->Port,  COMMAND_READ_ANALOG) < 0){
		saveFailedErrorAnalogInput(gpioInitialStructure->pinText, false);
 		return measuredVoltage;
	}else{
	 	uint8_t * readBuffer =  controller->uartHandler.readUartData(2);
	 	if(readBuffer == nullptr){
	 		saveFailedErrorAnalogInput(gpioInitialStructure->pinText, false);
	 		controller->logger->printToFileLogger("Error:AnalogInputTestProcess readAnalogValue failed NULL UART Data", false);
			return -1;
	 	}else{
	 		uint16_t adcValue = 0x0;
	 		adcValue = readBuffer[0] | (readBuffer[1] << 8);
	 		controller->logger->printToFileLogger("-> NUCLEO ADC VALUE: "+ to_string(adcValue), false);
	 		checkAnalogValueError(adcValue);
	 		delete[] readBuffer;
	 		measuredVoltage = (adcValue * 3.3)/ ADC_RESOLUTION;
	 	}
 	}
	return measuredVoltage;
}



void AnalogInputTestProcess::checkAnalogValueError(uint16_t adcValue){
	if(adcValue == ADC_INIT_ERROR){
 		controller->logger->printToFileLogger("Error:AnalogInputTestProcess failed to initial STM32-ADC", false);
	}else if(adcValue == ADC_SET_CHANNEL_ERROR){
 		controller->logger->printToFileLogger("Error:AnalogInputTestProcess failed to set STM32-ADC Channel", false);
	}else if(adcValue == ADC_START_ERROR){
 		controller->logger->printToFileLogger("Error:AnalogInputTestProcess failed to starts STM32-ADC conversion", false);
	}else if(adcValue == ADC_POLL_CONVERSION_ERROR){
 		controller->logger->printToFileLogger("Error:AnalogInputTestProcess failed to poll STM32-ADC for conversion", false);
	}else if(adcValue == ADC_STOP_ERROR){
 		controller->logger->printToFileLogger("Error:AnalogInputTestProcess failed to stop STM32-ADC ", false);
	}else if(adcValue == UNKNOWN_ADC){
 		controller->logger->printToFileLogger("Error:AnalogInputTestProcess unknown STM32-ADC", false);
	}
}

/* return a structure to initial GPIO Input
 * Parameter:
 *positionPullVector: to get the Pull Type
 *positionGpioPinVector: to get the Pin and Port of the GPIO Input
 */
bool AnalogInputTestProcess::isAnalogInputOk(Analog_Input_Test_Result_Struct  *testResult){
 	double maxDiff = 1;
	if(!checkNucleoAdcErrors(testResult->measuredVotalge1, testResult)){
		return false;
	}
	if(!checkNucleoAdcErrors(testResult->measuredVotalge2, testResult)){
		return false;
	}
	if(!checkNucleoAdcErrors(testResult->measuredVotalge3, testResult)){
		return false;
	}
	if(!checkNucleoAdcErrors(testResult->measuredVotalge4, testResult)){
		return false;
	}
	double diff1 = testResult->measuredVotalge1 - controller->voltageRefernces.ANALOG_NULL_VOLTAGE_VALUE;
	double diff2 = testResult->measuredVotalge2 - controller->voltageRefernces.LOW_INPUT_VOLTAGE_MAX_VALUE;
	double diff3 = testResult->measuredVotalge3 - controller->voltageRefernces.HIGH_INPUT_VOLTAGE_MIN_VALUE;
	double diff4 = testResult->measuredVotalge4 - controller->voltageRefernces.ANALOG_MAX_VOLTAGE_VALUE;
	if((diff1 > maxDiff || diff1 < -maxDiff) || (diff2 > maxDiff || diff2 < -maxDiff) || (diff3 > maxDiff || diff3 < -maxDiff) || (diff4 > maxDiff || diff4 < -maxDiff) ){
		controller->logger->printToFileLogger("Error measured Voltage out of range", false);
		testResult->comment = "Error measured Voltage out of range";
		return false;
	}else{
		return true;
	}
}

/* return a structure to initial GPIO Input
 * Parameter:
 *positionPullVector: to get the Pull Type
 *positionGpioPinVector: to get the Pin and Port of the GPIO Input
 */
bool AnalogInputTestProcess::checkNucleoAdcErrors(double measuredVotalge, Analog_Input_Test_Result_Struct  *testResult){
	if(measuredVotalge == ADC_INIT_ERROR){
		controller->logger->printToFileLogger("Error:AnalogInputTestProcess checkNucleoAdcErrors failed to initial STM32 ADC", false);
		testResult->comment = "failed to initial STM32 ADC";
		return false;
	}else if(measuredVotalge == ADC_SET_CHANNEL_ERROR){
		controller->logger->printToFileLogger("Error:AnalogInputTestProcess checkNucleoAdcErrors failed to set STM32 ADC Channel", false);
		testResult->comment = "failed to set STM32 ADC Channel";
		return false;
	}else if(measuredVotalge == ADC_START_ERROR){
		controller->logger->printToFileLogger("Error:AnalogInputTestProcess checkNucleoAdcErrors failed to start STM32 ADC", false);
		testResult->comment = "failed to start STM32 ADC";
		return false;
	}else if(measuredVotalge == ADC_POLL_CONVERSION_ERROR){
		controller->logger->printToFileLogger("Error:AnalogInputTestProcess checkNucleoAdcErrors failed poll STM32 ADC conversion", false);
		testResult->comment = "failed poll STM32 ADC conversion";
		return false;
	}else if(measuredVotalge == ADC_STOP_ERROR){
		controller->logger->printToFileLogger("Error:AnalogInputTestProcess checkNucleoAdcErrors failed to stop STM32 ADC", false);
		testResult->comment = "failed to stop STM32 ADC";
		return false;
	}else if(measuredVotalge == UNKNOWN_ADC){
		controller->logger->printToFileLogger("Error:AnalogInputTestProcess checkNucleoAdcErrors unknown Analog Pin Data", false);
		testResult->comment = "unknown Analog Pin Data";
		return false;
	}else{
		return true;
	}
}

/* return a structure to initial GPIO Input
 * Parameter:
 *positionPullVector: to get the Pull Type
 *positionGpioPinVector: to get the Pin and Port of the GPIO Input
 */
void AnalogInputTestProcess::handleInitialFailureAnalog(string pinText, bool isAppCommand){
	Analog_Input_Test_Result_Struct  testResult = {0};
	controller->logger->printToFileLogger("=> Failed to initial Analog Pin", false);
	if(isAppCommand){
		testResult.pin = pinText;
		testResult.measuredVotalge1 = -1;
		testResult.measuredVotalge2 = -1;
		testResult.measuredVotalge3 = -1;
		testResult.measuredVotalge4 = -1;
		testResult.comment = "Initial Analog Pin Error";
//		controller->database.updateBoardAnalogInputData(&testResult);
		analogInputTestResultVector.push_back(testResult);

	}

}

/* return a structure to initial GPIO Input
 * Parameter:
 *positionPullVector: to get the Pull Type
 *positionGpioPinVector: to get the Pin and Port of the GPIO Input
 */
void AnalogInputTestProcess::handleMultiplexersFailure(string pinText, bool isAppCommand, int multiplexerError){
	Analog_Input_Test_Result_Struct  testResult = {0};
	string errorText = Helper::getConnectionToPinTextError(multiplexerError);
	if(isAppCommand){
		testResult.pin = pinText;
		testResult.measuredVotalge1 = -1;
		testResult.measuredVotalge2 = -1;
		testResult.measuredVotalge3 = -1;
		testResult.measuredVotalge4 = -1;
		testResult.comment = errorText;
//		controller->database.updateBoardAnalogInputData(&testResult);
		analogInputTestResultVector.push_back(testResult);

	}
 	controller->logger->printToFileLogger(errorText, false);
}

/* add GPIO Text to string Vector
 * Parameter:
 * gpioText: GPIO
 * isError: to get if error or fail
 */
void AnalogInputTestProcess::saveFailedErrorAnalogInput(string pinText, bool isError){
	if(isError){
		auto iterator = std::find(analogInputError.begin(), analogInputError.end(), pinText);
		 if(iterator == analogInputError.end()){
			 analogInputError.push_back(pinText);
		 }
	}else{
		auto iterator = std::find(analogInputFail.begin(), analogInputFail.end(), pinText);
		 if(iterator == analogInputFail.end()){
			 analogInputFail.push_back(pinText);
		 }
	}
}

/* check Analog Input Test Errors and Fail vectors
 */
void AnalogInputTestProcess::checkAnalogInputTestResults(){
	string failedAnalogInputTestMessage = "-> " + to_string(analogInputFail.size()) + " failed Analog Input Test";
	failedAnalogInputTestMessage = Helper::addVectorStringToText(failedAnalogInputTestMessage, analogInputFail);
	string errorAnalogInputTestMessage = "-> " + to_string(analogInputError.size()) + " Analog Input Errors";
	errorAnalogInputTestMessage = Helper::addVectorStringToText(errorAnalogInputTestMessage, analogInputError);
	controller->logger->printToFileLogger("  Analog Input Test Result: ", true);
	controller->logger->printToFileLogger(failedAnalogInputTestMessage, false);
    controller->logger->printToFileLogger(errorAnalogInputTestMessage , false);
    if((analogInputFail.size() == 0) && (analogInputError.size() == 0)){
    	controller->logger->printToFileLogger("=> Analog Input Test pass, No Errors", true);
    }else{
    	controller->logger->printToFileLogger("=> Analog Input Test fail", true);
    }

}



int AnalogInputTestProcess::getFailedAnalogInput(){
	return analogInputFail.size();
}

int AnalogInputTestProcess::getErrorsAnalogInput(){
	return analogInputError.size();
}

string AnalogInputTestProcess::getFailedAnalogInputText(){
	return Helper::convertGpiosStrinVectorToString(analogInputFail);
}

string AnalogInputTestProcess::getErrorsAnalogInputText(){
	return Helper::convertGpiosStrinVectorToString(analogInputError);
}

void AnalogInputTestProcess::resetAnalogInputResultDatabase(){
	analogInputTestResultVector.clear();
	for(uint8_t positionAnalogPinVector = 0; positionAnalogPinVector < controller->board_analog_pins_vector.size(); positionAnalogPinVector++){
		Analog_Input_Test_Result_Struct testResult = {};
	  	uint16_t pin = static_cast<uint16_t>(controller->board_analog_pins_vector.at(positionAnalogPinVector));
		uint8_t port = static_cast<uint8_t>(controller->analog_ports_vector.at(positionAnalogPinVector));
		testResult.pin  = Helper::getPortText(port) + to_string(pin);
		testResult.measuredVotalge1 = -1;
		testResult.measuredVotalge2 = -1;
		testResult.measuredVotalge3 = -1;
		testResult.measuredVotalge4 = -1;
		testResult.comment = "testing";
//		controller->database.updateBoardAnalogInputData(&testResult);
		analogInputTestResultVector.push_back(testResult);
	}
}
