/*
 * DatabaseCommandListener.cpp
 *
 *  Created on: 03.09.2023
 *      Author: Astra
 */

#include "DatabaseCommandListener.hpp"


DatabaseCommandListener::~DatabaseCommandListener() {
	stopReadDatabaseNewCommmmand();
	currentApplicationTestCommand = TestCommand::notselected;
}


/* start Read DataBase New Command Thread
 */
void DatabaseCommandListener::startReadDatabaseNewCommmmand(){
	readDatabaseThread = std::thread(&DatabaseCommandListener::readDatabaseNewCommandProcess, this);
	readDatabaseThread.detach();
}

/* stop Read DataBase New Command Thread
 */
void DatabaseCommandListener::stopReadDatabaseNewCommmmand(){
 	if (readDatabaseThread.joinable()) {
		readDatabaseThread.join();
	}
}

/* control Command and start Process
 */
void DatabaseCommandListener::checkCommandAndStartProcess(){
	if(currentApplicationTestCommand != TestCommand::notselected){
		testProcessManager->startTestProcessThread(boardId, boardName, currentApplicationTestCommand, true, isCompleteTest);
		currentApplicationTestCommand = TestCommand::notselected;
	}
}

/* Read DataBase New Command Thread
 */
void DatabaseCommandListener::readDatabaseNewCommandProcess(){
	while(1){
		if(testProcessManager->getTestProcessState() == Test_Process_State::free){
			readBoardCommand();
			checkCommandAndStartProcess();
			std::this_thread::sleep_for(std::chrono::milliseconds(DATABSE_READ_THREAD_PERIOD));
			readBoardController();
			checkCommandAndStartProcess();
			std::this_thread::sleep_for(std::chrono::milliseconds(DATABSE_READ_THREAD_PERIOD));
		}
 	}
}


/* read Test Commands table
 */
void DatabaseCommandListener::readBoardCommand(){
	sql::Statement *statement;
	sql::ResultSet *result;
	isCompleteTest = false;
	try{
		statement = databaseReaderConnection->createStatement();
		result = statement->executeQuery("SELECT * FROM test_commands");
		while (result->next()){
			if(result->getInt(CONTINUITY_TEST_POSITION) == COMMAND_SET){
				setNewTestCommand(result, TestCommand::continuityTest);
			}else if(result->getInt(CURRENT_MEASURE_POSITION) == COMMAND_SET){
				setNewTestCommand(result, TestCommand::boardCurrent);
			}else if( result->getInt(POWER_GND_TEST_POSITION) == COMMAND_SET){
				setNewTestCommand(result, TestCommand::powerGndPinsTest);
			}else if( result->getInt(FLASH_TEST_POSITION) == COMMAND_SET){
				setNewTestCommand(result, TestCommand::flashBoard);
			}else if( result->getInt(TEMPERATURE_MEASURE_POSITION) == COMMAND_SET){
				setNewTestCommand(result, TestCommand::measureTemperature);
			}else if( result->getInt(GPIO_INPUT_TEST_POSITION) == COMMAND_SET){
				setNewTestCommand(result, TestCommand::testGpioInput);
			}else if( result->getInt(GPIO_OUTPUT_TEST_POSITION) == COMMAND_SET){
				setNewTestCommand(result, TestCommand::testGpioOutput);
			}else if( result->getInt(GPIO_SHORT_TEST_POSITION) == COMMAND_SET){
				setNewTestCommand(result, TestCommand::testGpioShortCircuit);
			}else if( result->getInt(ANALOG_INPUT_TEST_POSITION) == COMMAND_SET){
				setNewTestCommand(result, TestCommand::testAnalogInput);
			}
		}
	}catch(sql::SQLException &error){
   		controller->logger->printToFileLogger("Error:DataBaseController readBoardController: failed to read Board Controller Table cause: "   +  std::string(error.what()) , false);
	}
 	removeSqlResult(result);
	removeSqlStatement(statement);
}


void DatabaseCommandListener::setNewTestCommand(sql::ResultSet *result, TestCommand _command){
	currentApplicationTestCommand = _command;
	boardId = result->getInt(BOARD_ID_POSITION);
	boardName = result->getString(BOARD_NAME_POSITION);
	if(result->getInt(COMPLETE_TEST_POSITION) == COMMAND_SET){
		isCompleteTest = true;
	}
}

/* read Board controller table
 */
void DatabaseCommandListener::readBoardController(){
	sql::Statement *statement;
	sql::ResultSet *result;
	try{
		statement = databaseReaderConnection->createStatement();
		result = statement->executeQuery("SELECT * FROM control_commands");
		while (result->next()){
			if( result->getInt(1) == COMMAND_SET){
				currentApplicationTestCommand = TestCommand::turnUsbPortOn;
				boardId = result->getInt(1);
			}else if( result->getInt(3) == COMMAND_SET){
				currentApplicationTestCommand = TestCommand::powerBoardTesterOff;
				boardId = result->getInt(1);
			}else if( result->getInt(5) == COMMAND_SET){
				currentApplicationTestCommand = TestCommand::copyLogsUsb;
				boardId = result->getInt(1);
			}
		}
 	}catch(sql::SQLException &error){
 		controller->logger->printToFileLogger("Error:DataBaseController readBoardController: failed to read Board Controller Table cause: "  +  std::string(error.what()) , false);
	}
 	removeSqlResult(result);
	removeSqlStatement(statement);
}


/* remove SQL result
 */
void DatabaseCommandListener::removeSqlResult(sql::ResultSet *result){
	if(result != nullptr){
		delete result;
	}
}

/* remove SQL statement
 */
void DatabaseCommandListener::removeSqlStatement(sql::Statement *stmt){
	if(stmt != NULL)
		delete stmt;
}

/* return active BoardID
 */
int DatabaseCommandListener::getActiveBoardID(){
	return boardId;
}


/* open connection to read from DataBase
 */
void DatabaseCommandListener::openDataBaseReaderConnection(){
	sql::Driver *driver;
	try{
		driver = get_driver_instance();
		databaseReaderConnection = driver->connect(Strings::hostNameDatabse, Strings::userNameDatabse, Strings::passwordDatabse);
		if(databaseReaderConnection != NULL){
			databaseReaderConnection->setSchema(Strings::shemaDatabse);
		}else{
			controller->logger->printToFileLogger("Error:DataBaseController openDataBaseReaderConnection: failed to  open Connection" , false);
		}
	}catch (sql::SQLException &error){
		controller->logger->printToFileLogger("Error:DataBaseController openDataBaseReaderConnection: failed to initial connection to Database cause: " +  std::string(error.what()) , false);
	}
}
