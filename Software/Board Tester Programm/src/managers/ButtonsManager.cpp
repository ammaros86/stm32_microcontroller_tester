/*
 * ButtonsManager.cpp
 *
 *  Created on: 30.08.2023
 *      Author: Astra
 */

#include "ButtonsManager.hpp"


Buttons ButtonsManager::pressedButton = Buttons::None;


ButtonsManager::~ButtonsManager() {
    if (buttonsThread.joinable()) {
		buttonsThread.join();
	}
}

/* initialize Buttons Interrupt and Buttons Thread
 */
void ButtonsManager::initializeButtonsManagement(){
	wiringPiISR(GREEN_Button_PIN, INT_EDGE_RISING, &ButtonsManager::handleGreenButtonInterrupt);
	wiringPiISR(Yellow_Button_PIN, INT_EDGE_RISING, &ButtonsManager::handleYellowButtonInterrupt);
	wiringPiISR(BLUE_Button_PIN, INT_EDGE_RISING, &ButtonsManager::handleBlueButtonInterrupt);
	wiringPiISR(RED_Button_PIN, INT_EDGE_RISING, &ButtonsManager::handleRedButtonInterrupt);
	buttonsThread = std::thread(&ButtonsManager::handlePressedButtons, this);
}

/* handle Pressed Button Thread Function
 */
void ButtonsManager::handlePressedButtons(){
	while(1){
		if(pressedButton == Buttons::Yellow && interruptsEnabled){
 			if((displayManager->displayMenuCurrentItem  != Display_Menu_Items:: powerBoardTesterOffItem) && (testProcessManager->getTestProcessState() == Test_Process_State::free)){
				if(displayManager->scrollMenu(Display_Menu_Scroll_Type::down) == 1){
					TestCommand newCommand = static_cast<TestCommand>(static_cast<int>(testProcessManager->getCurrentTestCommand()) + 1);
					testProcessManager->setNewTestCommand(newCommand);
				}
			 }
 			pressedButton = Buttons::None;
		}else if(pressedButton == Buttons::Blue && interruptsEnabled){
 			if((displayManager->displayMenuCurrentItem  != Display_Menu_Items:: newBoardItem) && (testProcessManager->getTestProcessState() == Test_Process_State::free)){
				if(displayManager->scrollMenu(Display_Menu_Scroll_Type::up) == 1){
					TestCommand newCommand = static_cast<TestCommand>(static_cast<int>(testProcessManager->getCurrentTestCommand()) - 1);
					testProcessManager->setNewTestCommand(newCommand);
				}
			}
 			pressedButton = Buttons::None;
		}else if(pressedButton == Buttons::Green && interruptsEnabled){
			if(testProcessManager->getTestProcessState() == Test_Process_State::free){
				if(displayManager->displayMenuCurrentItem == Display_Menu_Items:: newBoardItem){
					controller->logger->printBoardID(false, TestCommand::newBoard, "");
					controller->setNewLedsCommand(LEDS_COMMAND::setAllOff);
					testProcessManager->setNewTestProcessState(Test_Process_State::working);
					controller->turnUsbPortOff();
					interruptsEnabled = false;
					displayManager->printNewBoard();
				}else{
					if(testProcessManager->getCurrentTestCommand() == TestCommand::completTest){
						testProcessManager->startCompleteBoardTest(false);
					}else{
						testProcessManager->startTestProcessThread(-1, "", testProcessManager->getCurrentTestCommand(), false, false);
					}
				}
			}
			pressedButton = Buttons::None;
		}else if((pressedButton == Buttons::Red) && (testProcessManager->getTestProcessState() == Test_Process_State::free)){
			displayManager->printMenuSection();
			pressedButton = Buttons::None;
		}else if((pressedButton == Buttons::Red) && (testProcessManager->getTestProcessState() == Test_Process_State::working)){
			if(!interruptsEnabled){
				startTimeButtonPress = std::chrono::steady_clock::now();
				checkNewBoardProcessFinish();
				delay(_200MS_DELAY);
			}
		}
	}
}

/* check connect new Board Process finish
 */
void ButtonsManager::checkNewBoardProcessFinish(){
    while(digitalRead(RED_Button_PIN) == HIGH) {
        std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
        std::chrono::milliseconds redButtonPressDuratiom = std::chrono::duration_cast<std::chrono::milliseconds>(now - startTimeButtonPress);
        if (redButtonPressDuratiom.count() >= longPressMinDuration) {
    		controller->setNewLedsCommand(LEDS_COMMAND::blinkAll);
        	handleNewBoardProcessFinish();
    		break;
        }
    }
    pressedButton = Buttons::None;
}

void ButtonsManager::handleNewBoardProcessFinish(){
	controller->lcd.initialLcd();
	displayManager->printWaitCheckConnection();
	controller->turnUsbPortOn();
  	int connectionState = boardConnectionController->getBoardConnectionState();
	if(connectionState == NUCLEO_CONNECTION_OK){
		controller->setNewLedsCommand(LEDS_COMMAND::setGreenOn);
	}else{
		controller->setNewLedsCommand(LEDS_COMMAND::setRedOn);
	}
	boardConnectionController->printNewConnectionState(connectionState);
  	pressedButton = Buttons::None;
	interruptsEnabled = true;
	testProcessManager->setNewTestProcessState(Test_Process_State::free);
}

/* Yellow Button Interrupt Service Routine, scroll down
 */
void ButtonsManager::handleYellowButtonInterrupt(){
	pressedButton = Buttons::Yellow;
}

/* Blue Button Interrupt Service Routine, scroll up
 */
void ButtonsManager::handleBlueButtonInterrupt(){
	pressedButton = Buttons::Blue;
}

/* Red Button Interrupt Service Routine, back to main Menu
 */
void ButtonsManager::handleRedButtonInterrupt(){
	pressedButton = Buttons::Red;
}

/* Green Button Interrupt Service Routine, select Test Process and start
 */
void ButtonsManager::handleGreenButtonInterrupt(){
	pressedButton = Buttons::Green;
}
