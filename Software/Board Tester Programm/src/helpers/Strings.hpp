/*
 * Strings.hpp
 *
 *  Created on: 25.07.2023
 *      Author: Astra
 */

#ifndef HELPERS_STRINGS_HPP_
#define HELPERS_STRINGS_HPP_



#include <string>

namespace Strings {

	const std::string newLine = "\n " ;
	const std::string doubleArrow = "=> " ;
	const std::string arrow = "-> " ;

	const std::string gpio = "GPIO:" ;
	const std::string error = "Error" ;
	const std::string ok = "OK" ;
	const std::string currentmA = " mA";
	const std::string volt = " V";
	const std::string testing = "testing";

    const  std::string shortGpio = "GPIO shorted to GND or VCC";
    const  std::string openGpio = "GPIO open or ESD-Diode damage";
    const  std::string gpioOk = "GPIO Continuity OK";
    const  std::string gpioStandardOpen = "GPIO Standard Open";

    const  std::string measureVoltageFailed = "failed measure voltage";
    const  std::string openMultiplexersFailed = "failed to open Multiplexers connection";
    const  std::string measureVoltage = "Measured Voltage = ";


	 const std::string gpioContinuityTestResult = "* GPIOs Continuity Test Result: ";
	 const std::string failedToTest = " failed Test " ;
	 const std::string gpios = "  GPIOs: ";
	 const std::string shortedToGndOrVcc = " Shorted GPIOs to GND or VCC ";
	 const std::string openGpios = " Open GPIOs or ESD-Damage ";
	 const std::string continuityTestPass = "=> GPIOs Continuity Test Pass";
	 const std::string continuityTestFail = "=> GPIOs Continuity Test Fail";
	 const std::string failedToTurnUsbPortOff = "failed to turn USB-PORT OFF";
	 const std::string failedToOpenConnectionToCurrentrSource = "failed to open Connection to Current Source";
	 const std::string failedContinuityTest = " Continuity Test failed cause: ";


	 const std::string microcontrollerCurrentConsumption = "* STM32 Microcontroller Current Consumption: ";
	 const std::string failedToMeasureCurrent = " => Failed to measure STM32 Microcontroller Current Consumption, check Current Measurement Cable Connection";
	 const std::string currentConsumptionMessagePrefix =  " => Microcontroller Current Consumption: ";
	 const std::string highCurrentConsumptionDetected = "=> Microcontroller High Current Consumption detected: ";

	 const std::string testPass = " pass";
	 const std::string testFail = " fail";


	 //POWER GND PINS
	 const std::string measuredVoltageOutOfRange = "-> Error Voltage Out of Range";
	 const std::string measuredVoltageOk = "-> Voltage OK";
	 const std::string measuredVoltage = "  Measured Voltage: ";
	 const std::string failedTestPin = " Failed to test Pins";
	 const std::string errorPin = " Error Pins";
	 const std::string powerGndPinsTestResult = "Power and GND Pins Test Result:";
	 const std::string powerGndPinsTestResultPass = "=> Power and GND Pins Test pass, No Errors";
	 const std::string powerGndPinsTestResultFail = "=> Power and GND Pins Test fail";


	 //LOG
	 const std::string simpleTestLogFileNamePrefix = "/var/www/LogFiles/SimpleLog_";
	 const std::string detailTestLogFileNamePrefix = "/var/www/LogFiles/DetailedLog_";
	 const std::string reportFileNameSuffix = ".txt";
	 const std::string reportFirstLine = "Test Nucleo Board ";

	 //MAIN
	 const std::string programmStart = "Program Start";
	 const std::string programmStartInitialError = "Board Tester initialization failed : ";
	 const std::string programmStartInitialOk = "Board Tester initialized correctly";

	 //DATABSE CONTROLLER
	 const std::string hostNameDatabse = "localhost";
	 const std::string userNameDatabse = "admin";
	 const std::string passwordDatabse = "Master2023";
	 const std::string shemaDatabse = "master";


	 //Flash
	 const std::string flashErrorUsbPort = "-> Error: Flash/writeTestProgrammToFlash: failed to identify active USB Serial Port";
	 const std::string flashWriteError =  "-> Error: Flash/writeTestProgrammToFlash: failed to write Test-Program to Flash Memory";
	 const std::string flashFailedText =  "Flash failed";
	 const std::string flashFailedToGetAckText =  "-> Error: Flash/getFlashInfosText failed to get Flash Text Acknowledge";

	const std::string USB_SERIAL_PORT_TTYACM0 = "/dev/ttyACM0";
	const std::string USB_SERIAL_PORT_TTYACM1 = "/dev/ttyACM1";
	const std::string I2C_FILE_PATH = "/dev/i2c-1";

	const std::string GET_ACTIV_USB_SERIAL_PORT_COMMMAND = "sudo ls /dev/ttyACM*";
	const std::string CHECK_NUCLEO_BOARD_USB_CONNECTED_COMMAND = "sudo /home/BoardTester/checkNucleoConnected.sh";
	const std::string POWER_OFF_USB_COMMAND = "echo '1-1' | sudo tee /sys/bus/usb/drivers/usb/unbind";
	const std::string POWER_ON_USB_COMMAND = "echo '1-1' | sudo tee /sys/bus/usb/drivers/usb/bind";
	const std::string USB_COMMAND_SET_ACK = "1-1";
	const std::string USB_BOARD_CONNECTED_ACK = "connected";
	const std::string USB_BOARD_NOT_CONNECTED_ACK = "not connected";

	// LCD-DISPLAY MENU
	const std::string PRINT_BOARD_TESTER = "Board Tester";
	const std::string NEW_BOARD = "New Board";
	const std::string CONTINUITY_TEST = "Continuity Test";
	const std::string BOARD_CURRENT = "Board Current";
	const std::string TEMPERATURE = "Temperature";
	const std::string POWER_GND = "Power Pins Test";
	const std::string FLASH = "Flash Test";
	const std::string GPIO_OUTPUT_TEST = "GPIO Output Test";
	const std::string GPIO_INPUT_TEST = "GPIO Input Test";
	const std::string GPIO_SHORT_TEST = "GPIO Short Circuit";
	const std::string ANALOG_TEST = "Analog Pins Test";
	const std::string COMPLETE_TEST = "Complete Test";
	const std::string PRINT_IPADDRESS = "Server Ip-Address";
	const std::string COPY_FILES = "Export Logs";
	const std::string USB_PORT_ON = "Activate USB Ports";
	const std::string POWER_OFF = "Power Off";

	const std::string DATE_TIME_FILE_FORMAT = "%02d_%02d_%04d_%02d_%02d_%02d";
	const std::string DATE_TIME_PRINT_FORMAT = "%02d/%02d/%04d %02d:%02d:%02d";

}



#endif /* HELPERS_STRINGS_HPP_ */
