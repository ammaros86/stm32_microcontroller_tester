/*
 * Helper.cpp
 *
 *  Created on: 29.05.2023
 *      Author: Astra
 */

#include "Helper.hpp"

namespace Helper{


	/* convert String Vector to String
	 * Parameter:
	 *textVector: String Vector
	 */
	string convertGpiosStrinVectorToString(std::vector<string> textVector){
		string textResult;
		for (size_t i = 0; i < textVector.size(); ++i) {
			textResult += textVector[i];
			if (i < textVector.size() - 1) {
				textResult += " , ";
			}
		}
		return textResult;
	}

	/* convert float to Text
	 * Parameter:
	 * value: float value
	 */
	string convertFloatToString(float value){
		std::ostringstream stream;
		stream << std::fixed << std::setprecision(2) << value;
		return stream.str();
	}

	/*  trim Text
	 * Parameter:
	 * text : Text to trim
	 */
	string trimString(string text){
		size_t startTrim = text.find_first_not_of(" \t\r\n");
		size_t endTrim = text.find_last_not_of(" \t\r\n");
		if (startTrim != std::string::npos && endTrim != std::string::npos) {
			text = text.substr(startTrim, endTrim - startTrim + 1);
		}
		return text;
	}


	/* Create and return a text string which contains GPIO Mode, pull type and speed
	 * Parameter:
	 * mode: GPIO Mode
	 * pullType: GPIO Pull type
	 * speed: GPIO Speed
	 */
	string getTestFileHeaderText(uint8_t mode, uint16_t pullType, uint16_t speed){
		string testType = "";
		if(mode == COMMAND_Init_DIGITAL_OUTPUT_OD){
			testType += "Output Open Drain Mode";
		}else if(mode == COMMAND_Init_DIGITAL_OUTPUT_PP){
			testType += "Output Push Pull Mode";
		}else if(mode == COMMAND_Init_DIGITAL_INPUT){
			testType += "Input Floating Mode";
		}else if(mode == COMMAND_Init_ANALOGE){
			testType += "Analog Mode";
		}else{
			testType += "Unknown Mode";
		}

		if(pullType == COMMAND_Init_NO_PULL){
			testType += ", No Pull";
		}else if(pullType == COMMAND_Init_PULL_UP){
			testType += ", Pull-Up";
		}else if(pullType == COMMAND_Init_PULL_DOWN){
			testType += ", Pull-Down";
		}else{
			testType += ", Unknown Pull-Type";
		}
		if(mode == COMMAND_Init_DIGITAL_OUTPUT_PP){
			if(speed == COMMAND_Init_GPIO_SPEED_FREQ_LOW){
				testType += ", Speed LOW";
			}else if(speed == COMMAND_Init_GPIO_SPEED_FREQ_MEDIUM){
				testType += ", Speed Medium";
			}else if(speed == COMMAND_Init_GPIO_SPEED_FREQ_HIGH){
				testType += ", Speed High";
			}else{
				testType += ", Unknown Speed";
			}
		}
		return testType;
	}

	/* Create and return a text string that can contain error while opening
	 * connection to Nucleo Board Pin
	 * Error can be failure to control I/O  expander or multiplexer 5, 6
	 * Parameter:
	 *error: error type
	 */
	string getConnectionToPinTextError(int error){
		string errorText = "";
		if(error < 0){
			switch(error){
				case MUX6_CONTROLL_ERROR:
					errorText = "Failed cause Multiplexer 6 set connection";
					break;
				case MUX5_CONTROLL_ERROR:
					errorText = "Failed cause Multiplexer 5 set connection";
					break;
				case UNKNOWN_CONNECTED_MULTIPLEXER_TO_NUCLEO_PIN:
					errorText = "Failed to find connected Multiplexer to NUCLEO Pin";
					break;
				case UNKNOWN_IO_EXPANDER_COMMAND:
					errorText = "Unknown I/O expander command";
					break;
				case IO_EXPNADER_WRITE_ERROR:
					errorText = "Failed to Write Command I/O Expander";
					break;
				case UNKNOWN_MULTIPLEXER_SELECT:
					errorText = "Unknown multiplexer number";
					break;
				default:
					errorText = "Unknown Error: " + to_string(error);
					break;
			}
			return errorText;
		}else{
			return errorText;
		}
	}


	/* Raspberry Pi has two serial USB ports:
	 *  /dev/ttyACM0
	 *  /dev/ttyACM1
	 * this method return which Port is reserved by the Raspberry Pi
	 * for the serial communication over USB
	 */
	int getActivUsbSerialPort(){
		string activ_usb_serial_port = executeCommandAndGetOutput(Strings::GET_ACTIV_USB_SERIAL_PORT_COMMMAND);
//		delay(_1S_DELAY);
		if (activ_usb_serial_port.find(Strings::USB_SERIAL_PORT_TTYACM0) != string::npos) {
			return USB_SERIAL_PORT_TTYACM0_ACK;
		}
		if(activ_usb_serial_port.find(Strings::USB_SERIAL_PORT_TTYACM1) != string::npos){
			return USB_SERIAL_PORT_TTYACM1_ACK;
		} else{
			return USB_SERIAL_PORT_0_ACK;
		}
	}

	/*  execute terminal command and return the output data
	 *  * Parameter:
	 *  command: text string which contain the terminal command
	 */
	string executeCommandAndGetOutput(string command){
	   char commandBuffer[TERMINAL_BUFFER_SIZE];
	   string commandResult = "";
	   FILE* pipe = popen(command.c_str(), "r");
	   if (!pipe) {
		   return PIPE_OPEN_FAILED_TEXT;
	   }
	   while (!feof(pipe)) {
		  if (fgets(commandBuffer, TERMINAL_BUFFER_SIZE, pipe) != NULL)
			  commandResult += commandBuffer;
	   }
	   pclose(pipe);
	   return commandResult;
	}

	/* Create and return a text string that contain current local date and time
	 *isFileName : if the output Text used for File Name
	 */
	std::string getCurrentDateTimeText(bool isFileName) {
		struct tm *localTimeStruct;
		time_t timeTypeDef;
		time(&timeTypeDef);
		localTimeStruct = localtime(&timeTypeDef);
		char timeBuffer[DATE_TIME_BUFFER_SIZE];
		if(isFileName){
			const char * dateTimeFormat = Strings::DATE_TIME_FILE_FORMAT.c_str();
			sprintf(timeBuffer, dateTimeFormat, localTimeStruct->tm_mday, localTimeStruct->tm_mon + 1, localTimeStruct->tm_year + 1900,
				 localTimeStruct->tm_hour, localTimeStruct->tm_min, localTimeStruct->tm_sec);
		}else{
			const char * dateTimeFormat = Strings::DATE_TIME_PRINT_FORMAT.c_str();
			sprintf(timeBuffer, dateTimeFormat, localTimeStruct->tm_mday, localTimeStruct->tm_mon + 1, localTimeStruct->tm_year + 1900,
				 localTimeStruct->tm_hour, localTimeStruct->tm_min, localTimeStruct->tm_sec);
		}
		return timeBuffer;
	}

	/* Create and return a structure that contain GPIO initialization values
	 * Parameter:
	 * pin: GPIO Pin
	 * port: GPIO Port (Port A, B, C,..)
	 * pullType: GPIO Pull type (No Pull, Pull Up, Pull Down)
	 * mode: GPIO Mode (Input, Output,..)
	 * speed: GPIO Speed (LOW, MEDIUM, HIGH)
	 */
	GPIO_Initial_Struct getInitialStructure(uint8_t pin, uint8_t port, uint16_t pullType, uint8_t mode, uint16_t speed ){
		GPIO_Initial_Struct  gpioInitialStructure = {};
		gpioInitialStructure.Mode = mode;
		gpioInitialStructure.Port = port;
		gpioInitialStructure.Pull = pullType;
		gpioInitialStructure.Speed = speed;
		gpioInitialStructure.Pin = pin;
		gpioInitialStructure.pinText  = getPortText(port) + to_string(pin);
		return gpioInitialStructure;
	}

	/*  return text string that can contain GPIO Speed
	 *  * Parameter:
	 *  speed: is the GPIO Speed, defined in common.hpp
	 */
	string getSpeedText(uint16_t speed){
		string speedText = "";
		if(speed  == COMMAND_Init_GPIO_SPEED_FREQ_LOW){
			speedText = "LOW";
		}else if(speed == COMMAND_Init_GPIO_SPEED_FREQ_MEDIUM){
			speedText = "MEDIUM";
		}else if(speed == COMMAND_Init_GPIO_SPEED_FREQ_HIGH){
			speedText = "HIGH";
		}else{
			speedText = "UNKNOWN";
		}
		return speedText;
	}

	/*  return text string that can contain GPIO Port
	 *  * Parameter:
	 *  port: is the GPIO Port Register, defined in common.hpp
	 */
	string getPortText(uint8_t port){
		if(port == PORT_A){
			return "PA";
		}else if(port == PORT_B){
			return "PB";
		}else if(port == PORT_C){
			return "PC";
		}else if(port == PORT_D){
			return "PD";
		}else if(port == PORT_E){
			return "PE";
		}else if(port == PORT_F){
			return "PF";
		}else{
			return "Unknown";
		}
	}

	/*  control if GPIO MODER Register correctly initialized
	 *  * Parameter:
	 *  moderRegisterState: state of GPIO MODER Register
	 *  gpioInitialStructure: GPIO initial structure
	 */
	bool isModerRegisterInitialized(uint32_t moderRegisterState, GPIO_Initial_Struct * gpioInitialStructure){
		uint32_t gpioLsbPositionModerRegister = (gpioInitialStructure->Pin) * 2;
		uint32_t gpioMsbPositionModerRegister = (gpioInitialStructure->Pin * 2) + 1;
		if(gpioInitialStructure->Mode == COMMAND_Init_DIGITAL_OUTPUT_PP || gpioInitialStructure->Mode == COMMAND_Init_DIGITAL_OUTPUT_OD){
			if((IS_BIT_SET(moderRegisterState, gpioLsbPositionModerRegister)) && (IS_BIT_CLEAR(moderRegisterState, gpioMsbPositionModerRegister))){
				return true;
			}else{
				return false;
			}
		}else if(gpioInitialStructure->Mode == COMMAND_Init_DIGITAL_INPUT){
			if((IS_BIT_CLEAR(moderRegisterState, gpioLsbPositionModerRegister)) && (IS_BIT_CLEAR(moderRegisterState, gpioMsbPositionModerRegister))){
				return true;
			}else{
				return false;
			}
		}else if(gpioInitialStructure->Mode == COMMAND_Init_ANALOGE){
			if((IS_BIT_SET(moderRegisterState, gpioLsbPositionModerRegister)) && (IS_BIT_SET(moderRegisterState, gpioMsbPositionModerRegister))){
				return true;
			}else{
				return false;
			}
		}else if(gpioInitialStructure->Mode == COMMAND_Init_DIGITAL_OUTPUT_AF_PP || gpioInitialStructure->Mode == COMMAND_Init_DIGITAL_OUTPUT_AF_OD) {
			if((IS_BIT_CLEAR(moderRegisterState, gpioLsbPositionModerRegister)) && (IS_BIT_SET(moderRegisterState, gpioMsbPositionModerRegister))){
				return true;
			}else{
				return false;
			}
		}else if(gpioInitialStructure->Mode == COMMAND_DeInit){
			if((IS_BIT_CLEAR(moderRegisterState, gpioLsbPositionModerRegister)) && (IS_BIT_CLEAR(moderRegisterState, gpioMsbPositionModerRegister))){
				return true;
			}else{
				return false;
			}
		}else{
			return false;
		}
	}

	/*  control if GPIO PupDr Register correctly initialized
	 *  * Parameter:
	 *  PupDrRegisterState: state of GPIO PupDr Register
	 *  gpioInitialStructure: GPIO initial structure
	 */
	bool isPullTypeRegisterInitialized(uint32_t PupDrRegisterState, GPIO_Initial_Struct * gpioInitialStructure){
		uint32_t gpioLsbPositionPupdrRegister = (gpioInitialStructure->Pin) * 2;
		uint32_t gpioMsbPositionPupdrRegister = (gpioInitialStructure->Pin * 2) + 1;
		if(gpioInitialStructure->Pull == COMMAND_Init_NO_PULL){
			if((IS_BIT_CLEAR(PupDrRegisterState, gpioLsbPositionPupdrRegister)) && (IS_BIT_CLEAR(PupDrRegisterState, gpioMsbPositionPupdrRegister))){
				return true;
			}else{
				return false;
			}
		 }else if(gpioInitialStructure->Pull == COMMAND_Init_PULL_UP){
				if((IS_BIT_SET(PupDrRegisterState, gpioLsbPositionPupdrRegister)) && (IS_BIT_CLEAR(PupDrRegisterState, gpioMsbPositionPupdrRegister))){
				return true;
			}else{
				return false;
			}
		}else if(gpioInitialStructure->Pull == COMMAND_Init_PULL_DOWN){
			if((IS_BIT_CLEAR(PupDrRegisterState, gpioLsbPositionPupdrRegister)) && (IS_BIT_SET(PupDrRegisterState, gpioMsbPositionPupdrRegister)) ){
				return true;
			}else{
				return false;
			}
		}else {
			return false;
		}
	}

	/*  control if GPIO OSpeed Register correctly initialized
	 *  * Parameter:
	 *  oSpeedRegister: state of GPIO OSpeed Register
	 *  gpioInitialStructure: GPIO initial structure
	 */
	bool isSpeedRegisterInitialized(uint32_t oSpeedRegisterState, GPIO_Initial_Struct * gpioInitialStructure){
		uint32_t gpioLsbPositionSpeedRegister =   (gpioInitialStructure->Pin) * 2;
		uint32_t gpioMsbPositionSpeedRegister =  (gpioInitialStructure->Pin*2) + 1;
		if(gpioInitialStructure->Speed == COMMAND_Init_GPIO_SPEED_FREQ_LOW){
			if(IS_BIT_CLEAR(oSpeedRegisterState, gpioLsbPositionSpeedRegister)){
				return true;
			}else{
				return false;
			}
		 }else if(gpioInitialStructure->Speed == COMMAND_Init_GPIO_SPEED_FREQ_MEDIUM){
			if((IS_BIT_SET(oSpeedRegisterState, gpioLsbPositionSpeedRegister)) && (IS_BIT_CLEAR(oSpeedRegisterState, gpioMsbPositionSpeedRegister))){
				return true;
			}else{
				return false;
			}
		}else if(gpioInitialStructure->Speed == COMMAND_Init_GPIO_SPEED_FREQ_HIGH){
			if((IS_BIT_SET(oSpeedRegisterState, gpioLsbPositionSpeedRegister)) && (IS_BIT_SET(oSpeedRegisterState, gpioMsbPositionSpeedRegister))){
				return true;
			}else{
				return false;
			}
		}else {
			return false;
		}
	}

	/*  control if GPIO Otype Register correctly initialized
	 *  * Parameter:
	 *  oTypeRegisterState: state of GPIO OType Register
	 *  gpioInitialStructure: GPIO initial structure
	 */
	bool isOtypeRegisterInitialized(uint32_t oTypeRegisterState, GPIO_Initial_Struct * gpioInitialStructure){
		uint32_t gpioPositionOtyperRegister =  gpioInitialStructure->Pin;
		if(gpioInitialStructure->Mode == COMMAND_Init_DIGITAL_OUTPUT_PP){
			if(IS_BIT_CLEAR(oTypeRegisterState, gpioPositionOtyperRegister)){
				return true;
			}else{
				return false;
			}
		}else {
			if(IS_BIT_SET(oTypeRegisterState, gpioPositionOtyperRegister)){
				return true;
			}else{
				return false;
			}
		}
	}

	/*  return text string that can contain Error while GPIO initializing
	 *  * Parameter:
	 *  initialState: class defined in common.hpp
	 */
	string getGpioInitialErrorText(GPIO_INITIAL_STATE initialState){
		string text ;
		switch(initialState){
			case GPIO_INITIAL_STATE::gpioInitialFail:
				text = "GPIO initial failed";
				break;
			case GPIO_INITIAL_STATE::errorSendCommand:
				text = "Send initial Command failed";
				break;
			case GPIO_INITIAL_STATE::errorReadCommand:
				text = "Read initial Acknowledge failed";
				break;
			default:
				text= "Unknown Error";
				break;
		}
		return text;
	}


	/*
	 * return Text string contains Acknowledge of the Register state after set GPIO Level
	 * Parameter:
	 * state: GPIO Register State after set Output Level
	 */
	string getHandleRegistersAckText(OUTPUT_HANDLE_STATE state){
		string text = "";
		if(state == OUTPUT_HANDLE_STATE::gpioUndefinedOutputLevel){
			text = "undefined Output Level";
		}else if(state == OUTPUT_HANDLE_STATE::gpioOutputHandleFailedToSendData){
			text = "failed to send data";
		}else if(state == OUTPUT_HANDLE_STATE::gpioFailedToReadRegisters){
			text = "failed to read data Registers";
		}else if(state == OUTPUT_HANDLE_STATE::gpioSetHighSuccess) {
			text = "Success";
		}else if(state == OUTPUT_HANDLE_STATE::gpioSetLowSuccess) {
			text = "Success";
		}else if(state == OUTPUT_HANDLE_STATE::gpioFailedSetHigh){
			text = "failed to set GPIO Level HIGH";
		}else {
			text = "Failed to set GPIO Level LOW"  ;
		}
		return text;
	}

	/* control if GPIO is PC13 (connected to Push-Button)
	 * Parameter:
	 *gpioInitialStructure: initialization structure of the GPIO Output
	*/
	bool isGpioConnectedToPushButton(GPIO_Initial_Struct *gpioInitialStructure){
		if((gpioInitialStructure->Pin == PIN13) && (gpioInitialStructure->Port == PORT_C)){
			return true;
		}else{
			return false;
		}
	}

	/*  return a structure that contain Nucleo GPIO Registers Values
	 *  * Parameter:
	 *  readBuffer: received Buffer from Nucleo Board
	 */
	GPIO_Registers_Struct getNucleoBoardRegisters(uint8_t * readBuffer){
		GPIO_Registers_Struct *gpio_registers_state = new GPIO_Registers_Struct ;
		if(readBuffer == NULL) {
			return *gpio_registers_state;
		}
		gpio_registers_state = reinterpret_cast<GPIO_Registers_Struct *>(readBuffer);
		return *gpio_registers_state;
	}

	/* return I/O Expander Value to control the multiplexer connected to pin under Test
	 * this value is token from the table of multiplexer 74HC4067 data sheet
	 *  * Parameter:
	 *  muxNumber: Multiplexer number which connected to pin
	 *  pinNumber: pin number of the multiplexer
	 *  mux_select_vector: select pins of the multiplexer which should be
	 *  set High to connect the pin with Z-Pin (output) of the multiplexer
	 */
	int getIoExpanderCommand(int muxNumber, uint8_t pinNumber, std::vector<IO_Expander_Multiplexer_Select_Commands> * mux_select_vector){
		uint8_t command = UNKNOWN_IO_EXPANDER_COMMAND;
		uint8_t s0Multiplexer = static_cast<uint8_t>(mux_select_vector->at(0));
		uint8_t s1Multiplexer = static_cast<uint8_t>(mux_select_vector->at(1));
		uint8_t s2Multiplexer = static_cast<uint8_t>(mux_select_vector->at(2));
		uint8_t s3Multiplexer = static_cast<uint8_t>(mux_select_vector->at(3));
		switch(pinNumber){
			case MuxY0 :
				command = 0x00;
				break;
			case MuxY1 :
				command =  s0Multiplexer;
				break;
			case MuxY2 :
				command =  s1Multiplexer;
				break;
			case MuxY3 :
				command =  s0Multiplexer | s1Multiplexer;
				break;
			case MuxY4 :
				command = s2Multiplexer;
				break;
			case MuxY5 :
				command =  s0Multiplexer | s2Multiplexer;
				break;
			case MuxY6 :
				command =  s1Multiplexer | s2Multiplexer;
				break;
			case MuxY7 :
				command =  s0Multiplexer | s1Multiplexer | s2Multiplexer;
				break;
			case MuxY8 :
				command =  s3Multiplexer;
				break;
			case MuxY9 :
				command =  s0Multiplexer | s3Multiplexer;
				break;
			case MuxY10 :
				command =  s1Multiplexer | s3Multiplexer;
				break;
			case MuxY11 :
				command =  s0Multiplexer | s1Multiplexer | s3Multiplexer;
				break;
			case MuxY12 :
				command =  s2Multiplexer| s3Multiplexer;
				break;
			case MuxY13 :
				command =  s0Multiplexer | s2Multiplexer | s3Multiplexer;
				break;
			case MuxY14 :
				command =  s1Multiplexer | s2Multiplexer | s3Multiplexer;
				break;
			case MuxY15 :
				command =   s0Multiplexer | s1Multiplexer | s2Multiplexer | s3Multiplexer ;
				break;
			default:
				command = UNKNOWN_IO_EXPANDER_COMMAND;
				break;
			}
			return command;
	}

	/* check which Multiplexer is active and save select Pins in a vector,
	 *  return 1 if the multiplexer value is valid
	 *  * Parameter:
	 *  muxNumber : Multiplexer number that is connected to Pin
	 *  mux_select_vector: pointer of the vector where the select Pins will be saved
	 */
	int initMultiplexerSelectPins(int muxNumber, std::vector<IO_Expander_Multiplexer_Select_Commands> *mux_select_vector){
		int ack = 1;
		mux_select_vector->clear();
		switch(muxNumber){
			case Mux1:
				mux_select_vector->push_back(IO_Expander_Multiplexer_Select_Commands::S0_MUX1);
				mux_select_vector->push_back(IO_Expander_Multiplexer_Select_Commands::S1_MUX1);
				mux_select_vector->push_back(IO_Expander_Multiplexer_Select_Commands::S2_MUX1);
				mux_select_vector->push_back(IO_Expander_Multiplexer_Select_Commands::S3_MUX1);
				break;
			case Mux2:
				mux_select_vector->push_back(IO_Expander_Multiplexer_Select_Commands::S0_MUX2);
				mux_select_vector->push_back(IO_Expander_Multiplexer_Select_Commands::S1_MUX2);
				mux_select_vector->push_back(IO_Expander_Multiplexer_Select_Commands::S2_MUX2);
				mux_select_vector->push_back(IO_Expander_Multiplexer_Select_Commands::S3_MUX2);
				break;
			case Mux3:
				mux_select_vector->push_back(IO_Expander_Multiplexer_Select_Commands::S0_MUX3);
				mux_select_vector->push_back(IO_Expander_Multiplexer_Select_Commands::S1_MUX3);
				mux_select_vector->push_back(IO_Expander_Multiplexer_Select_Commands::S2_MUX3);
				mux_select_vector->push_back(IO_Expander_Multiplexer_Select_Commands::S3_MUX3);
				break;
			case Mux4:
				mux_select_vector->push_back(IO_Expander_Multiplexer_Select_Commands::S0_MUX4);
				mux_select_vector->push_back(IO_Expander_Multiplexer_Select_Commands::S1_MUX4);
				mux_select_vector->push_back(IO_Expander_Multiplexer_Select_Commands::S2_MUX4);
				mux_select_vector->push_back(IO_Expander_Multiplexer_Select_Commands::S3_MUX4);
				break;
			default:
				ack = UNKNOWN_CONNECTED_MULTIPLEXER_TO_NUCLEO_PIN;
				break;
		}
		return ack ;
	}

	/* check and return true if GPIO Pin Testable
	* NOT Testable Pins: PC15, PC14, PF0, PA3, PA2, PA13, PA14
	*  * Parameter:
	*  port : GPIO Port of the Pin
	*  pin: Pin Number
	*/
	bool isGpioTestable(uint8_t port, uint8_t pin){
		if((pin == PIN15 && port == PORT_C) || (pin == PIN14 && port == PORT_C) || (pin == PIN0 && port == PORT_F)
				||(pin == PIN3 && port == PORT_A) || (pin == PIN2 && port == PORT_A)|| (pin == PIN13 && port == PORT_A)|| (pin == PIN14 && port == PORT_A)){
			return false;
		}else{
			return true;
		}
	}

	/* return String with Test Command Type
	*  * Parameter:
	*  command : Test Command
	*/

	string getTestCommandText(TestCommand command){
		string textCommand = "";
		switch(command){
			case TestCommand::newBoard:
				textCommand = "New Board Connection";
				break;
			case TestCommand::measureTemperature:
				textCommand = "Measure Nucleo Board Temperature";
				break;
			case TestCommand::boardCurrent:
				textCommand = "Measure STM32 Microcontroller Current Consumption";
				break;
			case TestCommand::flashBoard:
				textCommand = "Flash Test Program to Nucleo Board";
				break;
			case TestCommand::powerGndPinsTest:
				textCommand = "Power and GND Pins Test";
				break;
			case TestCommand::continuityTest:
				textCommand = "Test GPIOs Continuity";
				break;
			case TestCommand::testGpioOutput:
				textCommand = "Test GPIOs Output";
				break;
			case TestCommand::testGpioInput:
				textCommand = "Test GPIOs Input";
				break;
			case TestCommand::testAnalogInput:
				textCommand = "Test Analog Input";
				break;
			case TestCommand::testGpioShortCircuit:
				textCommand = "Test GPIOs Short Circuit";
				break;
			case TestCommand::getServerIpAddress:
				textCommand = "Identifying the server's IP address";
				break;
			case TestCommand::copyLogsUsb:
				textCommand = "Copying Test log files to the USB stick";
				break;
			case TestCommand::turnUsbPortOn:
				textCommand = "Turn USB Port On";
				break;
			case TestCommand::powerBoardTesterOff:
				textCommand = "Turn Board Tester Off";
				break;
			default:
				break;
		}
		return textCommand;
	}

	/*
	 * convert string vector to string and add it to text string
	 * Parameter:
	 * stringVector: string vector
	 * text: text String
	 */
	string addVectorStringToText(string text, std::vector<string> stringVector){
		for (size_t i = 0; i < stringVector.size(); ++i) {
			if (i == 0) {
				text += ": ";
			}
			text += stringVector[i];
			if (i != stringVector.size() - 1) {
				text += ", ";
			}
		}
		return text;
	}


}
