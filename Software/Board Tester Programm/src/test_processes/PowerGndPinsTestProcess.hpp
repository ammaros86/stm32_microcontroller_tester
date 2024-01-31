/*
 * PowerGndPinsTestProcess.hpp
 *
 *  Created on: 09.08.2023
 *      Author: Astra
 */

#ifndef TEST_PROCESSES_POWERGNDPINSTESTPROCESS_HPP_
#define TEST_PROCESSES_POWERGNDPINSTESTPROCESS_HPP_

#include "../controllers/PeripheralsInterfacesController.hpp"




class PowerGndPinsTestProcess {

public:

	PowerGndPinsTestProcess();
  	virtual ~PowerGndPinsTestProcess();

 	void setController(PeripheralsInterfacesController *_controller);
 	void testAllGndPowerPins();
  	int getFailedTest();
 	int getTestErrors();
	void resetPowerGndPinsTestData();
	void checkGndPowerPinsTest();

 	std::vector<string> failedPinsVector;
	std::vector<string> errorPinsVector;
	string getErrorsPinsText();
	string getFailedPinsText();

	std::vector<Power_GND_TEST_RESULT> powerGndPinsTestResultVector;

private:

 	void initialBoardPowerPinsTester();
	bool isE5VPinOk(double voltage);
	bool isGndPinOk(double voltage);
	bool is3VPinnOk(double voltage);
	bool is5VPinOk( double voltage);

	PeripheralsInterfacesController *controller;
 	std::vector<double> powerGndPinsVoltage;
 	std::vector<string> powerGndPinsVoltageTestErros;


};


#endif /* TEST_PROCESSES_POWERGNDPINSTESTPROCESS_HPP_ */
