/*
 * DatabaseController.cpp
 *
 *  Created on: 29.05.2023
 *      Author: Astra
 */


#include "DatabaseController.hpp"

DataBaseController::DataBaseController(){
//	databaseReaderConnection = nullptr;
	logger = NULL;
 }

DataBaseController::~DataBaseController() {
}

/* initialize Database Controller
 * Parameter:
 * logger: Pointer to Logger Object
 */
void DataBaseController::initialDataBaseController(Logger * _logger){
	logger = _logger;
}


/* open connection to write DataBase
 */
sql::Connection * DataBaseController::openDataBaseWriterConnection(){
 	sql::Connection *writerConnection;
	sql::Driver *driver;
	try{
		driver = get_driver_instance();
		writerConnection = driver->connect(Strings::hostNameDatabse, Strings::userNameDatabse, Strings::passwordDatabse);
	    if(writerConnection != NULL){
	    	writerConnection->setSchema(Strings::shemaDatabse);
	    }
	}catch (sql::SQLException &error){
 		logger->printToFileLogger("Error:DataBaseController openDataBaseWriterConnection: failed to initial connection to Database cause: " +  std::string(error.what()) , false);
	}
	return writerConnection;
}

/* close connection to DataBase
 */
void DataBaseController::closeDataBaseConnection(sql::Connection *connection){
	if(connection != nullptr){
		connection->close();
		delete connection;
	}
}

/* remove SQL result
 */
void DataBaseController::removeSqlResult(sql::ResultSet *result){
	if(result != nullptr){
		delete result;
	}
}

/* remove SQL statement
 */
void DataBaseController::removeSqlStatement(sql::Statement *stmt){
	if(stmt != NULL)
		delete stmt;
}

/* remove SQL post statement
 */
void DataBaseController::removeSqlPostStatement(sql::PreparedStatement *prepareStatement){
	if(prepareStatement != NULL)
		delete prepareStatement;
}

/* control BoardID
 */
bool DataBaseController::isBoardIdEmpty(int boardID){
	if(boardID == -1){
 		logger->printToFileLogger("Error:DataBaseController boardId is empty ", false);
		return true;
	}else{
		return false;
	}
}


/* UPDATE DataBase Controller Table USB Port State
 * Parameter:
 * value: value to Update
 */
void DataBaseController::updateBoardControllerUsbState(int value){
	sql::Connection *writerConnection = openDataBaseWriterConnection();
	sql::PreparedStatement *prepareStatement;
	try{
		prepareStatement = writerConnection->prepareStatement("UPDATE control_commands SET usb_port_state = ?, turn_usb_port_on = ? ");
		if(prepareStatement != NULL){
			prepareStatement->setInt(1, value);
			prepareStatement->setInt(2, 0);
			prepareStatement->executeQuery();
		}else{
			logger->printToFileLogger("Error:DataBaseController updateBoardControllerUsbState: failed to update Controller to Board Command cause Null Post Statement", false);
		}
	}catch(sql::SQLException &error){
		logger->printToFileLogger("Error:DataBaseController updateBoardControllerUsbState: failed to update Command to Board Controller cause Error: "+  std::string(error.what()) , false);
	}
	closeDataBaseConnection(writerConnection);
	removeSqlPostStatement(prepareStatement);
}

/* UPDATE DataBase Controller Table Board ON/OFF State
 * Parameter:
 * value: value to Update
 */
void DataBaseController::updateBoardControllerState(int value){
	sql::Connection *writerConnection = openDataBaseWriterConnection();
	sql::PreparedStatement *prepareStatement;
	try{
		prepareStatement = writerConnection->prepareStatement("UPDATE control_commands SET power_state = ?, power_off = ? ");
		if(prepareStatement != NULL){
			prepareStatement->setInt(1, value);
			prepareStatement->setInt(2, 0);
			prepareStatement->executeQuery();
		}else{
			logger->printToFileLogger("Error:DataBaseController updateBoardControllerState: to update Controller to Board Command cause Null Post Statement", false);
		}
	}catch(sql::SQLException &error){
		logger->printToFileLogger("Error:DataBaseController updateBoardControllerState: to update Command to Board Controller cause Error: "+  std::string(error.what()) , false);
	}
	closeDataBaseConnection(writerConnection);
	removeSqlPostStatement(prepareStatement);
}

/* UPDATE DataBase Controller Table Copy Log Files State
 * Parameter:
 * value: value to Update
 */
void DataBaseController::updateBoardControllerCopyState(int value){
	sql::Connection *writerConnection = openDataBaseWriterConnection();
	sql::PreparedStatement *prepareStatement;
	try{
		prepareStatement = writerConnection->prepareStatement("UPDATE control_commands SET usb_media_state = ?, copy_logs_usb = ? ");
		if(prepareStatement != NULL){
			prepareStatement->setInt(1, value);
			prepareStatement->setInt(2, 0);
			prepareStatement->executeQuery();
		}else{
			logger->printToFileLogger("Error:DataBaseController updateBoardControllerState: to update Controller to Board Command cause Null Post Statement", false);
		}
	}catch(sql::SQLException &error){
		logger->printToFileLogger("Error:DataBaseController updateBoardControllerState: to update Command to Board Controller cause Error: "+  std::string(error.what()) , false);
	}
	closeDataBaseConnection(writerConnection);
	removeSqlPostStatement(prepareStatement);
}


/* update Board controller table
 * Parameter:
 * testType: command to update
 * value: value to write in the table
 */
void DataBaseController::updateBoardCommand(const int boardID, const TestCommand command, const int value, const int errors){
	if(isBoardIdEmpty(boardID))
		return;
 	sql::Connection *writerConnection = openDataBaseWriterConnection();
 	sql::PreparedStatement *prepareStatement;
	try{
		switch(command){
				case TestCommand::boardCurrent:
					prepareStatement = writerConnection->prepareStatement("UPDATE test_commands SET measure_current = ?, errors = ? WHERE board_id = ?");
					break;
				case TestCommand::measureTemperature:
					prepareStatement = writerConnection->prepareStatement("UPDATE test_commands SET measure_temperature = ?, errors = ? WHERE board_id = ?");
					break;
				case TestCommand::flashBoard:
					prepareStatement = writerConnection->prepareStatement("UPDATE test_commands SET flash_board = ?, errors = ? WHERE board_id = ?");
					break;
				case TestCommand::powerGndPinsTest:
					prepareStatement = writerConnection->prepareStatement("UPDATE test_commands SET power_gnd_test = ?, errors = ? WHERE board_id = ?");
					break;
				case TestCommand::continuityTest:
					prepareStatement = writerConnection->prepareStatement("UPDATE test_commands SET gpio_continuity = ?, errors = ? WHERE board_id = ?");
					break;
				case TestCommand::testGpioOutput:
					prepareStatement = writerConnection->prepareStatement("UPDATE test_commands SET gpio_output_test = ?, errors = ? WHERE board_id = ?");
					break;
				case TestCommand::testGpioInput:
					prepareStatement = writerConnection->prepareStatement("UPDATE test_commands SET gpio_input_test = ?, errors = ? WHERE board_id = ?");
					break;
				case TestCommand::testAnalogInput:
					prepareStatement = writerConnection->prepareStatement("UPDATE test_commands SET analog_input_test = ?, errors = ? WHERE board_id = ?");
					break;
				case TestCommand::testGpioShortCircuit:
					prepareStatement = writerConnection->prepareStatement("UPDATE test_commands SET gpio_short_circuit_test = ?, errors = ? WHERE board_id = ?");
					break;
				case TestCommand::copyLogsUsb:
					prepareStatement = writerConnection->prepareStatement("UPDATE test_commands SET copy_logs_usb = ?, errors = ? WHERE board_id = ?");
					break;
				case TestCommand::turnUsbPortOn:
					prepareStatement = writerConnection->prepareStatement("UPDATE test_commands SET usb_port_on = ?, errors = ? WHERE board_id = ?");
					break;
				case TestCommand::powerBoardTesterOff:
					prepareStatement = writerConnection->prepareStatement("UPDATE test_commands SET power_off = ?, errors = ? WHERE board_id = ?");
					break;
				case TestCommand::completTest:
					prepareStatement = writerConnection->prepareStatement("UPDATE test_commands SET complete_test = ?, errors = ? WHERE board_id = ?");
					break;
				default:
 			  		logger->printToFileLogger("Error:DataBaseController updateBoardCommand: failed cause unknown Test Command", false);
					break;
			}
 		if(prepareStatement != NULL){
			prepareStatement->setInt(1, value);
			prepareStatement->setInt(2, errors);
			prepareStatement->setInt(3, boardID);
			prepareStatement->executeQuery();
		}else{
 	  		logger->printToFileLogger("Error:DataBaseController updateBoardCommand:  to update Command to Board Command cause Null Post Statement", false);
		}
	}catch(sql::SQLException &error){
   		logger->printToFileLogger("Error:DataBaseController updateBoardCommand: to update Command to Board Command cause Error: "+  std::string(error.what()) , false);
	}

	closeDataBaseConnection(writerConnection);
 	removeSqlPostStatement(prepareStatement);
}

//Continuity Test
/* reset Continuity tables
 */
void DataBaseController::resetBoardContinuityData(const int boardID){
	if(isBoardIdEmpty(boardID))
		return;
	sql::Connection *writerConnection = openDataBaseWriterConnection();
 	sql::PreparedStatement *prepareStatement;
 	sql::ResultSet *result;

	try{
	prepareStatement = writerConnection->prepareStatement("SELECT * FROM gpio_continuity WHERE board_id = ?");
	prepareStatement->setInt(1, boardID);
	result = prepareStatement->executeQuery();
	if (result->next()){
		prepareStatement = writerConnection->prepareStatement("UPDATE gpio_continuity SET measure_value = ?, comment = ? WHERE board_id = ? ");
		prepareStatement->setInt(1, -1);
 		prepareStatement->setString(2, "");
		prepareStatement->setInt(3, boardID);
		prepareStatement->executeQuery();
	}

	}catch(sql::SQLException &error){
 	logger->printToFileLogger("Error:DataBaseController resetBoardContinuityData failed to reset Continuity Tables cause Error: "+ std::string(error.what()) , false);
	}
	removeSqlResult(result);
	removeSqlPostStatement(prepareStatement);
	closeDataBaseConnection(writerConnection);
}

/* update Continuity Tables
 * Parameter:
 * testResult: Structure of Continuity Test Result
 */
void DataBaseController::updateGpioContinuityTestResult(const int boardID, const Continuity_Test_Result_Struct testResult){
	if(isBoardIdEmpty(boardID))
		return;
	sql::Connection *writerConnection = openDataBaseWriterConnection();
 	sql::PreparedStatement *prepareStatement;
 	sql::ResultSet *result;
 	try{
		prepareStatement = writerConnection->prepareStatement("SELECT * FROM gpio_continuity WHERE board_id = ? AND pin = ?");
		prepareStatement->setInt(1, boardID);
		prepareStatement->setString(2, testResult.pin);
		result = prepareStatement->executeQuery();
		if (result->next()){
			prepareStatement = writerConnection->prepareStatement("UPDATE gpio_continuity SET measure_value = ?, comment = ? WHERE board_id = ? AND pin = ?");
			prepareStatement->setDouble(1, testResult.voltage);
			prepareStatement->setString(2, testResult.comment);
			prepareStatement->setInt(3, boardID);
			prepareStatement->setString(4, testResult.pin);
			prepareStatement->executeQuery();
		}else{

			prepareStatement = writerConnection->prepareStatement("INSERT INTO gpio_continuity (pin, board_id, measure_value, comment) VALUE(?, ?, ?, ?)");
			prepareStatement->setString(1, testResult.pin);
			prepareStatement->setInt(2, boardID);
			prepareStatement->setDouble(3, testResult.voltage);
			prepareStatement->setString(4, testResult.comment);
			prepareStatement->executeQuery();
		}
	}catch(sql::SQLException &error){
 		logger->printToFileLogger("Error:DataBaseController updateBoardContinuityData failed to update Continuity Tables cause Error: "+ std::string(error.what()) , false);
	}
	closeDataBaseConnection(writerConnection);
	removeSqlResult(result);
	removeSqlPostStatement(prepareStatement);
}

//Current Consumption
/* reset Board Current Table Value
 */
void DataBaseController::resetCurrentConsumptionData(const int boardID){
	if(isBoardIdEmpty(boardID))
		return;
	sql::Connection *writerConnection = openDataBaseWriterConnection();
 	sql::PreparedStatement *prepareStatement;
 	sql::ResultSet *result;

	try{
	prepareStatement = writerConnection->prepareStatement("SELECT * FROM current_consumption WHERE board_id = ?");
	prepareStatement->setInt(1, boardID);
	result = prepareStatement->executeQuery();
	if (result->next()){
		prepareStatement = writerConnection->prepareStatement("UPDATE current_consumption SET current = ? WHERE board_id = ? ");
		prepareStatement->setDouble(1, -1);
 		prepareStatement->setInt(2, boardID);
		prepareStatement->executeQuery();
	}

	}catch(sql::SQLException &error){
 	logger->printToFileLogger("Error:DataBaseController resetCurrentConsumptionData failed to reset current consumption Tables cause Error: "+  std::string(error.what()) , false);
	}
	removeSqlResult(result);
	removeSqlPostStatement(prepareStatement);
	closeDataBaseConnection(writerConnection);
}

/* update Board Current
 * Parameter:
 * current: Microcontroller Current Consumption
 */
void DataBaseController::updateCurrentTestResult(const int boardID, float current){
	if(isBoardIdEmpty(boardID))
		return;
	sql::Connection *writerConnection = openDataBaseWriterConnection();
 	sql::PreparedStatement *prepareStatement;
 	sql::ResultSet *result;
 	try{
		prepareStatement = writerConnection->prepareStatement("SELECT * FROM current_consumption WHERE board_id = ?");
		prepareStatement->setInt(1, boardID);
 		result = prepareStatement->executeQuery();
		if (result->next()){
			prepareStatement = writerConnection->prepareStatement("UPDATE current_consumption SET current = ? WHERE board_id = ?");
			prepareStatement->setDouble(1, current);
 			prepareStatement->setInt(2, boardID);
 			prepareStatement->executeQuery();
		}else{
			prepareStatement = writerConnection->prepareStatement("INSERT INTO current_consumption (board_id, current) VALUE(?, ?)");
			prepareStatement->setInt(1, boardID);
			prepareStatement->setDouble(2, current);
			prepareStatement->executeQuery();
		}
	}catch(sql::SQLException &error){
 		logger->printToFileLogger("Error:DataBaseController updateCurrentTestResult failed to update Current Tables cause Error: "+ std::string(error.what()) , false);
	}
	closeDataBaseConnection(writerConnection);
	removeSqlResult(result);
	removeSqlPostStatement(prepareStatement);
}

//Flash
/* reset Flash table
 */
void DataBaseController::resetBoardFlashData(const int boardID){
	Flash_Memory_Test_Struct flashMemoryTest = {};
	flashMemoryTest.isUploadTestProgrammOK = -1;
	flashMemoryTest.flash_test = -1;
	flashMemoryTest.flash_errors = -1;
	flashMemoryTest.comment = "testing";
	if(isBoardIdEmpty(boardID))
		return;
	updateBoardFlashData(boardID, &flashMemoryTest);
}

/* update Flash table
 * Parameter:
 * flashState: Flash State
 	 * 1: Flash OK
 	 * 0: Flash failed
 	 * -1: Flash not done
 * comment: Flash comment
 */
void DataBaseController::updateBoardFlashData(const int boardID, Flash_Memory_Test_Struct * flashMemoryTest){
 	if(isBoardIdEmpty(boardID))
		return;
 	sql::Connection *writerConnection = openDataBaseWriterConnection();
 	sql::PreparedStatement *prepareStatement;
 	sql::ResultSet *result;
	try{
		prepareStatement = writerConnection->prepareStatement("SELECT * FROM board_flash WHERE board_id = ?");
		prepareStatement->setInt(1, boardID);
		result = prepareStatement->executeQuery();
		if (result->next()){
			prepareStatement = writerConnection->prepareStatement("UPDATE board_flash SET upload_progrgamm = ?, flash_test = ?, flash_errors = ?, comment = ? WHERE board_id = ?");

				prepareStatement->setInt(1, flashMemoryTest->isUploadTestProgrammOK);
				prepareStatement->setInt(2, flashMemoryTest->flash_test);
				prepareStatement->setInt(3, flashMemoryTest->flash_errors);
 				prepareStatement->setString(4, flashMemoryTest->comment);
 				prepareStatement->setInt(5, boardID);
				prepareStatement->executeQuery();
		}else{
 			prepareStatement = writerConnection->prepareStatement("INSERT INTO board_flash (board_id, upload_progrgamm, flash_test, flash_errors, comment) VALUE(?, ?, ?, ?, ?)");
			prepareStatement->setInt(1, boardID);
			prepareStatement->setInt(2, flashMemoryTest->isUploadTestProgrammOK);
			prepareStatement->setInt(3, flashMemoryTest->flash_test);
			prepareStatement->setInt(4, flashMemoryTest->flash_errors);
			prepareStatement->setString(5, flashMemoryTest->comment);
 			prepareStatement->executeQuery();
		}
	}catch(sql::SQLException &error){
 		logger->printToFileLogger("Error:DataBaseController updateBoardFlashData: failed to update Board Flash Table cause Error: "+ std::string(error.what()) , false);
	}
	closeDataBaseConnection(writerConnection);
	removeSqlResult(result);
	removeSqlPostStatement(prepareStatement);
}

//GPIO Input
/* reset GPIO Input tables
 */
void DataBaseController::resetBoardGpioInputData(const int boardID){
	if(isBoardIdEmpty(boardID))
		return;
	sql::Connection *writerConnection = openDataBaseWriterConnection();
 	sql::PreparedStatement *prepareStatement;
 	sql::ResultSet *result;
	for(int i=0; i<3; i++){
		string table = inputeTables[i];
		string highText = highValue[i];
		string lowText = lowValue[i];
		try{
			prepareStatement = writerConnection->prepareStatement("SELECT * FROM "+table+" WHERE board_id = ?");
			prepareStatement->setInt(1, boardID);
			result = prepareStatement->executeQuery();
			if (result->next()){
				prepareStatement = writerConnection->prepareStatement("UPDATE "+table+" SET  high_state = ?, low_state = ?, comment = ? WHERE board_id = ? ");
				prepareStatement->setInt(1, -1);
				prepareStatement->setInt(2, -1);
				prepareStatement->setString(3, "testing");
				prepareStatement->setInt(4, boardID);
				prepareStatement->executeQuery();
			}

		}catch(sql::SQLException &error){
 			logger->printToFileLogger("Error:DataBaseController resetBoardGpioInputData failed to reset Board GPIO Input Tables cause Error: "+ std::string(error.what()) , false);
		}
		removeSqlResult(result);
		removeSqlPostStatement(prepareStatement);
	}
	closeDataBaseConnection(writerConnection);
}

/* update GPIO Input Tables
 * Parameter:
 * testResult: Structure of GPIO Input Test Result
 */
void DataBaseController::updateBoardGpioInputData(const int boardID, const GPIO_Input_Test_Result_Struct testResult){
	if(isBoardIdEmpty(boardID))
		return;
	sql::Connection *writerConnection = openDataBaseWriterConnection();
 	sql::PreparedStatement *prepareStatement;
 	sql::ResultSet *result;
	string tableName = "";
	string speed = "";
	try{
		if(testResult.PullType == COMMAND_Init_NO_PULL){
			tableName = "gpio_input_no_pull";
		}else if(testResult.PullType == COMMAND_Init_PULL_UP){
			tableName = "gpio_input_pull_up";
		}else if(testResult.PullType == COMMAND_Init_PULL_DOWN){
			tableName = "gpio_input_pull_down";
		}else{
			logger->printToFileLogger( "Error:DataBaseController updateBoardGpioInputData : cause " +tableName+ " unknown Pull Type: " + to_string(testResult.PullType), false);
			return ;
		}
		prepareStatement = writerConnection->prepareStatement("SELECT * FROM " +tableName+ " WHERE board_id = ? AND pin = ?");
		prepareStatement->setInt(1, boardID);
		prepareStatement->setString(2, testResult.pin);
		result = prepareStatement->executeQuery();
		if (result->next()){
			if(testResult.comment.empty()){
				prepareStatement = writerConnection->prepareStatement("UPDATE " +tableName+ " SET high_state = ?, low_state = ? WHERE board_id = ? AND pin = ? ");
			}else{
				prepareStatement = writerConnection->prepareStatement("UPDATE " +tableName+ " SET high_state = ?, low_state = ?, comment = ? WHERE board_id = ? AND pin = ?");
			}
			prepareStatement->setInt(1, testResult.highLevelState);
			prepareStatement->setInt(2, testResult.lowLevelState);
			if(!testResult.comment.empty()){
				prepareStatement->setString(3, testResult.comment);
				prepareStatement->setInt(4, boardID);
				prepareStatement->setString(5, testResult.pin);
			}else{
				prepareStatement->setInt(3, boardID);
				prepareStatement->setString(4, testResult.pin);
			}
			prepareStatement->executeQuery();
		}else{
			if(testResult.comment.empty()){
				prepareStatement = writerConnection->prepareStatement("INSERT INTO " +tableName+ " (pin, board_id, high_state, low_state) VALUE(?, ?, ?, ?)");
			}else{
				prepareStatement = writerConnection->prepareStatement("INSERT INTO " +tableName+ " (pin, board_id, high_state, low_state, comment) VALUE(?, ?, ?, ?, ?)");
			}
			prepareStatement->setString(1, testResult.pin);
			prepareStatement->setInt(2, boardID);
			prepareStatement->setInt(3, testResult.highLevelState);
			prepareStatement->setInt(4, testResult.lowLevelState);
			if(!testResult.comment.empty()){
				prepareStatement->setString(5, testResult.comment);
			}
			prepareStatement->executeQuery();
		}
	}catch(sql::SQLException &error){
 		logger->printToFileLogger("Error:DataBaseController updateBoardGpioInputData failed to update GPIO Input Tables cause Error: "+ std::string(error.what()) , false);
	}
	closeDataBaseConnection(writerConnection);
	removeSqlResult(result);
	removeSqlPostStatement(prepareStatement);
}

//GPIO Output
/* reset GPIO Output Tables
 */
void DataBaseController::resetBoardGpioOutputData(const int boardID){
	if(isBoardIdEmpty(boardID))
		return;
	sql::Connection *writerConnection = openDataBaseWriterConnection();
	sql::PreparedStatement *prepareStatement;
	sql::ResultSet *result;
	for(int i=0; i<3; i++){
		try{
 			prepareStatement = writerConnection->prepareStatement("SELECT * FROM "+outputTables[i]+" WHERE board_id = ? ");
			prepareStatement->setInt(1, boardID);
			result = prepareStatement->executeQuery();
			if (result->next()){
				prepareStatement = writerConnection->prepareStatement("UPDATE "+outputTables[i]+" SET high_voltage = ?, low_voltage = ?, comment = ? WHERE board_id = ?");
				prepareStatement->setDouble(1, -1);
				prepareStatement->setDouble(2, -1);
				prepareStatement->setString(3, "");
				prepareStatement->setInt(4, boardID);
				prepareStatement->executeQuery();
			}
		}catch(sql::SQLException &error){
 			logger->printToFileLogger("Error:DataBaseController resetBoardGpioOutputData failed to reset GPIO Output Tables cause Error: "+  std::string(error.what()) , false);
		}
		removeSqlResult(result);
		removeSqlPostStatement(prepareStatement);
	}
	closeDataBaseConnection(writerConnection);
}

/* update GPIO Output Tables
 * Parameter:
 * testResult: Structure of GPIO Output Test Result
 */
void DataBaseController::updateBoardGpioOutputData(const int boardID, const GPIO_Output_Test_result_Struct testResult){
	if(isBoardIdEmpty(boardID))
		return;
	sql::Connection *writerConnection = openDataBaseWriterConnection();
	sql::PreparedStatement *prepareStatement;
	sql::ResultSet *result;
	try{
		string tableName = "";
		int speed = 0;
		if(testResult.PullType == COMMAND_Init_NO_PULL){
			tableName = "gpio_output_no_pull";
		}else if(testResult.PullType == COMMAND_Init_PULL_UP){
			tableName = "gpio_output_pull_up";
		}else if(testResult.PullType == COMMAND_Init_PULL_DOWN){
			tableName = "gpio_output_pull_down";
		}else{
 			logger->printToFileLogger("Error:DataBaseController updateBoardGpioOutputData unknown Pull Type" + to_string(testResult.PullType), false);
			return ;
		}

		if(testResult.speed == COMMAND_Init_GPIO_SPEED_FREQ_LOW){
			speed = 1;
		}else if(testResult.speed == COMMAND_Init_GPIO_SPEED_FREQ_MEDIUM){
			speed = 2;
		}else if(testResult.speed == COMMAND_Init_GPIO_SPEED_FREQ_HIGH){
			speed = 3;
		}else{
 			logger->printToFileLogger("Error:DataBaseController updateBoardGpioOutputData unknown Speed: " + to_string(testResult.speed), false);
			return ;
		}
 		prepareStatement = writerConnection->prepareStatement("SELECT * FROM " +tableName+ " WHERE board_id = ? AND  pin = ? AND speed = ?");
		prepareStatement->setInt(1, boardID);
		prepareStatement->setString(2, testResult.pin);
		prepareStatement->setInt(3, speed);
		result = prepareStatement->executeQuery();
		if (result->next()){
			if(testResult.comment.empty()){
				prepareStatement = writerConnection->prepareStatement("UPDATE " +tableName+ " SET high_voltage = ?, low_voltage = ?, WHERE board_id = ? AND  pin = ? AND speed = ?");
			}else{
				prepareStatement = writerConnection->prepareStatement("UPDATE " +tableName+ " SET high_voltage = ?, low_voltage = ?, comment = ? WHERE board_id = ? AND pin = ? AND speed = ?");
			}
			prepareStatement->setDouble(1, testResult.voltageHighLevel);
			prepareStatement->setDouble(2, testResult.voltageLowLevel);
			if(testResult.comment.empty()){
				prepareStatement->setInt(3, boardID);
				prepareStatement->setString(4, testResult.pin);
				prepareStatement->setInt(5, speed);
			}else{
				prepareStatement->setString(3, testResult.comment);
				prepareStatement->setInt(4, boardID);
				prepareStatement->setString(5, testResult.pin);
				prepareStatement->setInt(6, speed);
			}
			prepareStatement->executeQuery();
		}else{
			if(testResult.comment.empty()){
				prepareStatement = writerConnection->prepareStatement("INSERT INTO " +tableName+ " (pin, board_id, high_voltage, low_voltage, speed) VALUE(?, ?, ?, ?, ?)");
			}else{
				prepareStatement = writerConnection->prepareStatement("INSERT INTO " +tableName+ " (pin, board_id, high_voltage, low_voltage, comment, speed) VALUE(?, ?, ?, ?, ?, ?)");
			}
			prepareStatement->setString(1, testResult.pin);
			prepareStatement->setInt(2, boardID);
			prepareStatement->setDouble(3, testResult.voltageHighLevel);
			prepareStatement->setDouble(4, testResult.voltageLowLevel);

			if(!testResult.comment.empty()){
				prepareStatement->setString(5, testResult.comment);
				prepareStatement->setInt(6, speed);
			}else{
				prepareStatement->setInt(5,speed);
			}
			prepareStatement->executeQuery();
		}

	}catch(sql::SQLException &error){
 		logger->printToFileLogger("Error:DataBaseController updateBoardGpioOutputData failed to update GPIO Output Tables cause Error: " +  std::string(error.what()) , false);
	}
	closeDataBaseConnection(writerConnection);
	removeSqlResult(result);
	removeSqlPostStatement(prepareStatement);
}

//GPIO Short Circuits
/*reset GPIO Short Circuits Table
 */
void DataBaseController::resetGpioShortCircuitsData(const int boardID){
	if(isBoardIdEmpty(boardID))
		return;
	sql::Connection *writerConnection = openDataBaseWriterConnection();
	sql::PreparedStatement *prepareStatement;
	sql::ResultSet *result;
	try{
		writerConnection->setSchema("master");
		prepareStatement = writerConnection->prepareStatement("SELECT * FROM gpio_short_circuit WHERE board_id = ? ");
		prepareStatement->setInt(1, boardID);
		result = prepareStatement->executeQuery();
		if (result->next()){
			prepareStatement = writerConnection->prepareStatement("UPDATE gpio_short_circuit SET short_circuits_count = ?, comment = ? WHERE board_id = ?");
			prepareStatement->setInt(1, -1);
			prepareStatement->setString(2, "");
			prepareStatement->setInt(3, boardID);
			prepareStatement->executeQuery();
		}

	}catch(sql::SQLException &error){
		logger->printToFileLogger("Error:DataBaseController resetGpioShortCircuitsData failed to reset GPIO Short Circuit Tables cause Error: "+ std::string(error.what()) , false);
	}
	closeDataBaseConnection(writerConnection);
	removeSqlResult(result);
	removeSqlPostStatement(prepareStatement);

}

/* update GPIO Short Circuits Table
 * Parameter:
 * pin: pin
 * detectedShortCircuits: count of detected Short Circuits
 * comment: comment contains connected GPIOs
 */
void DataBaseController::updateGpioShortCircuitsData(const int boardID, const GPIO_SHORT_TEST_RESULT gpioShortCircuitResult){

	if(isBoardIdEmpty(boardID))
		return;
	sql::Connection *writerConnection = openDataBaseWriterConnection();
	sql::PreparedStatement *prepareStatement;
	sql::ResultSet *result;
	try{
		prepareStatement = writerConnection->prepareStatement("SELECT * FROM gpio_short_circuit WHERE board_id = ? AND pin = ?");
		prepareStatement->setInt(1, boardID);
		prepareStatement->setString(2, gpioShortCircuitResult.gpio);
		result = prepareStatement->executeQuery();
		if (result->next()){
			prepareStatement = writerConnection->prepareStatement("UPDATE gpio_short_circuit SET short_circuits_count = ?, comment = ? WHERE board_id = ? AND pin = ?");
			prepareStatement->setInt(1, gpioShortCircuitResult.detectedShortCircuits);
			prepareStatement->setString(2, gpioShortCircuitResult.comment);
			prepareStatement->setInt(3, boardID);
			prepareStatement->setString(4, gpioShortCircuitResult.gpio);
			prepareStatement->executeQuery();
		}else{
			prepareStatement = writerConnection->prepareStatement("INSERT INTO gpio_short_circuit (board_id, pin, short_circuits_count, comment) VALUE(?, ?, ?, ?)");
			prepareStatement->setInt(1, boardID);
			prepareStatement->setString(2, gpioShortCircuitResult.gpio);
			prepareStatement->setInt(3, gpioShortCircuitResult.detectedShortCircuits);
			prepareStatement->setString(4, gpioShortCircuitResult.comment);
			prepareStatement->executeQuery();
		}
	}catch(sql::SQLException &error){
		logger->printToFileLogger("Error:DataBaseController updateGpioShortCircuitsData failed to update Short Circuit Table cause Error: "+ std::string(error.what()) , false);
	}
	closeDataBaseConnection(writerConnection);
	removeSqlResult(result);
	removeSqlPostStatement(prepareStatement);

}

// Temperature
/* update Board Temperature Table
 * Parameter:
 * temperature: Board temperature
 */
void DataBaseController::updatBoardTemperature(const int boardID, const float temperature){
	if(isBoardIdEmpty(boardID))
		return;
	sql::Connection *writerConnection = openDataBaseWriterConnection();
	sql::PreparedStatement *prepareStatement;
	sql::ResultSet *result;
	try{
		prepareStatement = writerConnection->prepareStatement("SELECT * FROM board_temperature WHERE board_id = ?");
		prepareStatement->setInt(1, boardID);
		result = prepareStatement->executeQuery();
		if (result->next()){
			prepareStatement = writerConnection->prepareStatement("UPDATE board_temperature SET temperature = ? WHERE board_id = ?");
			prepareStatement->setDouble(1, temperature);
			prepareStatement->setInt(2, boardID);
			prepareStatement->executeQuery();
		}else{
			prepareStatement = writerConnection->prepareStatement("INSERT INTO board_temperature SET temperature = ?, board_id = ?");
			prepareStatement->setDouble(1, temperature);
			prepareStatement->setInt(2, boardID);
			prepareStatement->executeQuery();
		}
	}catch(sql::SQLException &error){
 		logger->printToFileLogger("Error:DataBaseController updatBoardTemperature failed to update Board Temperature Table cause Error: " + std::string(error.what()) , false);
	}
	closeDataBaseConnection(writerConnection);
	removeSqlResult(result);
	removeSqlPostStatement(prepareStatement);
}

// Power and GND Pins

/* update Board Power and GND Pins Table
 * Parameter:
 * pin: Pin Text
 * voltage: Pin Voltage
 * comment: Comment
 */
void DataBaseController::resetBoardPowerGndVoltage(const int boardID){
	if(isBoardIdEmpty(boardID))
		return;
	sql::Connection *writerConnection = openDataBaseWriterConnection();
	sql::PreparedStatement *prepareStatement;
	sql::ResultSet *result;
	try{
		prepareStatement = writerConnection->prepareStatement("SELECT * FROM power_gnd_pins WHERE board_id = ?");
		prepareStatement->setInt(1, boardID);
 		result = prepareStatement->executeQuery();
		if (result->next()){
			prepareStatement = writerConnection->prepareStatement("UPDATE power_gnd_pins SET voltage = ?, comment = ? WHERE board_id = ?");
			prepareStatement->setDouble(1, -1);
			prepareStatement->setString(2, "testing");
			prepareStatement->setInt(3, boardID);
 			prepareStatement->executeQuery();
		}
	}catch(sql::SQLException &error){
		logger->printToFileLogger("Error:DataBaseController resetBoardPowerGndVoltage failed to reset Power and GND Pins Voltage Table cause Error: " + std::string(error.what()) , false);
	}
	closeDataBaseConnection(writerConnection);
	removeSqlResult(result);
	removeSqlPostStatement(prepareStatement);
}

/* update Board Power and GND Pins Table
 * Parameter:
 * pin: Pin Text
 * voltage: Pin Voltage
 * comment: Comment
 */
void DataBaseController::updateBoardPowerGndVoltage(const int boardID, const Power_GND_TEST_RESULT testResult){
	if(isBoardIdEmpty(boardID))
		return;
	sql::Connection *writerConnection = openDataBaseWriterConnection();
	sql::PreparedStatement *prepareStatement;
	sql::ResultSet *result;
	try{
		prepareStatement = writerConnection->prepareStatement("SELECT * FROM power_gnd_pins WHERE board_id = ? AND pin = ?");
		prepareStatement->setInt(1, boardID);
		prepareStatement->setString(2, testResult.pin);
		result = prepareStatement->executeQuery();
		if (result->next()){
			prepareStatement = writerConnection->prepareStatement("UPDATE power_gnd_pins SET voltage = ?, comment = ? WHERE board_id = ? AND pin = ? ");
			prepareStatement->setDouble(1, testResult.voltage);
			prepareStatement->setString(2, testResult.comment);
			prepareStatement->setInt(3, boardID);
			prepareStatement->setString(4,  testResult.pin);
			prepareStatement->executeQuery();
		}else{
			string str = "INSERT INTO power_gnd_pins (board_id, pin, voltage, comment) VALUE(?, ?, ?, ?)";
			prepareStatement = writerConnection->prepareStatement(str);
			prepareStatement->setInt(1, boardID);
			prepareStatement->setString(2,  testResult.pin);
			prepareStatement->setDouble(3, testResult.voltage);
			prepareStatement->setString(4, testResult.comment);
			prepareStatement->executeQuery();
		}
	}catch(sql::SQLException &error){
		logger->printToFileLogger("Error:DataBaseController updateBoardPowerGndVoltage failed to update Power and GND Pins Voltage Table cause Error: " + std::string(error.what()) , false);
	}
	closeDataBaseConnection(writerConnection);
	removeSqlResult(result);
	removeSqlPostStatement(prepareStatement);
}

//Analog Input
/* update Analog Input Tables
 * Parameter:
 * testResult: Structure of GPIO Input Test Result
 */
void DataBaseController::updateBoardAnalogInputData(const int boardID, const Analog_Input_Test_Result_Struct testResult){
	if(isBoardIdEmpty(boardID))
		return;
	sql::Connection *writerConnection = openDataBaseWriterConnection();
 	sql::PreparedStatement *prepareStatement;
 	sql::ResultSet *result;
	string tableName = "";
 	try{

		prepareStatement = writerConnection->prepareStatement("SELECT * FROM analog_input WHERE board_id = ? AND pin = ?");
		prepareStatement->setInt(1, boardID);
		prepareStatement->setString(2, testResult.pin);
		result = prepareStatement->executeQuery();
		if (result->next()){
			prepareStatement = writerConnection->prepareStatement("UPDATE analog_input SET measure_value1 = ?, measure_value2 = ?, measure_value3 = ?, measure_value4 = ?, comment = ? WHERE board_id = ? AND pin = ? ");
			prepareStatement->setDouble(1, testResult.measuredVotalge1);
			prepareStatement->setDouble(2, testResult.measuredVotalge2);
			prepareStatement->setDouble(3, testResult.measuredVotalge3);
			prepareStatement->setDouble(4, testResult.measuredVotalge4);
			prepareStatement->setString(5, testResult.comment);
			prepareStatement->setInt(6, boardID);
			prepareStatement->setString(7, testResult.pin);
			prepareStatement->executeQuery();
		}else{
			prepareStatement = writerConnection->prepareStatement("INSERT INTO analog_input (pin, board_id, measure_value1, measure_value2, measure_value3, measure_value4, comment) VALUE(?, ?, ?, ?, ?, ?, ?)");
			prepareStatement->setString(1, testResult.pin);
			prepareStatement->setInt(2, boardID);
			prepareStatement->setDouble(3, testResult.measuredVotalge1);
			prepareStatement->setDouble(4, testResult.measuredVotalge2);
			prepareStatement->setDouble(5, testResult.measuredVotalge3);
			prepareStatement->setDouble(6, testResult.measuredVotalge4);
			prepareStatement->setString(7, testResult.comment);
			prepareStatement->executeQuery();
		}
	}catch(sql::SQLException &error){
 		logger->printToFileLogger("Error:DataBaseController updateBoardAnalogInputData failed to update Analog Input Tables cause Error: " + std::string(error.what()) , false);
	}
	closeDataBaseConnection(writerConnection);
	removeSqlResult(result);
	removeSqlPostStatement(prepareStatement);
}

