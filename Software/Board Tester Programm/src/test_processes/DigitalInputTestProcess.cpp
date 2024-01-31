/*
 * DigitalInputTestProcess.cpp
 *
 *  Created on: 29.05.2023
 *      Author: Astra
 */


#include "DigitalInputTestProcess.hpp"


DigitalInputTestProcess::DigitalInputTestProcess() {
	controller =  NULL;
}

DigitalInputTestProcess::~DigitalInputTestProcess() {

}

/*  set Peripheral Controller
 * Parameter:
 *_controller: Peripheral Controller
 */
void DigitalInputTestProcess::setController(PeripheralsInterfacesController *_controller){
	controller = _controller;
}

/*  main method to test GPIOs Input
 * Test GPIO Input with No Pull, Pull Down and Pull Up configuration
 * Parameter:
 *isAppCommand: to update Database
 */
void DigitalInputTestProcess::testAllGpiosInput() {
	gpiosInputTestResultVector.clear();
	digitalInputError.clear();
	digitalInputFail.clear();
	controller->measureVddAndGetGpioInputVoltagesLevels();
  	for(uint16_t positionPullVector=0; positionPullVector < controller->pullTypes.size(); positionPullVector++){
		for(uint8_t positionGpioPinVector = 0; positionGpioPinVector < controller->board_gpio_pins_vector.size(); positionGpioPinVector++){
			GPIO_Initial_Struct gpioInitialStructure = getDigitalInputInitialStructureFromVectorPosition(positionPullVector, positionGpioPinVector);
			controller->logger->printTestReportGpioNumberText(positionGpioPinVector, &gpioInitialStructure);
			if(Helper::isGpioTestable(gpioInitialStructure.Port, gpioInitialStructure.Pin)){
				int multiplexersConfigError = controller->openMultiplexerConnectionToGpio(positionGpioPinVector);
					if(multiplexersConfigError < 0){
						handleInputMultiplexersConnectionFailure( &gpioInitialStructure, multiplexersConfigError);
					}else{
						if(Helper::isGpioConnectedToPushButton(&gpioInitialStructure)){
							testGpioInputConnectedToPushButton(&gpioInitialStructure);
						}else{
							testSingleGpioInput(&gpioInitialStructure);
						}
					}
			}else{
				 handleNotTestableGpioInput(&gpioInitialStructure);
			}
 		}
		controller->logger->printTestSectionHeaderSeperator();
	}
	controller->mulutiplexerController.setMultiplexer7Conenction(Mulitplexer7_Config::free);
	controller->logger->printSectionSeperator(false);
	checkGpiosInputTestResults();
}

/* handle open Multiplexers connections to GPIO Pin
 * Parameter:
 *isAppCommand: to update Database
 *gpioInitialStructure: GPIO initial structure
 *configureMultiplexerAndSelectPinAck: Acknowledge from Multiplexers to define the Error
 */
void DigitalInputTestProcess::handleInputMultiplexersConnectionFailure(GPIO_Initial_Struct *gpioInitialStructure, int configureMultiplexerAndSelectPinAck){
	GPIO_Input_Test_Result_Struct testResult = {};
	testResult.pin = gpioInitialStructure->pinText;
	testResult.PullType = gpioInitialStructure->Pull;
	string errorMessage = Helper::getConnectionToPinTextError(configureMultiplexerAndSelectPinAck);
 	controller->logger->printToFileLogger("->Error GPIO Input Test: " + errorMessage, false);
 	testResult.comment = errorMessage;
	testResult.highLevelState = GPIO_FAILED_MULTIPLEXER_ERROR;
	testResult.lowLevelState = GPIO_FAILED_MULTIPLEXER_ERROR;
 	addGpioTextToVector(gpioInitialStructure->pinText, false);
 	gpiosInputTestResultVector.push_back(testResult);
}

/* return a structure to initial GPIO Input
 * Parameter:
 *positionPullVector: to get the Pull Type
 *positionGpioPinVector: to get the Pin and Port of the GPIO Input
 */
GPIO_Initial_Struct DigitalInputTestProcess::getDigitalInputInitialStructureFromVectorPosition(uint8_t positionPullVector, uint8_t positionGpioPinVector){
 	uint16_t pullType = static_cast<uint16_t>(controller->pullTypes.at(positionPullVector));
 	uint16_t pin = static_cast<uint16_t>(controller->board_gpio_pins_vector.at(positionGpioPinVector));
	uint8_t port = static_cast<uint8_t>(controller->board_gpio_ports_vector.at(positionGpioPinVector));
	return Helper::getInitialStructure(pin, port, pullType, COMMAND_Init_DIGITAL_INPUT, COMMAND_Init_GPIO_SPEED_FREQ_LOW);
}

/*
 * test digital GPIO Input
 * Parameter:
 * isAppCommand: to update Database
 * gpioInitialStructure: initial Values of the GPIO Pin
 */
void DigitalInputTestProcess::testSingleGpioInput(GPIO_Initial_Struct * gpioInitialStructure){
	GPIO_INITIAL_STATE  gpioInitialState =  controller->initialGpioAndCheckGpiosAckRegisters(gpioInitialStructure);
	if(gpioInitialState == GPIO_INITIAL_STATE::initialised){
		if(gpioInitialStructure->Pull == COMMAND_Init_PULL_DOWN){
			testDigitalInputPullDown(gpioInitialStructure);
		}else if(gpioInitialStructure->Pull == COMMAND_Init_PULL_UP){
			testDigitalInputPullUp(gpioInitialStructure);
		}else{
			testDigitalInputNoPull(gpioInitialStructure);
		}
	}else{
		addGpioTextToVector(gpioInitialStructure->pinText, false);
		saveGpioInputInitialErrorAndHandle(gpioInitialState, gpioInitialStructure);
 	}
}

/*
 * test digital GPIO Input connected to PC13 PUSH Button
 * Parameter:
 * isAppCommand: to update Database
 * gpioInitialStructure: initial Values of the GPIO Pin
 */
void DigitalInputTestProcess::testGpioInputConnectedToPushButton(GPIO_Initial_Struct * gpioInitialStructure){
	GPIO_Input_Test_Result_Struct testResult = {};
	testResult.pin = gpioInitialStructure->pinText;
	testResult.PullType = gpioInitialStructure->Pull;
	if(gpioInitialStructure->Pull == COMMAND_Init_NO_PULL){
		GPIO_INITIAL_STATE  gpioInitialState =  controller->initialGpioAndCheckGpiosAckRegisters(gpioInitialStructure);
		if(gpioInitialState == GPIO_INITIAL_STATE::initialised){
			GPIO_Level_STATE ackNoPullHighState  = readGpioDigitalInputState(gpioInitialStructure);
			testResult.highLevelState = checkInputHighLevelStateOk(ackNoPullHighState, gpioInitialStructure->pinText);
			int ackMux7Dac = setMux7AndDac(0);
	 	 	controller->logger->printToFileLogger("DAC SET LOW MAX VALUE: 0V", false);
		 	if(ackMux7Dac < 1){
		 		addGpioTextToVector(gpioInitialStructure->pinText, false);
		 		testResult.lowLevelState = ackMux7Dac;
				testResult.comment =  GPIO_TEST_ERROR;
		 	}else{
			  	GPIO_Level_STATE ackPullUpLowState  = readGpioDigitalInputState(gpioInitialStructure);
	 		 	testResult.lowLevelState = checkInputLowLevelStateOk(ackPullUpLowState, gpioInitialStructure->pinText);
	 			if((ackNoPullHighState == GPIO_Level_STATE::high) && (ackPullUpLowState == GPIO_Level_STATE::low)){
					testResult.comment =  OK;
					controller->logger->printToFileLogger(" => GPIO INPUT OK", false);
				}else{
					addGpioTextToVector(gpioInitialStructure->pinText, true);
 					testResult.comment = GPIO_TEST_ERROR;
					controller->logger->printToFileLogger(" => GPIO INPUT Error", false);
				}
		 	}
		}else{
			addGpioTextToVector(gpioInitialStructure->pinText, false);
			saveGpioInputInitialErrorAndHandle(gpioInitialState, gpioInitialStructure);
		}
	}else{
		testResult.highLevelState = TEST_RESET_VALUE;
		testResult.lowLevelState = TEST_RESET_VALUE;
		testResult.comment = GPIO_INCOMAPTIBLE_PULL_MODE;
		controller->logger->printToFileLogger(GPIO_INCOMAPTIBLE_PULL_MODE, false);
	}
 	gpiosInputTestResultVector.push_back(testResult);

}

/* add initial Error to Test vector and handle initialize failure
 * Parameter:
 * isAppCommand: to update Database
 * gpioInitialState: GPIO initialization state
 * gpioInitialStructure: GPIO initialization Structure
 */
void DigitalInputTestProcess::saveGpioInputInitialErrorAndHandle(GPIO_INITIAL_STATE  gpioInitialState, GPIO_Initial_Struct * gpioInitialStructure){
	GPIO_Input_Test_Result_Struct testResult = {};
	testResult.pin = gpioInitialStructure->pinText;
	testResult.PullType = gpioInitialStructure->Pull;
	if(gpioInitialState == GPIO_INITIAL_STATE::errorSendCommand || gpioInitialState == GPIO_INITIAL_STATE::errorReadCommand){
		testResult.lowLevelState = GPIO_TEST_UART_FAILED;
		testResult.highLevelState = GPIO_TEST_UART_FAILED;
	}else{
		testResult.lowLevelState = GPIO_FAILED_INITIAL_ERROR;
		testResult.highLevelState = GPIO_FAILED_INITIAL_ERROR;
	}
	testResult.comment = Helper::getGpioInitialErrorText(gpioInitialState);
 	gpiosInputTestResultVector.push_back(testResult);

}
/*
 * test digital GPIO Input with no pull
 * Parameter:
 * isAppCommand: to update Database
 * gpioInitialStructure: initial Values of the GPIO Pin
 */
void DigitalInputTestProcess::testDigitalInputNoPull(GPIO_Initial_Struct * gpioInitialStructure){
	GPIO_Input_Test_Result_Struct  testResult = {};
	testResult.pin = gpioInitialStructure->pinText;
	testResult.PullType = gpioInitialStructure->Pull;
 	controller->logger->printToFileLogger("DAC SET HIGH MIN VALUE: "+ to_string(controller->voltageRefernces.HIGH_INPUT_VOLTAGE_MIN_VALUE) + " V", false);
	int ackMux7Dac = setMux7AndDac(controller->voltageRefernces.HIGH_INPUT_VOLTAGE_MIN_VALUE);
 	if(ackMux7Dac < 1){
 		addGpioTextToVector(gpioInitialStructure->pinText, false);
 		testResult.highLevelState = ackMux7Dac;
		testResult.comment =  GPIO_TEST_ERROR;
 	}else{
 	  	GPIO_Level_STATE ackNoPullHighState  = readGpioDigitalInputState(gpioInitialStructure);
 	 	testResult.highLevelState = checkInputHighLevelStateOk(ackNoPullHighState, gpioInitialStructure->pinText);
 	 	controller->logger->printToFileLogger("DAC SET LOW MAX VALUE: " + to_string(controller->voltageRefernces.LOW_INPUT_VOLTAGE_MAX_VALUE)+ " V", false);
 		int ackMux7Dac = setMux7AndDac(controller->voltageRefernces.LOW_INPUT_VOLTAGE_MAX_VALUE);
 	 	if(ackMux7Dac < 1){
 	 		addGpioTextToVector(gpioInitialStructure->pinText, false);
 	 		testResult.lowLevelState = ackMux7Dac;
 			testResult.comment =  GPIO_TEST_ERROR;
 	 	}else{
 	 	   	GPIO_Level_STATE ackNoPullLowState = readGpioDigitalInputState(gpioInitialStructure);
 	 	  	testResult.lowLevelState = checkInputLowLevelStateOk(ackNoPullLowState, gpioInitialStructure->pinText);
 	 	  	if(isInputTestResultOk(ackNoPullHighState, ackNoPullLowState)){
 	 	  		testResult.comment =  OK;
 	 	   	}else{
 	 	   		addGpioTextToVector(gpioInitialStructure->pinText, true);
 	 	  		testResult.comment =  GPIO_TEST_ERROR;
 	 	  	}
 	 	}
 	}
 	gpiosInputTestResultVector.push_back(testResult);
 	controller->mulutiplexerController.setMultiplexer7Conenction(Mulitplexer7_Config::free);
 }

/*
 * test digital GPIO Input with pull down
 * Parameter:
 * isAppCommand: to update Database
 * gpioInitialStructure: initial Values of the GPIO Pin
 */
void DigitalInputTestProcess::testDigitalInputPullDown(GPIO_Initial_Struct * gpioInitialStructure){
	GPIO_Input_Test_Result_Struct testResult = {};
	testResult.pin = gpioInitialStructure->pinText;
	testResult.PullType = gpioInitialStructure->Pull;
 	GPIO_Level_STATE ackPullDownLowState  = readGpioDigitalInputState(gpioInitialStructure);
	testResult.lowLevelState = checkInputLowLevelStateOk(ackPullDownLowState, gpioInitialStructure->pinText);
  	controller->logger->printToFileLogger("DAC SET HIGH MIN VALUE: " + to_string(controller->voltageRefernces.HIGH_INPUT_VOLTAGE_MIN_VALUE)+ " V", false);
	int ackMux7Dac = setMux7AndDac(controller->voltageRefernces.HIGH_INPUT_VOLTAGE_MIN_VALUE);
 	if(ackMux7Dac < 1){
 		addGpioTextToVector(gpioInitialStructure->pinText, false);
 		testResult.highLevelState = ackMux7Dac;
		testResult.comment =  GPIO_TEST_ERROR;
 	}else{
 	  	GPIO_Level_STATE ackPullDownHighState  = readGpioDigitalInputState(gpioInitialStructure);
 	 	testResult.highLevelState = checkInputHighLevelStateOk(ackPullDownHighState, gpioInitialStructure->pinText);
 		if(isInputTestResultOk(ackPullDownHighState, ackPullDownLowState)){
 			testResult.comment =  OK;
 		}else{
 			addGpioTextToVector(gpioInitialStructure->pinText, true);
 			testResult.comment =  GPIO_TEST_ERROR;
 		}
 	}

 	gpiosInputTestResultVector.push_back(testResult);
 	controller->mulutiplexerController.setMultiplexer7Conenction(Mulitplexer7_Config::free);
}

/*
 * test digital GPIO Input with pull up
 * Parameter:
 * isAppCommand: to update Database
 * gpioInitialStructure: initial Values of the GPIO Pin
 */
void DigitalInputTestProcess::testDigitalInputPullUp(GPIO_Initial_Struct * gpioInitialStructure){
	GPIO_Input_Test_Result_Struct testResult = {};
	testResult.pin = gpioInitialStructure->pinText;
	testResult.PullType = gpioInitialStructure->Pull;
	GPIO_Level_STATE ackPullUpHighState  = readGpioDigitalInputState(gpioInitialStructure);
	testResult.highLevelState = checkInputHighLevelStateOk(ackPullUpHighState, gpioInitialStructure->pinText);
	controller->logger->printToFileLogger("DAC SET Voltage: 0 Volt", false);
	int ackMux7Dac = setMux7AndDac(0);
 	if(ackMux7Dac < 1){
 		addGpioTextToVector(gpioInitialStructure->pinText, false);
 		testResult.lowLevelState = ackMux7Dac;
		testResult.comment =  GPIO_TEST_ERROR;
 	}else{
 	  	GPIO_Level_STATE ackPullUpLowState  = readGpioDigitalInputState(gpioInitialStructure);
 	 	testResult.lowLevelState = checkInputLowLevelStateOk(ackPullUpLowState, gpioInitialStructure->pinText);
 		if(isInputTestResultOk(ackPullUpHighState, ackPullUpLowState)){
 			testResult.comment =  OK;
 		}else{
 			addGpioTextToVector(gpioInitialStructure->pinText, true);
 			testResult.comment =  GPIO_TEST_ERROR;
 		}
 	}
 	gpiosInputTestResultVector.push_back(testResult);
	controller->mulutiplexerController.setMultiplexer7Conenction(Mulitplexer7_Config::free);
}

/*
 * set Multiplexer 7 connection and DAC Voltage
 * Parameter:
 * voltage: DAC Voltage
 */
int DigitalInputTestProcess::setMux7AndDac(double voltage){
	if(controller->mulutiplexerController.setMultiplexer7Conenction(Mulitplexer7_Config::DacConnection) == MUX7_CONTROLL_ERROR){
		controller->logger->printToFileLogger("-> Error DigitalInputTestProcess/setMux7AndDac failed to configure Multiplexer 7", false);
		return GPIO_FAILED_MULTIPLEXER_ERROR;
	}else{
		if(controller->dacHandler.setDacVoltage(voltage) == DAC_SET_VALUE_ERROR){
			controller->logger->printToFileLogger("-> Error DigitalInputTestProcess/setMux7AndDac failed to set DAC Voltage", false);
			return DAC_SET_VALUE_ERROR;
		}else{
			return 1;
		}
	}
}

/*
 * get the GPIO Input State HIGH/LOW
 * Parameter:
 * pinNumber: GPIO Pin Number
 * port:  GPIO Port
 */
GPIO_Level_STATE DigitalInputTestProcess::readGpioDigitalInputState(GPIO_Initial_Struct * gpioInitialStructure){
	if(controller->handleGpio(gpioInitialStructure->Pin, gpioInitialStructure->Port,  COMMAND_READ_DIGITAL_LEVEL_STATE) < 0){
		addGpioTextToVector(gpioInitialStructure->pinText, false);
 		return GPIO_Level_STATE::failedToSendCommand;
	}else{
		uint8_t *readBuffer  =  controller->uartHandler.readUartData(UART_ACK_SIZE);
		GPIO_Registers_Struct gpio_registers_state = Helper::getNucleoBoardRegisters(readBuffer);
		if(readBuffer != nullptr){
			delete[] readBuffer;
		}else{
			controller->logger->printToFileLogger("-> Error DigitalInputTestProcess/readGpioDigitalInputState failed to read UART Data", false);
 			addGpioTextToVector(gpioInitialStructure->pinText, false);
			return GPIO_Level_STATE::failedToReadLevelState;
		}
		return controller->getGpioLevelState(&gpio_registers_state, gpioInitialStructure->Pin, COMMAND_Init_DIGITAL_INPUT);
	}
}

/*
 * check if GPIO Input LOW and HIGH Level State valid
 * Parameter:
 * highLevelTest: GPIO HIGH Level State defined in common.hpp
 * lowLevelTest: GPIO LOW Level State defined in common.hpp
 */
bool DigitalInputTestProcess::isInputTestResultOk(GPIO_Level_STATE highLevelTest, GPIO_Level_STATE lowLevelTest){
	if((highLevelTest == GPIO_Level_STATE::high) && (lowLevelTest == GPIO_Level_STATE::low)){
		controller->logger->printToFileLogger(" => GPIO INPUT OK", false);
		return true;
	}else{
		controller->logger->printToFileLogger(" => GPIO INPUT Error", false);
		return false;
	}
}

/*
 * check if GPIO Input HIGH Level State valid
 * Parameter:
 * levelState: GPIO Level State defined in common.hpp
 */
int DigitalInputTestProcess::checkInputHighLevelStateOk(GPIO_Level_STATE levelState, string gpioText){
	if(levelState == GPIO_Level_STATE::high){
		controller->logger->printToFileLogger("Pin State: HIGH", false);
		return GPIO_INPUT_HIGH_LEVEL_OK;
	}else if(levelState == GPIO_Level_STATE::low){
		controller->logger->printToFileLogger("Error DigitalInputTestProcess/checkInputHighLevelStateOk: High level state Error cause current state Low", false);
		return GPIO_INPUT_HIGH_LEVEL_ERROR;
	}else{
		controller->logger->printToFileLogger("Error DigitalInputTestProcess/checkInputHighLevelStateOk: failed to get GPIO Level State", false);
		addGpioTextToVector(gpioText, false);
		return GPIO_TEST_UART_FAILED;
	}
}

/*
 * check if GPIO Input LOW Level State valid
 * Parameter:
 * levelState: GPIO Level State defined in common.hpp
 */
int DigitalInputTestProcess::checkInputLowLevelStateOk(GPIO_Level_STATE levelState, string gpioText){
	if(levelState == GPIO_Level_STATE::low){
		controller->logger->printToFileLogger("Pin State: LOW", false);
		return GPIO_INPUT_LOW_LEVEL_OK;
	}else if(levelState == GPIO_Level_STATE::high){
		controller->logger->printToFileLogger("Error DigitalInputTestProcess/checkInputLowLevelStateOk: Low level state Error cause current state High", false);
		return GPIO_INPUT_LOW_LEVEL_ERROR;
	}else{
		controller->logger->printToFileLogger("Error DigitalInputTestProcess/checkInputLowLevelStateOk: failed to get GPIO Level State", false);
		addGpioTextToVector(gpioText, false);
		return GPIO_TEST_UART_FAILED;
	}
}

/* add GPIO Text to string Vector
 * Parameter:
 * gpioText: GPIO
 * isError: to get if error or fail
 */
void DigitalInputTestProcess::addGpioTextToVector(string gpioText, bool isError){
	if(isError){
		auto iterator = std::find(digitalInputError.begin(), digitalInputError.end(), gpioText);
		 if(iterator == digitalInputError.end()){
			 digitalInputError.push_back(gpioText);
		 }
	}else{
		auto iterator = std::find(digitalInputFail.begin(), digitalInputFail.end(), gpioText);
		 if(iterator == digitalInputFail.end()){
			 digitalInputFail.push_back(gpioText);
		 }
	}
}

/* check Digital Input Test Errors and Fail vectors
 */
void DigitalInputTestProcess::checkGpiosInputTestResults(){
	string failedDigitalInputTestMessage = "-> " + to_string(digitalInputFail.size()) + " failed Digital Input Test";
	failedDigitalInputTestMessage = Helper::addVectorStringToText(failedDigitalInputTestMessage, digitalInputFail);
	string errorDigitalInputTestMessage = "-> " + to_string(digitalInputError.size()) + " Digital Input Errors";
	errorDigitalInputTestMessage = Helper::addVectorStringToText(errorDigitalInputTestMessage, digitalInputError);
	controller->logger->printToFileLogger("  Digital Input Test Result: ", true);
	controller->logger->printToFileLogger(failedDigitalInputTestMessage, false);
    controller->logger->printToFileLogger(errorDigitalInputTestMessage , false);
    if((digitalInputFail.size() == 0) && (digitalInputError.size() == 0)){
    	controller->logger->printToFileLogger("=> Digital Input Test pass, No Errors", true);
    }else{
    	controller->logger->printToFileLogger("=> Digital Input Test fail", true);
    }

}

int DigitalInputTestProcess::getFailedGpioInput(){
	return digitalInputFail.size();
}

int DigitalInputTestProcess::getErrorsGpiosInput(){
	return digitalInputError.size();
}

string DigitalInputTestProcess::getFailedGpioInputText(){
	return Helper::convertGpiosStrinVectorToString(digitalInputFail);
}

string DigitalInputTestProcess::getErrorsGpiosInputText(){
	return Helper::convertGpiosStrinVectorToString(digitalInputError);
}


/* handle not testable GPIOs
 * Parameter:
 * isApp: to update Database
 * gpioInitialStructure: GPIO initialization structure
 */
void DigitalInputTestProcess::handleNotTestableGpioInput(GPIO_Initial_Struct *gpioInitialStructure){
	string speedText = Helper::getSpeedText(gpioInitialStructure->Speed);
	string portText = Helper::getPortText(gpioInitialStructure->Port);
	string pinNotConnectedText =  portText + to_string(gpioInitialStructure->Pin) + " is not testable";
	string pinText = Helper::getPortText(gpioInitialStructure->Port) + to_string(gpioInitialStructure->Pin);
	controller->logger->printToFileLogger(pinNotConnectedText, false);
	GPIO_Input_Test_Result_Struct inputTestResult = {};
	inputTestResult.pin = gpioInitialStructure->pinText;
	inputTestResult.PullType = gpioInitialStructure->Pull;
	inputTestResult.comment = "not testable";;
	inputTestResult.highLevelState = TEST_RESET_VALUE;
	inputTestResult.lowLevelState = TEST_RESET_VALUE;
	gpiosInputTestResultVector.push_back(inputTestResult);
}
