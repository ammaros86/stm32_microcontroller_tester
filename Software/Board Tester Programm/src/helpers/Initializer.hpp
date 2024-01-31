/*
 * Initializer.hpp
 *
 *  Created on: 30.08.2023
 *      Author: Astra
 */

#ifndef HELPERS_INITIALIZER_HPP_
#define HELPERS_INITIALIZER_HPP_

#include "../controllers/PeripheralsInterfacesController.hpp"
#include "../interfaces/I2cHandler.hpp"
#include "../interfaces/SpiHandler.hpp"

#define MCP23017Address 0x20
#define LCD_DISPLAY_ADDRESS 0x27
#define SPI_CLOCK_MCP3202 1800000    // 1.8 MHz
#define SPI_CLOCK_MCP4921_CH 2000000      // 20 MHz

class Initializer{

public:

	Initializer();
 	virtual ~Initializer();
 	Initializer(PeripheralsInterfacesController *_peripheralsController): peripheralsController(_peripheralsController){};

	int initializeBoardTester();
   	void initializeTestVoltagesValues();
	int initialGPIOs();
	int initialAdc();
	int initialDac();
	int uartInitialiser();
	int initialIOExpander();
	int initialCurrentSensor();
	int initialLcdDisplay();
 	void initializeGpiosTestVectors();
 	void initialPowerGndPinsTestVectors();

 private:

	PeripheralsInterfacesController *peripheralsController;

 	const char * spiChannel0 = "/dev/spidev0.0";
 	const char * spiChannel1 = "/dev/spidev0.1";
};


#endif /* HELPERS_INITIALIZER_HPP_ */
