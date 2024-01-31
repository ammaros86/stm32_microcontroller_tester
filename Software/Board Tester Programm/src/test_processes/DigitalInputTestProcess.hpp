/*
 * DigitalInputTestProcess.hpp
 *
 *  Created on: 29.05.2023
 *      Author: Astra
 */

#ifndef TEST_PROCESSES_DIGITALINPUTTESTPROCESS_HPP_
#define TEST_PROCESSES_DIGITALINPUTTESTPROCESS_HPP_

#include "../controllers/PeripheralsInterfacesController.hpp"

class DigitalInputTestProcess {

public:

	DigitalInputTestProcess();
  	virtual ~DigitalInputTestProcess();
 	void setController(PeripheralsInterfacesController *_controller);
 	void testAllGpiosInput();
  	void checkGpiosInputTestResults();

 	int getFailedGpioInput();
 	int getErrorsGpiosInput();
 	string getFailedGpioInputText();
 	string getErrorsGpiosInputText();

 	std::vector<GPIO_Input_Test_Result_Struct> gpiosInputTestResultVector;
private:

 	PeripheralsInterfacesController *controller;
	std::vector<string> digitalInputError;
	std::vector<string> digitalInputFail;

 	void handleInputMultiplexersConnectionFailure(GPIO_Initial_Struct *gpioInitialStructur, int configureMultiplexerAndSelectPinAck);
 	GPIO_Initial_Struct getDigitalInputInitialStructureFromVectorPosition(uint8_t positionPullVector, uint8_t positionGpioPinVector);
 	void testSingleGpioInput(GPIO_Initial_Struct * gpioInitialStructure);
 	void testGpioInputConnectedToPushButton(GPIO_Initial_Struct * gpioInitialStructure);
 	void saveGpioInputInitialErrorAndHandle(GPIO_INITIAL_STATE  gpioInitialState, GPIO_Initial_Struct * gpioInitialStructure);
 	void testDigitalInputNoPull(GPIO_Initial_Struct * gpioInitialStructure);
	void testDigitalInputPullDown(GPIO_Initial_Struct * gpioInitialStructure);
	void testDigitalInputPullUp(GPIO_Initial_Struct * gpioInitialStructure);
 	int setMux7AndDac(double voltage);
	GPIO_Level_STATE  readGpioDigitalInputState(GPIO_Initial_Struct * gpioInitialStructure);
 	bool isInputTestResultOk(GPIO_Level_STATE highLevelTest, GPIO_Level_STATE lowLevelTest);
 	int checkInputHighLevelStateOk(GPIO_Level_STATE levelState, string gpioText);
 	int checkInputLowLevelStateOk(GPIO_Level_STATE levelState, string gpioText);
 	void addGpioTextToVector(string gpioText, bool isError);
 	void handleNotTestableGpioInput(GPIO_Initial_Struct *gpioInitialStructure);

};

#endif /* TEST_PROCESSES_DIGITALINPUTTESTPROCESS_HPP_ */
