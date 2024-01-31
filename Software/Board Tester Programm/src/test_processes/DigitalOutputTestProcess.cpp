/*
 * DigitalOutputTestProcess.cpp
 *
 *  Created on: 29.05.2023
 *      Author: Astra
 */

#include "DigitalOutputTestProcess.hpp"


DigitalOutputTestProcess::DigitalOutputTestProcess() {
	controller =  NULL;
}


DigitalOutputTestProcess::~DigitalOutputTestProcess() {

}

/*  set Peripheral Controller
 * Parameter:
 *_controller: Peripheral Controller
 */
void DigitalOutputTestProcess::setController(PeripheralsInterfacesController *_controller){
	controller = _controller;
 }

/* Main method to test all GPIO Output
 * Test Output with No Pull, Pull Down and Pull Up with difference speed
 * Parameter:
 *isAppCommand: to update Database
 */
void DigitalOutputTestProcess::testAllGpiosOutput(){
	gpiosOutputTestResultVector.clear();
 	digitalOutputError.clear();
	digitalOutputFail.clear();
	controller->measureVddAndGetGpioOutputVoltagesLevels();
	int testNumber = 0;
	for(uint16_t positionPullVector = 0; positionPullVector < controller->pullTypes.size(); positionPullVector++){
		for(uint16_t positionSpeedVector = 0; positionSpeedVector < controller->speed.size(); positionSpeedVector++){
			for(uint8_t positionGpioPinVector = 0; positionGpioPinVector < controller->board_gpio_pins_vector.size(); positionGpioPinVector++){
				GPIO_Initial_Struct gpioInitialStructure = getDigitalOutputInitialStructure(positionGpioPinVector, positionPullVector, positionSpeedVector);
				controller->logger->printTestReportGpioNumberText(testNumber, &gpioInitialStructure);
				if(Helper::isGpioTestable(gpioInitialStructure.Port, gpioInitialStructure.Pin)){
					int multiplexersConfigError = controller->openMultiplexerConnectionToGpio(positionGpioPinVector);
					if(multiplexersConfigError < 0){
						handleOutputMultiplexersConnectionFailure(&gpioInitialStructure, multiplexersConfigError);
					}else{
						if(Helper::isGpioConnectedToPushButton(&gpioInitialStructure)){
							testGpioOutputConnectedToPushButton(&gpioInitialStructure);
						}else{
							testGpioOutput(&gpioInitialStructure);
						}
					}
				}else{
					 handleNotTestableGpioOutput(&gpioInitialStructure);
				}
				testNumber++;
			}
		}
	}
	controller->mulutiplexerController.setMultiplexer7Conenction(Mulitplexer7_Config::free);
	controller->logger->printSectionSeperator(false);
	checkGpiosOutputTestResults();
}

/* handle Multiplexers connection to GPIO Pin Error
 * Parameter:
 *isAppCommand: to update Database
 * gpioInitialStructure:  GPIO initial Structure
 *configureMultiplexerAndSelectPinAck: Error Acknowledge
 */
void DigitalOutputTestProcess::handleOutputMultiplexersConnectionFailure(GPIO_Initial_Struct *gpioInitialStructure, int configureMultiplexerAndSelectPinAck){
	GPIO_Output_Test_result_Struct gpioOutputResult = {};
	gpioOutputResult.pin = gpioInitialStructure->pinText;
	gpioOutputResult.PullType = gpioInitialStructure->Pull;
 	gpioOutputResult.speed = gpioInitialStructure->Speed;
 	gpioOutputResult.isGpioOutputTestOk = false;
 	string errorMessage = Helper::getConnectionToPinTextError(configureMultiplexerAndSelectPinAck);
 	controller->logger->printToFileLogger("->Error GPIO Output Test: " + errorMessage, false);
	gpioOutputResult.comment = errorMessage;
 	gpioOutputResult.setHighLevelState = GPIO_FAILED_MULTIPLEXER_ERROR;
 	gpioOutputResult.setLowLevelState = GPIO_FAILED_MULTIPLEXER_ERROR;
 	addGpioTextToVector(gpioInitialStructure->pinText, false);
 	gpiosOutputTestResultVector.push_back(gpioOutputResult);
}

/* get structure to initialize GPIO Output
 * Parameter:
 *positionPullVector: to get GPIO pull type
 *positionGpioPinVector: to get GPIO pin and port
 *positionSpeedVector: to get GPIO speed
 */
GPIO_Initial_Struct DigitalOutputTestProcess::getDigitalOutputInitialStructure(uint8_t positionGpioPinVector, uint16_t positionPullVector, uint16_t positionSpeedVector){
	uint8_t pin = static_cast<uint8_t>(controller->board_gpio_pins_vector.at(positionGpioPinVector));
	uint8_t port = static_cast<uint8_t>(controller->board_gpio_ports_vector.at(positionGpioPinVector));
	uint16_t pullType = static_cast<uint16_t>(controller->pullTypes.at(positionPullVector));
	uint16_t speed =  static_cast<uint16_t>(controller-> speed.at(positionSpeedVector));
	return Helper::getInitialStructure(pin, port, pullType, COMMAND_Init_DIGITAL_OUTPUT_PP, speed);
}

/* test GPIO Output connected to Push-Button with Pull-Up-Resistor, test with No-Pull and Low-Speed configuration
 * this test assume that Pull-Up-Resistor OK and GPIO connected to Pull-Up-Resistor
 * Parameter:
 * isAppCommand: to update Database in case of GPIO Output initialization failure
 *gpioInitialStructure: initialization structure of the GPIO Output
*/
void DigitalOutputTestProcess::testGpioOutputConnectedToPushButton(GPIO_Initial_Struct * gpioInitialStructure){
	GPIO_Output_Test_result_Struct  gpioOutputResult = {};
	gpioOutputResult.pin = gpioInitialStructure->pinText;
	gpioOutputResult.PullType = gpioInitialStructure->Pull;
	gpioOutputResult.speed = gpioInitialStructure->Speed;
	if(gpioInitialStructure->Pull == COMMAND_Init_NO_PULL){
		 if(setMux7AndCheckAck(&gpioOutputResult) > 0){
			 gpioInitialStructure->Mode = COMMAND_DeInit;
			 GPIO_INITIAL_STATE  gpioInitialDeInitState = controller->initialGpioAndCheckGpiosAckRegisters(gpioInitialStructure);
			 if(gpioInitialDeInitState == GPIO_INITIAL_STATE::initialised){
				 if(getAdcVoltageAndCheckAck(false, &gpioOutputResult)> 0){
					controller->logger->printToFileLogger("Pin is connected to Pull-Up-Resistor, measured Voltage:  " + to_string(gpioOutputResult.voltageHighLevel) +" V", false);
					gpioInitialStructure->Mode = COMMAND_Init_DIGITAL_OUTPUT_PP;
					GPIO_INITIAL_STATE  gpioInitialState = controller->initialGpioAndCheckGpiosAckRegisters(gpioInitialStructure);
					if(gpioInitialState == GPIO_INITIAL_STATE::initialised){
						OUTPUT_HANDLE_STATE setGpioLowLevelAck = controller->setGpioOutputLevelAndCheckOutputRegister(gpioInitialStructure, COMMAND_WRITE_DIGITIAL_OUTPUT_LEVEL_LOW);
						if(getAdcVoltageAndCheckAck(true, &gpioOutputResult)> 0){
							controller->logger->printToFileLogger("Measure Voltage After set GPIO Output LOW, Voltage:  " + to_string(gpioOutputResult.voltageLowLevel) +" V", false);
							gpioOutputResult.isGpioOutputTestOk = isGpioOutputConnectedToPushButtonOk(&gpioOutputResult, setGpioLowLevelAck);
						}else{
							controller->logger->printToFileLogger("->Error: measure Low Voltage failed cause ADC", false);
						}
					}else{
						addGpioTextToVector(gpioInitialStructure->pinText, false);
						saveGpioOutputInitialErrorAndHandle(&gpioOutputResult, gpioInitialState);
					}
				 }else{
					controller->logger->printToFileLogger("->Error: measure High Voltage failed cause ADC", false);
				 }
 			 }else{
 				addGpioTextToVector(gpioInitialStructure->pinText, false);
				saveGpioOutputInitialErrorAndHandle(&gpioOutputResult, gpioInitialDeInitState);
			 }
			controller->mulutiplexerController.setMultiplexer7Conenction(Mulitplexer7_Config::free);
		 }
	}else{
		gpioOutputResult.voltageHighLevel = TEST_RESET_VALUE;
		gpioOutputResult.voltageLowLevel = TEST_RESET_VALUE;
		gpioOutputResult.comment = GPIO_INCOMAPTIBLE_PULL_MODE;
		controller->logger->printToFileLogger(GPIO_INCOMAPTIBLE_PULL_MODE, false);
	}
 	gpiosOutputTestResultVector.push_back(gpioOutputResult);
}

/* control GPIO Output connected to Push-Button Test result return true if no Errors
 * gpioOutputResult: structure contains Test result
 * setGpioLowLevelAck: Acknowledge of Set GPIO Output LOW
*/
bool DigitalOutputTestProcess::isGpioOutputConnectedToPushButtonOk(GPIO_Output_Test_result_Struct *gpioOutputResult, OUTPUT_HANDLE_STATE setGpioLowLevelAck){
 	string setGpioRegisterLowLevelAckText = Helper::getHandleRegistersAckText(setGpioLowLevelAck);
  	controller->logger->printToFileLogger("GPIO Output Registers: SET LOW ACK:  " + setGpioRegisterLowLevelAckText , false);
 	if(setGpioLowLevelAck == OUTPUT_HANDLE_STATE::gpioSetLowSuccess){
		if(controller->isOutputVoltageLevelMeasurementOk(true, gpioOutputResult)){
			return true;
		}else{
			addGpioTextToVector(gpioOutputResult->pin, true);
			return false;
		}
	}else{
		addGpioTextToVector(gpioOutputResult->pin, true);
		gpioOutputResult->comment = setGpioRegisterLowLevelAckText ;
		return false;
	}
}

/*
 * test digital GPIO Output except PC13 (GPIO connected to Push-Button)
 * Parameter:
 * isAppCommand: to update Database
 * gpioInitialStructure: initial structure of the GPIO Output
 */
void DigitalOutputTestProcess::testGpioOutput(GPIO_Initial_Struct * gpioInitialStructure){
	GPIO_Output_Test_result_Struct  gpioOutputResult = {};
 	gpioOutputResult.pin = gpioInitialStructure->pinText;
	gpioOutputResult.PullType = gpioInitialStructure->Pull;
	gpioOutputResult.speed = gpioInitialStructure->Speed;
	if(setMux7AndCheckAck(&gpioOutputResult) > 0){
		GPIO_INITIAL_STATE  gpioInitialState = controller->initialGpioAndCheckGpiosAckRegisters(gpioInitialStructure);
		if(gpioInitialState == GPIO_INITIAL_STATE::initialised){
			OUTPUT_HANDLE_STATE setGpioHighLevelAck = controller->setGpioOutputLevelAndCheckOutputRegister(gpioInitialStructure, COMMAND_WRITE_DIGITIAL_OUTPUT_LEVEL_HIGH);
			if(getAdcVoltageAndCheckAck(false, &gpioOutputResult)> 0){
				controller->logger->printToFileLogger("Measure Voltage After set GPIO Output HIGH, Voltage:  " + to_string(gpioOutputResult.voltageHighLevel) +" V", false);
				OUTPUT_HANDLE_STATE setGpioLowLevelAck = controller->setGpioOutputLevelAndCheckOutputRegister(gpioInitialStructure, COMMAND_WRITE_DIGITIAL_OUTPUT_LEVEL_LOW);
				if(getAdcVoltageAndCheckAck(true, &gpioOutputResult)> 0){
					controller->logger->printToFileLogger("Measure Voltage After set GPIO Output LOW, Voltage:  " + to_string(gpioOutputResult.voltageLowLevel) +" V", false);
					gpioOutputResult.isGpioOutputTestOk = controller->isGpioOutputOk(&gpioOutputResult, setGpioHighLevelAck, setGpioLowLevelAck);
					gpioOutputResult.setHighLevelState = static_cast<int>(setGpioHighLevelAck);
					gpioOutputResult.setLowLevelState = static_cast<int>(setGpioLowLevelAck);
					if(gpioOutputResult.isGpioOutputTestOk){
						controller->logger->printToFileLogger("=>GPIO Output OK", false);
					}else{
						addGpioTextToVector(gpioInitialStructure->pinText, true);
						controller->logger->printToFileLogger("=>GPIO Output Error", false);
					}
				}else{
					controller->logger->printToFileLogger("->Error: measure low Voltage failed", false);
				}
			}else{
				controller->logger->printToFileLogger("->Error: measure High Voltage failed", false);
			}
		}else{
			addGpioTextToVector(gpioInitialStructure->pinText, false);
			saveGpioOutputInitialErrorAndHandle(&gpioOutputResult, gpioInitialState);
		}
		controller->mulutiplexerController.setMultiplexer7Conenction(Mulitplexer7_Config::free);
	}
 	gpiosOutputTestResultVector.push_back(gpioOutputResult);

}

/* set Multiplexer 7 to connect ADC
 * Parameter:
 *gpioOutputResult: GPIO Output Test Result Structure
 */
int DigitalOutputTestProcess::setMux7AndCheckAck(GPIO_Output_Test_result_Struct*  gpioOutputResult){
	int ack = controller->mulutiplexerController.setMultiplexer7Conenction(Mulitplexer7_Config::AdcConnection);
	if(ack != static_cast<int>(Mulitplexer7_Config::AdcConnection)){
		addGpioTextToVector(gpioOutputResult->pin, false);
		controller->logger->printToFileLogger("->Error: failed to set ADC Connection to GPIO cause Multiplexer 7", false);
		gpioOutputResult->isGpioOutputTestOk = false;
		gpioOutputResult->setHighLevelState = GPIO_FAILED_MULTIPLEXER_ERROR;
		gpioOutputResult->setLowLevelState = GPIO_FAILED_MULTIPLEXER_ERROR;
		gpioOutputResult->voltageHighLevel = TEST_RESET_VALUE;
		gpioOutputResult->voltageLowLevel = TEST_RESET_VALUE;
 		return -1;
	}else{
		return 1;
	}
}

/* measure ADC Voltage and check Result
 * Parameter:
 * isLowVoltageMeasurement: is measure Low Level Voltage
 * gpioOutputResult: GPIO Output Test Result Structure
 */
int DigitalOutputTestProcess::getAdcVoltageAndCheckAck(bool isLowVoltageMeasurement, GPIO_Output_Test_result_Struct*  gpioOutputResult){
	double measuredVoltage = controller->adcHandler.measureVoltage();
	if(measuredVoltage == MEASURE_VOLTAGE_ERROR){
		addGpioTextToVector(gpioOutputResult->pin, false);
		gpioOutputResult->isGpioOutputTestOk = false;
		if(isLowVoltageMeasurement){
			gpioOutputResult->setLowLevelState = MEASURE_VOLTAGE_ERROR;
		}else{
			gpioOutputResult->setHighLevelState = MEASURE_VOLTAGE_ERROR;
		}
		gpioOutputResult->voltageLowLevel = TEST_RESET_VALUE;
		gpioOutputResult->voltageHighLevel = TEST_RESET_VALUE;
		return -1;
	}else{
		if(isLowVoltageMeasurement){
			gpioOutputResult->voltageLowLevel = measuredVoltage;
 		}else{
			gpioOutputResult->voltageHighLevel = measuredVoltage;
 		}
		return 1;
	}
}

/* add initial Error to Test vector and handle initialize failure
 * Parameter:
 * gpioOutputResult: GPIO Output Test Result Structure
 * gpioInitialState: GPIO initialization state
 */
void DigitalOutputTestProcess::saveGpioOutputInitialErrorAndHandle(GPIO_Output_Test_result_Struct *gpioOutputResult, GPIO_INITIAL_STATE  gpioInitialState){
	gpioOutputResult->isGpioOutputTestOk = false;
	if(gpioInitialState == GPIO_INITIAL_STATE::errorSendCommand || gpioInitialState == GPIO_INITIAL_STATE::errorReadCommand){
		gpioOutputResult->setHighLevelState = GPIO_TEST_UART_FAILED;
		gpioOutputResult->setLowLevelState = GPIO_TEST_UART_FAILED;
	}else{
		gpioOutputResult->setHighLevelState = GPIO_FAILED_INITIAL_ERROR;
		gpioOutputResult->setLowLevelState = GPIO_FAILED_INITIAL_ERROR;
	}
	gpioOutputResult->voltageHighLevel = GPIO_FAILED_INITIAL_ERROR;
	gpioOutputResult->voltageLowLevel = GPIO_FAILED_INITIAL_ERROR;
	gpioOutputResult->comment = Helper::getGpioInitialErrorText(gpioInitialState);
}


/* update Database if errors occurs while opening Multiplexer connection to GPIO Pin
 * Parameter:
 * gpioInitialStructure: GPIO Initial Structure
 * configureMultiplexerAndSelectPinAck: Multiplexer Error Acknowledge
 */
void DigitalOutputTestProcess::updateDataBaseCauseMultiplexerConnectionError(GPIO_Initial_Struct *gpioInitialStructure, int configureMultiplexerAndSelectPinAck){
	GPIO_Output_Test_result_Struct gpioOutputResult = {};
	gpioOutputResult.pin = gpioInitialStructure->Pin;
	gpioOutputResult.PullType = gpioInitialStructure->Pull;
 	gpioOutputResult.speed = gpioInitialStructure->Speed;
 	gpioOutputResult.isGpioOutputTestOk = false;
 	gpioOutputResult.voltageHighLevel = -1;
 	gpioOutputResult.voltageLowLevel = -1;
	gpioOutputResult.comment =Helper::getConnectionToPinTextError(configureMultiplexerAndSelectPinAck);
//	controller->database.updateBoardGpioOutputData(gpioOutputResult);
 	gpiosOutputTestResultVector.push_back(gpioOutputResult);

}


/* add GPIO Text to string Vector
 * Parameter:
 * gpioText: GPIO
 * isError: to get if error or fail
 */
void DigitalOutputTestProcess::addGpioTextToVector(string gpioText, bool isError){
	if(isError){
		auto iterator = std::find(digitalOutputError.begin(), digitalOutputError.end(), gpioText);
		 if(iterator == digitalOutputError.end()){
			 digitalOutputError.push_back(gpioText);
		 }
	}else{
		auto iterator = std::find(digitalOutputFail.begin(), digitalOutputFail.end(), gpioText);
		 if(iterator == digitalOutputFail.end()){
			 digitalOutputFail.push_back(gpioText);
		 }
	}
}

/* check Digital Output Test Errors and Fail vectors
 */
void DigitalOutputTestProcess::checkGpiosOutputTestResults(){
	string failedDigitalInputTestMessage = "-> " + to_string(digitalOutputFail.size()) + " failed Digital Output Test";
	failedDigitalInputTestMessage = Helper::addVectorStringToText(failedDigitalInputTestMessage, digitalOutputFail);
	string errorDigitalInputTestMessage = "-> " + to_string(digitalOutputError.size()) + " Digital Output Errors";
	errorDigitalInputTestMessage = Helper::addVectorStringToText(errorDigitalInputTestMessage, digitalOutputError);
	controller->logger->printToFileLogger("  Digital Output Test Result: ", true);
	controller->logger->printToFileLogger(failedDigitalInputTestMessage, false);
    controller->logger->printToFileLogger(errorDigitalInputTestMessage , false);
    if((digitalOutputFail.size() == 0) && (digitalOutputError.size() == 0)){
    	controller->logger->printToFileLogger("=> Digital Output Test pass, No Errors", true);
    }else{
    	controller->logger->printToFileLogger("=> Digital Output Test fail", true);
    }
}

int DigitalOutputTestProcess::getFailedGpioOutput(){
	return digitalOutputFail.size();
}

int DigitalOutputTestProcess::getErrorsGpiosOutput(){
	return digitalOutputError.size();
}

string DigitalOutputTestProcess::getFailedGpioOutputText(){
	return Helper::convertGpiosStrinVectorToString(digitalOutputFail);
}

string DigitalOutputTestProcess::getErrorsGpiosOutputText(){
	return Helper::convertGpiosStrinVectorToString(digitalOutputError);
}

/* handle not testable GPIOs Output
 * Parameter:
 * isApp: to update Database
 * gpioInitialStructure: GPIO initialization structure
 */
void DigitalOutputTestProcess::handleNotTestableGpioOutput(GPIO_Initial_Struct *gpioInitialStructure){
	string speedText = Helper::getSpeedText(gpioInitialStructure->Speed);
	string portText = Helper::getPortText(gpioInitialStructure->Port);
	string pinNotConnectedText =  portText + to_string(gpioInitialStructure->Pin) + " is not testable";
	string pinText = Helper::getPortText(gpioInitialStructure->Port) + to_string(gpioInitialStructure->Pin);
	controller->logger->printToFileLogger(pinNotConnectedText, false);
	GPIO_Output_Test_result_Struct testResult = {};
	testResult.voltageHighLevel = 0;
	testResult.voltageLowLevel = 0;
	testResult.speed = gpioInitialStructure->Speed;
	testResult.pin = gpioInitialStructure->pinText;
	testResult.PullType = gpioInitialStructure->Pull;
	testResult.comment = "not testable";
	gpiosOutputTestResultVector.push_back(testResult);
}
