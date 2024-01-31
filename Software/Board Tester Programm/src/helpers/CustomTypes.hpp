/*
 * CommandStructEnum.hpp
 *
 *  Created on: 13.08.2023
 *      Author: Astra
 */

#ifndef HELPERS_CUSTOMTYPES_HPP_
#define HELPERS_CUSTOMTYPES_HPP_

#include <vector>
#include <iostream>
using namespace std;
#include "Constants.hpp"


//NUCLEO BOARD Connection
enum class UsbPortState: int {None = 0, ON = 1, OFF = 2};

//NUCLEO BOARD Connection
enum class Buttons: int {None = 0, Green = 1, Yellow = 2, Blue = 3, Red = 4};

//NUCLEO BOARD Connection
enum class HEADER_CONNECTION_STATE: int {connected = 1, notConnected = 2, notCorrectlyConnectedOrErros = 3};


//LEDS Commands
enum class LEDS_COMMAND : int{none, setGreenOn, setGreenOFF, setRedOn, setRedOff, setAllOn, setAllOff, blinkRed, blinkGreen, blinkAll};


//LCD Display Menu Current Item
enum class Display_Menu_Items: uint16_t {title = 1, newBoardItem = 2, completeItem = 3, continuityItem = 4, powerPinsItem = 5,  flashItem = 6, boardCurrentItem = 7,  temperatureItem = 8, gpioInputItem = 9, gpioOutputItem = 10,
		gpioShortCircuitItem = 11, analogItem = 12, ipAddressItem = 13, copyLogsUsbItem = 14, turnUsbPortOnItem = 15, powerBoardTesterOffItem = 16};


//LCD Display Menu Scroll Type
enum class Display_Menu_Scroll_Type: uint16_t {up = 1, down = 2};


//LCD Display Menu Sections
enum class Display_Menu_Sections: uint16_t {section1 = 1, section2 = 2, section3 = 3, section4 = 4, section5 = 5, section6 = 6, section7 = 7, section8 = 8, section9 = 9, section10 = 10, section11 = 11, section12 = 12, section13 = 13};


//Test Process State
enum class Test_Process_State: uint16_t {free = 0, working = 1};


//Test Commands
enum class TestCommand{notselected = 0, newBoard = 1,  completTest = 2, continuityTest = 3, powerGndPinsTest = 4, flashBoard = 5, boardCurrent = 6, measureTemperature = 7, testGpioInput = 8, testGpioOutput = 9,
	testGpioShortCircuit = 10, testAnalogInput = 11, getServerIpAddress = 12, copyLogsUsb = 13, turnUsbPortOn = 14, powerBoardTesterOff = 15};


//Test Commands
enum class BoardControllerCommand{notselected = 0,  copyLogsUsb = 1, turnUsbPortOn = 2, nucleoConnectionState = 3, powerBoardTesterOff = 4};


//Multiplexer 7 Connection
enum class Mulitplexer7_Config : int{none, free, AdcConnection, DacConnection, AdcDacConnection, CurrentSourceConnection};


//Connected Multiplexers to NUCLEO Board Pins
enum class Connected_Multiplexer: uint16_t {MUX1 = 1, MUX2 = 2, MUX3 = 3, MUX4 = 4, MUX5 = 5};


//Multiplexer Pins
enum class MUTLIPLEXER_PINS : uint8_t {Y0 = 0, Y1 = 1, Y2 = 2, Y3 = 3, Y4 = 4, Y5 = 5, Y6 = 6, Y7 = 7, Y8 = 8, Y9 = 9, Y10 = 10, Y11 = 11, Y12 = 12, Y13 = 13, Y14 = 14, Y15 = 15};


//NUCLEO Board Power Pins Type
enum class POWER_PIN_TYPE : uint8_t {GND = 0, POWER3V = 1, POWER5V = 2, E5V = 3, VIN = 4 };


//Connected Multiplexers Pins to NUCLEO Board Power and GND Pins
enum class Power_GND_Connected_Multiplexer_Pin : uint16_t {U5V = 4, GND1 = 15, AGND = 9, AVDD = 11, GND2 = 12, VDD = 10, GND3 = 15, VBAT = 1, E5V = 0, GND4 = 3, IOREF = 4,
											P3V3 = 14, P5V = 13, GND5 = 12, GND6 = 11, VIN = 10};

//GPIO Initialization State
enum class GPIO_INITIAL_STATE: uint8_t{initialised = 1, gpioInitialFail = 2, errorSendCommand = 3, errorReadCommand = 4};


//NUCLEO Board GPIO Ports
enum class GPIO_PINS_PORTS : uint8_t {portA = PORT_A, portB = PORT_B, portC = PORT_C, portD = PORT_D, portE = PORT_E, portF = PORT_F};


//GPIO Input Level State
enum class GPIO_Level_STATE: uint8_t{failedToReadLevelState = 0, failedToSendCommand =1, high = 2, low = 3};


//GPIO Handle State
enum class  OUTPUT_HANDLE_STATE: uint8_t{ gpioUndefinedOutputLevel= 0, gpioOutputHandleFailedToSendData = 1, gpioFailedToReadRegisters = 2, gpioSetHighSuccess = 3, gpioSetLowSuccess = 4, gpioFailedSetHigh = 5, gpioFailedSetLow = 6};


//Multiplexer 1, 2, 3, 4 Select Pins I/O Expander Command
enum class IO_Expander_Multiplexer_Select_Commands: uint8_t{S0_MUX1= 0x80, S1_MUX1 = 0x40, S2_MUX1 = 0x20, S3_MUX1 = 0x10,
							S0_MUX2 =  0x08,  S1_MUX2 =  0x04, S2_MUX2 =  0x02,  S3_MUX2 = 0x01,
							S0_MUX3= 0x01, S1_MUX3 = 0x02, S2_MUX3 = 0x04, S3_MUX3 = 0x08,
							S0_MUX4= 0x10, S1_MUX4 = 0x20, S2_MUX4 = 0x40, S3_MUX4 = 0x80,
							};


//NUCLEO Board GPIOs
enum class GPIO_Pins : uint8_t {PB11 = PIN11, PC8 = PIN8, PC6 = PIN6, PC5 = PIN5, PA12 = PIN12, PA11 = PIN11, PB12 = PIN12, PB2 = PIN2,
							PB1 = PIN1, PB15 = PIN15, PB14 = PIN14, PB13 = PIN13, PC4 = PIN4, PC9 = PIN9, PB8 = PIN8, PB9 = PIN9,
							PA5 = PIN5, PA6 = PIN6, PA7 = PIN7, PB6 = PIN6, PC7 = PIN7, PA9 = PIN9, PA8 = PIN8, PB10 = PIN10, PB4 = PIN4,
							PB5 = PIN5, PB3 = PIN3, PA10 = PIN10, PA2 = PIN2, PA3 = PIN3, PC11 = PIN11, PD2 = PIN2, PA0 = PIN0, PA1 = PIN1, PA4 = PIN4,
							PB0 = PIN0, PC1 = PIN1, PC0 = PIN0, PC10 = PIN10, PC12 = PIN12, PA13 = PIN13, PA14 = PIN14, PA15 = PIN15, PB7 = PIN7,
							PC13 = PIN13, PC14 = PIN14, PC15 = PIN15, PF0 = PIN0, PF1 = PIN1, PC2 = PIN2, PC3 = PIN3};


//NUCLEO Board Analog Pins
enum class GPIO_TYPE : uint8_t {FT = 1, TT=2 };


//NUCLEO Board Analog Pins
enum class Analog_Pins : uint8_t {PC5 = PIN5, PB12 = PIN12, PB11 = PIN11, PB2 = PIN2, PB1 = PIN1, PB15 = PIN15, PB14 = PIN14, PB13 = PIN13, PC4 = PIN4,
	PA7 = PIN7, PA6 = PIN6, PA5 = PIN5, PC0 = PIN0, PC1 = PIN1, PB0 = PIN0, PA4 = PIN4, PA1 = PIN1, PA0 = PIN0, PC3 = PIN3, PC2 = PIN2 };




//GPIO SHort Circuit Test Result Structure
typedef struct{
	string gpio;
	int detectedShortCircuits;
	string comment;
}GPIO_SHORT_TEST_RESULT;

//Power GND Pins Test Result Structure
typedef struct{
	string pin;
	float voltage;
	string comment;
}Power_GND_TEST_RESULT;

//GPIO Initialize Structure
typedef struct{
	uint8_t Pin;
	uint8_t Port;
	uint8_t Mode;
	uint16_t Pull;
	uint16_t Speed;
	string pinText;
}GPIO_Initial_Struct;


//GPIO Continuity Test Result Structure
typedef struct{
	string pin;
 	double voltage;
 	string comment;
}Continuity_Test_Result_Struct;


//GPIO Output Test Result Structure
typedef struct{
	double voltageHighLevel;
 	double voltageLowLevel;
 	string comment;
	string pin;
	uint16_t PullType;
	uint16_t speed;
	bool isGpioOutputTestOk;
	int setHighLevelState;
	int setLowLevelState;
}GPIO_Output_Test_result_Struct;


//GPIO Input Test Result Structure
typedef struct{
 	int highLevelState;
 	int lowLevelState;
 	string comment;
	string pin;
	uint16_t PullType;
}GPIO_Input_Test_Result_Struct;


//GPIO Analog Input Test Result Structure
typedef struct{
	double measuredVotalge1;
	double measuredVotalge2;
	double measuredVotalge3;
	double measuredVotalge4;
 	string comment;
	string pin;
}Analog_Input_Test_Result_Struct;


//NUCLEO Board GPIOs Registers
typedef struct{
  uint32_t ModerRegiste;
  uint32_t OtypeRegister;
  uint32_t OSpeedRegister;
  uint32_t PupDrRegister;
  uint32_t IdrRegister;
  uint32_t OdrRegister;
}GPIO_Registers_Struct;


//Flash Handle Errors while flashing Check Structure
typedef struct{
	int mux7Ack;
	int dacAck0V;
	int dacAck3V;
	int mux6Ack;
	int connectionPin;
	int dac2Ack0V;
	int mux7Ack2;
	int usbRestartAck;
	int uartOpenAck;
	int uartCloseAck;
}Flash_Handle_Check_Struct;


//Flash Test Result Structure
typedef struct{
 int isUploadTestProgrammOK;
 string comment;
 int flash_test;
 int flash_errors;
}Flash_Memory_Test_Struct;


//Voltage References Structure
typedef struct{
	float POWER_VOLTAGE_5_MIN;
	float POWER_VOLTAGE_5_MAX;
	float GND_VOLTAGE_MIN;
	float GND_VOLTAGE_MAX;
	float POWER_VOLTAGE_3V3_MIN;
	float POWER_VOLTAGE_3V3_MAX;
	float POWER_VOLTAGE_E5V_MIN;
	float POWER_VOLTAGE_E5V_MAX;
	float ADC_REFERENCE_VOLATGE;
	float DAC_REFERENCE_VOLATGE;
	float HIGH_INPUT_VOLTAGE_MIN_VALUE;
	float LOW_INPUT_VOLTAGE_MAX_VALUE;
	float HIGH_OUTPUT_VOLTAGE_MIN_VALUE;
	float LOW_OUTPUT_VOLTAGE_MAX_VALUE;
	float ANALOG_NULL_VOLTAGE_VALUE;
	float ANALOG_MAX_VOLTAGE_VALUE;
}Voltages_References_Struct;


//Power and GND Pins Vectors Structure
typedef struct{
	std::vector<Power_GND_Connected_Multiplexer_Pin> * powerGndMuxPinVectors;
	std::vector<Connected_Multiplexer> * powerGndMuxConnectionVector;
 	std::vector<string> * powerGndPinsStringVector;
	std::vector<POWER_PIN_TYPE> * powerGndPinsType;
}Power_GND_Vectors_Struct;


//Power and GND Pins Measured Voltage Structure
typedef struct{
	double u5vVoltage;
	double gnd1Voltage;
	double agndVoltae;
	double avddVoltage;
	double gnd2Voltage;
	double vddVoltage;
	double gnd3Voltage;
	double vbatVoltage;
  	double gnd4Voltage;
 	double iorefVoltage;
 	double _3vVoltage;
 	double _5vVoltage;
 	double gnd5Voltage;
 	double gnd6Voltage;
}Power_GND_Test_Result_Struct;


//NUCLEO Board Board Connection Result
typedef struct{
	HEADER_CONNECTION_STATE cn10ConnectionState;
	HEADER_CONNECTION_STATE cn7ConnectionState;
	bool isUsbConnected;
}BoardConnectionStructure;


//GPIOs Short-Circuit
typedef struct{
	std::string pin1;
	std::string pin2;
}ShortCircuit;


//Flash-Memory Test Result
typedef struct{
	uint16_t testedPages ;
    uint16_t erasedPages ;
    uint32_t writedAddresses ;
    uint32_t writedAddressesError;
    uint32_t readedAddresses ;
    uint32_t readedAddressesError;
    uint32_t erasedPagesError ;
} FLASH_TestResultTypeDef;



#endif /* HELPERS_CUSTOMTYPES_HPP_ */
