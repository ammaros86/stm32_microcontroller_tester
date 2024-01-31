/*
 * PeripheralsInterfacesController.cpp
 *
 *  Created on: 30.08.2023
 *      Author: Astra
 */


#include "../controllers/PeripheralsInterfacesController.hpp"


PeripheralsInterfacesController::~PeripheralsInterfacesController(){
}


//Multiplexers Connections

/* get connected Multiplexer to GPIO and connect Y to Z
 * Parameter:
 * positionFromGpioVector: to get which Multiplexer 1, 2, 3, 4 or 5 connected to Pin
 */
int PeripheralsInterfacesController::openMultiplexerConnectionToGpio(uint8_t positionGpioPinVector){
	uint8_t multiplexerPin = static_cast<uint8_t>(multiplexer_pin.at(positionGpioPinVector));
 	int connectedMultiplexerToGpioPin = static_cast<int>(connected_multiplexer_gpio.at(positionGpioPinVector));
 	if(connectedMultiplexerToGpioPin > 0){
 		int mux6SetAck = mulutiplexerController.setMultiplexer6Conenction(connectedMultiplexerToGpioPin);
		if(mux6SetAck > 0){
			int frontMuxSetAck = connectFrontMultiplexerYtoZpin(connectedMultiplexerToGpioPin, multiplexerPin);
			if(frontMuxSetAck > 0){
				return 1;
			}else{
				logger->printToFileLogger("Error: PeripheralsInterfacesController openMultiplexerConnectionToGpio : failed to select front Multiplexer: " +to_string(connectedMultiplexerToGpioPin), false);
				return  frontMuxSetAck;
			}
		}else{
			logger->printToFileLogger("Error: PeripheralsInterfacesController openMultiplexerConnectionToGpio : failed to select Multiplexer 6", false);
			return mux6SetAck;
		}
	}else{
		logger->printToFileLogger("Error: PeripheralsInterfacesController openMultiplexerConnectionToGpio : Unknown Pin " + to_string(positionGpioPinVector), false);
		return connectedMultiplexerToGpioPin;
	}
}

/* connect front Multiplexer (1, 2, 3, 4,5) Yn Pin to Z Pin
 * Parameter:
 * muxNumber: Multiplexer Number 1, 2, 3, 4 or 5
 * pinNumber: Multiplexer Pin
 */
int PeripheralsInterfacesController::connectFrontMultiplexerYtoZpin(int multiplexerNumber, uint8_t pinNumber){
	if(multiplexerNumber == Mux5){
		int mux5ControllAck =  mulutiplexerController.setMultiplexer5Conenction(pinNumber);
		if(mux5ControllAck == MUX5_CONTROLL_ERROR){
			logger->printToFileLogger("Error: PeripheralsInterfacesController selectPin failed to select Pin:" + to_string(pinNumber) +" from Multiplexer 5", false);
 		}
		return mux5ControllAck;
	}else if(multiplexerNumber < Mux5 && multiplexerNumber >= Mux1){
		if(Helper::initMultiplexerSelectPins(multiplexerNumber, &mux_select_vector) == UNKNOWN_CONNECTED_MULTIPLEXER_TO_NUCLEO_PIN){
			return UNKNOWN_CONNECTED_MULTIPLEXER_TO_NUCLEO_PIN;
		}else{
			int ioExpnaderCommand = Helper::getIoExpanderCommand(multiplexerNumber, pinNumber, &mux_select_vector);
			if(ioExpnaderCommand == UNKNOWN_IO_EXPANDER_COMMAND){
				logger->printToFileLogger("Error: PeripheralsInterfacesController selectPin unknown I/O Expander Command: " + to_string(ioExpnaderCommand), false);
				return UNKNOWN_IO_EXPANDER_COMMAND;
			}else{
 				ioExpander.resetRegisters();
				if(multiplexerNumber == Mux1 || multiplexerNumber == Mux2){
					return ioExpander.writeDataToRegister(OLATA,  ioExpnaderCommand);
 				}else{
 					return ioExpander.writeDataToRegister(OLATB,  ioExpnaderCommand);
 				}
			}
		}
	}else{
		logger->printToFileLogger("Error: PeripheralsInterfacesController selectPin unknown Multiplexer Number : " + to_string(multiplexerNumber), false);
		return UNKNOWN_MULTIPLEXER_SELECT;
	}
}

/* open Pin Connection to test Power GND Pins
 * Parameter:
 * position: vector position
 * powerPinsVectors: Power and GND Pin vectors structure
 */
int PeripheralsInterfacesController::openMuxConnectionToTestPowerGndPin(bool printVoltage, int position){
	uint8_t pin =  static_cast<uint8_t>(powerGndMuxPinVectors.at(position));
	uint8_t muxNumber =  static_cast<uint8_t>(powerGndMuxConnectionVector.at(position));
	string pinText =  static_cast<string>(powerGndPinsStringVector.at(position));
	if(mulutiplexerController.setMultiplexer6Conenction(muxNumber)){
		return connectFrontMultiplexerYtoZpin(muxNumber, pin);
	}else{
		return MUX6_CONTROLL_ERROR;
	}
}

/* open Multiplexer Connection to Analog Pin
 * Parameter:
 * positionAnalogPinVector: vector position
 */
int PeripheralsInterfacesController::openMultiplexerConnectionToAnalog(uint8_t positionAnalogPinVector){
	uint8_t multiplexerPin = static_cast<uint8_t>(analog_multiplexer_pins.at(positionAnalogPinVector));
	int connectedMultiplexerToAnalogPin = static_cast<int>(connected_multiplexer_analog.at(positionAnalogPinVector));
	int mux6SetAck = mulutiplexerController.setMultiplexer6Conenction(connectedMultiplexerToAnalogPin);
	if(mux6SetAck > 0){
		int frontMuxSetAck = connectFrontMultiplexerYtoZpin(connectedMultiplexerToAnalogPin, multiplexerPin);
		if(frontMuxSetAck > 0){
			return 1;
		}else{
			logger->printToFileLogger("Error: DigitalInputTestProcess: openMultiplexerConnectionAndTestGpioInput : failed to select front Multiplexer 4"  , false);
			return  frontMuxSetAck;
		}
	}else{
		logger->printToFileLogger("Error: DigitalInputTestProcess: openMultiplexerConnectionAndTestGpioInput : failed to select Multiplexer 6", false);
		return mux6SetAck;
	}
}

//GPIOs

/* initialize Nucleo-Board GPIO and return Acknowledge contains Nucleo-Board registers state
 * Parameter:
 * gpioInitialStructure: GPIO initialization Structure
 */
GPIO_INITIAL_STATE PeripheralsInterfacesController::initialGpioAndCheckGpiosAckRegisters(GPIO_Initial_Struct * gpioInitialStructure){
	if(initialGpiO(gpioInitialStructure) < UART_SEND_BUFFER_SIZE){
 		return GPIO_INITIAL_STATE::errorSendCommand;
	}else{
 		uint8_t *readBuffer  =  uartHandler.readUartData(UART_ACK_SIZE);
 		GPIO_Registers_Struct gpio_registers_state = Helper::getNucleoBoardRegisters(readBuffer);
 		if(readBuffer != nullptr){
 			delete[] readBuffer;
 		}else{
 	 		logger->printToFileLogger("->Error:PeripheralsInterfacesController/initialGpioAndCheckGpiosAckRegisters: failed to read UART Data: ", false);
 			return GPIO_INITIAL_STATE::errorReadCommand;
 		}
		return getGpioInitialStateAck(&gpio_registers_state, gpioInitialStructure);
	}
}

/* initialize Nucleo-Board GPIO
 * Parameter:
 * gpioInitialStructure: GPIO initialization Structure
 */
int PeripheralsInterfacesController::initialGpiO(GPIO_Initial_Struct * gpio_initial_structure){
	uint16_t uartCommand = getGpioInitialCommand(gpio_initial_structure);
 	int sendAck  = uartHandler.sendUartData(uartCommand);
 	if(sendAck < UART_SEND_BUFFER_SIZE){
 		logger->printToFileLogger("->Error:PeripheralsInterfacesController/initialGpiO: failed to send Initial Command: " +  to_string(uartCommand), false);
	}
	return sendAck;
}


/* return UART Command to initialize Nucleo-Board GPIO
 * Parameter:
 * gpioInitialStructure: GPIO initialization Structure
 */
uint16_t PeripheralsInterfacesController::getGpioInitialCommand(GPIO_Initial_Struct * gpio_initial_structure){
	uint16_t command = 0;
	command |= COMMAND_TYPE_INIT;
	command |= gpio_initial_structure->Port;
	command |= gpio_initial_structure->Mode;
	command |= gpio_initial_structure->Pull;
	command |= gpio_initial_structure->Speed;
 	command |= (gpio_initial_structure->Pin +1) << COMMAND_INIT_GPIO_SHIFT;
	char hexValue[4];
	sprintf(hexValue, "%x", command);
	string hexCommand ;
	hexCommand.append(hexValue);
  	return command;
}

/* return Acknowledge contains Nucleo-Board registers state
 * Parameter:
 * gpio_registers_state: GPIO Register State Structure
 * gpioInitialStructure: GPIO initialization Structure
 */
GPIO_INITIAL_STATE  PeripheralsInterfacesController::getGpioInitialStateAck(GPIO_Registers_Struct* gpio_registers_state, GPIO_Initial_Struct * gpioInitialStructure){
	bool moderRegisterInitialized = Helper::isModerRegisterInitialized(gpio_registers_state->ModerRegiste, gpioInitialStructure);
	bool pullTypRegisterInitialized = Helper::isPullTypeRegisterInitialized(gpio_registers_state->PupDrRegister, gpioInitialStructure);
	bool speedRegisterInitialized = Helper::isSpeedRegisterInitialized(gpio_registers_state->OSpeedRegister, gpioInitialStructure);
	bool oTypeRegisterInitialized = Helper::isOtypeRegisterInitialized(gpio_registers_state->OtypeRegister, gpioInitialStructure);

	if(gpioInitialStructure->Mode == COMMAND_Init_ANALOGE){
		if(moderRegisterInitialized){
			return GPIO_INITIAL_STATE::initialised;
		}else{
			return GPIO_INITIAL_STATE::gpioInitialFail;
		}
	} else if(gpioInitialStructure->Mode == COMMAND_Init_DIGITAL_OUTPUT_PP || gpioInitialStructure->Mode == COMMAND_Init_DIGITAL_OUTPUT_OD){
			if(moderRegisterInitialized && pullTypRegisterInitialized && oTypeRegisterInitialized &&  speedRegisterInitialized){
				return GPIO_INITIAL_STATE::initialised;
			}else{
				return GPIO_INITIAL_STATE::gpioInitialFail;
			}
	}else{
		if(moderRegisterInitialized && pullTypRegisterInitialized){
			return GPIO_INITIAL_STATE::initialised;
		}else{
			return GPIO_INITIAL_STATE::gpioInitialFail;
		}
	}
}

/* handle GPIO (read Input state, set Output HIGH/LOW)
 * Parameter:
 * pinNumber: GPIO Pin
 * port: GPIO Port
 * handleCommand: Command to handle GPIO
 */
int PeripheralsInterfacesController::handleGpio(uint16_t pinNumber, uint8_t port, uint8_t handleCommand){
	uint16_t uartCommand = getGpioUartHandleCommand(pinNumber, port, handleCommand);
  	int sendAck = uartHandler.sendUartData(uartCommand);
 	if(sendAck < 0){
 		logger->printToFileLogger("->Error: PeripheralsInterfacesController/handleGpio failed to send UART Command: " +  to_string(uartCommand), false);
	}
	return sendAck;
}

/* return handle GPIO UART command
 * Parameter:
 * pinNumber: GPIO Pin
 * port: GPIO Port
 * handleCommand: Command to handle GPIO
 */
uint16_t PeripheralsInterfacesController::getGpioUartHandleCommand(uint16_t pinNumber, uint8_t port, uint8_t handleCommand){
	uint32_t command = 0;
	command |= COMMAND_TYPE_HANDLE;
	command |= port;
	command |= handleCommand;
  	command |= (pinNumber +1) << COMMAND_HANDLE_GPIO_SHIFT;
 	char hexValue[4];
	sprintf(hexValue, "%x", command);
	string hexCommand ;
	hexCommand.append(hexValue);
 	return command;
}


/* return GPIO Level State if valid
 * Parameter:
 * gpio_registers_state: GPIO Registers State Vectors
 * pin: GPIO-Pin
 * mode: GPIO-Mode
 */
GPIO_Level_STATE PeripheralsInterfacesController::getGpioLevelState(GPIO_Registers_Struct* gpio_registers_state, uint16_t pin, uint8_t mode){
 	if(mode == COMMAND_Init_DIGITAL_OUTPUT_PP || mode == COMMAND_Init_DIGITAL_OUTPUT_OD){
 		uint32_t odrRegister = gpio_registers_state->OdrRegister;
 		if(IS_BIT_SET(odrRegister, pin)){
			return GPIO_Level_STATE::high;
		}else{
			return GPIO_Level_STATE::low;
		}
	}else{
		uint32_t idrRegister = gpio_registers_state->IdrRegister;
		if(IS_BIT_SET(idrRegister, pin)){
			return GPIO_Level_STATE::high;
		}else{
			return GPIO_Level_STATE::low;
		}
	}
}

/*
 * set GPIO Output HIGH or LOW
 * read GPIO Register and check if Register value set
 * Parameter:
 * gpioInitialStructure: initial Values of the GPIO Pin
 * level: GPIO Output Level State HIGH or LOW
 */
OUTPUT_HANDLE_STATE PeripheralsInterfacesController::setGpioOutputLevelAndCheckOutputRegister(GPIO_Initial_Struct * gpioInitialStructure, uint8_t level){
 	if(handleGpio(gpioInitialStructure->Pin, gpioInitialStructure->Port,  level) < 0){
 		logger->printToFileLogger("-> Error PeripheralsInterfacesController/setGpioOutputLevelAndCheckOutputRegister: Failed to send UART Data", false);
		return OUTPUT_HANDLE_STATE::gpioOutputHandleFailedToSendData;
	}else{
		uint8_t *readBuffer  = uartHandler.readUartData(UART_ACK_SIZE);
 		if(readBuffer == nullptr){
 			logger->printToFileLogger("-> Error PeripheralsInterfacesController/setGpioOutputLevelAndCheckOutputRegister: Failed to read UART Data", false);
 			return OUTPUT_HANDLE_STATE::gpioFailedToReadRegisters;
 		}else{
 			GPIO_Registers_Struct gpio_registers_state = Helper::getNucleoBoardRegisters(readBuffer);
 			delete[] readBuffer;
 			GPIO_Level_STATE gpioLevelAckState = getGpioLevelState(&gpio_registers_state, gpioInitialStructure->Pin, gpioInitialStructure->Mode);
 			if(level == COMMAND_WRITE_DIGITIAL_OUTPUT_LEVEL_HIGH){
 				return checkHighLevelSetToOutput(gpioLevelAckState);
 			}else if(level == COMMAND_WRITE_DIGITIAL_OUTPUT_LEVEL_LOW){
 				return checkLowLevelSetToOutput(gpioLevelAckState);
 			}else{
 				logger->printToFileLogger("-> Error PeripheralsInterfacesController/setGpioOutputLevelAndCheckOutputRegister: Failed to set GPIO Output Level cause wrong Register Values", false);
 				return OUTPUT_HANDLE_STATE::gpioUndefinedOutputLevel;
 			}
 		}
	}
}

/*
 * control if HIGH State correctly Set
 * Parameter:
 * gpioLevelAckState: Output Level State HIGH/LOW
 */
OUTPUT_HANDLE_STATE PeripheralsInterfacesController::checkHighLevelSetToOutput(GPIO_Level_STATE gpioLevelAckState){
	if(gpioLevelAckState == GPIO_Level_STATE::high ){
		logger->printToFileLogger("-> GPIO set Output Register HIGH success", false);
		return OUTPUT_HANDLE_STATE::gpioSetHighSuccess;
	}else{
		logger->printToFileLogger("-> GPIO set Output Register HIGH failed", false);
		return OUTPUT_HANDLE_STATE::gpioFailedSetHigh;
	}
}

/*
 * control if LOW State correctly Set
 * Parameter:
 * gpioLevelAckState: Output Level State HIGH/LOW
 */
OUTPUT_HANDLE_STATE PeripheralsInterfacesController::checkLowLevelSetToOutput(GPIO_Level_STATE	gpioLevelAckState){
	if(gpioLevelAckState == GPIO_Level_STATE::low ){
		logger->printToFileLogger("-> GPIO set Output Register LOW success", false);
		return OUTPUT_HANDLE_STATE::gpioSetLowSuccess;
	}else {
		logger->printToFileLogger("-> GPIO set Output Register LOW failed", false);
		return OUTPUT_HANDLE_STATE::gpioFailedSetLow;
	}
}
/*
 * check GPIO Output Test OK for HIGH and LOW Level
 * check Voltage and Register Values
 * Parameter:
 * gpioInitialStructure: initial Values of the GPIO Pin
 * setGpioHighLevelAck: Output High Level State
 * setGpioLowLevelAck: Output LOW Level State
 */
bool PeripheralsInterfacesController::isGpioOutputOk(GPIO_Output_Test_result_Struct *gpioOutputResult, OUTPUT_HANDLE_STATE setGpioHighLevelAck, OUTPUT_HANDLE_STATE setGpioLowLevelAck){
	string setGpioRegisterLowLevelAckText  = Helper::getHandleRegistersAckText(setGpioLowLevelAck);
	string setGpioRegisterHighLevelAckText = Helper::getHandleRegistersAckText(setGpioHighLevelAck);
 	if((setGpioHighLevelAck == OUTPUT_HANDLE_STATE::gpioSetHighSuccess) && (setGpioLowLevelAck == OUTPUT_HANDLE_STATE::gpioSetLowSuccess)){
		if(isOutputVoltageLevelMeasurementOk(false, gpioOutputResult)){
			return true;
		}else{
			return false;
		}
	}else{
		logger->printToFileLogger("Error GPIO Output Registers: failed SET HIGH ACK:  " + setGpioRegisterHighLevelAckText + " SET LOW ACK:  " + setGpioRegisterLowLevelAckText , false);
		gpioOutputResult->comment = setGpioRegisterHighLevelAckText + " " + setGpioRegisterLowLevelAckText ;
		return false;
	}
}

/*
 * check GPIO Output Measurement OK
 * Parameter:
 * gpioOutputResult: Output Test Result
 */
bool PeripheralsInterfacesController::isOutputVoltageLevelMeasurementOk(bool gpioConnectedToPushButton, GPIO_Output_Test_result_Struct *gpioOutputResult){
	if(!gpioConnectedToPushButton){
		if((gpioOutputResult->voltageHighLevel >= voltageRefernces.HIGH_OUTPUT_VOLTAGE_MIN_VALUE) && (gpioOutputResult->voltageLowLevel <=  voltageRefernces.LOW_OUTPUT_VOLTAGE_MAX_VALUE)){
 			gpioOutputResult->comment = GPIO_OUTPUT_VOLTAGE_OK;
			return true;
		}else{
			logger->printToFileLogger("-> GPIO Output Voltage Measurement Error", false);
			gpioOutputResult->comment = GPIO_OUTPUT_VOLTAGE_ERROR;
			return false;
		}
	}else{
		if(gpioOutputResult->voltageHighLevel >= voltageRefernces.HIGH_OUTPUT_VOLTAGE_MIN_VALUE){
			logger->printToFileLogger("-> GPIO connected to Pull-Up, Voltage Measurement OK ", false);
			gpioOutputResult->comment = GPIO_OUTPUT_VOLTAGE_OK;
			return true;
		}else{
			logger->printToFileLogger("-> GPIO connected to Pull-Up, Voltage Measurement Not OK ", false);
			gpioOutputResult->comment = GPIO_OUTPUT_VOLTAGE_ERROR;
			return false;
		}
	}
}


/* return measured Analog-Digital-Converter Voltage
 */
float PeripheralsInterfacesController::getAdcVoltage(){
  	int mux7State = mulutiplexerController.setMultiplexer7Conenction(Mulitplexer7_Config::AdcConnection);
   	if(mux7State != MUX7_CONTROLL_ERROR){
		float voltage = adcHandler.measureVoltage();
	 	mulutiplexerController.setMultiplexer7Conenction(Mulitplexer7_Config::free);
	 	return voltage;
	}else{
	 	mulutiplexerController.setMultiplexer7Conenction(Mulitplexer7_Config::free);
		return mux7State;
	}
}

//Measure VDD and calculate GPIO Output Levels Voltages
void PeripheralsInterfacesController::measureVddAndGetGpioOutputVoltagesLevels(){
	voltageRefernces.HIGH_OUTPUT_VOLTAGE_MIN_VALUE = OUTPUT_MIN_HIGH_VOLTAGE_DEFAULT;
	voltageRefernces.LOW_OUTPUT_VOLTAGE_MAX_VALUE =  OUTPUT_MAX_LOW_VOLTAGE_DEFAULT;
 	if(mulutiplexerController.setMultiplexer6Conenction(Mux3)){
 		connectFrontMultiplexerYtoZpin(Mux3, MuxY10);
		double vddVoltage = getAdcVoltage();
		if(vddVoltage > VDD_DEFAULT_MIN && vddVoltage < VDD_DEFAULT_MMAX){
			voltageRefernces.HIGH_OUTPUT_VOLTAGE_MIN_VALUE = vddVoltage - OUTPUT_MAX_LOW_VOLTAGE_DEFAULT;
			voltageRefernces.LOW_OUTPUT_VOLTAGE_MAX_VALUE =  OUTPUT_MAX_LOW_VOLTAGE_DEFAULT;
		}else if(vddVoltage > LOW_VDD_MIN && vddVoltage < LOW_VDD_MAX){
			voltageRefernces.HIGH_OUTPUT_VOLTAGE_MIN_VALUE = vddVoltage - OUTPUT_MAX_LOW_VOLTAGE_LOW_VDD;
			voltageRefernces.LOW_OUTPUT_VOLTAGE_MAX_VALUE =  OUTPUT_MAX_LOW_VOLTAGE_LOW_VDD;
		}else{
			logger->printToFileLogger("-> Error PeripheralsInterfacesController/measureVddAndGetGpioOutputVoltagesLevels: undefined VDD Voltage: " + to_string(vddVoltage), true);
		}
	}else{
		logger->printToFileLogger("-> Error PeripheralsInterfacesController/measureVddAndGetGpioOutputVoltagesLevels: failed to measure VDD Voltage cause Multiplexer 6 connection Error", true);
	}
}

//Measure VDD and calculate GPIO Input Levels Voltages
void  PeripheralsInterfacesController::measureVddAndGetGpioInputVoltagesLevels(){
	voltageRefernces.HIGH_INPUT_VOLTAGE_MIN_VALUE = INPUT_MIN_HIGH_VOLTAGE_DEFAULT;
	voltageRefernces.LOW_INPUT_VOLTAGE_MAX_VALUE = INPUT_MAX_LOW_VOLTAGE_DEFAULT;
	mulutiplexerController.setMultiplexer6Conenction(Mux3);
	connectFrontMultiplexerYtoZpin(Mux3, MuxY10);
	float vddVoltage = getAdcVoltage();
	if(vddVoltage > 0){
		voltageRefernces.HIGH_INPUT_VOLTAGE_MIN_VALUE = 0.7 * vddVoltage;
		voltageRefernces.LOW_INPUT_VOLTAGE_MAX_VALUE = 0.3 * vddVoltage;
	}
	//TODO check


}

/* turn Raspberry Pi OFF
 */
int PeripheralsInterfacesController::shutdownPi(){
	logger->printToFileLogger("=> Preparing for shutdown Device-Tester",true);
 	int ack = system(TURN_DEVICE_TESTER_OFF_COMMAND);
 	if(ack < 0){
 		logger->printToFileLogger("PeripheralsInterfacesController:turnBoardTestOff failed to turn Device Tester Off",true);
 	}
 	return ack;
}


/* Turn USB Port ON and update Database
 */
void PeripheralsInterfacesController::turnUsbPortOn(){
	int ack = usbPortshandler.powerUsbPortOn();
	delay(_3S_DELAY);
	if(ack == 1){
		 database.updateBoardControllerUsbState(USB_PORT_SET_ON_SUCCESS);
		 usbPortStateOn = UsbPortState::ON;
	}else{
		database.updateBoardControllerUsbState(USB_PORT_SET_ON_FAILED);
 	}
}

/* turn RaspberryPi USB-Port OFF and save State
 */
int PeripheralsInterfacesController::turnUsbPortOff(){
	if(usbPortshandler.powerUsbPortOff() == 1){
		usbPortStateOn = UsbPortState::OFF;
		uartHandler.closeUart();
		database.updateBoardControllerUsbState(USB_PORT_OFF);
  		logger->printToFileLogger("-> USB Port turned OFF", false);
		return 1;
	}else{
 		logger->printToFileLogger("PeripheralsInterfacesController:turnUsbPortOff failed to turn USB Port Off", true);
		database.updateBoardControllerUsbState(USB_PORT_SET_OFF_FAILED);
		return TURN_USB_PORT_OFF_ERROR;
	}
}

/* return USB Port State
 */
UsbPortState PeripheralsInterfacesController::getUsbPortState(){
	return usbPortStateOn;
}


//LEDS
/* set LEDS New State
 */
void PeripheralsInterfacesController::setNewLedsCommand(LEDS_COMMAND command){
	ledsCommand = command;
}

/* get LEDS State
 */
LEDS_COMMAND PeripheralsInterfacesController::getLedsCommand(){
	return ledsCommand;
}

/* save Flash Process State Value
 */
void PeripheralsInterfacesController::setFlashProcessStartState(bool isStarted){
	flashProcessStateStarted = isStarted;
}

/* get Flash Process State Value
 */
bool PeripheralsInterfacesController::isFlashProcessStarted(){
	return flashProcessStateStarted;
}

/* save Continuity Process State Value
 */
void PeripheralsInterfacesController::setContinuityProcessStartState(bool isStarted){
	flashProcessStateStarted = isStarted;
}

/* get Continuity Process State Value
 */
bool PeripheralsInterfacesController::isContinuityProcessStarted(){
	return flashProcessStateStarted;
}


/* set Complete Test Process State Value
 */
void PeripheralsInterfacesController::setCompleteTestStartState(bool isStarted){
	isCompleteProcessStart = isStarted;
}

/* get Complete Test Process State Value
 */
bool PeripheralsInterfacesController::isCompelteProcessStarted(){
	return isCompleteProcessStart;
}


/* controller USB Port State and initial UART
 */
void PeripheralsInterfacesController::checkUsbPortStateAndInitialUart(){
	uartHandler.closeUart();
	if(usbPortStateOn != UsbPortState::ON){
		turnUsbPortOn();
	}
 	int uartAck = 0;
	for(int i=0; i<10; i++){
		uartAck = uartHandler.initialUart();
 		if(uartAck > 0)
			break;
		delay(_100MS_DELAY);
	}
}

bool PeripheralsInterfacesController::isSystemTerminateActivated(){
	return systemTermination;
}

void PeripheralsInterfacesController::activateSystemTerminatiom(){
	systemTermination = true;
}
