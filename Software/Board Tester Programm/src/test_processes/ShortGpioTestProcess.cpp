/*
 * ShortGpioTestProcess.cpp
 *
 *  Created on: 07.06.2023
 *      Author: Astra
 */
#include "ShortGpioTestProcess.hpp"


ShortGpioTestProcess::ShortGpioTestProcess() {
	controller  = NULL;
}

ShortGpioTestProcess::~ShortGpioTestProcess() {

}

/*  set Peripheral Controller
 * Parameter:
 *_controller: Peripheral Controller
 */
void ShortGpioTestProcess::setController(PeripheralsInterfacesController *_controller){
	controller = _controller;
}

/*  main method to test GPIO Short Circuits
 * Parameter:
 *isAppCommand: to update Database
 */
void ShortGpioTestProcess::testAllGpiosShortCircuit(){
	gpiosShortCircuitsTestResultVector.clear();
 	shortCircuitsVector.clear();
	gpioFailedTestShortCircuits.clear();
	controller->measureVddAndGetGpioOutputVoltagesLevels();
 	for(uint8_t positionGpioPinVector = 0; positionGpioPinVector < controller->board_gpio_pins_vector.size(); positionGpioPinVector++){
 		uint8_t pin = static_cast<uint8_t>(controller->board_gpio_pins_vector.at(positionGpioPinVector));
		uint8_t port = static_cast<uint8_t>(controller->board_gpio_ports_vector.at(positionGpioPinVector));
		GPIO_Initial_Struct gpioInitialStructure =  Helper::getInitialStructure(pin, port, COMMAND_Init_NO_PULL, COMMAND_Init_DIGITAL_OUTPUT_PP, COMMAND_Init_GPIO_SPEED_FREQ_LOW);
		controller->logger->printTestReportGpioShortNumberText(positionGpioPinVector, &gpioInitialStructure);
 		if(Helper::isGpioTestable(gpioInitialStructure.Port, gpioInitialStructure.Pin)){
 			int multiplexersConfigError = controller->openMultiplexerConnectionToGpio(positionGpioPinVector);
			if(multiplexersConfigError < 0){
				addFailedGpioToVector(gpioInitialStructure.pinText);
 				controller->logger->printToFileLogger("-> Error ShortGpioTestProcess/testShortCircuitGpios failed to configure Multiplexers Connection to GPIO", false);
				updateDataBaseCauseMultiplexerConnectionError(gpioInitialStructure.pinText, multiplexersConfigError);
			}else{
				if(Helper::isGpioConnectedToPushButton(&gpioInitialStructure)){
					testShortCircuitsWithPushButtonGpio(positionGpioPinVector, gpioInitialStructure.Pin, gpioInitialStructure.Port);
				}else{
					setGpioOutputAndTestShortCircuits(positionGpioPinVector, &gpioInitialStructure);
				}
			}
 		}else{
 			controller->logger->printToFileLogger("-> GPIO is not testable", false);
 			GPIO_SHORT_TEST_RESULT notTestableGpio = {};
 			notTestableGpio.gpio = gpioInitialStructure.pinText;
 			notTestableGpio.detectedShortCircuits = -1;
 			notTestableGpio.comment = "not testable";
  			gpiosShortCircuitsTestResultVector.push_back(notTestableGpio);
  		}
	}
	controller->mulutiplexerController.setMultiplexer7Conenction(Mulitplexer7_Config::free);
	controller->logger->printSectionSeperator(false);
	cout << "short Circuit vector size: " << shortCircuitsVector.size() << endl;
	checkAndPrintGpiosShortCircuitsResult();
}

/*  test GPIOs Short Circuits with PC13 (User Push Button)
 * Parameter:
 * positionGpioPinVector: position from GPIOs Vector used to open Multiplexers Connection to GPIO
 * isAppCommand: to update Database
 * pin: GPIO Pin
 * port: GPIO Port
 */
void ShortGpioTestProcess::testShortCircuitsWithPushButtonGpio(uint8_t positionGpioPinVector, uint8_t pin, uint8_t port){
	GPIO_Initial_Struct gpioInitialStructure =  Helper::getInitialStructure(pin, port, COMMAND_Init_NO_PULL, COMMAND_Init_DIGITAL_INPUT, COMMAND_Init_GPIO_SPEED_FREQ_LOW);
	GPIO_SHORT_TEST_RESULT shortCircuitTestResult = {};
	GPIO_Output_Test_result_Struct gpioOutputTestResult = {};
 	int shortCircuitsDetected = 0;
	int restHighGpios = 0;
	gpioOutputTestResult.pin = gpioInitialStructure.pinText;
	gpioOutputTestResult.PullType = gpioInitialStructure.Pull;
	gpioOutputTestResult.speed = gpioInitialStructure.Speed;
	shortCircuitTestResult.gpio = gpioInitialStructure.pinText;
 	GPIO_INITIAL_STATE  gpioInitialState = controller->initialGpioAndCheckGpiosAckRegisters(&gpioInitialStructure);
	if(gpioInitialState == GPIO_INITIAL_STATE::initialised){
	  	if(setGpioOutputHighAndCheck(&gpioInitialStructure, &gpioOutputTestResult) > 0){
	  		restHighGpios = measureVoltageRestGpiosHighState(gpioInitialStructure.pinText);
	  		int multiplexersConfigError = controller->openMultiplexerConnectionToGpio(positionGpioPinVector);
			if(multiplexersConfigError < 0){
				addFailedGpioToVector(gpioInitialStructure.pinText);
				controller->logger->printToFileLogger("-> Error ShortGpioTestProcess/testShortCircuitsWithPushButtonGpio failed to configure Multiplexers Connection to GPIO", false);
				updateDataBaseCauseMultiplexerConnectionError(gpioInitialStructure.pinText, multiplexersConfigError);
			}else{
		  		GPIO_Initial_Struct gpioInitialLowStructure =  Helper::getInitialStructure(pin, port, COMMAND_Init_NO_PULL, COMMAND_Init_DIGITAL_OUTPUT_PP, COMMAND_Init_GPIO_SPEED_FREQ_LOW);
		  	 	GPIO_INITIAL_STATE  gpioInitialStateLow = controller->initialGpioAndCheckGpiosAckRegisters(&gpioInitialLowStructure);
		  		if(gpioInitialStateLow == GPIO_INITIAL_STATE::initialised){
					if(setGpioOutputLowAndCheck(&gpioInitialLowStructure, &gpioOutputTestResult) > 0){
						if(restHighGpios > 0){
							shortCircuitsDetected = measureVoltageRestGpiosLowState(gpioInitialLowStructure.pinText);
						}else{
							controller->logger->printToFileLogger("=>No Short Circuits detected", false);
						}
						shortCircuitTestResult.detectedShortCircuits = shortCircuitsDetected;
						shortCircuitTestResult.comment = getShortCircuitsGpioText();
						gpiosShortCircuitsTestResultVector.push_back(shortCircuitTestResult);
					}else{
				  		shortCircuitTestResult.detectedShortCircuits = -1;
				  		shortCircuitTestResult.comment = "failed to set GPIO LOW";
			  			gpiosShortCircuitsTestResultVector.push_back(shortCircuitTestResult);
					}
				}else{
					addFailedGpioToVector(gpioInitialLowStructure.pinText);
			 		 handleInitalizeFailure(gpioInitialState, &gpioInitialStructure);
				}
			}
	  	}else{
	  		shortCircuitTestResult.detectedShortCircuits = -1;
	  		shortCircuitTestResult.comment = "failed PC13 is level is Low";
  			gpiosShortCircuitsTestResultVector.push_back(shortCircuitTestResult);
	  	}
	}else{
		addFailedGpioToVector(gpioInitialStructure.pinText);
		 handleInitalizeFailure(gpioInitialState, &gpioInitialStructure);
	}
}



/* set GPIO PIN X to Output, change Output LEVEL HIGH, LOW and check Connected GPIOS
 * Parameter:
 *isAppCommand: to update Database
 *gpioInitialStructure: GPIO initialization Structure of GPIO X
 *gpioShortCircuitTestResult: structure to save detected Short Circuits
 */
void ShortGpioTestProcess::setGpioOutputAndTestShortCircuits(uint8_t positionGpioPinVector,  GPIO_Initial_Struct * gpioInitialStructure){
	GPIO_Output_Test_result_Struct  gpioOutputTestResult = {};
	GPIO_SHORT_TEST_RESULT shortCircuitTestResult = {};
 	int shortCircuitsDetected = 0;
	int restHighGpios = 0;
	shortCircuitTestResult.gpio = gpioInitialStructure->pinText;
	gpioOutputTestResult.pin = gpioInitialStructure->pinText;
	gpioOutputTestResult.PullType = gpioInitialStructure->Pull;
	gpioOutputTestResult.speed = gpioInitialStructure->Speed;
 	GPIO_INITIAL_STATE  gpioInitialState = controller->initialGpioAndCheckGpiosAckRegisters(gpioInitialStructure);
	if(gpioInitialState == GPIO_INITIAL_STATE::initialised){
	  	if(setGpioOutputHighAndCheck(gpioInitialStructure, &gpioOutputTestResult) > 0){
			restHighGpios = measureVoltageRestGpiosHighState(gpioInitialStructure->pinText);
			int multiplexersConfigError = controller->openMultiplexerConnectionToGpio(positionGpioPinVector);
			if(multiplexersConfigError < 0){
				addFailedGpioToVector(gpioInitialStructure->pinText);
 				controller->logger->printToFileLogger("-> Error ShortGpioTestProcess/setGpioOutputAndTestShortCircuits failed to configure Multiplexers Connection to GPIO", false);
				updateDataBaseCauseMultiplexerConnectionError(gpioInitialStructure->pinText, multiplexersConfigError);
			}else{
				if(setGpioOutputLowAndCheck(gpioInitialStructure, &gpioOutputTestResult) > 0){
					if(restHighGpios > 0){
						shortCircuitsDetected = measureVoltageRestGpiosLowState(gpioInitialStructure->pinText);
					}else{
						controller->logger->printToFileLogger("=>No Short Circuits detected", false);
					}
					shortCircuitTestResult.detectedShortCircuits = shortCircuitsDetected;
					shortCircuitTestResult.comment = getShortCircuitsGpioText();
					gpiosShortCircuitsTestResultVector.push_back(shortCircuitTestResult);
				}else{
					shortCircuitTestResult.detectedShortCircuits = -1;
					shortCircuitTestResult.comment = "failed to set GPIO LOW";
					gpiosShortCircuitsTestResultVector.push_back(shortCircuitTestResult);
				}
			}
	  	}else{
			shortCircuitTestResult.detectedShortCircuits = -1;
			shortCircuitTestResult.comment = "failed to set GPIO HIGH";
			gpiosShortCircuitsTestResultVector.push_back(shortCircuitTestResult);
	  	}
	}else{
		addFailedGpioToVector(gpioInitialStructure->pinText);
 		 handleInitalizeFailure(gpioInitialState, gpioInitialStructure);
	}

}

/* set GPIO Output High and control Result
 * Parameter:
 * gpioInitialStructure: GPIO Output Initial Structure
 * gpioOutputTestResult: GPIO Output Test Result Structure
 */
int ShortGpioTestProcess::setGpioOutputHighAndCheck(GPIO_Initial_Struct * gpioInitialStructure, GPIO_Output_Test_result_Struct  *gpioOutputTestResult){
	if(Helper::isGpioConnectedToPushButton(gpioInitialStructure)){
		gpioOutputTestResult->voltageHighLevel = controller->getAdcVoltage();
		controller->logger->printToFileLogger("->" + gpioInitialStructure->pinText+ " is connected to Pull-Up Resistor, Voltage:  " + to_string(gpioOutputTestResult->voltageHighLevel) +" V", false);
		if((gpioOutputTestResult->voltageHighLevel < controller->voltageRefernces.LOW_OUTPUT_VOLTAGE_MAX_VALUE)){
			controller->logger->printToFileLogger("-> Test Failed cause: failed to set GPIO HIGH Level", false);
			addFailedGpioToVector(gpioInitialStructure->pinText);
			return -1;
		}
	}else{
		OUTPUT_HANDLE_STATE setGpioHighLevelAck = controller->setGpioOutputLevelAndCheckOutputRegister(gpioInitialStructure, COMMAND_WRITE_DIGITIAL_OUTPUT_LEVEL_HIGH);
		gpioOutputTestResult->voltageHighLevel = controller->getAdcVoltage();
		controller->logger->printToFileLogger("->" + gpioInitialStructure->pinText+ " Voltage After set Output Register HIGH, Voltage:  " + to_string(gpioOutputTestResult->voltageHighLevel) +" V", false);
		if(!isOuputHighLevelOk(setGpioHighLevelAck, gpioOutputTestResult->voltageHighLevel)){
			controller->logger->printToFileLogger("-> Test Failed cause: failed to set GPIO HIGH Level", false);
	 		addFailedGpioToVector(gpioInitialStructure->pinText);
			return -1;
		}
	}
	return 1;
}

/* set GPIO Output Low and control Result
 * Parameter:
 * gpioInitialStructure: GPIO Output Initial Structure
 * gpioOutputTestResult: GPIO Output Test Result Structure
 */
int ShortGpioTestProcess::setGpioOutputLowAndCheck(GPIO_Initial_Struct * gpioInitialStructure, GPIO_Output_Test_result_Struct  *gpioOutputTestResult){
	OUTPUT_HANDLE_STATE setGpioLowLevelAck = controller->setGpioOutputLevelAndCheckOutputRegister(gpioInitialStructure, COMMAND_WRITE_DIGITIAL_OUTPUT_LEVEL_LOW);
	gpioOutputTestResult->voltageLowLevel = controller->getAdcVoltage();
	controller->logger->printToFileLogger("->" + gpioInitialStructure->pinText+ " Voltage After set Output Register LOW, Voltage:  " + to_string(gpioOutputTestResult->voltageLowLevel) +" V", false);
	if(!isOuputLowLevelOk(setGpioLowLevelAck, gpioOutputTestResult->voltageLowLevel)){
		controller->logger->printToFileLogger("-> Test Failed cause: failed to set GPIO LOW Level", false);
 		addFailedGpioToVector(gpioInitialStructure->pinText);
		return -1;
	}
	return 1;
}

/* measure Voltage rest GPIOs while GPIO X Output set HIGH
 * Parameter:
 *pinTextHighOutput: GPIO X
 *gpioShortCircuitTestResult: structure to save detected Short Circuits
 */
int ShortGpioTestProcess::measureVoltageRestGpiosHighState(string pinTextHighOutput){
	int restGpiosStateHighShortCircuit = 0;
	highVoltagerRestGpiosVector.clear();
	controller->logger->printToFileLogger("CHECK REST GPIOs WHILE GPIO" + pinTextHighOutput + " HIGH", false);
	for(uint8_t position = 0; position < controller->board_gpio_pins_vector.size(); position++){
		uint8_t pin = static_cast<uint8_t>(controller->board_gpio_pins_vector.at(position));
		uint8_t port = static_cast<uint8_t>(controller->board_gpio_ports_vector.at(position));
		string pintText = Helper::getPortText(port) + to_string(pin);
		if(pintText != pinTextHighOutput){
			if(Helper::isGpioTestable(port, pin)){
 				int configureMultiplexerAndSelectPinAck = controller->openMultiplexerConnectionToGpio(position);
 				if(configureMultiplexerAndSelectPinAck < 0){
					controller->logger->printToFileLogger("-> Error: ShortGpioTestProcess/measureVoltageRestGpiosHighState failed to open Connection to GPIO: "+ pintText, true);
 					addFailedGpioToVector(pintText);
 				}else{
					double voltage = controller->getAdcVoltage();
 					if(voltage >= controller->voltageRefernces.LOW_OUTPUT_VOLTAGE_MAX_VALUE){
 						controller->logger->printToFileLogger("   => GPIO: " + Helper::getPortText(port) + to_string(pin) +": HIGH !" +  " measured Voltage: " + to_string(voltage) +" V", false);
						highVoltagerRestGpiosVector.push_back(position);
						restGpiosStateHighShortCircuit ++;
					}
				}
			}
		}
	}
	return restGpiosStateHighShortCircuit ;
}

/* measure Voltage rest GPIOs while GPIO X Output set LOW
 * Parameter:
 *pinTextHighOutput: GPIO X
 *gpioShortCircuitTestResult: structure to save detected Short Circuits
 */
int ShortGpioTestProcess::measureVoltageRestGpiosLowState(string pinTextLowOutput){
 	controller->logger->printToFileLogger("CHECK REST GPIOs WHILE" + pinTextLowOutput + " LOW", false);
	int gpiosShortCircuits = 0;
	gpiosShortVector.clear();
	for(uint8_t position = 0; position < highVoltagerRestGpiosVector.size(); position++){
		uint8_t pinPositoionGpioPinVector = static_cast<uint8_t>(highVoltagerRestGpiosVector.at(position));
		uint8_t pin = static_cast<uint8_t>(controller->board_gpio_pins_vector.at(pinPositoionGpioPinVector));
		uint8_t port = static_cast<uint8_t>(controller->board_gpio_ports_vector.at(pinPositoionGpioPinVector));
		string pintText = Helper::getPortText(port) + to_string(pin);
  		int configureMultiplexerAndSelectPinAck = controller->openMultiplexerConnectionToGpio(pinPositoionGpioPinVector);
		if(configureMultiplexerAndSelectPinAck < 0){
 			controller->logger->printToFileLogger("-> Error: ShortGpioTestProcess/measureVoltageRestGpiosLowState failed to open Connection to GPIO: "+ pintText, true);
 			addFailedGpioToVector(pintText);
		}else{
			double voltage = controller->getAdcVoltage();
  			if(voltage <= controller->voltageRefernces.HIGH_OUTPUT_VOLTAGE_MIN_VALUE){
  				string pin1 = pinTextLowOutput;
  				string pin2 = Helper::getPortText(port) + to_string(pin);
  			   if (!hasDuplicate(pin1, pin2)) {
  					cout << "short Circuit added pin1: " << pin1 << " & pin2" << pin2 << endl;
  				   shortCircuitsVector.push_back({pin1, pin2});
				}
  			   gpiosShortVector.push_back(pin2);
  			   controller->logger->printToFileLogger("   => GPIO: " + Helper::getPortText(port) + to_string(pin) +": LOW !" +  " measured Voltage: " + to_string(voltage) +" V", false);
  			   controller->logger->printToFileLogger("   => GPIO Short Circuit detected with"  + Helper::getPortText(port) + to_string(pin), false);
  			   gpiosShortCircuits++;
			}
		}
	}
	if(gpiosShortCircuits == 0){
		controller->logger->printToFileLogger("=>No Short Circuits detected", false);
	}
	return gpiosShortCircuits;
}


/* control if Short Circuits Vector has duplicate
 * gpio1: GPIO Number 1
 * gpio2: GPIO Number 2
 */
bool ShortGpioTestProcess::hasDuplicate(const std::string& gpio1, const std::string& gpio2){
    auto iterator = std::find_if(shortCircuitsVector.begin(), shortCircuitsVector.end(), [gpio1, gpio2](const ShortCircuit& element) {
         return (element.pin1 == gpio1 && element.pin2 == gpio2) || (element.pin1 == gpio2 && element.pin2 == gpio1);
     });
     return (iterator != shortCircuitsVector.end());
}

/* get Text with All detected GPIOs connected to GPIO X
 */
string ShortGpioTestProcess::getShortCircuitsGpioText(){
	string shortCircuitText = "";
	for(uint8_t position = 0; position < gpiosShortVector.size(); position++){
		string shortedGpio = static_cast<string>(gpiosShortVector.at(position));
		shortCircuitText += shortedGpio;
	}
	gpiosShortVector.clear();
	return shortCircuitText;
}

/* update Database if errors occurs while opening Multiplexer connection to GPIO Pin
 */
void ShortGpioTestProcess::updateDataBaseCauseMultiplexerConnectionError(string pinText, int configureMultiplexerAndSelectPinAck){
	GPIO_SHORT_TEST_RESULT shortCircuitTestResult = {};
	string comment =Helper::getConnectionToPinTextError(configureMultiplexerAndSelectPinAck);
	shortCircuitTestResult.gpio = pinText;
	shortCircuitTestResult.detectedShortCircuits = MULTIPLEXERS_ERROR_GPIO_SHORT;
	shortCircuitTestResult.comment = comment;
	gpiosShortCircuitsTestResultVector.push_back(shortCircuitTestResult);

//	controller->database.updateGpioShortCircuitsData(isAppCommand, pinText, MULTIPLEXERS_ERROR_GPIO_SHORT, comment);
}

/* Check GPIO set HIGH Level Register success and voltage OK
 * Parameter:
 * setGpioLowLevelAck: Acknowledge Register Set
 * voltage: measured voltage
 */
bool ShortGpioTestProcess::isOuputHighLevelOk(OUTPUT_HANDLE_STATE setGpioHighLevelAck , double voltage){
	if((setGpioHighLevelAck == OUTPUT_HANDLE_STATE::gpioSetHighSuccess) && (voltage >= controller->voltageRefernces.LOW_OUTPUT_VOLTAGE_MAX_VALUE)){
		return true;
	}else{
		return false;
	}
}

/* Check GPIO set LOW Level Register success and voltage OK
 * Parameter:
 * setGpioLowLevelAck: Acknowledge Register Set
 * voltage: measured voltage
 */
bool ShortGpioTestProcess::isOuputLowLevelOk(OUTPUT_HANDLE_STATE setGpioLowLevelAck , double voltage){
	if((setGpioLowLevelAck == OUTPUT_HANDLE_STATE::gpioSetLowSuccess) && (voltage < controller->voltageRefernces.LOW_OUTPUT_VOLTAGE_MAX_VALUE)){
		return true;
	}else{
		return false;
	}
}

/* add GPIO Text to string Vector
 * Parameter:
 * gpioText: GPIO
 * isError: to get if error or fail
 */
void ShortGpioTestProcess::addFailedGpioToVector(string gpioText){
	auto iterator = std::find(gpioFailedTestShortCircuits.begin(), gpioFailedTestShortCircuits.end(), gpioText);
	 if(iterator == gpioFailedTestShortCircuits.end()){
		 gpioFailedTestShortCircuits.push_back(gpioText);
	 }
}


/* control Test Results
 */

void ShortGpioTestProcess::checkAndPrintGpiosShortCircuitsResult(){
	string failedShortCircuitsTestMessage = "-> " + to_string(gpioFailedTestShortCircuits.size()) + " failed Test";
	string shortCircuitsTestMessage = "-> " + to_string(shortCircuitsVector.size()) + " Short Circuits detected";
	failedShortCircuitsTestMessage = Helper::addVectorStringToText(failedShortCircuitsTestMessage, gpioFailedTestShortCircuits);
	controller->logger->printToFileLogger(" GPIOs Short Circuit Test Result: ", true);
	controller->logger->printToFileLogger(failedShortCircuitsTestMessage, false);
	controller->logger->printToFileLogger(shortCircuitsTestMessage, false);
    for (const auto& shortCircuit : shortCircuitsVector) {
     	controller->logger->printToFileLogger("  => Short circuit between Pin " + shortCircuit.pin1  +  " and Pin " + shortCircuit.pin2 +" detected!", true);
    }
    if(gpioFailedTestShortCircuits.size() == 0 && shortCircuitsVector.size() == 0){
    	controller->logger->printToFileLogger("=> Test pass: 0 Short Circuits, 0 Errors", true);
	}else{
		controller->logger->printToFileLogger("=> GPIOs Short Circuits Test fail", true);
	}
}


int ShortGpioTestProcess::getFailedGpioShortTest(){
	return gpioFailedTestShortCircuits.size();
}

int ShortGpioTestProcess::getShortedGpios(){
	return shortCircuitsVector.size();
}

string ShortGpioTestProcess::getFailedGpioShortTestText(){
	return Helper::convertGpiosStrinVectorToString(gpioFailedTestShortCircuits);
}

string ShortGpioTestProcess::getShortedGpiosText(){
	string shortedGpios = "";
     for (size_t i = 0; i < shortCircuitsVector.size(); ++i) {
    	 shortedGpios += "(";
    	 shortedGpios += shortCircuitsVector[i].pin1;
    	 shortedGpios += " , ";
    	 shortedGpios += shortCircuitsVector[i].pin2;
    	 shortedGpios += ")";
        if (i < shortCircuitsVector.size() - 1) {
        	shortedGpios += " , ";
        }
    }
     return shortedGpios;
}


/* handle GPIO initialization failure
 * Parameter:
 * gpioInitialState: GPIO initialization state
 * isApp: to update Database
 * gpioInitialStructure: GPIO initialization Structure
 */
void ShortGpioTestProcess::handleInitalizeFailure(GPIO_INITIAL_STATE  gpioInitialState, GPIO_Initial_Struct * gpioInitialStructure){
	GPIO_SHORT_TEST_RESULT gpioShortInitialFailStruct = {};
	string gpioInitialErrorText = Helper::getGpioInitialErrorText(gpioInitialState);
	controller->logger->printToFileLogger("->Error failed to initial GPIO: " +  gpioInitialErrorText, false);
	gpioShortInitialFailStruct.gpio = Helper::getPortText(gpioInitialStructure->Port) + to_string(gpioInitialStructure->Pin);
	gpioShortInitialFailStruct.detectedShortCircuits = OUPUT_INITIAL_FAILURE_SHORT;
	gpioShortInitialFailStruct.comment = gpioInitialErrorText;

	gpiosShortCircuitsTestResultVector.push_back(gpioShortInitialFailStruct);

}
