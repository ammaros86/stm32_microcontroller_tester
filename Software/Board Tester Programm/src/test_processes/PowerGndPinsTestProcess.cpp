/*
 * PowerGndPinsTestProcess.cpp
 *
 *  Created on: 09.08.2023
 *      Author: Astra
 */





#include "PowerGndPinsTestProcess.hpp"



PowerGndPinsTestProcess::PowerGndPinsTestProcess(){
	controller =  NULL;
}

PowerGndPinsTestProcess::~PowerGndPinsTestProcess(){

}

void PowerGndPinsTestProcess::setController(PeripheralsInterfacesController *_controller){
	controller = _controller;
}

/*  initialize Power and GND Vectors
 */
void PowerGndPinsTestProcess::initialBoardPowerPinsTester(){
	powerGndPinsTestResultVector.clear();
	failedPinsVector.clear();
	errorPinsVector.clear();
}

/* Main Method to test Power Pins
 */
void PowerGndPinsTestProcess::testAllGndPowerPins(){
	initialBoardPowerPinsTester();
	Power_GND_TEST_RESULT voltageTestResultStruct = {};
   	for(uint8_t vectorPosition = 0; vectorPosition < controller->powerGndMuxPinVectors.size(); vectorPosition++){
   		string pin = static_cast<string>(controller->powerGndPinsStringVector.at(vectorPosition));
   		POWER_PIN_TYPE pintype = controller->powerGndPinsType.at(vectorPosition);
		controller->logger->printToFileLogger("- " + to_string(vectorPosition+1) + ": "+ pin, false);
 		int openConnectionToPinAck = controller->openMuxConnectionToTestPowerGndPin(true, vectorPosition);
		delay(_50MS_DELAY);
 		if(openConnectionToPinAck < 0){
 			string failureMessage =  Helper::getConnectionToPinTextError(openConnectionToPinAck);
			controller->logger->printToFileLogger(Strings::arrow + " " + Strings::error + " " + Strings::openMultiplexersFailed, false);
			failedPinsVector.push_back(pin);
 			voltageTestResultStruct = {pin, -1, failureMessage};
 			powerGndPinsTestResultVector.push_back(voltageTestResultStruct);
 		}else{
 			float voltage =  controller->getAdcVoltage();
			controller->logger->printToFileLogger(Strings::measuredVoltage+ Helper::convertFloatToString(voltage) + Strings::volt , false);
			powerGndPinsVoltage.push_back(voltage);
 			if(voltage < 0){
 				controller->logger->printToFileLogger(Strings::arrow + " " + Strings::error + " " + Strings::measureVoltageFailed, false);
 				failedPinsVector.push_back(pin);
 				voltageTestResultStruct = {pin, -1, Strings::measureVoltageFailed};
	 			powerGndPinsTestResultVector.push_back(voltageTestResultStruct);
 			}else{
 				 if(((pintype == POWER_PIN_TYPE::GND) && (!isGndPinOk(voltage))) || ((pintype ==POWER_PIN_TYPE::POWER3V) && (!is3VPinnOk(voltage)))  || ((pintype == POWER_PIN_TYPE::POWER5V) &&(!is5VPinOk(voltage))) ||
 						 ((pintype ==POWER_PIN_TYPE::VIN) && (!isGndPinOk(voltage))) || ((pintype ==POWER_PIN_TYPE::E5V) && (!isE5VPinOk(voltage)))	 ){
 					 controller->logger->printToFileLogger(Strings::measuredVoltageOutOfRange , false);
 	 				 errorPinsVector.push_back(pin);
 					voltageTestResultStruct = {pin, voltage, Strings::error};
 				 }else{
 	 				controller->logger->printToFileLogger(Strings::measuredVoltageOk , false);
 					voltageTestResultStruct = {pin, voltage, Strings::ok};
 				 }
 		 			powerGndPinsTestResultVector.push_back(voltageTestResultStruct);
 			}


 		}
 		controller->logger->printSectionSeperator(false);
	}
   	checkGndPowerPinsTest();
}

/* check Power/GND Failed and Errors
 */
void PowerGndPinsTestProcess::checkGndPowerPinsTest(){
	string failedPinsText = Strings::failedTestPin;
	string errorPinsText = Strings::errorPin;
	controller->logger->printToFileLogger(Strings::powerGndPinsTestResult, true);
    failedPinsText += getFailedPinsText();
    errorPinsText += getErrorsPinsText();
 	controller->logger->printToFileLogger(Strings::arrow + to_string(getFailedTest()) + failedPinsText , true);
 	controller->logger->printToFileLogger(Strings::arrow + to_string(getTestErrors()) + errorPinsText , true);
	if((failedPinsVector.size() == 0) && (errorPinsVector.size() == 0)){
    	controller->logger->printToFileLogger(Strings::powerGndPinsTestResultPass, true);
    }else{
    	controller->logger->printToFileLogger(Strings::powerGndPinsTestResultFail, true);
    }
}

/* Get Power and GND Pins Error Text from Vector
 */
string PowerGndPinsTestProcess::getErrorsPinsText(){
	string errorPinsText = "";
    if(errorPinsVector.size() > 0){
        for (size_t testNumber = 0; testNumber < errorPinsVector.size(); ++testNumber) {
        	if (testNumber == 0) {
        		errorPinsText += ": ";
			}
        	errorPinsText += errorPinsVector[testNumber];
        	if (testNumber != errorPinsVector.size() - 1) {
        		errorPinsText += ", ";
        	}
        }
    }
    return errorPinsText;
}

/* Get Power and GND Pins failed Test Text from Vector
 */
string PowerGndPinsTestProcess::getFailedPinsText(){
	string failedPinsText = "";
    if(failedPinsVector.size() > 0){
        for (size_t testNumber = 0; testNumber < failedPinsVector.size(); ++testNumber) {
        	if (testNumber == 0) {
				failedPinsText += ": ";
			}
        	failedPinsText += failedPinsVector[testNumber];
        	if (testNumber != failedPinsVector.size() - 1) {
        		failedPinsText += ", ";
        	}
        }
    }
    return failedPinsText;
}

/*  check E5V Pin Test Result valid
 */
bool PowerGndPinsTestProcess::isE5VPinOk(double voltage){
	return (voltage > controller->voltageRefernces.POWER_VOLTAGE_E5V_MIN && voltage <= controller->voltageRefernces.POWER_VOLTAGE_E5V_MAX);
}


/*  check GND Pin Test Result
 */
bool PowerGndPinsTestProcess::isGndPinOk(double voltage){
	return (voltage >= controller->voltageRefernces.GND_VOLTAGE_MIN &&  voltage < controller->voltageRefernces.GND_VOLTAGE_MAX);
}


/*  check 3V Pin Test Result valid
 */
bool PowerGndPinsTestProcess::is3VPinnOk(double voltage){
 	return (voltage > controller->voltageRefernces.POWER_VOLTAGE_3V3_MIN &&  voltage < controller->voltageRefernces.POWER_VOLTAGE_3V3_MAX);
}

/*  check 5V Pin Test Result valid
 */
bool PowerGndPinsTestProcess::is5VPinOk(double voltage){
 	return (voltage > controller->voltageRefernces.POWER_VOLTAGE_5_MIN &&  voltage < controller->voltageRefernces.POWER_VOLTAGE_5_MAX);
}

/*  reset Power and GND Pins Test Results
 */
void PowerGndPinsTestProcess::resetPowerGndPinsTestData(){
	powerGndPinsVoltage.clear();
	powerGndPinsVoltageTestErros.clear();
}

/* get failed Test Numbers
 */
int PowerGndPinsTestProcess::getFailedTest(){
	return failedPinsVector.size();
}

/* get Errors Numbers
 */
int PowerGndPinsTestProcess::getTestErrors(){
	return errorPinsVector.size();
}


