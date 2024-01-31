/*
 * AnalogInputTestProcess.hpp
 *
 *  Created on: 24.06.2023
 *      Author: Astra
 */

#ifndef TEST_PROCESSES_ANALOGINPUTTESTPROCESS_HPP_
#define TEST_PROCESSES_ANALOGINPUTTESTPROCESS_HPP_


#include "../controllers/PeripheralsInterfacesController.hpp"

#define ADC_INIT_ERROR 0x3000
#define ADC_SET_CHANNEL_ERROR 0x5000
#define ADC_START_ERROR 0x7000
#define ADC_POLL_CONVERSION_ERROR 0x9000
#define ADC_STOP_ERROR 0xB000
#define UNKNOWN_ADC 0xF000

class AnalogInputTestProcess {

public:

	AnalogInputTestProcess();
  	virtual ~AnalogInputTestProcess();
 	void setController(PeripheralsInterfacesController *_controller);
 	void initialAndTestAllAnalogInput(bool isAppCommand);

 	void checkAnalogInputTestResults();
 	int getFailedAnalogInput();
 	int getErrorsAnalogInput();
 	string getFailedAnalogInputText();
 	string getErrorsAnalogInputText();
 	void resetAnalogInputResultDatabase();

 	std::vector<Analog_Input_Test_Result_Struct> analogInputTestResultVector;

private:
 	PeripheralsInterfacesController *controller;
	std::vector<string> analogInputError;
	std::vector<string> analogInputFail;

	GPIO_Initial_Struct getAnalogInputInitialStructure(uint8_t positionGpioPinVector);
 	void testSingleAnalogInput(bool isAppCommand, GPIO_Initial_Struct * gpioInitialStructure);
	double readAnalogValue(GPIO_Initial_Struct * gpioInitialStructure);
 	void checkAnalogValueError(uint16_t adcValue);
 	bool isAnalogInputOk(Analog_Input_Test_Result_Struct  *testResult);
	bool checkNucleoAdcErrors(double measuredVotalge, Analog_Input_Test_Result_Struct  *testResult);

	void handleInitialFailureAnalog(string pinText, bool isAppCommand );
	void handleMultiplexersFailure(string pinText, bool isAppCommand, int multiplexerError);
	void saveFailedErrorAnalogInput(string pinText, bool isError);
};



#endif /* TEST_PROCESSES_ANALOGINPUTTESTPROCESS_HPP_ */
