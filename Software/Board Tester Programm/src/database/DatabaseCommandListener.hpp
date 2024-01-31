/*
 * DatabaseCommandListener.hpp
 *
 *  Created on: 03.09.2023
 *      Author: Astra
 */

#ifndef DATABASE_DATABASECOMMANDLISTENER_HPP_
#define DATABASE_DATABASECOMMANDLISTENER_HPP_



#include "../controllers/PeripheralsInterfacesController.hpp"
#include "../managers/TestProcessManager.hpp"

#define DATABSE_READ_THREAD_PERIOD 10
#define COMMAND_SET 1

#define BOARD_ID_POSITION 1
#define BOARD_NAME_POSITION 2
#define CONTINUITY_TEST_POSITION 3
#define CURRENT_MEASURE_POSITION 4
#define POWER_GND_TEST_POSITION 5
#define FLASH_TEST_POSITION 6
#define TEMPERATURE_MEASURE_POSITION 7
#define GPIO_INPUT_TEST_POSITION 8
#define GPIO_OUTPUT_TEST_POSITION 9
#define GPIO_SHORT_TEST_POSITION 10
#define ANALOG_INPUT_TEST_POSITION 11
#define COMPLETE_TEST_POSITION 12

class DatabaseCommandListener {

public:

	DatabaseCommandListener(PeripheralsInterfacesController* _controller, TestProcessManager* _testProcessManager) : controller(_controller), testProcessManager(_testProcessManager){
		databaseReaderConnection = NULL;
		currentApplicationTestCommand = TestCommand::notselected;
		isCompleteTest = false;
		boardId =  -1;
		openDataBaseReaderConnection();
 	};

 	virtual ~DatabaseCommandListener();
   	void startReadDatabaseNewCommmmand();
 	void stopReadDatabaseNewCommmmand();
 	void readDatabaseNewCommandProcess();
private:

 	PeripheralsInterfacesController *controller;

  	TestProcessManager *testProcessManager;
 	std::thread readDatabaseThread;

	sql::Connection *databaseReaderConnection;

	TestCommand currentApplicationTestCommand;

	bool isCompleteTest;

	int boardId;
	string boardName;
	void readBoardCommand();
 	void checkCommandAndStartProcess();
	void readBoardController();

	void removeSqlResult(sql::ResultSet *result);
	void removeSqlStatement(sql::Statement *stmt);
  	int getActiveBoardID();
  	void openDataBaseReaderConnection();
  	void setNewTestCommand(sql::ResultSet *result, TestCommand _command);
};

#endif /* DATABASE_DATABASECOMMANDLISTENER_HPP_ */
