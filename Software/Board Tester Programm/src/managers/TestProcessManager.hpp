/*
 * TestProcessManager.hpp
 *
 *  Created on: 08.06.2023
 *      Author: Astra
 */

#ifndef TESTPROCESSMANAGER_HPP_
#define TESTPROCESSMANAGER_HPP_


#include <thread>

#include "../controllers/PeripheralsInterfacesController.hpp"
#include "DisplayManager.hpp"
#include "../test_processes/DigitalOutputTestProcess.hpp"
#include "../test_processes/DigitalInputTestProcess.hpp"
#include "../test_processes/PowerGndPinsTestProcess.hpp"
#include "../test_processes/TemperatureProcess.hpp"
#include "../test_processes/FlashProcess.hpp"
#include "../test_processes/ShortGpioTestProcess.hpp"
#include "../test_processes/AnalogInputTestProcess.hpp"
#include "../test_processes/ContinuityTestProcess.hpp"

#define COPY_LOGS_SUCCESS 1
#define COPY_LOGS_FAILED_NO_MEDIA_DETECTED -1
#define COPY_LOGS_FAILED -2

#define BOARD_STATE_ON 1
#define BOARD_STATE_OFF 0
#define BOARD__SET_STATE_OFF_FAILED -1


class TestProcessManager {

public:


 	TestProcessManager(PeripheralsInterfacesController* _controller, DisplayManager *_displayManager) : controller(_controller), displayManager(_displayManager){
 		currentTestCommand = TestCommand::newBoard;
		testProcessStateValue = Test_Process_State::free;
  		micorcontrollerCurrent = -1;
 		micorcontrollerTemperature = -1;
 		boardID = -1;
	};

 	virtual ~TestProcessManager();
 	void initial();
 	void setTestProcessStart(TestCommand command);
 	void setTestProcessFinish();
 	void setLedStateTestResult(int testErrorsResult);
 	void joinTestProcessThread();
 	void startTestProcessThread(int boardID, string boardName, TestCommand testCommand, bool _isAppCommmand, bool isCompleteTest);
 	void startCompleteBoardTest(bool _isAppCommmand);
 	void printCompleteTestResult();
 	void continuityTest();
  	void testPowerGndPins();
 	void measureBoardCurrent(bool isCompleteTest);
  	void getBoardTemperature(bool isCompleteTest);
  	void flashHandle();
 	void testGpioInput();
 	void testAnalogInput();
 	void testGpioOutput();
 	void testGpiosShortCircuit();
 	void getServerIpAddress();
 	void copyLogFilesToUsb();
 	void turnUsbPortOn();
 	void turnBoardTesterOff();

 	int getShortCircuits();
 	int getFailedTests();

 	void setNewTestCommand(TestCommand command);
 	TestCommand getCurrentTestCommand();

 	void setNewTestProcessState(Test_Process_State newTestProcessState);
 	Test_Process_State getTestProcessState();

private:
	pthread_mutex_t mutex;
	PeripheralsInterfacesController *controller;
 	DisplayManager * displayManager;
	ContinuityTestProcess continuityTestProcess;
	PowerGndPinsTestProcess powerGndPinsTestProcess;
	FlashProcess flashProcess;
	TemperatureMeasurementProcess temperatureProcess;
	DigitalInputTestProcess digitalInputTesterProcess;
	DigitalOutputTestProcess digitalOutputTesterProcess;
	ShortGpioTestProcess shortGpiosTestProcess;
	AnalogInputTestProcess analogInputTestProcess;

	std::thread workingThread;

	int boardID;
	string boardName;

  	bool isAppCommmand = false;
  	string usbError = "No USB Device Found";
 	string usbCopySuccess = "done";
 	float micorcontrollerCurrent;
 	float micorcontrollerTemperature;

    const char * GET_SERVER_IP_COMMAND = "hostname -I | cut -d' ' -f1";
    const char * COPY_TO_USB_COMMAND = "sudo /home/BoardTester/copyLogsToUsb.sh";

    int getBoardConnectionState();
   	void getCn10PhysicalConnectionState(Power_GND_Test_Result_Struct * measuredVoltages, BoardConnectionStructure * boardConnectionState);
 	void getCn7PhysicalConnectionState(Power_GND_Test_Result_Struct * measuredVoltages, BoardConnectionStructure * boardConnectionState);
 	int checkBoardUsbAndPhysicalConnectionState(BoardConnectionStructure *nucleoBoardConnectionState);
   	void printNewConnectionState(int boardConnectionNewState);

	TestCommand currentTestCommand;
 	Test_Process_State testProcessStateValue;

  };


#endif /* TESTPROCESSMANAGER_HPP_ */
