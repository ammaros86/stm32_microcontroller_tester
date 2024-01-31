/*
 * BoardConnectionController.hpp
 *
 *  Created on: 30.08.2023
 *      Author: Astra
 */

#ifndef CONTROLLERS_BOARDCONNECTIONCONTROLLER_HPP_
#define CONTROLLERS_BOARDCONNECTIONCONTROLLER_HPP_

#include "../helpers/Constants.hpp"
#include "../helpers/CustomTypes.hpp"
#include "PeripheralsInterfacesController.hpp"
#include "../managers/DisplayManager.hpp"

#define NUCLEO_USB_OK_PYHSICAL_DISCONNECTED 2
#define NUCLEO_CONNECTION_CN7_ERROR 3
#define NUCLEO_CONNECTION_CN10_ERROR 4
#define NUCLEO_USB_OK_PYHSICAL_NOT_CORRECTLY_CONNECTED 5

class BoardConnectionController{

public:


	BoardConnectionController(PeripheralsInterfacesController* _controller, DisplayManager *_displayManager) : controller(_controller), displayManager(_displayManager){

	};
 	virtual ~BoardConnectionController();

	int getBoardConnectionState();
	void printNewConnectionState(int boardConnectionNewState);

private:
	PeripheralsInterfacesController* controller;
	DisplayManager *displayManager;

 	void testPowerGndPinsConnectionCheck(Power_GND_Test_Result_Struct * measuredVoltages);
	int checkBoardUsbAndPhysicalConnectionState(BoardConnectionStructure *nucleoBoardConnectionState);
	void getCn10PhysicalConnectionState(Power_GND_Test_Result_Struct * measuredVoltages, BoardConnectionStructure * boardConnectionState);
	void getCn7PhysicalConnectionState(Power_GND_Test_Result_Struct * measuredVoltages, BoardConnectionStructure * boardConnectionState);

	double maxGndVoltage =  0.3;
	double min5vVoltage =  4;
};



#endif /* CONTROLLERS_BOARDCONNECTIONCONTROLLER_HPP_ */
