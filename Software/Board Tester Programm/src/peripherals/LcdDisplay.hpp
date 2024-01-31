/*
 * LcdDisplay.hpp
 *
 *  Created on: 29.05.2023
 *      Author: Astra
 */

#ifndef PERIPHERALS_LCDDISPLAY_HPP_
#define PERIPHERALS_LCDDISPLAY_HPP_

#include "../interfaces/I2cHandler.hpp"
#include "../helpers/Logger.hpp"
#include <String.h>
#include <cmath>

#define LCD_I2C_ADDRESS 0x27
#define LCD_INITAL_COMMAND1 0x33
#define LCD_INITAL_COMMAND2 0x32
#define LCD_DISPLAY_ON_CURSROR_OFF_COMMAND 0x0c // Display On Cursor Off Blinking Off
#define LCD_DISPLAY_ENTRY_MODE_COMMAND 0x06 // Cursor increment
#define LCD_DISPLAY_FUNCTIONN_SET_COMMAND 0x28//4 bits, 2 lines, 5 × 8 dots
#define LCD_COMMAND_DISPLAY_CLEAR  0x01
#define LCD_BACKLIGHT_ON 0x08  // On
#define LCD_BACKLIGHT_OFF 0x00  // On
#define LCD_DISPLAY_SELECT_LINE1 0x80
#define LCD_DISPLAY_SELECT_LINE2 0xC0
#define LCD_DISPLAY_SELECT_LINE3 0x94
#define LCD_DISPLAY_SELECT_LINE4 0xD4
#define DEGREE_CHARACTER 0xDF
#define MAX_LINE_CHARS 20
#define LCD_CHR 1 // Mode - Sending data
#define LCD_CMD 0 // Mode - Sending command
#define ENABLE 0x04 // Enable bit

class LCD_Display{

public:

	LCD_Display();
	virtual ~LCD_Display();

 	void setLoggerI2cController(Logger *_logger, I2cHandler * _i2cHandler);
 	int initialLcd();
	void printText(const char *text, int lineNumber);
	void printCenteredText(const char *text, int lineNumber);
	void printDegreeCharacter();
	void clearLcd();
	void turnLcdOff();
	void turnLcdOn();

 private:

	Logger * logger;
	I2cHandler *i2cHandler;
	int lcdBackLight;

	int sendDataToLcd(int bits, int mode);
 	int toggle_enable(int bits) ;
	void setCurrentLine(int currentLine);
	void printEmptyKey(int numberEmptyKeys);
	int sendCommandLcd(unsigned char command);

	bool isWritingLcd;
};

#endif /* PERIPHERALS_LCDDISPLAY_HPP_ */
