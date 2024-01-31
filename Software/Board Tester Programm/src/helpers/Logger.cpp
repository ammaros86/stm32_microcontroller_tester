 /*
 * Logger.cpp
 *
 *  Created on: 14.06.2023
 *      Author: Astra
 */

#include "Logger.hpp"

Logger::Logger(){

}

Logger::~Logger(){
}

/* Open and initialize Log Files
 */
string Logger::initializeLogFiles(){
 	std::string currentTimeFile = Helper::getCurrentDateTimeText(true);
 	std::string currentTime = Helper::getCurrentDateTimeText(false);
	string reportFileName;
	string detailReportFileName;
	reportFileName.append(Strings::simpleTestLogFileNamePrefix);
	reportFileName.append(currentTimeFile);
	reportFileName.append(Strings::reportFileNameSuffix);
	reportLogger.open(reportFileName, std::ios::app);
	detailReportFileName.append(Strings::detailTestLogFileNamePrefix);
	detailReportFileName.append(currentTimeFile);
	detailReportFileName.append(Strings::reportFileNameSuffix);
	detailsLogger.open(detailReportFileName, std::ios::app);
	detailsLogger << Strings::reportFirstLine << currentTime << std::endl;
	reportLogger << Strings::reportFirstLine << currentTime << std::endl;
 	return detailReportFileName;
}


/* print Section Header Separator
 */
void Logger::printTestSectionHeaderSeperator(){
	for(int i=0; i< 80; i++){
		detailsLogger << '='  ;
		reportLogger << '=' ;
	}
	detailsLogger << std::endl;
 	reportLogger << std::endl;
}

/* print Section Separator
 */
void Logger::printSectionSeperator(bool isSimpleReport){
	for(int i=0; i< 80; i++){
		detailsLogger << '-'  ;
		if(isSimpleReport){
			reportLogger << '-' ;
		}
	}
	detailsLogger << std::endl;
	if(isSimpleReport){
		reportLogger << std::endl;
	}
 }

/* print GPIO test number, pin and port in Logger file
 * Parameter:
 * testNumber: test number
 * gpioInitialStructure:  GPIO initialization structure
 */
void Logger::printTestReportGpioNumberText(int testNumber, GPIO_Initial_Struct *gpioInitialStructure){
	if(testNumber > 0){
		printSectionSeperator(false);
	}else{
		printToFileLogger("\n", false);
	}
	printToFileLogger("- " + std::to_string(testNumber+1) + " Test GPIO :" + Helper::getPortText(gpioInitialStructure->Port) +  std::to_string(gpioInitialStructure->Pin) + " " +Helper::getTestFileHeaderText(gpioInitialStructure->Mode, gpioInitialStructure->Pull, gpioInitialStructure->Speed), false);
}

/* print GPIO test number, pin and port in Logger file for Short Circuit Test
 * Parameter:
 * testNumber: test number
 * gpioInitialStructure:  GPIO initialization structure
 */
void Logger::printTestReportGpioShortNumberText(int testNumber, GPIO_Initial_Struct *gpioInitialStructure){
	if(testNumber > 0){
		printSectionSeperator(false);
	}else{
		printToFileLogger("\n", false);
	}
	printToFileLogger(std::to_string(testNumber+1) + " Testing GPIOs for Short Circuit with GPIO Pin:" + Helper::getPortText(gpioInitialStructure->Port) +  std::to_string(gpioInitialStructure->Pin), false);
}

/* print Text to Log File
 * Parameter:
 * text: text to print
 * isSimpleReport: to print it in Detail and Simple Log or only in Detail Log File
 */
void Logger::printToFileLogger(string text, bool isSimpleReport){
	detailsLogger << text << std::endl;
	if(isSimpleReport){
		reportLogger << text << std::endl;
	}
}

/* print centered Text to Log File
 * Parameter:
 * text: text to print
 * isSimpleReport: to print it in Detail and Simple Log or only in Detail Log File
 */
void Logger::printCenterTextFileLogger(string text, bool isSimpleReport){
 	int textLength = text.length();
	if (textLength >= LOG_LINE_WIDTH) {
		printToFileLogger(text, isSimpleReport);
	} else {
		int textPadding = (LOG_LINE_WIDTH - textLength) / 2;
		std::string centredText = std::string(textPadding, ' ') + text;
		printToFileLogger(centredText, isSimpleReport);
 	}
}

/* print Board ID and Current Time before Test begin
 * Parameter:
 * isAppCommand: Mobile-Application Command
 * command: Test Command
 * BoardID: BoardID
 */
void Logger::printBoardID(bool isAppCommand, TestCommand command, string BoardID){
	printTestSectionHeaderSeperator();
	std::string currentTime = Helper::getCurrentDateTimeText(false);
	string commandText = Helper::getTestCommandText(command);
	printCenterTextFileLogger(commandText  , true);
	if(isAppCommand){
		printCenterTextFileLogger("Nucleo Board: " + BoardID, true);
	}else{
		string textBoardID = "Unknown Nucleo Board";
		printCenterTextFileLogger(textBoardID, true);
	}
	printCenterTextFileLogger(currentTime , true);
}

/* close Files
 */
void Logger::closeFiles(){
	detailsLogger.close();
	reportLogger.close();
}
