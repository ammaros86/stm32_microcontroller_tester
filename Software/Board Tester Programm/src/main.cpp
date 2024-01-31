//============================================================================
// Name        : Main.cpp
// Author      : Ammar Salhi
//============================================================================

#include "main.hpp"



/* Main initialization
 */
void initialize(){
	logFileName = logger.initializeLogFiles();
	logger.printToFileLogger(Strings::programmStart, true);
  	int initialAck = initializer.initializeBoardTester();
  	displayManager.printWelcomeMenu();
	if(initialAck != BOARD_INITIAL_OK){
		displayManager.printInitializationFailed();
		logger.printToFileLogger(Strings::programmStartInitialError + to_string(initialAck), false);
		controller.setNewLedsCommand(LEDS_COMMAND::setRedOn);
	}else{
		logger.printToFileLogger(Strings::programmStartInitialOk, false);
		controller.setNewLedsCommand(LEDS_COMMAND::setGreenOn);
	}
	controller.database.updateBoardControllerState(BOARD_STATE_ON);
  	testProcessManager.initial();
  	displayManager.printMenuSection();
  	buttonsManager.initializeButtonsManagement();
  	databaseCommandListener.startReadDatabaseNewCommmmand();
}

/* Exit Function
 */
void exitFunction() {
	logger.closeFiles();
    controller.lcd.turnLcdOff();
    if(controller.shutdownPi() < 0){
    	logger.detailsLogger.open(logFileName, std::ios::app);
        controller.database.updateBoardControllerState(BOARD__SET_STATE_OFF_FAILED);
        controller.logger->printToFileLogger("=> Error : Failed Turn Board Tester OFF", false);
        controller.lcd.turnLcdOn();
        displayManager.printWaitFailedSetBoardTesterOff();
    }
}

int main(int argc, char **argv){
  	initialize();
  	while(!controller.isSystemTerminateActivated());
  	exitFunction();
  	return 0;
}
