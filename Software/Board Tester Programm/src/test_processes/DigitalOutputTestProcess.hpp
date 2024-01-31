/*
 * DigitalOutputTestProcess.hpp
 *
 *  Created on: 29.05.2023
 *      Author: Astra
 */

#ifndef TEST_PROCESSES_DIGITALOUTPUTTESTPROCESS_HPP_
#define TEST_PROCESSES_DIGITALOUTPUTTESTPROCESS_HPP_

#include "../controllers/PeripheralsInterfacesController.hpp"

class DigitalOutputTestProcess {

public:

	DigitalOutputTestProcess();
  	virtual ~DigitalOutputTestProcess();
 	void setController(PeripheralsInterfacesController *_controller);
 	void testAllGpiosOutput();
  	void checkGpiosOutputTestResults();
 	int getFailedGpioOutput();
 	int getErrorsGpiosOutput();
 	string getFailedGpioOutputText();
 	string getErrorsGpiosOutputText();

 	std::vector<GPIO_Output_Test_result_Struct> gpiosOutputTestResultVector;
private:

 	PeripheralsInterfacesController *controller;
	std::vector<string> digitalOutputError;
	std::vector<string> digitalOutputFail;

  	void handleOutputMultiplexersConnectionFailure(GPIO_Initial_Struct *gpioInitialStructur, int configureMultiplexerAndSelectPinAck);
	GPIO_Initial_Struct getDigitalOutputInitialStructure(uint8_t positionGpioPinVector, uint16_t positionPullVector, uint16_t positionSpeedVector);
 	void testGpioOutputConnectedToPushButton(GPIO_Initial_Struct * gpioInitialStructure);
	bool isGpioOutputConnectedToPushButtonOk(GPIO_Output_Test_result_Struct *gpioOutputResult, OUTPUT_HANDLE_STATE setGpioLowLevelAck);
	void testGpioOutput(GPIO_Initial_Struct * gpioInitialStructure);
  	int setMux7AndCheckAck(GPIO_Output_Test_result_Struct*  gpioOutputResult);
 	int getAdcVoltageAndCheckAck(bool isLowVoltageMeasurement, GPIO_Output_Test_result_Struct*  gpioOutputResult);
 	void saveGpioOutputInitialErrorAndHandle(GPIO_Output_Test_result_Struct  *gpioOutputResult, GPIO_INITIAL_STATE  gpioInitialState);
	void updateDataBaseCauseMultiplexerConnectionError(GPIO_Initial_Struct *gpioInitialStructure, int configureMultiplexerAndSelectPinAck);
 	void addGpioTextToVector(string gpioText, bool isError);
 	void handleNotTestableGpioOutput(GPIO_Initial_Struct *gpioInitialStructure);
};

#endif /* TEST_PROCESSES_DIGITALOUTPUTTESTPROCESS_HPP_ */
