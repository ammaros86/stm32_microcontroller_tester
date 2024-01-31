/*
 * ContinuityTestProcess.cpp
 *
 *  Created on: 10.07.2023
 *      Author: Astra
 */

#include "ContinuityTestProcess.hpp"



ContinuityTestProcess::ContinuityTestProcess() {
	controller =  NULL;
	openVoltage = 0;
	shortedVoltage = 0;
}

ContinuityTestProcess::~ContinuityTestProcess() {

}

/*  set Peripheral Controller
 * Parameter:
 *_controller: Peripheral Controller
 */
void ContinuityTestProcess::setController(PeripheralsInterfacesController *_controller){
	controller = _controller;
}

/*  Test GPIOs Continuity
 * Parameter:
 */
void ContinuityTestProcess::testGpiosContinuity(){
   configureContinuityTest();
	for(uint8_t positionGpioPinVector = 0; positionGpioPinVector < controller->board_gpio_pins_vector.size(); positionGpioPinVector++){
		Continuity_Test_Result_Struct testResult = {};
		testResult.pin = getGpioPinText(positionGpioPinVector);
		controller->logger->printToFileLogger("-" + to_string(positionGpioPinVector+1) +" GPIO:" + testResult.pin, false);
		int multiplexersConfigError = controller->openMultiplexerConnectionToGpio(positionGpioPinVector);
		delay(_100MS_DELAY);
		if(multiplexersConfigError > 0){
			testResult.voltage = controller->adcHandler.measureVoltage();
			if(testResult.voltage  == MEASURE_VOLTAGE_ERROR){
				setTestFailed(MEASURE_VOLTAGE_ERROR, &testResult);
			}else{
				checkGpioContinuityTestResult(positionGpioPinVector, &testResult);
			}
		}else{
			setTestFailed(multiplexersConfigError, &testResult);
		}
		continuityTestResultVector.push_back(testResult);
		controller->logger->printSectionSeperator(false);
	}
	printGpiosContinuityTestResult();
	resetConitinuityTestConfiguration();

}


/* Configure Continuity Test
 * Parameter:
 */
void ContinuityTestProcess::configureContinuityTest(){
	continuityTestResultVector.clear();
 	failedTestGpiosVector.clear();
	shortGpiosVector.clear();
	openGpiosVector.clear();
 	controller->turnUsbPortOff();
	delay(_100MS_DELAY);
	controller->mulutiplexerController.setMultiplexer7Conenction(Mulitplexer7_Config::CurrentSourceConnection);
  	controller->mulutiplexerController.setMultiplexer6Conenction(Mux1);
 	controller->connectFrontMultiplexerYtoZpin(Mux1, MuxY15); // GND1
	delay(_20MS_DELAY);
	shortedVoltage = controller->adcHandler.measureVoltage();
 	controller->mulutiplexerController.setMultiplexer6Conenction(Mux3);
 	controller->connectFrontMultiplexerYtoZpin(Mux3, MuxY7); // PA3 standard Open
	delay(_20MS_DELAY);
	openVoltage = controller->adcHandler.measureVoltage();
	if((shortedVoltage > SHORTED_GPIO_DEFAULT_MAX_VOLTAGE) || (shortedVoltage < SHORTED_GPIO_DEFAULT_MIN_VOLTAGE)){
		shortedVoltage = SHORTED_GPIO_DEFAULT_MAX_VOLTAGE;
	}
	if((openVoltage < OPEN_GPIO_DEFAULT_MIN_VOLTAGE) || (openVoltage > OPEN_GPIO_DEFAULT_MAX_VOLTAGE)){
		openVoltage = OPEN_GPIO_DEFAULT_MIN_VOLTAGE;
	}else{
		openVoltage = openVoltage - OPEN_GPIO_CORRECTION_VOLTAGE;
	}
}

/* control Continuity Test Result
 * Parameter:
 *positionGpioPinVector: position from GPIOs Vector
 *testResult: GPIO Continuity Test Result
 */
void ContinuityTestProcess::checkGpioContinuityTestResult(uint8_t positionGpioPinVector, Continuity_Test_Result_Struct *testResult){
	controller->logger->printToFileLogger(Strings::measureVoltage+ to_string(testResult->voltage ), false);
	if((positionGpioPinVector ==  PA2_VECTOR_POSITION) || (positionGpioPinVector == PA3_VECTOR_POSITION) || (positionGpioPinVector == PF0_VECTOR_POSITION)
			|| (positionGpioPinVector == PC14_VECTOR_POSITION) || (positionGpioPinVector == PC15_VECTOR_POSITION)){
		controller->logger->printToFileLogger(Strings::doubleArrow + Strings::gpioStandardOpen, false);
		testResult->comment = Strings::gpioStandardOpen;
  	}else if(testResult->voltage <= shortedVoltage){
		controller->logger->printToFileLogger(Strings::doubleArrow + Strings::shortGpio, false);
		testResult->comment = Strings::shortGpio;
		shortGpiosVector.push_back(testResult->pin);
 	}else if(testResult->voltage >= openVoltage ){
		controller->logger->printToFileLogger(Strings::doubleArrow + Strings::openGpio, false);
		testResult->comment = Strings::openGpio;
		openGpiosVector.push_back(testResult->pin);
 	}else{
		controller->logger->printToFileLogger(Strings::doubleArrow + Strings::gpioOk, false);
		testResult->comment = Strings::ok;
 	}
}

/* print GPIOs Continuity Test Results
 */
void ContinuityTestProcess::printGpiosContinuityTestResult(){
    controller->logger->printToFileLogger(Strings::gpioContinuityTestResult, true);
    if(failedTestGpiosVector.size() > 0 ){
        controller->logger->printToFileLogger(Strings::arrow + to_string(failedTestGpiosVector.size()) + Strings::failedToTest  + "( " + Helper::convertGpiosStrinVectorToString(failedTestGpiosVector) + " )", true);
    }else{
        controller->logger->printToFileLogger(Strings::arrow + to_string(failedTestGpiosVector.size()) + Strings::failedToTest, true);
    }
    if(shortGpiosVector.size() > 0 ){
        controller->logger->printToFileLogger(Strings::arrow + to_string(shortGpiosVector.size()) + Strings::shortedToGndOrVcc + "( " + Helper::convertGpiosStrinVectorToString(shortGpiosVector)  + " )" , true);
    }else{
        controller->logger->printToFileLogger(Strings::arrow + to_string(shortGpiosVector.size()) + Strings::shortedToGndOrVcc, true);
    }
    if(openGpiosVector.size() > 0 ){
         controller->logger->printToFileLogger(Strings::arrow + to_string(openGpiosVector.size()) + Strings::openGpios + "( " + Helper::convertGpiosStrinVectorToString(openGpiosVector)  + " )" , true);
     }else{
         controller->logger->printToFileLogger(Strings::arrow + to_string(openGpiosVector.size()) + Strings::openGpios, true);
     }
    if((failedTestGpiosVector.size() == 0) && (shortGpiosVector.size() == 0) && (openGpiosVector.size() == 0)){
        controller->logger->printToFileLogger(Strings::continuityTestPass , true);
    }else{
    	  controller->logger->printToFileLogger(Strings::continuityTestFail , true);
    }
}

/* Manage Configuration Failure
 * Parameter:
 * error: Error Code
 */
void ContinuityTestProcess::setTestGpiosContinuityFailed(int error){
	string errorText ;
	if(error == TURN_USB_PORT_OFF_ERROR){
		errorText = Strings::failedToTurnUsbPortOff;
	}else{
		errorText = Strings::failedToOpenConnectionToCurrentrSource;
	}
	for(uint8_t positionGpioPinVector = 0; positionGpioPinVector < controller->board_gpio_pins_vector.size(); positionGpioPinVector++){
		Continuity_Test_Result_Struct testResult = {};
		testResult.pin = getGpioPinText(positionGpioPinVector);
		testResult.voltage = -1;
		testResult.comment = errorText;
		continuityTestResultVector.push_back(testResult);
		failedTestGpiosVector.push_back(testResult.pin);
		controller->logger->printToFileLogger(Strings::gpio + testResult.pin + Strings::failedContinuityTest + errorText, false);
	}
}


/* Get GPIO Text Port:Number
 * Parameter:
 *positionGpioPinVector: position from GPIOs Vector
 */
string ContinuityTestProcess::getGpioPinText(uint8_t positionGpioPinVector){
  	uint16_t pin = static_cast<uint16_t>(controller->board_gpio_pins_vector.at(positionGpioPinVector));
	uint8_t port = static_cast<uint8_t>(controller->board_gpio_ports_vector.at(positionGpioPinVector));
	string pinText = Helper::getPortText(port) + to_string(pin);
	return pinText;
}

/* Set GPIO Continuity Test failed
 * Parameter:
 *error: Error Code
 *testResult: GPIO Continuity Test Result Structure
 */
void ContinuityTestProcess::setTestFailed(int error, Continuity_Test_Result_Struct *testResult){
	if(error == MEASURE_VOLTAGE_ERROR){
		testResult->comment = Strings::measureVoltageFailed;
	}else{
		testResult->comment = Strings::openMultiplexersFailed;
	}
	testResult->voltage = -1;
	failedTestGpiosVector.push_back(testResult->pin);
	controller->logger->printToFileLogger("Error: ContinuityTestProcess ", false);
}

/* reset Continuity Test Configuration
 * Set Multiplexer 7 free
 * Turn USB-Ports ON
 * Initial UART
 */
void ContinuityTestProcess::resetConitinuityTestConfiguration(){
	controller->usbPortshandler.restartUsbPort();
 	controller->checkUsbPortStateAndInitialUart();
    controller->mulutiplexerController.setMultiplexer7Conenction(Mulitplexer7_Config::free);
}

/* return Text contains failed Continuity Test GPIOs
 */
string ContinuityTestProcess::getFailedGpiosText(){
	return Helper::convertGpiosStrinVectorToString(failedTestGpiosVector);
}

/* return open GPIOs Text
 */
string ContinuityTestProcess::getOpenGpiosText(){
	return Helper::convertGpiosStrinVectorToString(openGpiosVector);
}

/* return short GPIOs Text
 */
string ContinuityTestProcess::getShortGpiosText(){
	return Helper::convertGpiosStrinVectorToString(shortGpiosVector);
}

/* return Number of failed Continuity Test
 */
int ContinuityTestProcess::getFailedContinuityTest(){
	return failedTestGpiosVector.size();
}

/* return Number of shorted GPIOs to GND/VCC
 */
int ContinuityTestProcess::getShortGpios(){
	return shortGpiosVector.size();
}

/* return Number of open GPIOs
 */
int ContinuityTestProcess::getOpenGpios(){
	return openGpiosVector.size();
}

/* return Number of All Continuity Test Errors
 */
int ContinuityTestProcess::getTestErrors(){
	return failedTestGpiosVector.size() + shortGpiosVector.size() + openGpiosVector.size();
}

