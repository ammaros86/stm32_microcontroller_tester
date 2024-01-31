/*
 * Logger.hpp
 *
 *  Created on: 14.06.2023
 *      Author: Astra
 */

#ifndef HELPERS_LOGGER_HPP_
#define HELPERS_LOGGER_HPP_

#include "Helper.hpp"

#define LOG_LINE_WIDTH 100

class Logger {

public:

	Logger();
  	virtual ~Logger();
  	string initializeLogFiles();
  	void intialLoggerFiles();
	void printTestSectionHeaderSeperator();
  	void printSectionSeperator(bool isSimpleReport);
  	void printTestReportGpioNumberText(int testNumber, GPIO_Initial_Struct *gpioInitialStructure);
 	void printTestReportGpioShortNumberText(int testNumber, GPIO_Initial_Struct *gpioInitialStructure);
	void printToFileLogger(string text, bool isDetailLogger);
	void printCenterTextFileLogger(string text, bool isSimpleReport);
	void printBoardID(bool isAppCommand, TestCommand command, string BoardID);
	void closeFiles();
 	std::fstream detailsLogger;

private:

	std::fstream reportLogger;

};



#endif /* HELPERS_LOGGER_HPP_ */
