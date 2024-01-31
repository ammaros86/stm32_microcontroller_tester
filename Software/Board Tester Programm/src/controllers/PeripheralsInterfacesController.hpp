/*
 * PeripheralsInterfacesController.hpp
 *
 *  Created on: 30.08.2023
 *      Author: Astra
 */

#ifndef CONTROLLERS_PERIPHERALSINTERFACESCONTROLLER_HPP_
#define CONTROLLERS_PERIPHERALSINTERFACESCONTROLLER_HPP_


#include "../database/databasecontroller.hpp"
#include "../peripherals/AnalogDigitalConverter.hpp"
#include "../peripherals/DigitalAnalogConverter.hpp"
#include "../peripherals/IoExpander.hpp"
#include "../peripherals/LcdDisplay.hpp"
#include "../peripherals/Mulitplexers.hpp"
#include "../peripherals/CurrentSensor.hpp"
#include "../interfaces/UartHandler.hpp"
#include "../interfaces/UsbPortsHandler.hpp"

#define COMMAND_TYPE_INIT 0x00
#define COMMAND_TYPE_HANDLE 0x01
#define COMMAND_INIT_GPIO_SHIFT 11
#define COMMAND_HANDLE_GPIO_SHIFT 7
#define OUTPUT_MIN_HIGH_VOLTAGE_DEFAULT 2
#define OUTPUT_MAX_LOW_VOLTAGE_DEFAULT 1.3
#define OUTPUT_MAX_LOW_VOLTAGE_LOW_VDD 0.4
#define INPUT_MIN_HIGH_VOLTAGE_DEFAULT 2.31
#define INPUT_MAX_LOW_VOLTAGE_DEFAULT 0.99
#define LOW_VDD_MIN 2
#define LOW_VDD_MAX 2.7
#define VDD_DEFAULT_MIN 2.7
#define VDD_DEFAULT_MMAX 3.6
#define OUPUT_INITIAL_FAILURE_SHORT -4

class PeripheralsInterfacesController {

public:

	PeripheralsInterfacesController(Logger *_Logger): logger(_Logger){
		ledsCommand = LEDS_COMMAND::none;
		usbPortStateOn = UsbPortState::None;

	    flashProcessStateStarted = false;
	    continuityProcessStateStart = false;
		isCompleteProcessStart = false;
		systemTermination = false;
 		voltageRefernces = {};
	 };

	PeripheralsInterfacesController();
 	virtual ~PeripheralsInterfacesController();

	int openMultiplexerConnectionToGpio(uint8_t positionGpioPinVector);
  	int connectFrontMultiplexerYtoZpin(int multiplexerNumber, uint8_t pinNumber);
 	int openMuxConnectionToTestPowerGndPin(bool printVoltage, int position);
	int openMultiplexerConnectionToAnalog(uint8_t positionAnalogPinVector);

 	GPIO_INITIAL_STATE initialGpioAndCheckGpiosAckRegisters(GPIO_Initial_Struct * gpioInitialStructure);
 	int handleGpio(uint16_t pinNumber, uint8_t port, uint8_t handleCommand);
	GPIO_Level_STATE getGpioLevelState(GPIO_Registers_Struct* gpio_registers_state, uint16_t Pin, uint8_t mode);
 	OUTPUT_HANDLE_STATE setGpioOutputLevelAndCheckOutputRegister(GPIO_Initial_Struct * gpioInitialStructure, uint8_t level);
 	bool isGpioOutputOk(GPIO_Output_Test_result_Struct *gpioOutputResult, OUTPUT_HANDLE_STATE setGpioHighLevelAck, OUTPUT_HANDLE_STATE setGpioLowLevelAck);
 	bool isOutputVoltageLevelMeasurementOk(bool gpioConnectedToPushButton, GPIO_Output_Test_result_Struct *gpioOutputResult);

 	float getAdcVoltage();
	void measureVddAndGetGpioOutputVoltagesLevels();
	void measureVddAndGetGpioInputVoltagesLevels();

	int shutdownPi();
	void turnUsbPortOn();
	int turnUsbPortOff();
	UsbPortState getUsbPortState();

 	void setNewLedsCommand(LEDS_COMMAND command);
 	LEDS_COMMAND getLedsCommand();

 	void setFlashProcessStartState(bool isStarted);
 	bool isFlashProcessStarted();

 	void setContinuityProcessStartState(bool isStarted);
 	bool isContinuityProcessStarted();



 	void setCompleteTestStartState(bool isStarted);
 	bool isCompelteProcessStarted();

 	void setEmpergencyUsbPortOffState(bool isOff);
 	bool isUsbPortEmergencyOff();

 	void checkUsbPortStateAndInitialUart();

	Logger *logger;
	AnalogDigitalConverter adcHandler;
	DigitalAnalogConverter dacHandler;
	IoExpander ioExpander;
 	LCD_Display lcd;
 	DataBaseController database;
	Multiplexers mulutiplexerController;
	UartHandler uartHandler;
	UsbPortsHandler usbPortshandler;
	CurrentSensor currentSensor;

	Voltages_References_Struct voltageRefernces;

	std::vector<IO_Expander_Multiplexer_Select_Commands> mux_select_vector;
	std::vector<GPIO_PINS_PORTS> board_gpio_ports_vector;
	std::vector<MUTLIPLEXER_PINS> multiplexer_pin;
	std::vector<GPIO_Pins> board_gpio_pins_vector;
	std::vector<uint16_t> pullTypes;
	std::vector<uint16_t> speed;
	std::vector<Analog_Pins> board_analog_pins_vector;
	std::vector<GPIO_PINS_PORTS> analog_ports_vector;
	std::vector<MUTLIPLEXER_PINS> analog_multiplexer_pins;
	std::vector<GPIO_TYPE> gpio_type_vector;
	std::vector<int> connected_multiplexer_analog;
	std::vector<int> connected_multiplexer_gpio;


  	Power_GND_Vectors_Struct powerPinsVectors;
	std::vector<Power_GND_Connected_Multiplexer_Pin> powerGndMuxPinVectors;
	std::vector<Connected_Multiplexer> powerGndMuxConnectionVector;
 	std::vector<POWER_PIN_TYPE> powerGndPinsType;
 	std::vector<string> powerGndPinsStringVector;

 	bool isSystemTerminateActivated();
 	void activateSystemTerminatiom();
private:

 	UsbPortState usbPortStateOn;
 	LEDS_COMMAND ledsCommand;
 	bool flashProcessStateStarted;
 	bool continuityProcessStateStart;
 	bool isCompleteProcessStart;
 	bool systemTermination;

	int initialGpiO(GPIO_Initial_Struct * gpio_initial_structure);
	uint16_t getGpioInitialCommand(GPIO_Initial_Struct * gpio_initial_structure);
 	GPIO_INITIAL_STATE getGpioInitialStateAck(GPIO_Registers_Struct* gpio_registers_state, GPIO_Initial_Struct * gpioInitialStructure);
	uint16_t getGpioUartHandleCommand(uint16_t pinNumber, uint8_t port, uint8_t handleCommand);
	OUTPUT_HANDLE_STATE checkHighLevelSetToOutput(GPIO_Level_STATE gpioLevelAckState);
	OUTPUT_HANDLE_STATE checkLowLevelSetToOutput(GPIO_Level_STATE	gpioLevelAckState);
	void terminate();

	const char *  TURN_DEVICE_TESTER_OFF_COMMAND = "sudo shutdown -h now";

};




#endif /* CONTROLLERS_PERIPHERALSINTERFACESCONTROLLER_HPP_ */
