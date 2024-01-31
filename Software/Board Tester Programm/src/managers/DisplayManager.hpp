/*
 * DisplayManager.hpp
 *
 *  Created on: 30.08.2023
 *      Author: Astra
 */

#ifndef MANAGERS_DISPLAYMANAGER_HPP_
#define MANAGERS_DISPLAYMANAGER_HPP_




#include "../controllers/PeripheralsInterfacesController.hpp"

#define MENU_ARRAY_SIZE 16

class DisplayManager{

public:

 	virtual ~DisplayManager();

 	DisplayManager(PeripheralsInterfacesController *_controller): controller(_controller){
 		controller = _controller;
 		displayMenuCurrentItem = Display_Menu_Items::newBoardItem;
 		displayMenuCurrentSection = Display_Menu_Sections::section1;
 		lcdPointer = 1;
	};

  	void printWelcomeMenu();
  	int scrollMenu(Display_Menu_Scroll_Type scrollType);
 	void moveMenuSection(Display_Menu_Scroll_Type scrolltype);
 	void printMenuSection();
 	int getMenuSectionFirstItemPosition(Display_Menu_Sections menuSection);
 	int getMenuSectionLastItemPosition(Display_Menu_Sections menuSection);
  	void printTemperatureResult(float temperature);
   	void printWaitFlashMemoryTest();
  	void printFlashMemoryFlashTestResult(Flash_Memory_Test_Struct * flashMemoryTest);
  	void printPowerPinsResult(int errors, int failedTest);
  	void printContinuityResult(int failedTest, int shortGpios, int openGpios);
  	void printBoardCurrentResult(double current);

  	void printCopyFilesUsbResult(bool isUsbFound, bool isCopyOk);
 	void printIpAddress(const char* ipAddressText);
   	void printGpioOutputResult(int errors, int failedTest);
   	void printGpiosInputResult(int errors, int failedTest);
 	void printAnalogInputResult(int errors, int failedTest);
 	void printShortCircuitGpioResult(int failedTest, int shortCircuits);
 	void printCompleteTestResult(int failedTest, int errors);
 	void printWaitTextTurnUsbOn(bool isUsbPortOn);
 	void printUsbTurnOn(bool isUsbTurnOn);
 	void printUsbPortState(bool isUsbTurnOn);
 	void printWaitTurnBoardTesterOff();
 	void printWaitFailedSetBoardTesterOff();

 	void printUsbDisconnted();
 	void printInitializationFailed();
 	void printNewBoard();
 	void printWaitCheckConnection();
 	void printLcdWaitTestProcess(TestCommand testCommand);
 	Display_Menu_Items displayMenuCurrentItem;
 	Display_Menu_Sections displayMenuCurrentSection;
  	int lcdPointer;

private:

  	PeripheralsInterfacesController *controller;
	string MenuText[MENU_ARRAY_SIZE] = {Strings::PRINT_BOARD_TESTER, Strings::NEW_BOARD, Strings::COMPLETE_TEST, Strings::CONTINUITY_TEST, Strings::POWER_GND, Strings::FLASH, Strings::BOARD_CURRENT,
			Strings::TEMPERATURE, Strings::GPIO_INPUT_TEST, Strings::GPIO_OUTPUT_TEST, Strings::GPIO_SHORT_TEST, Strings::ANALOG_TEST,
			Strings::PRINT_IPADDRESS, Strings::COPY_FILES, Strings::USB_PORT_ON, Strings::POWER_OFF};

};






#endif /* MANAGERS_DISPLAYMANAGER_HPP_ */
