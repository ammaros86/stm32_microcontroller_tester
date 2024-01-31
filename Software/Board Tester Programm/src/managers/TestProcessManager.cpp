/*
 * TestProcessManager.cpp
 *
 *  Created on: 08.06.2023
 *      Author: Astra
 */


#include "TestProcessManager.hpp"

TestProcessManager::~TestProcessManager() {
    joinTestProcessThread();
}

/* initialize Test Process Manager :
 * initialize MUTEX and set Controller for all Test Processes
 * initialize Test Process State to free
 * initialize current Test Command
 */
void TestProcessManager::initial(){
	if(pthread_mutex_init(&mutex, NULL) != 0){
        controller->logger->printToFileLogger("Error: TestProcessManager failed to initialize MUTEX", true);
    }
	temperatureProcess.setController(controller);
    powerGndPinsTestProcess.setController(controller);
    continuityTestProcess.setController(controller);
    flashProcess.setController(controller);
    digitalOutputTesterProcess.setController(controller);
    digitalInputTesterProcess.setController(controller);
    analogInputTestProcess.setController(controller);
    shortGpiosTestProcess.setController(controller);
}


/* Start Test Process Thread
 * Parameter:
 * testCommand: Test Command
 * _isAppCommand: is Command from Mobile Application
 */
void TestProcessManager::startTestProcessThread(int _boardID, string _boardName, TestCommand testCommand, bool _isAppCommmand, bool isCompleteTest){
	boardID = _boardID;
	boardName = _boardName;
	controller->setCompleteTestStartState(isCompleteTest);
    isAppCommmand = _isAppCommmand;
    switch(testCommand){
    	case TestCommand::continuityTest:
    		workingThread = std::thread(&TestProcessManager::continuityTest, this);
    		joinTestProcessThread();
    		break;
    	case TestCommand::boardCurrent:
    		workingThread = std::thread(&TestProcessManager::measureBoardCurrent, this, isCompleteTest);
    	    joinTestProcessThread();
    	    break;
        case TestCommand::powerGndPinsTest:
            workingThread = std::thread(&TestProcessManager::testPowerGndPins,this);
            joinTestProcessThread();
            break;
        case TestCommand::flashBoard:
            workingThread = std::thread(&TestProcessManager::flashHandle,this);
            joinTestProcessThread();
            break;
        case TestCommand::measureTemperature:
            workingThread = std::thread(&TestProcessManager::getBoardTemperature,this, isCompleteTest);
    	    joinTestProcessThread();
            break;
        case TestCommand::testGpioInput:
            workingThread = std::thread(&TestProcessManager::testGpioInput,this);
    	    joinTestProcessThread();
            break;
        case TestCommand::testGpioOutput:
            workingThread = std::thread(&TestProcessManager::testGpioOutput,this);
    	    joinTestProcessThread();
            break;
        case TestCommand::testGpioShortCircuit:
        	workingThread = std::thread(&TestProcessManager::testGpiosShortCircuit,this);
            joinTestProcessThread();
            break;
        case TestCommand::testAnalogInput:
        	workingThread = std::thread(&TestProcessManager::testAnalogInput,this);
			joinTestProcessThread();
			if(isCompleteTest){
				cout << "complete test done" << endl;
				printCompleteTestResult();
			}
			break;
        case TestCommand::getServerIpAddress:
            workingThread = std::thread(&TestProcessManager::getServerIpAddress, this);
            joinTestProcessThread();
            break;
        case TestCommand::copyLogsUsb:
            workingThread = std::thread(&TestProcessManager::copyLogFilesToUsb, this );
            joinTestProcessThread();
            break;
        case TestCommand::turnUsbPortOn:
            workingThread = std::thread(&TestProcessManager::turnUsbPortOn, this );
            joinTestProcessThread();
            break;
        case TestCommand::powerBoardTesterOff:
            turnBoardTesterOff();
            break;
        default:
            break;
    }
}

/* GPIO Continuity Test Process Thread
 * Parameter:
 * error:. pointer of Errors Count
 */
void TestProcessManager::continuityTest(){
    setTestProcessStart(TestCommand::continuityTest);
    controller->setContinuityProcessStartState(true);
    if(isAppCommmand){
    	controller->database.resetBoardContinuityData(boardID);
    }
    continuityTestProcess.testGpiosContinuity();
    int error = continuityTestProcess.getTestErrors();
    if(isAppCommmand){
		for (const Continuity_Test_Result_Struct testResult : continuityTestProcess.continuityTestResultVector) {
			controller->database.updateGpioContinuityTestResult(boardID, testResult);
		}
    	controller->database.updateBoardCommand(boardID, TestCommand::continuityTest, 0, error);
    }
    displayManager->printContinuityResult(continuityTestProcess.getFailedContinuityTest(), continuityTestProcess.getShortGpios(), continuityTestProcess.getOpenGpios());
    setLedStateTestResult(error);
    controller->setContinuityProcessStartState(false);
    setTestProcessFinish();
}

/* Measure Board Current Test Process Thread
 * Parameter:
 * error:. pointer of Errors Count
 */
void TestProcessManager::measureBoardCurrent(bool isCompleteTest){
	int error = 0;
    setTestProcessStart(TestCommand::boardCurrent);
    if(isAppCommmand){
         controller->database.resetCurrentConsumptionData(boardID);
    }
    micorcontrollerCurrent = controller->currentSensor.measureCurrentConsumption();
 	if(micorcontrollerCurrent < 1){
		error = 1;
		controller->logger->printToFileLogger(Strings::failedToMeasureCurrent, true);
	}else if(micorcontrollerCurrent < MAX_CURRENT_CONSUMPTION){
		error = 0;
		controller->logger->printToFileLogger(Strings::currentConsumptionMessagePrefix + Helper::convertFloatToString(micorcontrollerCurrent) + Strings::currentmA, true);
		controller->logger->printToFileLogger(Strings::currentConsumptionMessagePrefix + Strings::testPass , true);
	}else{
		error = 1;
		controller->logger->printToFileLogger(Strings::highCurrentConsumptionDetected + Helper::convertFloatToString(micorcontrollerCurrent) + Strings::currentmA, true);
		controller->logger->printToFileLogger(Strings::currentConsumptionMessagePrefix + Strings::testFail, true);
 		if(!isCompleteTest){
 			controller->turnUsbPortOff();
 		}
	}
 	displayManager->printBoardCurrentResult(micorcontrollerCurrent);
    setLedStateTestResult(error);
    if(isAppCommmand){
		controller->database.updateCurrentTestResult(boardID, micorcontrollerCurrent);
		controller->database.updateBoardCommand(boardID, TestCommand::boardCurrent, 0, error);
   }
    setTestProcessFinish();
}


/* Power and GND Pins Test Process Thread
 * Parameter:
 * error:. pointer of Errors Count
 */
void TestProcessManager::testPowerGndPins(){
	setTestProcessStart(TestCommand::powerGndPinsTest);
	if(isAppCommmand){
		controller->database.resetBoardPowerGndVoltage(boardID);
	}
	powerGndPinsTestProcess.testAllGndPowerPins();
	int errors = powerGndPinsTestProcess.getTestErrors() + powerGndPinsTestProcess.getFailedTest();
	if(isAppCommmand){
		for (const Power_GND_TEST_RESULT testResult : powerGndPinsTestProcess.powerGndPinsTestResultVector) {
			controller->database.updateBoardPowerGndVoltage(boardID, testResult);
 		}
		controller->database.updateBoardCommand(boardID, TestCommand::powerGndPinsTest, 0, errors);
	}
	setLedStateTestResult(errors);
	displayManager->printPowerPinsResult(powerGndPinsTestProcess.getTestErrors() , powerGndPinsTestProcess.getFailedTest());
   setTestProcessFinish();
}


/* Flash NUCLEO Board Process Thread
 * Parameter:
 * error:. pointer of Errors Count
 */
void TestProcessManager::flashHandle(){
	int error = 0;
    setTestProcessStart(TestCommand::flashBoard);
    controller->setFlashProcessStartState(true);
    if(isAppCommmand){
        controller->database.resetBoardFlashData(boardID);
    }
 	if(flashProcess.flashNucleoBoard()){
 		displayManager->printWaitFlashMemoryTest();
 		flashProcess.testFlashMemory();
 	}
	error = flashProcess.flashMemoryTestStrcture.flash_errors;
    if(isAppCommmand){
        controller->database.updateBoardFlashData(boardID, &flashProcess.flashMemoryTestStrcture);
        controller->database.updateBoardCommand(boardID, TestCommand::flashBoard, 0, error);
    }
	displayManager->printFlashMemoryFlashTestResult(&flashProcess.flashMemoryTestStrcture);
    setLedStateTestResult(error);
    controller->setFlashProcessStartState(false);
    setTestProcessFinish();
}

/* Temperature Test Process Thread
 * Parameter:
 * error:. pointer of Errors Count
 */
void TestProcessManager::getBoardTemperature(bool isCompleteTest){
	int error = 0;
    setTestProcessStart(TestCommand::measureTemperature);
     if(isAppCommmand){
        controller->database.updatBoardTemperature(boardID, -1);
    }
     micorcontrollerTemperature = temperatureProcess.readBoardTemperatureSensor();
     if (micorcontrollerTemperature > BOARD_MAX_TEMPERATURE){
 		error = 1;
 		controller->logger->printToFileLogger("\n => Board temperature: " + Helper::convertFloatToString(micorcontrollerTemperature), true);
 		displayManager->printTemperatureResult(micorcontrollerTemperature);
 		if(!isCompleteTest){
 			controller->turnUsbPortOff();
  		}
      }else if(micorcontrollerTemperature < 0){
         error = 1;
         controller->logger->printToFileLogger("\n => Measurement failed: no valid value"  , true);
         displayManager->printTemperatureResult(TEMPERATURE_MEASURE_FAILED);
     }else{
         error = 0;
         controller->logger->printToFileLogger("\n => Board temperature: " + Helper::convertFloatToString(micorcontrollerTemperature) + " Celcius", true);
         displayManager->printTemperatureResult(micorcontrollerTemperature);
     }
     setLedStateTestResult(error);
     if(isAppCommmand){
    	 controller->database.updatBoardTemperature(boardID, micorcontrollerTemperature);
    	 controller->database.updateBoardCommand(boardID, TestCommand::measureTemperature, 0, error);
     }
     setTestProcessFinish();
}

/* GPIO Input Test Process Thread
 * Parameter:
 * error:. pointer of Errors Count
 */
void TestProcessManager::testGpioInput(){
     setTestProcessStart(TestCommand::testGpioInput);
    if(isAppCommmand){
    	controller->database.resetBoardGpioInputData(boardID);
    }
	digitalInputTesterProcess.testAllGpiosInput();
	int errors = digitalInputTesterProcess.getErrorsGpiosInput();
	int failedTest = digitalInputTesterProcess.getFailedGpioInput();
    if(isAppCommmand){
		for (const GPIO_Input_Test_Result_Struct testResult : digitalInputTesterProcess.gpiosInputTestResultVector) {
			controller->database.updateBoardGpioInputData(boardID, testResult);
		}
        controller->database.updateBoardCommand(boardID, TestCommand::testGpioInput, 0, errors);
    }
	displayManager->printGpiosInputResult(errors, failedTest);
    setLedStateTestResult(failedTest + errors);
    setTestProcessFinish();
}

/* GPIO Output Test Process Thread
 * Parameter:
 * error:. pointer of Errors Count
 */
void TestProcessManager::testGpioOutput(){
    setTestProcessStart(TestCommand::testGpioOutput);
    if(isAppCommmand)
    	controller->database.resetBoardGpioOutputData(boardID);
    digitalOutputTesterProcess.testAllGpiosOutput();
	int errors = digitalOutputTesterProcess.getErrorsGpiosOutput();
	int failedTest = digitalOutputTesterProcess.getFailedGpioOutput();

    if(isAppCommmand){
		for (const GPIO_Output_Test_result_Struct testResult : digitalOutputTesterProcess.gpiosOutputTestResultVector) {
			controller->database.updateBoardGpioOutputData(boardID, testResult);
		}
    }
	displayManager->printGpioOutputResult(errors, failedTest);
	setLedStateTestResult(failedTest + errors);
	controller->database.updateBoardCommand(boardID, TestCommand::testGpioOutput, 0, errors);
    setTestProcessFinish();
}

/* GPIO Input Test Process Thread
 * Parameter:
 * error:. pointer of Errors Count
 */
void TestProcessManager::testAnalogInput(){
    setTestProcessStart(TestCommand::testAnalogInput);
    if(isAppCommmand){
    	analogInputTestProcess.resetAnalogInputResultDatabase();
        for (const Analog_Input_Test_Result_Struct testResult : analogInputTestProcess.analogInputTestResultVector) {
            controller->database.updateBoardAnalogInputData(boardID, testResult);
        }
    }

    analogInputTestProcess.initialAndTestAllAnalogInput(isAppCommmand);
	int errors = analogInputTestProcess.getErrorsAnalogInput();
	int failedTest = analogInputTestProcess.getFailedAnalogInput();
    if(isAppCommmand){
		for (const Analog_Input_Test_Result_Struct testResult : analogInputTestProcess.analogInputTestResultVector) {
			controller->database.updateBoardAnalogInputData(boardID, testResult);
		}
        controller->database.updateBoardCommand(boardID, TestCommand::testAnalogInput, 0, errors);
    }
	displayManager->printAnalogInputResult(errors, failedTest);
    setLedStateTestResult(errors + failedTest);
    setTestProcessFinish();
}



/* GPIO Short Circuits Test Process Thread
 * Parameter:
 * error:. pointer of Errors Count
 */
void TestProcessManager::testGpiosShortCircuit(){
    setTestProcessStart(TestCommand::testGpioShortCircuit);
    if(isAppCommmand)
    	controller->database.resetGpioShortCircuitsData(boardID);
    shortGpiosTestProcess.testAllGpiosShortCircuit();
	int failedTest = shortGpiosTestProcess.getFailedGpioShortTest();
	int shortCircuits = shortGpiosTestProcess.getShortedGpios();
	int errors = failedTest + shortCircuits;
    setLedStateTestResult(errors);
    displayManager->printShortCircuitGpioResult(failedTest, shortCircuits);
    if(isAppCommmand){
		for (const GPIO_SHORT_TEST_RESULT gpioShortCircuitResult : shortGpiosTestProcess.gpiosShortCircuitsTestResultVector ) {
			controller->database.updateGpioShortCircuitsData(boardID, gpioShortCircuitResult);
		}
        controller->database.updateBoardCommand(boardID, TestCommand::testGpioShortCircuit, 0, errors);
    }
    setTestProcessFinish();
}

/* Get Raspberry Pi Ip-Address Process Thread
*/
void TestProcessManager::getServerIpAddress(){
    setTestProcessStart(TestCommand::getServerIpAddress);
    string ipAddressText = Helper::executeCommandAndGetOutput(GET_SERVER_IP_COMMAND);
    ipAddressText = Helper::trimString(ipAddressText);
    displayManager->printIpAddress(ipAddressText.c_str());
    controller->logger->printToFileLogger("\n => Server Ip-Address: " + ipAddressText, true);
    controller->setNewLedsCommand(LEDS_COMMAND::setGreenOn);
    setTestProcessFinish();
}

/* Copy Logs to USB Port Process Thread
*/
void TestProcessManager::copyLogFilesToUsb(){
    setTestProcessStart(TestCommand::copyLogsUsb);
    string copyToUsbAckText = Helper::executeCommandAndGetOutput(COPY_TO_USB_COMMAND);
    copyToUsbAckText = Helper::trimString(copyToUsbAckText);
    if(copyToUsbAckText == usbCopySuccess){
		controller->database.updateBoardControllerCopyState(COPY_LOGS_SUCCESS);
    	displayManager->printCopyFilesUsbResult(true, true);
        controller->setNewLedsCommand(LEDS_COMMAND::setGreenOn);
        controller->logger->printToFileLogger("\n => Copy Log Files to USB-Media: success", true);
    }else if (copyToUsbAckText == usbError ){
        controller->database.updateBoardControllerCopyState(COPY_LOGS_FAILED_NO_MEDIA_DETECTED);
        controller->setNewLedsCommand(LEDS_COMMAND::setRedOn);
        displayManager->printCopyFilesUsbResult(false, false);
        controller->logger->printToFileLogger("\n => Copy Log Files to USB-Media: failed No USB-Media detected", true);
    }else{
        controller->database.updateBoardControllerCopyState(COPY_LOGS_FAILED);
    	displayManager->printCopyFilesUsbResult(true, false);
        controller->setNewLedsCommand(LEDS_COMMAND::setRedOn);
        controller->logger->printToFileLogger("\n => Copy Log Files to USB-Media: failed", true);
    }
    setTestProcessFinish();
}

/* Turn USB Port ON and initialize
*/
void TestProcessManager::turnUsbPortOn(){
    setTestProcessStart(TestCommand::turnUsbPortOn);
    if(controller->getUsbPortState() == UsbPortState::ON){
        controller->logger->printToFileLogger("Current USB-Port State ON", true);
		controller->setNewLedsCommand(LEDS_COMMAND::setGreenOn);
        displayManager->printUsbPortState(true);
    }else{
        controller->logger->printToFileLogger("Turn USB-Port ON", true);
        controller->checkUsbPortStateAndInitialUart();
    	delay(_2S_DELAY);
    	if(controller->getUsbPortState() == UsbPortState::ON){
      		controller->logger->printToFileLogger("=> Turn USB Port ON Success", true);
    		controller->setNewLedsCommand(LEDS_COMMAND::setGreenOn);
    		displayManager->printUsbTurnOn(true);
     	}else{
      		controller->logger->printToFileLogger("=> Turn USB Port ON failed", true);
    		controller->setNewLedsCommand(LEDS_COMMAND::setRedOn);
    		displayManager->printUsbTurnOn(false);
     	}
    }
    setTestProcessFinish();
}

/* Turn Board Tester OFF
*/
void TestProcessManager::turnBoardTesterOff(){
    controller->setNewLedsCommand(LEDS_COMMAND::blinkAll);
    displayManager->printWaitTurnBoardTesterOff();
    controller->logger->printToFileLogger("=> Turn Board Tester OFF", true);
    controller->database.updateBoardControllerState(BOARD_STATE_OFF);
    delay(_2S_DELAY);
    controller->activateSystemTerminatiom();
}

/* Start Complete Board Test
 * Parameter:
 * _isAppCommand: is Command from Mobile Application
 */
void TestProcessManager::startCompleteBoardTest(bool _isAppCommmand){
	startTestProcessThread(boardID, boardName, TestCommand::continuityTest, _isAppCommmand, true);
	delay(_1S_DELAY);
	startTestProcessThread(boardID, boardName, TestCommand::powerGndPinsTest, _isAppCommmand, true);
    delay(_1S_DELAY);
    startTestProcessThread(boardID, boardName, TestCommand::flashBoard, _isAppCommmand, true);
    delay(_1S_DELAY);
	startTestProcessThread(boardID, boardName, TestCommand::boardCurrent, _isAppCommmand, true);
	delay(_1S_DELAY);
    startTestProcessThread(boardID, boardName, TestCommand::measureTemperature, _isAppCommmand, true);
    delay(_1S_DELAY);
    startTestProcessThread(boardID, boardName, TestCommand::testGpioInput, _isAppCommmand, true);
    delay(_1S_DELAY);
    startTestProcessThread(boardID, boardName, TestCommand::testGpioOutput, _isAppCommmand, true);
    delay(_1S_DELAY);
    startTestProcessThread(boardID, boardName, TestCommand::testGpioShortCircuit, _isAppCommmand, true);
    delay(_1S_DELAY);
    startTestProcessThread(boardID, boardName, TestCommand::testAnalogInput, _isAppCommmand, true);
    delay(_1S_DELAY);
 }


void TestProcessManager::printCompleteTestResult(){
	controller->logger->printTestSectionHeaderSeperator();
	int errors = 0;
	int failedTest = 0;
    controller->logger->printToFileLogger("Complete Test done: ", true);
 	if(micorcontrollerCurrent < 1){
	    controller->logger->printToFileLogger("- Failed to measure Microcontroller Current Consumption", true);
		failedTest++;
	}else if(micorcontrollerCurrent < MAX_CURRENT_CONSUMPTION){
		controller->logger->printToFileLogger("- Current Consumption: " + Helper::convertFloatToString(micorcontrollerCurrent) + Strings::currentmA , true);
	}else{
		controller->logger->printToFileLogger("- High Current Consumption detected: " + Helper::convertFloatToString(micorcontrollerCurrent) + Strings::currentmA, true);
		errors ++;
 	}

    if (micorcontrollerTemperature > BOARD_MAX_TEMPERATURE){
  		controller->logger->printToFileLogger("- Board Over heated ! Temperature: " + Helper::convertFloatToString(micorcontrollerTemperature), true);
  		errors ++;
     }else if(micorcontrollerTemperature < 0){
        controller->logger->printToFileLogger("- Board Temperature: failed to measure"  , true);
        failedTest++;
    }else{
         controller->logger->printToFileLogger("- Board Temperature: " + Helper::convertFloatToString(micorcontrollerTemperature) + " Celcius" , true);
    }

    if((powerGndPinsTestProcess.getTestErrors() + powerGndPinsTestProcess.getFailedTest()) == 0){
        controller->logger->printToFileLogger("- Microcontroller Power and GND Pins OK", true);
    }else{
		if(powerGndPinsTestProcess.getFailedTest() > 0){
			controller->logger->printToFileLogger("- Power and GND Pins: " + to_string(powerGndPinsTestProcess.getFailedTest()) +" failed Test (" + powerGndPinsTestProcess.getFailedPinsText()+ " )", true);
			failedTest += powerGndPinsTestProcess.getFailedTest();
		}
		if(powerGndPinsTestProcess.getTestErrors() > 0){
			controller->logger->printToFileLogger("- Power and GND Pins: " + to_string(powerGndPinsTestProcess.getTestErrors()) +" Errors (" + powerGndPinsTestProcess.getErrorsPinsText() + " )", true);
			errors += powerGndPinsTestProcess.getTestErrors();
		}
    }

    if(continuityTestProcess.getTestErrors() == 0 ){
        controller->logger->printToFileLogger("- GPIOs Continuity Test: pass", true);
    }else{
        if(continuityTestProcess.getFailedContinuityTest() > 0){
            controller->logger->printToFileLogger("- GPIOs Continuity Test: " + to_string(continuityTestProcess.getFailedContinuityTest()) +" failed Test " + continuityTestProcess.getFailedGpiosText(), true);
            failedTest += continuityTestProcess.getFailedContinuityTest();
        }
        if(continuityTestProcess.getOpenGpios() > 0){
        	controller->logger->printToFileLogger("- GPIOs Continuity Test: " + to_string(continuityTestProcess.getOpenGpios()) +" Open-GPIOs/Defect-ESD (" + continuityTestProcess.getOpenGpiosText() + " )", true);
        	errors += continuityTestProcess.getOpenGpios();
        }
		if(continuityTestProcess.getShortGpios() > 0){
			controller->logger->printToFileLogger("- GPIOs Continuity Test: " + to_string(continuityTestProcess.getShortGpios()) +" Short-Circuits to GND/VCC detected (" + continuityTestProcess.getShortGpiosText() + " )", true);
			errors += continuityTestProcess.getShortGpios();
		}
    }

	if(flashProcess.flashMemoryTestStrcture.isUploadTestProgrammOK){
		controller->logger->printToFileLogger("- Flash Test-Programm: OK", true);
	}else{
		controller->logger->printToFileLogger("- Flash Test-Programm: failed", true);
		failedTest++;
	}

	if(flashProcess.flashMemoryTestStrcture.flash_errors == 0){
		controller->logger->printToFileLogger("- Flash Memory: OK", true);
	}else{
		controller->logger->printToFileLogger("- Flash Memory: " + to_string(flashProcess.flashMemoryTestStrcture.flash_errors) + " Errors", true);
		errors += flashProcess.flashMemoryTestStrcture.flash_errors;
	}


	if((digitalInputTesterProcess.getFailedGpioInput() + digitalInputTesterProcess.getErrorsGpiosInput() == 0 )){
		controller->logger->printToFileLogger("- GPIOs Input Test: pass", true);
	}else{
		if(digitalInputTesterProcess.getFailedGpioInput() > 0){
			controller->logger->printToFileLogger("- GPIOs Input Test: " + to_string(digitalInputTesterProcess.getFailedGpioInput()) +" failed Test (" + digitalInputTesterProcess.getFailedGpioInputText() + " )", true);
			failedTest += digitalInputTesterProcess.getFailedGpioInput();
		}
		if(digitalInputTesterProcess.getErrorsGpiosInput() > 0){
			controller->logger->printToFileLogger("- GPIOs Input Test: " + to_string(digitalInputTesterProcess.getErrorsGpiosInput()) +" Errors (" + digitalInputTesterProcess.getErrorsGpiosInputText() + " )", true);
			errors += digitalInputTesterProcess.getErrorsGpiosInput();
		}
	}

    if((digitalOutputTesterProcess.getFailedGpioOutput() + digitalOutputTesterProcess.getErrorsGpiosOutput() == 0 )){
        controller->logger->printToFileLogger("- GPIOs Output Test: pass", true);
    }else{
        if(digitalOutputTesterProcess.getFailedGpioOutput() > 0){
            controller->logger->printToFileLogger("- GPIOs Output Test: " + to_string(digitalOutputTesterProcess.getFailedGpioOutput()) +" failed Test (" + digitalOutputTesterProcess.getFailedGpioOutputText() + " )", true);
            failedTest += digitalOutputTesterProcess.getFailedGpioOutput();
        }
        if(digitalOutputTesterProcess.getErrorsGpiosOutput() > 0){
        	controller->logger->printToFileLogger("- GPIOs Output Test: " + to_string(digitalOutputTesterProcess.getErrorsGpiosOutput()) +" Errors (" + digitalOutputTesterProcess.getErrorsGpiosOutputText() + " )", true);
        	errors += digitalOutputTesterProcess.getErrorsGpiosOutput();
        }
    }

    if((shortGpiosTestProcess.getFailedGpioShortTest() + shortGpiosTestProcess.getShortedGpios() == 0 )){
        controller->logger->printToFileLogger("- GPIOs Short Circuit Test: pass", true);
    }else{
        if(shortGpiosTestProcess.getFailedGpioShortTest() > 0){
            controller->logger->printToFileLogger("- GPIOs Short Circuit Test: " + to_string(shortGpiosTestProcess.getFailedGpioShortTest()) +" failed Test (" + shortGpiosTestProcess.getFailedGpioShortTestText() + " )", true);
            failedTest += shortGpiosTestProcess.getFailedGpioShortTest();
        }
        if(shortGpiosTestProcess.getShortedGpios() > 0){
        	controller->logger->printToFileLogger("- GPIOs Short Circuit Test: " + to_string(shortGpiosTestProcess.getShortedGpios()) +" Errors: " + shortGpiosTestProcess.getShortedGpiosText(), true);
        	errors += shortGpiosTestProcess.getShortedGpios();
        }
    }

    if((analogInputTestProcess.getFailedAnalogInput() + analogInputTestProcess.getErrorsAnalogInput() == 0 )){
        controller->logger->printToFileLogger("- Analog Input Test: pass", true);
    }else{
        if(analogInputTestProcess.getFailedAnalogInput() > 0){
            controller->logger->printToFileLogger("- Analog Input failed Test: " + to_string(analogInputTestProcess.getFailedAnalogInput()) +" failed Test (" + analogInputTestProcess.getFailedAnalogInputText() + " )", true);
            failedTest += analogInputTestProcess.getFailedAnalogInput();
        }
        if(analogInputTestProcess.getErrorsAnalogInput() > 0){
        	controller->logger->printToFileLogger("- Analog Input Test: " + to_string(analogInputTestProcess.getErrorsAnalogInput()) +" Errors: (" + analogInputTestProcess.getErrorsAnalogInputText() + " )", true);
        	errors += analogInputTestProcess.getErrorsAnalogInput();
        }
    }

	controller->logger->printSectionSeperator(true);
    if((failedTest == 0) && (errors == 0)){
        controller->logger->printToFileLogger("=> Micrcontroller OK, No Errors", true);
    }else{
    	if(failedTest > 0){
            controller->logger->printToFileLogger("=> Micrcontroller failed Tests: " + to_string(failedTest) , true);
    	}
    	if(errors > 0){
            controller->logger->printToFileLogger("=> Micrcontroller Errors: " + to_string(errors) , true);
    	}
    }
    if((micorcontrollerCurrent > MAX_CURRENT_CONSUMPTION) || (micorcontrollerTemperature > BOARD_MAX_TEMPERATURE)){
		controller->turnUsbPortOff();
     }
    delay(_1S_DELAY);
    displayManager->printCompleteTestResult(failedTest, errors);
    setLedStateTestResult(errors + failedTest);
    if(isAppCommmand){
         controller->database.updateBoardCommand(boardID, TestCommand::completTest, 0, errors + failedTest);
    }
}
/* Set Test Process Start
 * Lock MUTEX
 * Set Test Process State Working
 * SET LED blink green
 * print Board-ID if it is known
 */
void TestProcessManager::setTestProcessStart(TestCommand command){
    pthread_mutex_lock(&mutex);
    setNewTestProcessState(Test_Process_State::working);
    controller->setNewLedsCommand(LEDS_COMMAND::blinkGreen);
    displayManager->printLcdWaitTestProcess(command);
    controller->logger->printBoardID(isAppCommmand, command, boardName);
}

/* Set Test Process Finish
 * Set Test Process State free
 */
void TestProcessManager::setTestProcessFinish(){
    pthread_mutex_unlock(&mutex);
	setNewTestProcessState(Test_Process_State::free);
}


/* Set LED New State
 *Parameter:
 *testErrorsResult: Test Errors
 */
void TestProcessManager::setLedStateTestResult(int testErrorsResult){
    if(testErrorsResult == 0){
         controller->setNewLedsCommand(LEDS_COMMAND::setGreenOn);
    }else{
        controller->setNewLedsCommand(LEDS_COMMAND::setRedOn);
    }
}

/* Stop working Thread
 */
void TestProcessManager::joinTestProcessThread(){
	if (workingThread.joinable()) {
		workingThread.join();
	 }
}



void TestProcessManager::setNewTestCommand(TestCommand command){
	currentTestCommand = command;
}

TestCommand TestProcessManager::getCurrentTestCommand() {
	return currentTestCommand;
}

void TestProcessManager::setNewTestProcessState(Test_Process_State newTestProcessState){
	testProcessStateValue = newTestProcessState;
}

Test_Process_State TestProcessManager::getTestProcessState(){
	return testProcessStateValue;
}

