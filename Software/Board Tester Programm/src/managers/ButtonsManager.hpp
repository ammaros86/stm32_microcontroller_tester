/*
 * ButtonsManager.hpp
 *
 *  Created on: 30.08.2023
 *      Author: Astra
 */

#ifndef MANAGERS_BUTTONSMANAGER_HPP_
#define MANAGERS_BUTTONSMANAGER_HPP_


#include "../controllers/PeripheralsInterfacesController.hpp"
#include "DisplayManager.hpp"
#include "TestProcessManager.hpp"
#include "../controllers/BoardConnectionController.hpp"

class ButtonsManager{

public:


 	ButtonsManager(PeripheralsInterfacesController* _controller, TestProcessManager* _testProcessManager, DisplayManager *_displayManager, BoardConnectionController *_boardConnectionController) :
 		controller(_controller), testProcessManager(_testProcessManager), displayManager(_displayManager), boardConnectionController(_boardConnectionController){
 		interruptsEnabled = true;
 	};

 	virtual ~ButtonsManager();

 	void initializeButtonsManagement();

private:

 	PeripheralsInterfacesController *controller;
 	TestProcessManager *testProcessManager;
 	DisplayManager * displayManager;
 	BoardConnectionController *boardConnectionController;

	bool interruptsEnabled;
	std::chrono::steady_clock::time_point startTimeButtonPress;
	const int longPressMinDuration = 2000;
	std::thread buttonsThread;

 	void handlePressedButtons();
	void checkNewBoardProcessFinish();
	void handleNewBoardProcessFinish();

	static Buttons pressedButton;
	static void handleYellowButtonInterrupt();
	static void handleBlueButtonInterrupt();
	static void handleRedButtonInterrupt();
	static void handleGreenButtonInterrupt();

};


#endif /* MANAGERS_BUTTONSMANAGER_HPP_ */
