/*
 * LcdDisplay.cpp
 *
 *  Created on: 29.05.2023
 *      Author: Astra
 */

#include "../peripherals/LcdDisplay.hpp"

LCD_Display::LCD_Display(){
 	lcdBackLight = LCD_BACKLIGHT_ON;
	logger = NULL;
	i2cHandler = NULL;
	isWritingLcd = false;
}

LCD_Display::~LCD_Display() {
	i2cHandler->closeI2cConnection();
	delete (logger);
	delete (i2cHandler);
}


/* set LCD Log and I2C Interface
 * Parameter:
 * _logger: logger object
 * _i2cController: I2C Interface
 */
void LCD_Display::setLoggerI2cController(Logger *_logger, I2cHandler * _i2cHandler){
	logger = _logger;
	i2cHandler = _i2cHandler;
}

/* initialize LCD Display
 */
int LCD_Display::initialLcd(){
	if(sendCommandLcd(LCD_INITAL_COMMAND1) < 0){
		logger->printToFileLogger("Error:LCD_Display initLCD: failed to write LCD_INITAL_COMMAND1", false);
		return -1;
	}
	if(sendCommandLcd(LCD_INITAL_COMMAND2) < 0){
		logger->printToFileLogger("Error:LCD_Display initLCD: failed to write LCD_INITAL_COMMAND2", false);
		return -1;
	}
	if(sendCommandLcd(LCD_DISPLAY_ON_CURSROR_OFF_COMMAND) < 0){
		logger->printToFileLogger("Error:LCD_Display initLCD: failed to write LCD_DISPLAY_ON_CURSROR_OFF_COMMAND", false);
		return -1;
	}
	if(sendCommandLcd(LCD_DISPLAY_ENTRY_MODE_COMMAND) < 0){
		logger->printToFileLogger("Error:LCD_Display initLCD: failed to write LCD_DISPLAY_ON_CURSROR_OFF_COMMAND", false);
		return -1;
	}
	if(sendCommandLcd(LCD_DISPLAY_FUNCTIONN_SET_COMMAND) < 0){
		logger->printToFileLogger("Error: LCD_Display initLCD: failed to write DISPLAY CLEAR COMMAND", false);
		return -1;
	}
	if(sendCommandLcd(LCD_COMMAND_DISPLAY_CLEAR) < 0){
		logger->printToFileLogger("Error: LCD_Display initLCD: failed to write DISPLAY CLEAR COMMAND", false);
		return -1;
	}
	delayMicroseconds(_500MS_DELAY);
	return 1;
}

/* send Data to LCD Display
 * Parameter:
 * bits: Data
 * mode: write mode
 */
int LCD_Display::sendDataToLcd(int bits, int mode){
	uint8_t high_bits = mode | (bits & 0xF0)  | lcdBackLight;
	uint8_t low_bits = mode | ((bits << 4) & 0xF0) | lcdBackLight ;
	int ackHighBits = i2cHandler->writeToI2cSlave(high_bits) ;
	int ackFirstEnable = toggle_enable(high_bits);
	int ackLowBits = i2cHandler->writeToI2cSlave(low_bits) ;
	int ackSecondEnable = toggle_enable(low_bits);
	if((ackHighBits < 0) || (ackFirstEnable < 0) || (ackLowBits < 0) || (ackSecondEnable < 0)){
		logger->printToFileLogger("Error:LCD_Display sendDataLcd: failed to send Bits:  " + to_string(bits) + " Mode: " + to_string(mode) , false);
		return -1;
	}else{
		return 1;
	}
}

/* toggle LCD Display Enable
 * Parameter:
 * bits: Data
 */
int LCD_Display::toggle_enable(int bits){
  	int ackEnable = i2cHandler->writeToI2cSlave( (bits | ENABLE)) ;
	delayMicroseconds(500);
	int ackInvertEnable = i2cHandler->writeToI2cSlave((bits & ~ENABLE)) ;
	delayMicroseconds(500);
	if((ackEnable < 0) || (ackInvertEnable < 0)){
		logger->printToFileLogger("Error:LCD_Display lcd_toggle_enable: failed to toggle LCD enable", false);
		return -1;
	}else{
		return 1;
	}
}

/* set LCD Display current line
 * Parameter:
 * currentLine: line number
 */
void LCD_Display::setCurrentLine(int currentLine){
	switch(currentLine){
		case 1:
			sendCommandLcd(LCD_DISPLAY_SELECT_LINE1);
			break;
		case 2:
			sendCommandLcd(LCD_DISPLAY_SELECT_LINE2);
			break;
		case 3:
			sendCommandLcd(LCD_DISPLAY_SELECT_LINE3);
			break;
		case 4:
			sendCommandLcd(LCD_DISPLAY_SELECT_LINE4);
			break;
		default:
 			logger->printToFileLogger("Error:LCD_Display setCurrentLine: unknown line number : " + to_string(currentLine), false);
			break;
	}
}

/* send LCD Display Command
 * Parameter:
 * command: Command
 */
int LCD_Display::sendCommandLcd(unsigned char command){
	return sendDataToLcd(command, LCD_CMD);
}

/* print Text on LCD Display
 * Parameter:
 * text: Text
 * lineNumber: line number
 */
void LCD_Display::printText(const char *text, int lineNumber){
 	setCurrentLine(lineNumber);
	while (*text){
		sendDataToLcd(*(text++), LCD_CHR);
 	}
}

/* print Text center on LCD Display
 * Parameter:
 * text: Text
 * lineNumber: line number
 */
void LCD_Display::printCenteredText(const char *text, int lineNumber){
	if(!isWritingLcd){
		isWritingLcd = true;
	 	cout << text << endl;
		int emptyKeys = 0;
		setCurrentLine(lineNumber);
		emptyKeys = (MAX_LINE_CHARS - strlen(text)) / 2;
		if(MAX_LINE_CHARS -  strlen(text) %2 != 0){
			emptyKeys = std::ceil(emptyKeys);
			printEmptyKey(emptyKeys);
		}
		while (*text){
			sendDataToLcd(*(text++), LCD_CHR);
		}
		isWritingLcd = false;
	}

}

/* print empty Key on LCD Display
 * Parameter:
 * numberEmptyKeys: Number of empty Keys
 */
void LCD_Display::printEmptyKey(int numberEmptyKeys){
	char emptyKey = ' ';
	for (int i = 0; i < numberEmptyKeys; i++){
		sendDataToLcd(emptyKey, LCD_CHR);
	}
}

/* clear LCD Display
 */
void LCD_Display::LCD_Display::clearLcd(){
	sendCommandLcd(LCD_COMMAND_DISPLAY_CLEAR);
}

/* turn LCD Display back light off
 */
void LCD_Display::turnLcdOff(){
	lcdBackLight = LCD_BACKLIGHT_OFF;
	sendCommandLcd(LCD_BACKLIGHT_OFF);
}

/* turn LCD Display back light off
 */
void LCD_Display::turnLcdOn(){
	lcdBackLight = ~LCD_BACKLIGHT_OFF;
	sendCommandLcd(LCD_BACKLIGHT_OFF);
}
/* print "°" character
 */
void LCD_Display::printDegreeCharacter(){
 	sendDataToLcd(DEGREE_CHARACTER, LCD_CHR);
	sendDataToLcd('C', LCD_CHR);
}

