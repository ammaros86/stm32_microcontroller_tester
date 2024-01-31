/*
 * DisplayManager.cpp
 *
 *  Created on: 30.08.2023
 *      Author: Astra
 */

#include "DisplayManager.hpp"


DisplayManager::~DisplayManager() {

}


/* print welcome text and wait 2 seconds
 */
void DisplayManager::printWelcomeMenu(){
	controller->lcd.clearLcd();
	controller->lcd.printCenteredText("Welcome", 1);
	controller->lcd.printCenteredText(" Nucleo Boards", 2);
	controller->lcd.printCenteredText("Tester", 3);
}

/* scroll menu up/down
 * Parameter:
 * scrollType: defined in common.hpp
 */
int DisplayManager::scrollMenu(Display_Menu_Scroll_Type scrollType){
	int ack = 0;
 	int lcdCurrentItemInt = static_cast<int>(displayMenuCurrentItem);
 	int firstMenuItem = static_cast<int>(Display_Menu_Items::newBoardItem);
	int lastMenuItem = static_cast<int>(Display_Menu_Items::powerBoardTesterOffItem);
	moveMenuSection(scrollType);
 	if(scrollType == Display_Menu_Scroll_Type::down){
		if(lcdCurrentItemInt < lastMenuItem){
			lcdPointer++;
			cout << "LCD Pointer: " << lcdPointer << endl;
			displayMenuCurrentItem = static_cast<Display_Menu_Items>(static_cast<int>(displayMenuCurrentItem) + 1);
			printMenuSection();
			ack = 1;
		}else{
			ack = 0;
		}
	}else{
 		if(lcdCurrentItemInt > firstMenuItem){
			lcdPointer--;
			cout << "LCD Pointer: " << lcdPointer << endl;
			displayMenuCurrentItem = static_cast<Display_Menu_Items>(static_cast<int>(displayMenuCurrentItem) - 1);
			printMenuSection();
			ack = 1;
		}else if(lcdCurrentItemInt == 2){
			printMenuSection();
			ack = 1;
		}else{
			ack = 0;
		}
	}
	return ack;
}

/* increment/decrement Menu section
 * Menu contains 9 sections
 * Parameter:
 * scrollType: defined in common.hpp
 */
void DisplayManager::moveMenuSection(Display_Menu_Scroll_Type scrolltype){
	int DisplayManagerCurrentSectionInt = static_cast<int>(displayMenuCurrentSection);
	if(scrolltype == Display_Menu_Scroll_Type::down ){
		if((displayMenuCurrentSection == Display_Menu_Sections::section1 && displayMenuCurrentItem == Display_Menu_Items::continuityItem)
				||(displayMenuCurrentSection == Display_Menu_Sections::section2 && displayMenuCurrentItem == Display_Menu_Items::powerPinsItem)
				||(displayMenuCurrentSection == Display_Menu_Sections::section3 && displayMenuCurrentItem == Display_Menu_Items::flashItem)
 				||(displayMenuCurrentSection == Display_Menu_Sections::section4 && displayMenuCurrentItem == Display_Menu_Items::boardCurrentItem)
				||(displayMenuCurrentSection == Display_Menu_Sections::section5 && displayMenuCurrentItem == Display_Menu_Items::temperatureItem)
				||(displayMenuCurrentSection == Display_Menu_Sections::section6 && displayMenuCurrentItem == Display_Menu_Items::gpioInputItem)
				||(displayMenuCurrentSection == Display_Menu_Sections::section7 && displayMenuCurrentItem == Display_Menu_Items::gpioOutputItem)
				||(displayMenuCurrentSection == Display_Menu_Sections::section8 && displayMenuCurrentItem == Display_Menu_Items::gpioShortCircuitItem)
				||(displayMenuCurrentSection == Display_Menu_Sections::section9 && displayMenuCurrentItem == Display_Menu_Items::analogItem)
				||(displayMenuCurrentSection == Display_Menu_Sections::section10 && displayMenuCurrentItem == Display_Menu_Items::ipAddressItem)
				||(displayMenuCurrentSection == Display_Menu_Sections::section11 && displayMenuCurrentItem == Display_Menu_Items::copyLogsUsbItem)
				||(displayMenuCurrentSection == Display_Menu_Sections::section12 && displayMenuCurrentItem == Display_Menu_Items::turnUsbPortOnItem)){
			displayMenuCurrentSection = Display_Menu_Sections(DisplayManagerCurrentSectionInt + 1);
		}
	}else{
		if((displayMenuCurrentSection != Display_Menu_Sections::section1)
				||(displayMenuCurrentSection == Display_Menu_Sections::section13 && displayMenuCurrentItem == Display_Menu_Items::ipAddressItem)
				||(displayMenuCurrentSection == Display_Menu_Sections::section12 && displayMenuCurrentItem == Display_Menu_Items::analogItem)
				||(displayMenuCurrentSection == Display_Menu_Sections::section11 && displayMenuCurrentItem == Display_Menu_Items::gpioShortCircuitItem)
				||(displayMenuCurrentSection == Display_Menu_Sections::section10 && displayMenuCurrentItem == Display_Menu_Items::gpioOutputItem)
				||(displayMenuCurrentSection == Display_Menu_Sections::section9 && displayMenuCurrentItem == Display_Menu_Items::gpioInputItem)
				||(displayMenuCurrentSection == Display_Menu_Sections::section8  && displayMenuCurrentItem == Display_Menu_Items::temperatureItem)
				||(displayMenuCurrentSection == Display_Menu_Sections::section7 && displayMenuCurrentItem == Display_Menu_Items::boardCurrentItem)
				||(displayMenuCurrentSection == Display_Menu_Sections::section6 && displayMenuCurrentItem == Display_Menu_Items::flashItem)
				||(displayMenuCurrentSection == Display_Menu_Sections::section5 && displayMenuCurrentItem == Display_Menu_Items::powerPinsItem)
				||(displayMenuCurrentSection == Display_Menu_Sections::section4 && displayMenuCurrentItem == Display_Menu_Items::continuityItem)){
			displayMenuCurrentSection = Display_Menu_Sections(DisplayManagerCurrentSectionInt - 1);
		}
	}
}

/* print menu section on the LCD Display
 * Menu contains 8 sections
 * Parameter:
 * scrollType: defined in common.hpp
 */
void DisplayManager::printMenuSection(){
	controller->lcd.clearLcd();
	cout << "**************************"<< endl;
	cout << "printMenuSection"<< endl;

	cout << "LCD lcdPointer: " << lcdPointer << endl;
	int firstItem = getMenuSectionFirstItemPosition(displayMenuCurrentSection);
	cout << "LCD firstItem: " << firstItem << endl;
	int lastItem = getMenuSectionLastItemPosition(displayMenuCurrentSection);
	cout << "LCD lastItem: " << lastItem << endl;
	int linesCounter = 1;
	if((firstItem != -1) && (lastItem != -1)){
		for(int i=firstItem; i< lastItem ; i++){
			string text = MenuText[i];
			cout << "i: " << i << " text: " << text << endl;
			if(i == lcdPointer && lcdPointer != 0)
				text = "->" + text;
 			if(linesCounter == 1 && displayMenuCurrentSection == Display_Menu_Sections::section1){
				controller->lcd.printCenteredText(text.c_str(), linesCounter);
			}else{
				controller->lcd.printText(text.c_str(), linesCounter);
			}
			linesCounter++;
 		}
	}else{
		controller->logger->printToFileLogger("Error:LCD_MENU printMenuSection unknown menu data", false);
	}
}

/* return the first item position of a section
 * Menu contains 8 sections
 * Parameter:
 * menuSection: defined in common.hpp
 */
int DisplayManager::getMenuSectionFirstItemPosition(Display_Menu_Sections menuSection){
	int firstSectionItemPosition = -1;
	firstSectionItemPosition =  static_cast<int>(menuSection);
	return firstSectionItemPosition - 1;
}

/* return the last item position of a section
 * Menu contains 8 sections
 * Parameter:
 * menuSection: defined in common.hpp
 */
int DisplayManager::getMenuSectionLastItemPosition(Display_Menu_Sections menuSection){
	int lastSectionItemPosition = -1;
	lastSectionItemPosition = static_cast<int>(menuSection);
	return lastSectionItemPosition + 3;
}


/* print the temperature measurement result on the LCD Display
 * Parameter:
 * temperature: measured temperature
 */
void DisplayManager::printTemperatureResult(float temperature){
	string tempartureText =  Helper::convertFloatToString(temperature) + " ";
	controller->lcd.clearLcd();
	if (temperature > BOARD_MAX_TEMPERATURE) {
		controller->lcd.printCenteredText("Microcontroller", 1);
		controller->lcd.printCenteredText("Over heated !", 2);
		controller->lcd.printCenteredText(tempartureText.c_str(), 3);
		controller->lcd.printCenteredText("USB-Power OFF", 4);
 	}else if(temperature == TEMPERATURE_MEASURE_FAILED) {
		controller->lcd.printCenteredText("Temperature", 1);
		controller->lcd.printCenteredText("Measurement", 2);
		controller->lcd.printCenteredText("failed", 3);
 	}else{
		controller->lcd.printCenteredText("Nucleo Board", 1);
		controller->lcd.printCenteredText("Temperature:", 2);
		controller->lcd.printCenteredText(tempartureText.c_str(), 3);
		controller->lcd.printDegreeCharacter();
 	}
}

/* print flash result on the LCD Display
 * Parameter:
 * isFlashOk: flash result state
 */

void DisplayManager::printWaitFlashMemoryTest(){
	controller->lcd.clearLcd();
	controller->lcd.printCenteredText("Flash success", 1);
	controller->lcd.printCenteredText("Test Flash Memory", 2);
	controller->lcd.printCenteredText("please wait..", 3);
}

/* print flash result on the LCD Display
 * Parameter:
 * isFlashOk: flash result state
 */
void DisplayManager::printFlashMemoryFlashTestResult(Flash_Memory_Test_Struct * flashMemoryTest){
	controller->lcd.clearLcd();
	controller->lcd.printCenteredText("Flash Test Finish", 1);
	if(flashMemoryTest->isUploadTestProgrammOK == 1){
		controller->lcd.printCenteredText("Flash Program pass", 2);
		if(flashMemoryTest->flash_test == 1){
			if(flashMemoryTest->flash_errors == 0){
				controller->lcd.printCenteredText("Flash Test pass", 3);
			}else{
				controller->lcd.printCenteredText("Flash Test fail", 3);
				string errorsText = "Flash Errors: " + to_string(flashMemoryTest->flash_errors);
				controller->lcd.printCenteredText(errorsText.c_str() , 4);
			}
		}else{
			controller->lcd.printCenteredText("Flash Test fail", 3);
		}
	}else{
		controller->lcd.printCenteredText("Flash Program fail", 2);
	}
}

/* print Power and GND Pins test result on the LCD Display
 * Parameter:
 * gndPinsTestErrors: number of GND Pins test errors
 * powerPinsTestErrors: number of Power Pins test errors
 */
void DisplayManager::printPowerPinsResult(int errors, int failedTest){
	controller->lcd.clearLcd();
	controller->lcd.printCenteredText("Power GND Pins TEST", 1);
	int lineCounter = 2;
	if(failedTest > 0){
		string errorsText = to_string(int(failedTest)) +  " failed";
		controller->lcd.printCenteredText(errorsText.c_str(), lineCounter);
		lineCounter++;
	}
	if(errors > 0){
		string errorsText = to_string(int(errors)) +  " Errors";
		controller->lcd.printCenteredText(errorsText.c_str(), lineCounter);
		lineCounter++;
	}
	if(failedTest == 0 &&  errors == 0){
		controller->lcd.printCenteredText("=> Test pass", 2);
	}else{
		controller->lcd.printCenteredText("=> Test fail", lineCounter);
	}
}

/* print Power and GND Pins test result on the LCD Display
 * Parameter:
 * gndPinsTestErrors: number of GND Pins test errors
 * powerPinsTestErrors: number of Power Pins test errors
 */
void DisplayManager::printContinuityResult(int failedTest, int shortGpios, int openGpios){
	controller->lcd.clearLcd();
 	int nextLine = 1;
	if((failedTest == 0) && (shortGpios == 0) && (openGpios == 0)){
		controller->lcd.printCenteredText("GPIOs Continuity", nextLine);
		controller->lcd.printCenteredText("=> Test pass", nextLine+1);
	}else{
		if(failedTest > 0){
			string failedToTest = "Failed to test: " + to_string(failedTest);
			controller->lcd.printCenteredText(failedToTest.c_str(), nextLine);
			nextLine ++;
		}
		if(shortGpios > 0){
			string shortGpiosText = "Shorted GND/VCC: " + to_string(shortGpios);
			controller->lcd.printCenteredText(shortGpiosText.c_str(), nextLine);
			nextLine ++;
		}
		if(openGpios > 0){
			string openGpiosText = "Open GPIOS: " + to_string(openGpios);
			controller->lcd.printCenteredText(openGpiosText.c_str(), nextLine);
			nextLine ++;
		}
		controller->lcd.printCenteredText("Continuity Test fail", nextLine);
	}
}


/* print Board Current
 * Parameter:
 * gndPinsTestErrors: number of GND Pins test errors
 * powerPinsTestErrors: number of Power Pins test errors
 */
void DisplayManager::printBoardCurrentResult(double current){
	string currentText = Helper::convertFloatToString(current) + " mA";
	controller->lcd.clearLcd();
 	if(current < 1){
		controller->lcd.printCenteredText("Measure Current", 1);
		controller->lcd.printCenteredText("failed", 2);
		controller->lcd.printCenteredText("Check Current Cable", 3);
	}else{
		if(current > MAX_CURRENT_CONSUMPTION){
		controller->lcd.printCenteredText("High Current", 1);
			controller->lcd.printCenteredText("Consumption detected", 2);
			controller->lcd.printCenteredText(currentText.c_str(), 3);
			controller->lcd.printCenteredText("USB-Power OFF", 4);
		}else{
			controller->lcd.printCenteredText("Current Consumption", 1);
			controller->lcd.printCenteredText("pass", 2);
			controller->lcd.printCenteredText(currentText.c_str(), 3);
 		}
	}
}

/* print copy Logger files test result on the LCD Display
 * Parameter:
 * isUsbFound: USB-Device connection State
 * isCopyOk: Copy Logger files to USB-Device State
 */
void DisplayManager::printCopyFilesUsbResult(bool isUsbFound, bool isCopyOk){
	if(isUsbFound){
		if(isCopyOk){
			controller->lcd.clearLcd();
			controller->lcd.printCenteredText("Copy Log", 1);
			controller->lcd.printCenteredText("Files to USB-Media:", 2);
			controller->lcd.printCenteredText("Success", 3);
		}else{
			controller->lcd.clearLcd();
			controller->lcd.printCenteredText("Copy Log", 1);
			controller->lcd.printCenteredText("Files to USB-Media:", 2);
			controller->lcd.printCenteredText("Failed", 3);
		}
	}else{
		controller->lcd.clearLcd();
		controller->lcd.printCenteredText("Copy Log", 1);
		controller->lcd.printCenteredText("Files:", 2);
 		controller->lcd.printCenteredText("failed No USB Media", 3);
 		controller->lcd.printCenteredText("detected", 4);
	}
}

/* print Raspberry Pi IP-Address on the LCD Display
 * Parameter:
 * ipAddressText: Raspberry Pi IP-Address
 */
void DisplayManager::printIpAddress(const char* ipAddressText){
	controller->lcd.clearLcd();
	controller->lcd.printCenteredText("Server IP-Address:", 1);
 	controller->lcd.printCenteredText(ipAddressText, 3);
}


/* print wait text on the LCD Display while testing GPIO Output/Input
 * Parameter:
 * erros: GPIO Test Errors
 * isInputTest: is Input or Output Test
 */
void DisplayManager::printGpioOutputResult(int errors, int failedTest){
	controller->lcd.clearLcd();
	controller->lcd.printCenteredText("GPIOs Output Test", 1);
	int lineCounter = 2;
	if(failedTest > 0){
		string errorsText = to_string(failedTest) +  " GPIOs failed";
		controller->lcd.printCenteredText(errorsText.c_str(), lineCounter);
		lineCounter++;
	}
	if(errors > 0){
		string errorsText = to_string(errors) +  " Errors";
		controller->lcd.printCenteredText(errorsText.c_str(), lineCounter);
		lineCounter++;
	}
	if(failedTest == 0 && errors== 0){
		controller->lcd.printCenteredText("=> Test pass", 2);
	}else{
		controller->lcd.printCenteredText("=> Test fail", lineCounter);
	}
}


void DisplayManager::printGpiosInputResult(int errors, int failedTest){
	controller->lcd.clearLcd();
	controller->lcd.printCenteredText("GPIOs Input Test", 1);
	int lineCounter = 2;
	if(failedTest > 0){
		string errorsText = to_string(failedTest) +  " GPIOs failed";
		controller->lcd.printCenteredText(errorsText.c_str(), lineCounter);
		lineCounter++;
	}
	if(errors > 0){
		string errorsText = to_string(errors) +  " Errors";
		controller->lcd.printCenteredText(errorsText.c_str(), lineCounter);
		lineCounter++;
	}
	if(errors == 0 && failedTest == 0){
		controller->lcd.printCenteredText("=> Test pass", 2);
	}else{
		controller->lcd.printCenteredText("=> Test fail", lineCounter);
	}
}

/* print analog Input Test Result
 * Parameter:
 * Errors: Analog Input Test Errors
 */
void DisplayManager::printAnalogInputResult(int errors, int failedTest){
	controller->lcd.clearLcd();
	controller->lcd.printCenteredText("Analog Input Test", 1);
	int lineCounter = 2;
	if(failedTest > 0){
		string errorsText = to_string(failedTest) +  " failed";
		controller->lcd.printCenteredText(errorsText.c_str(), lineCounter);
		lineCounter++;
	}
	if(errors > 0){
		string errorsText = to_string(errors) +  " Errors";
		controller->lcd.printCenteredText(errorsText.c_str(), lineCounter);
		lineCounter++;
	}
	if(errors == 0 && failedTest == 0){
		controller->lcd.printCenteredText("=> Test pass", 2);
	}else{
		controller->lcd.printCenteredText("=> Test fail", lineCounter);
	}
}

/* print Short Circuits Test Result
 * Parameter:
 * errors: Errors while testing
 * shortCircuits: detected Short Circuits Count
 */
void DisplayManager::printShortCircuitGpioResult(int failedTest, int shortCircuits){
	controller->lcd.clearLcd();
	controller->lcd.printCenteredText("Short Circuit Test", 1);
	int lineCounter = 2;
	if(failedTest > 0){
		string errorsText = to_string(failedTest) +  " failed Test";
		controller->lcd.printCenteredText(errorsText.c_str(), lineCounter);
		lineCounter++;
	}
	if(shortCircuits > 0){
		string errorsText = to_string(shortCircuits) +  " Short Circuits";
		controller->lcd.printCenteredText(errorsText.c_str(), lineCounter);
		lineCounter++;
	}
	if(failedTest == 0 && shortCircuits == 0){
		controller->lcd.printCenteredText("=> Test pass", 2);
	}else{
		controller->lcd.printCenteredText("=> Test fail", lineCounter);
	}
}


/* print complete Test result
 * Parameter:
 * erros: Complete Test Errors
 */
void DisplayManager::printCompleteTestResult(int failedTest, int errors){
	controller->lcd.clearLcd();
	controller->lcd.printCenteredText("Complete Test", 1);
 	int lineCounter = 2;
	if(failedTest > 0){
		string errorsText = "=> " + to_string(failedTest) +  " failed Test";
		controller->lcd.printCenteredText(errorsText.c_str(), lineCounter);
		lineCounter++;
	}
	if(errors > 0){
		string errorsText = "=> " + to_string(errors) +  " Errors";
		controller->lcd.printCenteredText(errorsText.c_str(), lineCounter);
		lineCounter++;
	}
	if(failedTest == 0 && errors == 0){
		controller->lcd.printCenteredText("Mircrocontroller OK", lineCounter);
	}

}

/* print wait text on the LCD Display while turn Raspberry Pi USB-Ports On
 */
void DisplayManager::printWaitTextTurnUsbOn(bool isUsbPortOn){
	controller->lcd.clearLcd();
	controller->lcd.printCenteredText("Turn USB-Port ON", 1);
	if(isUsbPortOn){
		controller->lcd.printCenteredText("Current State: ON", 3);
 	}else{
		controller->lcd.printCenteredText("Current State: OFF", 3);
	 	controller->lcd.printCenteredText("please wait...", 4);
 	}
}


void DisplayManager::printUsbPortState(bool isUsbPortOn){
	controller->lcd.clearLcd();
	controller->lcd.printCenteredText("Current USB Port", 1);
	if(isUsbPortOn){
		controller->lcd.printCenteredText("State: ON", 2);
	}else{
		controller->lcd.printCenteredText("State: OFF", 2);
	}
}

/* print result of turning USB-Ports On
 * Parameter:
 * isUsbPortOn: USB-Ports State
 */
void DisplayManager::printUsbTurnOn(bool isUsbTurnOn){
	controller->lcd.clearLcd();
	controller->lcd.printCenteredText("Turn USB-Port ON:", 1);
	if(isUsbTurnOn){
		controller->lcd.printCenteredText("success", 2);
	}else{
		controller->lcd.printCenteredText("failed", 2);
	}
}

/* print wait text on the LCD Display before shutting down Board-Tester
 */
void DisplayManager::printWaitTurnBoardTesterOff(){
	controller->lcd.clearLcd();
	controller->lcd.printCenteredText("Turn Board", 1);
	controller->lcd.printCenteredText("Tester", 2);
	controller->lcd.printCenteredText("OFF", 3);
}

void DisplayManager::printWaitFailedSetBoardTesterOff(){
	controller->lcd.clearLcd();
	controller->lcd.printCenteredText("Turn Board", 1);
	controller->lcd.printCenteredText("Tester", 2);
	controller->lcd.printCenteredText("OFF failed", 3);
}

/* print wait text on the LCD Display before shutting down Board-Tester
 */
void DisplayManager::printUsbDisconnted(){
	controller->lcd.clearLcd();
	controller->lcd.printCenteredText("Nucleo Board", 1);
	controller->lcd.printCenteredText("Disconnected", 2);
	controller->lcd.printCenteredText("please check", 3);
	controller->lcd.printCenteredText("USB-Connection", 4);
}

/* print Text when initialization failed
 */
void DisplayManager::printInitializationFailed(){
	controller->lcd.clearLcd();
	controller->lcd.printCenteredText("Failed to initial", 1);
	controller->lcd.printCenteredText("Board Tester", 2);
}

void DisplayManager::printNewBoard(){
	controller->lcd.clearLcd();
	controller->lcd.printCenteredText("Connect", 1);
	controller->lcd.printCenteredText("New Nucleo Board", 2);
	controller->lcd.printCenteredText("Board Tester Locked", 3);
	controller->lcd.printCenteredText("Press Red to unlock", 4);
}

void DisplayManager::printWaitCheckConnection(){
	controller->lcd.clearLcd();
	controller->lcd.printCenteredText("Check Nucleo Board", 1);
	controller->lcd.printCenteredText("Connection", 2);
	controller->lcd.printCenteredText("please wait", 3);
}


void DisplayManager::printLcdWaitTestProcess(TestCommand testCommand){
	controller->lcd.clearLcd();
	switch (testCommand) {
		case TestCommand::measureTemperature:
			controller->lcd.printCenteredText("Temperature", 1);
			controller->lcd.printCenteredText("Measurement", 2);
			break;
		case TestCommand::powerGndPinsTest:
			controller->lcd.printCenteredText("Power & GND Test", 2);
			break;
		case TestCommand::continuityTest:
			controller->lcd.printCenteredText("GPIOs Continuity", 1);
			controller->lcd.printCenteredText("Test", 2);
			break;
		case TestCommand::boardCurrent:
			controller->lcd.printCenteredText("Measure Board", 1);
			controller->lcd.printCenteredText("Current", 2);
			break;
		case TestCommand::flashBoard:
			controller->lcd.printCenteredText("Flashing Board", 2);
			break;
		case TestCommand::testGpioOutput:
			controller->lcd.printCenteredText("GPIOs Output Test", 2);
 			break;
		case TestCommand::testGpioInput:
			controller->lcd.printCenteredText("GPIOs Input Test", 2);
			break;
		case TestCommand::testAnalogInput:
			controller->lcd.printCenteredText("Analog Input", 1);
			controller->lcd.printCenteredText("Test", 2);
 			break;
		case TestCommand::testGpioShortCircuit:
			controller->lcd.printCenteredText("GPIOs Short circuit", 1);
			controller->lcd.printCenteredText("Test", 2);
 			break;
		case TestCommand::getServerIpAddress:
			break;
		case TestCommand::copyLogsUsb:
			controller->lcd.printCenteredText("Copy Log Files", 2);
  			break;
		case TestCommand::turnUsbPortOn:
			controller->lcd.printCenteredText("Turn USB ON", 2);
				break;
		case TestCommand::powerBoardTesterOff:
			controller->lcd.printCenteredText("Power OFF", 2);
				break;
		default:
			break;
	}
 	controller->lcd.printCenteredText("please wait...", 3);
}

