/*
 * ContinuityTestProcess.hpp
 *
 *  Created on: 10.07.2023
 *      Author: Astra
 */

#ifndef TEST_PROCESSES_CONTINUITYTESTPROCESS_HPP_
#define TEST_PROCESSES_CONTINUITYTESTPROCESS_HPP_

#include "../controllers/PeripheralsInterfacesController.hpp"

#define PA2_VECTOR_POSITION 14
#define PA3_VECTOR_POSITION 13
#define PF0_VECTOR_POSITION 41
#define PC15_VECTOR_POSITION 42
#define PC14_VECTOR_POSITION 43
#define SHORTED_GPIO_DEFAULT_MAX_VOLTAGE 0.2
#define SHORTED_GPIO_DEFAULT_MIN_VOLTAGE 0.01
#define OPEN_GPIO_DEFAULT_MIN_VOLTAGE  2.1
#define OPEN_GPIO_DEFAULT_MAX_VOLTAGE  2.5
#define OPEN_GPIO_CORRECTION_VOLTAGE  0.30

class ContinuityTestProcess {

public:

	ContinuityTestProcess();
  	virtual ~ContinuityTestProcess();

 	void setController(PeripheralsInterfacesController *_controller);
 	void testGpiosContinuity();
 	int getFailedContinuityTest();
 	int getShortGpios();
 	int getOpenGpios();
 	int getTestErrors();
 	void printGpiosContinuityTestResult();
 	string getFailedGpiosText();
 	string getOpenGpiosText();
 	string getShortGpiosText();

 	std::vector<Continuity_Test_Result_Struct> continuityTestResultVector;

private:

 	PeripheralsInterfacesController *controller;
  	float openVoltage;
  	float shortedVoltage;
  	std::vector<string> failedTestGpiosVector;
 	std::vector<string> shortGpiosVector;
 	std::vector<string> openGpiosVector;

  	void configureContinuityTest();
 	void checkGpioContinuityTestResult(uint8_t positionGpioPinVector, Continuity_Test_Result_Struct *testResult);
 	void setTestGpiosContinuityFailed(int error);
  	string getGpioPinText(uint8_t positionGpioPinVector);
 	void setTestFailed(int error, Continuity_Test_Result_Struct *testResult);
 	void resetConitinuityTestConfiguration();

};

#endif /* TEST_PROCESSES_CONTINUITYTESTPROCESS_HPP_ */
