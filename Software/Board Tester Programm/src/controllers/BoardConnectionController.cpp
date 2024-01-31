/*
 * BoardConnectionController.cpp
 *
 *  Created on: 30.08.2023
 *      Author: Astra
 */

#include "BoardConnectionController.hpp"


BoardConnectionController::~BoardConnectionController(){

}


/* return Board Connection State
 */
int BoardConnectionController::getBoardConnectionState(){
	BoardConnectionStructure nucleoBoardConnectionState = {};
	Power_GND_Test_Result_Struct measuredVoltages = {};
	nucleoBoardConnectionState.isUsbConnected = controller->usbPortshandler.isMicorcontrollerConnectedToBoardTester();
	if(nucleoBoardConnectionState.isUsbConnected){
 		controller->checkUsbPortStateAndInitialUart();
	}
	testPowerGndPinsConnectionCheck(&measuredVoltages);
	getCn10PhysicalConnectionState(&measuredVoltages, &nucleoBoardConnectionState);
	getCn7PhysicalConnectionState(&measuredVoltages, &nucleoBoardConnectionState);
	return checkBoardUsbAndPhysicalConnectionState(&nucleoBoardConnectionState);
}

/*  test Power GND Pins for physically connection check
 * Parameter:
 * measuredVoltages: Pins voltages structure
 */
void BoardConnectionController::testPowerGndPinsConnectionCheck(Power_GND_Test_Result_Struct * measuredVoltages){
    	for(uint8_t i=0; i < controller->powerGndMuxPinVectors.size(); i++){
 		controller->openMuxConnectionToTestPowerGndPin(false, i);
   		double voltage = controller->getAdcVoltage();

  		if(controller->powerGndPinsStringVector.at(i) == "U5V" ){
			measuredVoltages->u5vVoltage = voltage;
		}

		if(controller->powerGndPinsStringVector.at(i) == "GND1" ){
			measuredVoltages->gnd1Voltage = voltage;
		}

		if(controller->powerGndPinsStringVector.at(i) == "AGND" ){
			measuredVoltages->agndVoltae = voltage;
		}

		if(controller->powerGndPinsStringVector.at(i) == "AVDD" ){
			measuredVoltages->avddVoltage = voltage;
		}

		if(controller->powerGndPinsStringVector.at(i) == "GND2" ){
			measuredVoltages->gnd2Voltage = voltage;
		}

		if(controller->powerGndPinsStringVector.at(i) == "VDD" ){
			measuredVoltages->vddVoltage = voltage;
		}

		if(controller->powerGndPinsStringVector.at(i) == "GND3" ){
			measuredVoltages->gnd3Voltage = voltage;
		}

		if(controller->powerGndPinsStringVector.at(i) == "VBAT" ){
			measuredVoltages->vbatVoltage = voltage;
		}

		if(controller->powerGndPinsStringVector.at(i) == "GND4" ){
			measuredVoltages->gnd4Voltage = voltage;
		}

		if(controller->powerGndPinsStringVector.at(i) == "IOREF" ){
			measuredVoltages->iorefVoltage = voltage;
		}

		if(controller->powerGndPinsStringVector.at(i) == "3.3V" ){
			measuredVoltages->_3vVoltage = voltage;
		}

		if(controller->powerGndPinsStringVector.at(i) == "5V" ){
			measuredVoltages->_5vVoltage = voltage;
		}

		if(controller->powerGndPinsStringVector.at(i) == "GND5" ){
			measuredVoltages->gnd5Voltage = voltage;
		}

		if(controller->powerGndPinsStringVector.at(i) == "GND6" ){
			measuredVoltages->gnd6Voltage = voltage;
		}
	}
}

/* get CN10 physical connection state
 * Parameters:
 * measuredVoltages: Pins voltages structure
 * boardConnectionState: board connection state
 */
void BoardConnectionController::getCn10PhysicalConnectionState(Power_GND_Test_Result_Struct * measuredVoltages, BoardConnectionStructure * boardConnectionState){
	bool isU5VNotConnected = true;
	bool isGND1Connected = true;
	bool isAGNDConnected = true;
	bool isAVDDConnected = true;
	bool isGND2Connected = true;

	if((measuredVoltages->u5vVoltage) <  maxGndVoltage){
		isU5VNotConnected = false;
	}
	if((measuredVoltages->gnd1Voltage) >  min5vVoltage){
		isGND1Connected = false;
	}
	if((measuredVoltages->agndVoltae) >  min5vVoltage){
		isAGNDConnected = false;
	}
	if((measuredVoltages->avddVoltage) < maxGndVoltage){
		isAVDDConnected = false;
	}
	if((measuredVoltages->gnd2Voltage) >  min5vVoltage){
		isGND2Connected = false;
	}

	if(isU5VNotConnected && isGND1Connected && isAGNDConnected && isAVDDConnected && isGND2Connected){
		boardConnectionState->cn10ConnectionState =  HEADER_CONNECTION_STATE::connected;
	}else if((!isU5VNotConnected) && (!isGND1Connected) && (!isAGNDConnected) && (!isAVDDConnected) && (!isGND2Connected)){
		boardConnectionState->cn10ConnectionState = HEADER_CONNECTION_STATE::notConnected;
	}else{
		boardConnectionState->cn10ConnectionState = HEADER_CONNECTION_STATE::notCorrectlyConnectedOrErros;
	}
}

/* get CN7 physical connection state
 * Parameters:
 * measuredVoltages: Pins voltages structure
 * boardConnectionState: board connection state
 */
void BoardConnectionController::getCn7PhysicalConnectionState(Power_GND_Test_Result_Struct * measuredVoltages, BoardConnectionStructure * boardConnectionState){
	bool isVDDConnected = true;
	bool isGND3Connected = true;
	bool isVBATConnected = true;
  	bool isGND4Connected = true;
 	bool isIOREFConnected = true;
 	bool is3VConnected = true;
 	bool is5VConnected = true;
 	bool isGND5Connected = true;
 	bool isGND6Connected = true;

	if((measuredVoltages->vddVoltage) <  maxGndVoltage){
		isVDDConnected = false;
	}

	if((measuredVoltages->gnd3Voltage) >  min5vVoltage){
		isGND3Connected = false;
	}

	if((measuredVoltages->vbatVoltage) < maxGndVoltage){
		isVBATConnected = false;
	}

	if((measuredVoltages->gnd4Voltage) > min5vVoltage){
		isGND4Connected = false;
	}

	if((measuredVoltages->iorefVoltage) < maxGndVoltage){
		isIOREFConnected = false;
	}

	if((measuredVoltages->_3vVoltage) < maxGndVoltage){
		is3VConnected = false;
	}

	if((measuredVoltages->_5vVoltage) < maxGndVoltage){
		is5VConnected = false;
	}

	if((measuredVoltages->gnd5Voltage) > min5vVoltage){
		isGND5Connected = false;
	}

	if((measuredVoltages->gnd6Voltage) > min5vVoltage){
		isGND6Connected = false;
	}

	if(isVDDConnected && isGND3Connected && isVBATConnected && isGND4Connected && isIOREFConnected &&
			is3VConnected && is5VConnected && isGND5Connected && isGND6Connected ){
		boardConnectionState->cn7ConnectionState =  HEADER_CONNECTION_STATE::connected;
	}else if((!isVDDConnected) && (!isGND3Connected) && (!isVBATConnected) && (!isGND4Connected) && (!isIOREFConnected) &&
			(!is3VConnected) && (!is5VConnected) && (!isGND5Connected) && (!isGND6Connected) ){
		boardConnectionState->cn7ConnectionState = HEADER_CONNECTION_STATE::notConnected;
	}else{
		boardConnectionState->cn7ConnectionState = HEADER_CONNECTION_STATE::notCorrectlyConnectedOrErros;
	}
}

/* check NUCLEO Board USB and physical connection
 * Parameter:
 * boardConnectionState: board connection state
 */
int BoardConnectionController::checkBoardUsbAndPhysicalConnectionState(BoardConnectionStructure *nucleoBoardConnectionState){
	if(nucleoBoardConnectionState->isUsbConnected){
		if (nucleoBoardConnectionState->cn10ConnectionState == HEADER_CONNECTION_STATE::connected && nucleoBoardConnectionState->cn7ConnectionState == HEADER_CONNECTION_STATE::connected){
			return NUCLEO_CONNECTION_OK;
		}else if((nucleoBoardConnectionState->cn10ConnectionState == HEADER_CONNECTION_STATE::notConnected) && (nucleoBoardConnectionState->cn7ConnectionState == HEADER_CONNECTION_STATE::notConnected)){
			return NUCLEO_USB_OK_PYHSICAL_DISCONNECTED;
		}else if((nucleoBoardConnectionState->cn10ConnectionState == HEADER_CONNECTION_STATE::connected)&&(nucleoBoardConnectionState->cn7ConnectionState == HEADER_CONNECTION_STATE::notConnected)){
			return NUCLEO_CONNECTION_CN7_ERROR;
		}else if((nucleoBoardConnectionState->cn7ConnectionState == HEADER_CONNECTION_STATE::connected)&&(nucleoBoardConnectionState->cn10ConnectionState == HEADER_CONNECTION_STATE::notConnected)){
			return NUCLEO_CONNECTION_CN10_ERROR;
		}else{
			return NUCLEO_USB_OK_PYHSICAL_NOT_CORRECTLY_CONNECTED;
		}
	}else{
		return NUCLEO_USB_DISCONNECTED;
	}
}

/* check Connection to Nucleo Board and print Result
 * Parameter:
 * boardConnectionNewState: board connection new state
 */
void BoardConnectionController::printNewConnectionState(int boardConnectionNewState){
	controller->lcd.clearLcd();
	controller->lcd.printCenteredText("Board Connection ", 1);
	if(boardConnectionNewState == NUCLEO_CONNECTION_OK){
		controller->logger->printToFileLogger("=> USB Connection Ok", true);
		controller->logger->printToFileLogger("=> Nucleo Board detected", true);
		controller->lcd.printCenteredText("Connection OK", 2);
	}else if(boardConnectionNewState == NUCLEO_USB_OK_PYHSICAL_DISCONNECTED){
		controller->logger->printToFileLogger("=> USB Connection Ok", true);
		controller->logger->printToFileLogger("=> Nucleo Board physically disconnected", true);
		controller->lcd.printCenteredText("USB Connection OK", 2);
		controller->lcd.printCenteredText("Physical Connection:",3);
		controller->lcd.printCenteredText("Disconnected",4);
	}else if(boardConnectionNewState == NUCLEO_CONNECTION_CN7_ERROR){
		controller->logger->printToFileLogger("=> USB Connection Ok", true);
		controller->logger->printToFileLogger("=> CN7 not detected or CN7 Power and GND Pins have Errors !", true);
		controller->lcd.printCenteredText("USB Connection OK", 2);
		controller->lcd.printCenteredText("CN7 Connection:",3);
		controller->lcd.printCenteredText("Error",4);
	}else if(boardConnectionNewState == NUCLEO_CONNECTION_CN10_ERROR){
		controller->logger->printToFileLogger("=> USB Connection Ok", true);
		controller->logger->printToFileLogger("=> CN10 not detected or CN10 Power and GND Pins have Errors !", true);
		controller->lcd.printCenteredText("USB Connection OK", 2);
 		controller->lcd.printCenteredText("CN10 Connection:",3);
		controller->lcd.printCenteredText("Error",4);
	}else if(boardConnectionNewState == NUCLEO_USB_OK_PYHSICAL_NOT_CORRECTLY_CONNECTED){
		controller->logger->printToFileLogger("=> USB Connection Ok", true);
		controller->logger->printToFileLogger("=> Board Not correctly connected!", true);
		controller->lcd.printCenteredText("USB Connection OK", 2);
		controller->lcd.printCenteredText("Physical Connection:",3);
		controller->lcd.printCenteredText("Error",4);
	}else{
		controller->logger->printToFileLogger("=> USB Disconnected", true);
		controller->lcd.printCenteredText("USB Disconnected", 2);
	}
}
