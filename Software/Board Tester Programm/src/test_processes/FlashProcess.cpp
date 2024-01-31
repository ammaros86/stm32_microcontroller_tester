/*
 * FlashProcess.cpp
 *
 *  Created on: 29.05.2023
 *      Author: Astra
 */

#include "FlashProcess.hpp"


FlashProcess::FlashProcess() {
	controller =  NULL;
}

FlashProcess:: ~FlashProcess(){

}

void FlashProcess::setController(PeripheralsInterfacesController *_controller){
	controller = _controller;
}

/* Write Test Program to the Nucleo-Board Flash Memory
 */
bool FlashProcess::flashNucleoBoard(){
	flashMemoryTestStrcture = {};
	falshFailureStructure = {};
    controller->logger->printToFileLogger("\n", true);
    controller->logger->printToFileLogger("Upload Test-Programm to Nucleo Board Flash", true);
	falshFailureStructure.uartCloseAck = controller->uartHandler.closeUart();
	falshFailureStructure.mux7Ack = controller->mulutiplexerController.setMultiplexer7Conenction(Mulitplexer7_Config::DacConnection);
	falshFailureStructure.mux6Ack = controller->mulutiplexerController.setMultiplexer6Conenction(Mux3);
	falshFailureStructure.connectionPin = controller->connectFrontMultiplexerYtoZpin(Mux3, MuxY11);
	falshFailureStructure.dacAck0V = controller->dacHandler.setDacVoltage(controller->voltageRefernces.LOW_INPUT_VOLTAGE_MAX_VALUE);
 	falshFailureStructure.dacAck3V = controller->dacHandler.setDacVoltage(FLASH_VOLTAGE);
	string ackText = writeTestProgrammToFlash();
	cout << "ACk Text: " << ackText << endl;
	falshFailureStructure.dac2Ack0V = controller->dacHandler.setDacVoltage(controller->voltageRefernces.LOW_INPUT_VOLTAGE_MAX_VALUE);
	falshFailureStructure.mux7Ack2 = controller->mulutiplexerController.setMultiplexer7Conenction(Mulitplexer7_Config::free);
	falshFailureStructure.usbRestartAck = controller->usbPortshandler.restartUsbPort();
	controller->checkUsbPortStateAndInitialUart();
 	falshFailureStructure.uartOpenAck = controller->uartHandler.getFileDescriptor();
	checkFlashHandleValues();
	if(isFlashOk(ackText)){
		string comment = getFlashInfosText(ackText);
	    controller->logger->printToFileLogger("=> Write Test-Programm success", true);
	    controller->logger->printToFileLogger(comment, true);
	    flashMemoryTestStrcture.isUploadTestProgrammOK = 1;
	    flashMemoryTestStrcture.comment = comment;
	    return true;
	}else{
	    controller->logger->printToFileLogger("=> Upload Test-Programm failed", true);
	    flashMemoryTestStrcture.isUploadTestProgrammOK = 0;
		flashMemoryTestStrcture.flash_errors = 1;
		flashMemoryTestStrcture.flash_test = 2;
 	    return false;
	}
}


int FlashProcess::testFlashMemory(){
	 controller->logger->printSectionSeperator(true);
    controller->logger->printToFileLogger("Test Flash Memory", true);
	uint32_t uartCommand = 0xFFF;
 	int ack = controller->uartHandler.sendUartData(uartCommand);
	if(ack == 2){
		delay(14500);
 	 	uint8_t * readBuffer =  controller->uartHandler.readUartData(FLASH_MEMORY_TEST_BUFFER);
	 	if(readBuffer == nullptr){
	 		controller->logger->printToFileLogger("Error:FlashProcess testFlashMemory failed to read Flash Memory Test Result over UART Data ", false);
			flashMemoryTestStrcture.flash_test = 2;
			flashMemoryTestStrcture.flash_errors = 1;
	 		return -1;
 	 	}else{
 	 		FLASH_TestResultTypeDef  *flashTestResult = reinterpret_cast<FLASH_TestResultTypeDef *>(readBuffer);
 	 		return (getFlashMemoryErrors(flashTestResult));
	 	}
	}else{
		flashMemoryTestStrcture.flash_test = 2;
		flashMemoryTestStrcture.flash_errors = 1;
		controller->logger->printToFileLogger("Error:FlashProcess testFlashMemory failed to send UART command to test Flash Memory", false);
		return -2;
	}
}

int FlashProcess::getFlashMemoryErrors(FLASH_TestResultTypeDef * flashTestResult){
	controller->logger->printToFileLogger("->Used Test Pattern: 0xA1A1A1A1", false);
	controller->logger->printToFileLogger("->Tested Memory Flash Pages: " + to_string(flashTestResult->testedPages), false);
	controller->logger->printToFileLogger("->Remove Test Pattern from: " + to_string(flashTestResult->erasedPages) + " Pages", false);
	controller->logger->printToFileLogger("->Write Test Pattern into " + to_string(flashTestResult->writedAddresses) + " 32-Bits Memory Cells", false);
	controller->logger->printToFileLogger("->Write Test Pattern Errors: " + to_string(flashTestResult->writedAddressesError), false);
	controller->logger->printToFileLogger("->Read Test Pattern from " + to_string(flashTestResult->readedAddresses) + " Memory Cells", false);
	controller->logger->printToFileLogger("->Read Test Pattern Errors: " + to_string(flashTestResult->readedAddressesError), false);
	controller->logger->printToFileLogger("->Remove Test Pattern Errors: " + to_string(flashTestResult->erasedPagesError), false);
	 controller->logger->printSectionSeperator(true);
	int flashMemoryErrors = flashTestResult->erasedPagesError + flashTestResult->readedAddressesError + flashTestResult->writedAddressesError;
	if(flashMemoryErrors > 0){
		flashMemoryTestStrcture.flash_errors = flashMemoryErrors;
		flashMemoryTestStrcture.flash_test = 1;
		 controller->logger->printToFileLogger("=> Flash Memory Test Errors " + to_string(flashMemoryErrors), true);
 	}else{
		flashMemoryTestStrcture.flash_errors = 0;
		flashMemoryTestStrcture.flash_test = 1;
		 controller->logger->printToFileLogger("=> Flash Memory Test pass", true);
 	}
	return flashMemoryErrors;
}

/* check write Flash Errors
 */
void FlashProcess::checkFlashHandleValues(){
	if(falshFailureStructure.uartCloseAck < 0){
		controller->logger->printToFileLogger("Error:FlashProcess flashNucleoBoard uartCloseAck failed to close UART Connection", false);
		flashMemoryTestStrcture.comment = "failed to close UART Connection";
	}
	if(falshFailureStructure.mux7Ack < 0){
		controller->logger->printToFileLogger("Error:FlashProcess flashNucleoBoard mux7Ack failed to select Multiplexer 7 ", false);
		flashMemoryTestStrcture.comment = "failed to select Multiplexer 7";
	}
	if(falshFailureStructure.dacAck0V < 0){
		controller->logger->printToFileLogger("Error:FlashProcess flashNucleoBoard dacAck0V failed to set DAC Voltage 0V", false);
		flashMemoryTestStrcture.comment = "failed to set DAC Voltage 0V";
	}
	if(falshFailureStructure.dacAck3V < 0){
		controller->logger->printToFileLogger("Error:FlashProcess flashNucleoBoard dacAck3V failed to set DAC Voltage 3V", false);
		flashMemoryTestStrcture.comment = "failed to set DAC Voltage 3V";
	}
	if(falshFailureStructure.mux6Ack < 0){
		controller->logger->printToFileLogger("Error:FlashProcess flashNucleoBoard mux6Ack failed to select Multiplexer 6 ", false);
		flashMemoryTestStrcture.comment = "failed to select Multiplexer 6 ";
	}
	if(falshFailureStructure.dac2Ack0V < 0){
		controller->logger->printToFileLogger("Error:FlashProcess flashNucleoBoard dac2Ack0V failed to set DAC Voltage 0V", false);
		flashMemoryTestStrcture.comment = "failed to set DAC Voltage 0V";
	}
	if(falshFailureStructure.mux7Ack2 < 0){
		controller->logger->printToFileLogger("Error:FlashProcess flashNucleoBoard mux7Ack2 failed to select Multiplexer 7 ", false);
		flashMemoryTestStrcture.comment = "failed to select Multiplexer 7";
	}
	if(falshFailureStructure.usbRestartAck < 0){
		controller->logger->printToFileLogger("Error:FlashProcess flashNucleoBoard usbRestartAck failed to restart USB Connection", false);
		flashMemoryTestStrcture.comment = "failed to restart USB Connection";
	}
	if(falshFailureStructure.uartOpenAck < 0){
		controller->logger->printToFileLogger("Error:FlashProcess flashNucleoBoard uartOpenAck failed to open UART Connection", false);
		flashMemoryTestStrcture.comment = "failed to open UART Connection";
	}
	if(falshFailureStructure.connectionPin < 0){
		if(falshFailureStructure.connectionPin == MUX5_CONTROLL_ERROR){
			controller->logger->printToFileLogger("Error:FlashProcess flashNucleoBoard failed to select Multiplexer 5", false);
			flashMemoryTestStrcture.comment = "failed to select Multiplexer 5";
		}
		if(falshFailureStructure.connectionPin == UNKNOWN_CONNECTED_MULTIPLEXER_TO_NUCLEO_PIN){
			controller->logger->printToFileLogger("Error:FlashProcess flashNucleoBoard failed Unknown Multiplexer", false);
			flashMemoryTestStrcture.comment = "failed Unknown Multiplexer";
		}
		if(falshFailureStructure.connectionPin == UNKNOWN_IO_EXPANDER_COMMAND){
			controller->logger->printToFileLogger("Error:FlashProcess flashNucleoBoard failed unknown I/O Expander Command", false);
			flashMemoryTestStrcture.comment = "failed unknown I/O Expander Command";
		}
		if(falshFailureStructure.connectionPin == IO_EXPNADER_WRITE_ERROR){
			controller->logger->printToFileLogger("Error:FlashProcess flashNucleoBoard failed to write I/O Expander Command", false);
			flashMemoryTestStrcture.comment = "failed to write I/O Expander Command";
		}
	}
}



/* Flash NUCLEO Board with the Test Program
 */
string FlashProcess::writeTestProgrammToFlash(){
 	string flashData;
	if(controller->usbPortshandler.restartUsbPort() == RESTART_USB_PORT_DONE){
		int activUsbSerialPort = Helper::getActivUsbSerialPort();
		if(activUsbSerialPort == USB_SERIAL_PORT_TTYACM0_ACK){
			flashData =  Helper::executeCommandAndGetOutput(getFlashCommandInterface1);
		}else if(activUsbSerialPort == USB_SERIAL_PORT_TTYACM1_ACK){
			flashData =  Helper::executeCommandAndGetOutput(getFlashCommandInterface2);
		}else{
			controller->logger->printToFileLogger(Strings::flashErrorUsbPort, false);
 		}
		if(flashData.find(flashDoneText) != string::npos) {
 			return flashData;
		}else{
			controller->logger->printToFileLogger(Strings::flashWriteError, false);
 			return flashData;
		}
	}else{
 		return Strings::flashFailedText;
	}
}

/* return Flash Information Text after Flash finish
 * Parameter:
 * falshAck: Flash Text
 */
std::string FlashProcess::getFlashInfosText(string falshAck){
	if((falshAck.find(flashVersionText) != string::npos) && (falshAck.find(flashWriteText)!= string::npos)){
		string text = falshAck.substr(falshAck.find(flashVersionText));
		size_t pos = text.find(flashWriteText);
		return text.substr(0, pos);
	}else{
 		return Strings::flashFailedToGetAckText;
	}
}

/* check flash Test-Programm written successful
 * Parameter:
 * flashAck: Flash Text
 */
bool FlashProcess::isFlashOk(string flashAck){
 	if (flashAck.find(flashDoneText) != string::npos) {
		return true;
	}else{
		return false;
	}
}
