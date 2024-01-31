/*
 * FlashProcess.hpp
 *
 *  Created on: 29.05.2023
 *      Author: Astra
 */

#ifndef TEST_PROCESSES_FLASHPROCESS_HPP_
#define TEST_PROCESSES_FLASHPROCESS_HPP_


#include "../controllers/PeripheralsInterfacesController.hpp"

#define FLASH_VOLTAGE 3
#define FLASH_MEMORY_TEST_BUFFER 24

class FlashProcess{

public:

	FlashProcess();
 	virtual ~FlashProcess();

	void setController(PeripheralsInterfacesController *_controller);
	bool flashNucleoBoard();
	void checkFlashHandleValues();
	int testFlashMemory();
	int getFlashMemoryErrors(FLASH_TestResultTypeDef * flashTestResult);

  	string writeTestProgrammToFlash();
	std::string getFlashInfosText(string flashAck);
	bool isFlashOk(string flashAck);

	Flash_Handle_Check_Struct falshFailureStructure;
	Flash_Memory_Test_Struct flashMemoryTestStrcture;

private:

	PeripheralsInterfacesController *controller;

	const char* getFlashCommandInterface1 = "sudo /home/BoardTester/stm32flash/stm32flash -b 115200 -v -w /home/BoardTester/stm32flash/TestProgramm.hex /dev/ttyACM0";
 	const char* getFlashCommandInterface2 =  "sudo /home/BoardTester/stm32flash/stm32flash -b 115200 -v -w /home/BoardTester/stm32flash/TestProgramm.hex /dev/ttyACM1";
	const char* flashDoneText = "(100.00%) Done";
	const char* flashVersionText = "Version";
	const char* flashWriteText = "Write";
};

#endif /* TEST_PROCESSES_FLASHPROCESS_HPP_ */
