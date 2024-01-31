/*
 * ShortGpioTestProcess.hpp
 *
 *  Created on: 07.06.2023
 *      Author: Astra
 */

#ifndef TEST_PROCESSES_SHORTGPIOTESTPROCESS_HPP_
#define TEST_PROCESSES_SHORTGPIOTESTPROCESS_HPP_

#include "../controllers/PeripheralsInterfacesController.hpp"

#define MULTIPLEXERS_ERROR_GPIO_SHORT -2

class ShortGpioTestProcess {

public:

	ShortGpioTestProcess();
 	virtual ~ShortGpioTestProcess();

 	void setController(PeripheralsInterfacesController *_controller);
 	void testAllGpiosShortCircuit();
  	void checkAndPrintGpiosShortCircuitsResult();
 	int getFailedGpioShortTest();
 	int getShortedGpios();
 	string getFailedGpioShortTestText();
 	string getShortedGpiosText();

 	std::vector<GPIO_SHORT_TEST_RESULT> gpiosShortCircuitsTestResultVector;
private:

 	PeripheralsInterfacesController *controller;
 	std::vector<ShortCircuit> shortCircuitsVector;
 	std::vector<string> gpioFailedTestShortCircuits;
 	std::vector<uint8_t> highVoltagerRestGpiosVector;
 	std::vector<string> gpiosShortVector;

 	void testShortCircuitsWithPushButtonGpio(uint8_t positionGpioPinVector, uint8_t pin, uint8_t port);
  	void setGpioOutputAndTestShortCircuits(uint8_t positionGpioPinVector, GPIO_Initial_Struct * gpioInitialStructure);
  	int setGpioOutputHighAndCheck(GPIO_Initial_Struct * gpioInitialStructure, GPIO_Output_Test_result_Struct  *gpioOutputTestResult);
  	int setGpioOutputLowAndCheck(GPIO_Initial_Struct * gpioInitialStructure, GPIO_Output_Test_result_Struct  *gpioOutputTestResult);
	int measureVoltageRestGpiosHighState(string pinTextHighOutput);
	int measureVoltageRestGpiosLowState(string pinText);
 	bool hasDuplicate(const std::string& gpio1, const std::string& gpio2);
  	string getShortCircuitsGpioText();
 	void updateDataBaseCauseMultiplexerConnectionError(string pinText, int configureMultiplexerAndSelectPinAck);
 	bool isOuputHighLevelOk(OUTPUT_HANDLE_STATE setGpioHighLevelAck , double voltage);
 	bool isOuputLowLevelOk(OUTPUT_HANDLE_STATE setGpioLowLevelAck , double voltage);
 	void addFailedGpioToVector(string gpioText);
 	void handleInitalizeFailure(GPIO_INITIAL_STATE  gpioInitialState, GPIO_Initial_Struct * gpioInitialStructure);
};


#endif /* TEST_PROCESSES_SHORTGPIOTESTPROCESS_HPP_ */
