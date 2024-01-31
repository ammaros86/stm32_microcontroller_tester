/*
 * DatabaseController.hpp
 *
 *  Created on: 29.05.2023
 *      Author: Astra
 */

#ifndef DATABASE_DATABASECONTROLLER_HPP_
#define DATABASE_DATABASECONTROLLER_HPP_

#include <mysql_connection.h>
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
#include <thread>
#include <string>
#include <fstream>

#include "../helpers/CustomTypes.hpp"
#include "../helpers/Logger.hpp"

using namespace std;
using namespace sql;

class DataBaseController{

public :

	DataBaseController();
	virtual ~DataBaseController();
	void initialDataBaseController(Logger *_logger);

	sql::Connection * openDataBaseWriterConnection();

	void closeDataBaseConnection(sql::Connection * connection);
	void removeSqlResult(sql::ResultSet *result);
	void removeSqlStatement(sql::Statement *stmt);
	void removeSqlPostStatement(sql::PreparedStatement *pstmt);
	bool isBoardIdEmpty(int boardID);


	//Board Controller
 	void updateBoardControllerUsbState(int value);
 	void updateBoardControllerState(int value);
 	void updateBoardControllerCopyState(int value);


	//Board Commands
	void updateBoardCommand(const int boardID, const TestCommand command, const int value, const int errors);

 	//Continuity
 	void resetBoardContinuityData(const int boardID);
 	void updateGpioContinuityTestResult(const int  boardID, Continuity_Test_Result_Struct testResult);

 	// Current
 	void resetCurrentConsumptionData(const int boardID);
 	void updateCurrentTestResult(const int boardID, float current);

	//Flash
	void resetBoardFlashData(const int boardID);
 	void updateBoardFlashData(const int boardID, Flash_Memory_Test_Struct * flashMemoryTest);

 	//GPIO Input
 	void resetBoardGpioInputData(const int boardID);
 	void updateBoardGpioInputData(const int boardID, const GPIO_Input_Test_Result_Struct testResult);

	//GPIO Output
  	void resetBoardGpioOutputData(const int boardID);
 	void updateBoardGpioOutputData(const int boardID, const GPIO_Output_Test_result_Struct testResult);

	//GPIO Short Circuit
  	void resetGpioShortCircuitsData(const int boardID);
 	void updateGpioShortCircuitsData(const int boardID, const GPIO_SHORT_TEST_RESULT gpioShortCircuitResult);

 	//Temperature
 	void updatBoardTemperature(const int boardID, const float temperature);

 	//Power and GND Pins
 	void resetBoardPowerGndVoltage(const int boardID);
 	void updateBoardPowerGndVoltage(const int boardID, const Power_GND_TEST_RESULT testResult);

 	//Analog Input
  	void updateBoardAnalogInputData(const int boardID, const Analog_Input_Test_Result_Struct testResult);


private:
	Logger *logger;
	string inputeTables[3]={"gpio_input_no_pull", "gpio_input_pull_up", "gpio_input_pull_down"};
	string highValue[3]={"noPullHigh", "pullUpHigh", "pullDownHigh"};
	string lowValue[3]={"noPullLow", "pullUpLow", "pullDownLow"};
	string outputTables[3] = {"gpio_output_no_pull", "gpio_output_pull_down", "gpio_output_pull_up"};

};


#endif /* DATABASE_DATABASECONTROLLER_HPP_ */
