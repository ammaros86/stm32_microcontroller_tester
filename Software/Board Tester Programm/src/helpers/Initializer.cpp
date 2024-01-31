/*
 * Initializer.cpp
 *
 *  Created on: 30.08.2023
 *      Author: Astra
 */


#include "Initializer.hpp"


Initializer::~Initializer(){
}

/* initialize Board Tester
 */
int Initializer::initializeBoardTester(){
	int initialErrors = BOARD_INITIAL_OK;
   	peripheralsController->database.initialDataBaseController(peripheralsController->logger);
   	initializeTestVoltagesValues();
  	initializeGpiosTestVectors();
  	initialPowerGndPinsTestVectors();
   	if(initialGPIOs() < 0){
   		initialErrors++;
		peripheralsController->logger->printToFileLogger("Error Initializer:initializeBoardTester cause failed to initialize GPIOs", false);
	}
	if(initialAdc() < 0){
		initialErrors++;
		peripheralsController->logger->printToFileLogger("Error Initializer:initializeBoardTester cause failed to initialize ADC", false);
	}
	if(initialDac() < 0){
		initialErrors++;
		peripheralsController->logger->printToFileLogger("Error Initializer:initializeBoardTester cause failed to initialize DAC", false);
	}
	if(uartInitialiser() <= 0){
		initialErrors++;
		peripheralsController->logger->printToFileLogger("Error Initializer:initializeBoardTester cause failed to initialize UART", false);
	}
	if(initialLcdDisplay() < 0){
		initialErrors++;
		peripheralsController->logger->printToFileLogger("Error Initializer:initializeBoardTester cause failed to initialize LCD Display", false);
	}
	if(initialIOExpander() < 0){
		initialErrors++;
		peripheralsController->logger->printToFileLogger("Error Initializer:initializeBoardTester cause failed to initialize I/O Expander", false);
	}
	if(!peripheralsController->mulutiplexerController.initialControllMultiplexers(peripheralsController->logger)){
		initialErrors++;
		peripheralsController->logger->printToFileLogger("Error Initializer:initializeBoardTester cause failed to initialize Multiplexers", false);
	}
 	initialCurrentSensor();
  	return initialErrors;
}

/* initialize Test Voltage Values
 */
void Initializer::initializeTestVoltagesValues(){
	peripheralsController->voltageRefernces.POWER_VOLTAGE_5_MIN = 4.9;
	peripheralsController->voltageRefernces.POWER_VOLTAGE_5_MAX = 5.1;
	peripheralsController->voltageRefernces.GND_VOLTAGE_MIN = 0;
	peripheralsController->voltageRefernces.GND_VOLTAGE_MAX = 0.1;
	peripheralsController->voltageRefernces.POWER_VOLTAGE_3V3_MIN = 2.7;
	peripheralsController->voltageRefernces.POWER_VOLTAGE_3V3_MAX = 3.6;
	peripheralsController->voltageRefernces.ADC_REFERENCE_VOLATGE = 5;
	peripheralsController->voltageRefernces.DAC_REFERENCE_VOLATGE = 5;
	peripheralsController->voltageRefernces.POWER_VOLTAGE_E5V_MIN = 0;
	peripheralsController->voltageRefernces.POWER_VOLTAGE_E5V_MAX = 0.5;
	peripheralsController->voltageRefernces.ANALOG_NULL_VOLTAGE_VALUE = 0;
	peripheralsController->voltageRefernces.ANALOG_MAX_VOLTAGE_VALUE = 3.3;
}


/* initialize Raspberry Pi GPIOs
 */
int Initializer::initialGPIOs(){
	if(wiringPiSetup() == -1){
 		return -1;
	}
	if(wiringPiSetupGpio() == -1){
 		return -1;
	}

	pinMode(Yellow_Button_PIN, INPUT);
	pinMode(BLUE_Button_PIN, INPUT);
	pinMode(RED_Button_PIN, INPUT);
	pinMode(GREEN_Button_PIN, INPUT);

	pullUpDnControl(Yellow_Button_PIN, PUD_OFF);
	pullUpDnControl(BLUE_Button_PIN, PUD_OFF);
	pullUpDnControl(RED_Button_PIN, PUD_OFF);
	pullUpDnControl(GREEN_Button_PIN, PUD_OFF);

	pinMode(LED_GREEN_PI_PIN, OUTPUT);
	pinMode(LED_RED_PI_PIN, OUTPUT);
	digitalWrite(LED_GREEN_PI_PIN, LOW);
	digitalWrite(LED_RED_PI_PIN, LOW);

	pinMode(S0_MUX5, OUTPUT);
	pinMode(S1_MUX5, OUTPUT);
	pinMode(S2_MUX5, OUTPUT);

	pinMode(S0_MUX6, OUTPUT);
	pinMode(S1_MUX6, OUTPUT);
	pinMode(S2_MUX6, OUTPUT);

 	pinMode(S1_MUX7, OUTPUT);
	pinMode(S2_MUX7, OUTPUT);
	pinMode(S3_MUX7, OUTPUT);

	pinMode(CS0, OUTPUT);
	pinMode(CS1, OUTPUT);
	pullUpDnControl(CS0, PUD_OFF);
	pullUpDnControl(CS0, PUD_OFF);
	return 1;
}

/* initialize ADC
 */
int Initializer::initialAdc(){
	SpiHandler * spiHandler = new SpiHandler();
  	if(spiHandler->initialSpiInterface(spiChannel1, SPI_CLOCK_MCP3202) <= 0 ){
 		return -1;
	}
  	peripheralsController->adcHandler.initialAdc(spiHandler, peripheralsController->voltageRefernces.ADC_REFERENCE_VOLATGE);
	return 1;
}

/* initialize DAC
 */
int Initializer::initialDac(){
	SpiHandler * spiHandler = new SpiHandler();
	if(spiHandler->initialSpiInterface(spiChannel0, SPI_CLOCK_MCP4921_CH) <= 0 ){
	 		return -1;
	}
	peripheralsController->dacHandler.initialDac(spiHandler, peripheralsController->voltageRefernces.DAC_REFERENCE_VOLATGE);
 	peripheralsController->dacHandler.setDacVoltage(0);
 	return 1;
}

/* initialize UART Handler
 */
int Initializer::uartInitialiser(){
	 return peripheralsController->uartHandler.initialUart();
}

/* initialize I/O Expander
 */
int Initializer::initialIOExpander(){
	I2cHandler * i2cHandler = new I2cHandler();
	int initialAck = i2cHandler->initialI2cInterface(MCP23017Address);
	if(initialAck < 0){
		peripheralsController->logger->printToFileLogger("Error:I2cController initialI2cInterface Current Sensor failed", false);
 	}
    peripheralsController->ioExpander.initialIoExpander(peripheralsController->logger, i2cHandler);
    return 1;
}

/* initialize Current Sensor
 */
int Initializer::initialCurrentSensor(){
	I2cHandler * i2cHandler = new I2cHandler();
	int initialAck = i2cHandler->initialI2cInterface(CURRENT_SENSOR_ADDRESS);
	if(initialAck < 0){
		peripheralsController->logger->printToFileLogger("Error:I2cController initialI2cInterface Current Sensor failed", false);
	}
	peripheralsController->currentSensor.initialCurrentSensor(peripheralsController->logger, i2cHandler);
	peripheralsController->currentSensor.resetCurrentSensor();
	peripheralsController->currentSensor.calibrateCurrentSensor();
    peripheralsController->currentSensor.configureCurrentSensor();
	return 1;
}

/* initialize LCD-Display
 */
int Initializer::initialLcdDisplay(){
	I2cHandler * i2cHandler = new I2cHandler();
	int initialAck = i2cHandler->initialI2cInterface(LCD_DISPLAY_ADDRESS);
	if(initialAck < 0){
		peripheralsController->logger->printToFileLogger("Error:I2cController initialI2cInterface Current Sensor failed", false);
	}
	peripheralsController->lcd.setLoggerI2cController(peripheralsController->logger, i2cHandler);
	int ack = peripheralsController->lcd.initialLcd();
	return ack;
}

/* initialize GPIOs Vectors Structure
 */
void Initializer::initializeGpiosTestVectors(){
	peripheralsController->speed.clear();
	peripheralsController->pullTypes.clear();
	peripheralsController->board_gpio_pins_vector.clear();
	peripheralsController->multiplexer_pin.clear();
	peripheralsController->board_gpio_ports_vector.clear();
	peripheralsController->connected_multiplexer_gpio.clear();

	peripheralsController->board_analog_pins_vector.clear();
	peripheralsController->analog_ports_vector.clear();
	peripheralsController->analog_multiplexer_pins.clear();
	peripheralsController->connected_multiplexer_analog.clear();

	peripheralsController->pullTypes.push_back(COMMAND_Init_NO_PULL);
	peripheralsController->pullTypes.push_back(COMMAND_Init_PULL_UP);
	peripheralsController->pullTypes.push_back(COMMAND_Init_PULL_DOWN);

	peripheralsController->speed.push_back(COMMAND_Init_GPIO_SPEED_FREQ_LOW);
	peripheralsController->speed.push_back(COMMAND_Init_GPIO_SPEED_FREQ_MEDIUM);
	peripheralsController->speed.push_back(COMMAND_Init_GPIO_SPEED_FREQ_HIGH);


	peripheralsController->board_gpio_pins_vector.push_back(GPIO_Pins::PC8);
	peripheralsController->multiplexer_pin.push_back(MUTLIPLEXER_PINS::Y7); //Y7 MUX1
	peripheralsController->board_gpio_ports_vector.push_back(GPIO_PINS_PORTS::portC);
	peripheralsController->connected_multiplexer_gpio.push_back(Mux1);

	peripheralsController->board_gpio_pins_vector.push_back(GPIO_Pins::PC6);
	peripheralsController->multiplexer_pin.push_back(MUTLIPLEXER_PINS::Y6); //Y6 MUX1
	peripheralsController->board_gpio_ports_vector.push_back(GPIO_PINS_PORTS::portC);
	peripheralsController->connected_multiplexer_gpio.push_back(Mux1);

	peripheralsController->board_gpio_pins_vector.push_back(GPIO_Pins::PC5);  //Y5 MUX1
	peripheralsController->multiplexer_pin.push_back(MUTLIPLEXER_PINS::Y5);
	peripheralsController->board_gpio_ports_vector.push_back(GPIO_PINS_PORTS::portC);
	peripheralsController->connected_multiplexer_gpio.push_back(Mux1);

	peripheralsController->board_gpio_pins_vector.push_back(GPIO_Pins::PA12);  //Y3 MUX1
	peripheralsController->multiplexer_pin.push_back(MUTLIPLEXER_PINS::Y3);
	peripheralsController->board_gpio_ports_vector.push_back(GPIO_PINS_PORTS::portA);
	peripheralsController->connected_multiplexer_gpio.push_back(Mux1);

	peripheralsController->board_gpio_pins_vector.push_back(GPIO_Pins::PA11); //Y2 MUX1
	peripheralsController->multiplexer_pin.push_back(MUTLIPLEXER_PINS::Y2);
	peripheralsController->board_gpio_ports_vector.push_back(GPIO_PINS_PORTS::portA);
	peripheralsController->connected_multiplexer_gpio.push_back(Mux1);

	peripheralsController->board_gpio_pins_vector.push_back(GPIO_Pins::PB12); //Y1 MUX1
	peripheralsController->multiplexer_pin.push_back(MUTLIPLEXER_PINS::Y1);
	peripheralsController->board_gpio_ports_vector.push_back(GPIO_PINS_PORTS::portB);
	peripheralsController->connected_multiplexer_gpio.push_back(Mux1);

	peripheralsController->board_gpio_pins_vector.push_back(GPIO_Pins::PB11); //Y0 MUX1
	peripheralsController->multiplexer_pin.push_back(MUTLIPLEXER_PINS::Y0);
	peripheralsController->board_gpio_ports_vector.push_back(GPIO_PINS_PORTS::portB);
	peripheralsController->connected_multiplexer_gpio.push_back(Mux1);

	peripheralsController->board_gpio_pins_vector.push_back(GPIO_Pins::PB2);
	peripheralsController->multiplexer_pin.push_back(MUTLIPLEXER_PINS::Y14);
	peripheralsController->board_gpio_ports_vector.push_back(GPIO_PINS_PORTS::portB);
	peripheralsController->connected_multiplexer_gpio.push_back(Mux1);

	peripheralsController->board_gpio_pins_vector.push_back(GPIO_Pins::PB1);
	peripheralsController->multiplexer_pin.push_back(MUTLIPLEXER_PINS::Y13);
	peripheralsController->board_gpio_ports_vector.push_back(GPIO_PINS_PORTS::portB);
	peripheralsController->connected_multiplexer_gpio.push_back(Mux1);

	peripheralsController->board_gpio_pins_vector.push_back(GPIO_Pins::PB15);
	peripheralsController->multiplexer_pin.push_back(MUTLIPLEXER_PINS::Y12);
	peripheralsController->board_gpio_ports_vector.push_back(GPIO_PINS_PORTS::portB);
	peripheralsController->connected_multiplexer_gpio.push_back(Mux1);

	peripheralsController->board_gpio_pins_vector.push_back(GPIO_Pins::PB14);
	peripheralsController->multiplexer_pin.push_back(MUTLIPLEXER_PINS::Y11);
	peripheralsController->board_gpio_ports_vector.push_back(GPIO_PINS_PORTS::portB);
	peripheralsController->connected_multiplexer_gpio.push_back(Mux1);

	peripheralsController->board_gpio_pins_vector.push_back(GPIO_Pins::PB13);
	peripheralsController->multiplexer_pin.push_back(MUTLIPLEXER_PINS::Y10);
	peripheralsController->board_gpio_ports_vector.push_back(GPIO_PINS_PORTS::portB);
	peripheralsController->connected_multiplexer_gpio.push_back(Mux1);

	peripheralsController->board_gpio_pins_vector.push_back(GPIO_Pins::PC4);
	peripheralsController->multiplexer_pin.push_back(MUTLIPLEXER_PINS::Y8);
	peripheralsController->board_gpio_ports_vector.push_back(GPIO_PINS_PORTS::portC);
	peripheralsController->connected_multiplexer_gpio.push_back(Mux1);

	peripheralsController->board_gpio_pins_vector.push_back(GPIO_Pins::PA3);
	peripheralsController->multiplexer_pin.push_back(MUTLIPLEXER_PINS::Y7);
	peripheralsController->board_gpio_ports_vector.push_back(GPIO_PINS_PORTS::portA);
	peripheralsController->connected_multiplexer_gpio.push_back(Mux3);

	peripheralsController->board_gpio_pins_vector.push_back(GPIO_Pins::PA2);
	peripheralsController->multiplexer_pin.push_back(MUTLIPLEXER_PINS::Y6);
	peripheralsController->board_gpio_ports_vector.push_back(GPIO_PINS_PORTS::portA);
	peripheralsController->connected_multiplexer_gpio.push_back(Mux3);

	peripheralsController->board_gpio_pins_vector.push_back(GPIO_Pins::PA10);
	peripheralsController->multiplexer_pin.push_back(MUTLIPLEXER_PINS::Y5);
	peripheralsController->board_gpio_ports_vector.push_back(GPIO_PINS_PORTS::portA);
	peripheralsController->connected_multiplexer_gpio.push_back(Mux3);

	peripheralsController->board_gpio_pins_vector.push_back(GPIO_Pins::PB3);
	peripheralsController->multiplexer_pin.push_back(MUTLIPLEXER_PINS::Y7);
	peripheralsController->board_gpio_ports_vector.push_back(GPIO_PINS_PORTS::portB);
	peripheralsController->connected_multiplexer_gpio.push_back(Mux2);

	peripheralsController->board_gpio_pins_vector.push_back(GPIO_Pins::PB5);
	peripheralsController->multiplexer_pin.push_back(MUTLIPLEXER_PINS::Y6);
	peripheralsController->board_gpio_ports_vector.push_back(GPIO_PINS_PORTS::portB);
	peripheralsController->connected_multiplexer_gpio.push_back(Mux2);

	peripheralsController->board_gpio_pins_vector.push_back(GPIO_Pins::PB4);
	peripheralsController->multiplexer_pin.push_back(MUTLIPLEXER_PINS::Y5);
	peripheralsController->board_gpio_ports_vector.push_back(GPIO_PINS_PORTS::portB);
	peripheralsController->connected_multiplexer_gpio.push_back(Mux2);

	peripheralsController->board_gpio_pins_vector.push_back(GPIO_Pins::PB10);
	peripheralsController->multiplexer_pin.push_back(MUTLIPLEXER_PINS::Y4);
	peripheralsController->board_gpio_ports_vector.push_back(GPIO_PINS_PORTS::portB);
	peripheralsController->connected_multiplexer_gpio.push_back(Mux2);

	peripheralsController->board_gpio_pins_vector.push_back(GPIO_Pins::PA8);
	peripheralsController->multiplexer_pin.push_back(MUTLIPLEXER_PINS::Y3);
	peripheralsController->board_gpio_ports_vector.push_back(GPIO_PINS_PORTS::portA);
	peripheralsController->connected_multiplexer_gpio.push_back(Mux2);

	peripheralsController->board_gpio_pins_vector.push_back(GPIO_Pins::PA9);
	peripheralsController->multiplexer_pin.push_back(MUTLIPLEXER_PINS::Y2);
	peripheralsController->board_gpio_ports_vector.push_back(GPIO_PINS_PORTS::portA);
	peripheralsController->connected_multiplexer_gpio.push_back(Mux2);

	peripheralsController->board_gpio_pins_vector.push_back(GPIO_Pins::PC7);
	peripheralsController->multiplexer_pin.push_back(MUTLIPLEXER_PINS::Y1);
	peripheralsController->board_gpio_ports_vector.push_back(GPIO_PINS_PORTS::portC);
	peripheralsController->connected_multiplexer_gpio.push_back(Mux2);

	peripheralsController->board_gpio_pins_vector.push_back(GPIO_Pins::PB6);
	peripheralsController->multiplexer_pin.push_back(MUTLIPLEXER_PINS::Y0);
	peripheralsController->board_gpio_ports_vector.push_back(GPIO_PINS_PORTS::portB);
	peripheralsController->connected_multiplexer_gpio.push_back(Mux2);

	peripheralsController->board_gpio_pins_vector.push_back(GPIO_Pins::PA7);
	peripheralsController->multiplexer_pin.push_back(MUTLIPLEXER_PINS::Y15);
	peripheralsController->board_gpio_ports_vector.push_back(GPIO_PINS_PORTS::portA);
	peripheralsController->connected_multiplexer_gpio.push_back(Mux2);

	peripheralsController->board_gpio_pins_vector.push_back(GPIO_Pins::PA6);
	peripheralsController->multiplexer_pin.push_back(MUTLIPLEXER_PINS::Y14);
	peripheralsController->board_gpio_ports_vector.push_back(GPIO_PINS_PORTS::portA);
	peripheralsController->connected_multiplexer_gpio.push_back(Mux2);

	peripheralsController->board_gpio_pins_vector.push_back(GPIO_Pins::PA5);
	peripheralsController->multiplexer_pin.push_back(MUTLIPLEXER_PINS::Y13);
	peripheralsController->board_gpio_ports_vector.push_back(GPIO_PINS_PORTS::portA);
	peripheralsController->connected_multiplexer_gpio.push_back(Mux2);

	peripheralsController->board_gpio_pins_vector.push_back(GPIO_Pins::PB9);
	peripheralsController->multiplexer_pin.push_back(MUTLIPLEXER_PINS::Y10);
	peripheralsController->board_gpio_ports_vector.push_back(GPIO_PINS_PORTS::portB);
	peripheralsController->connected_multiplexer_gpio.push_back(Mux2);

	peripheralsController->board_gpio_pins_vector.push_back(GPIO_Pins::PB8);
	peripheralsController->multiplexer_pin.push_back(MUTLIPLEXER_PINS::Y9);
	peripheralsController->board_gpio_ports_vector.push_back(GPIO_PINS_PORTS::portB);
	peripheralsController->connected_multiplexer_gpio.push_back(Mux2);

	peripheralsController->board_gpio_pins_vector.push_back(GPIO_Pins::PC9);
	peripheralsController->multiplexer_pin.push_back(MUTLIPLEXER_PINS::Y8);
	peripheralsController->board_gpio_ports_vector.push_back(GPIO_PINS_PORTS::portC);
	peripheralsController->connected_multiplexer_gpio.push_back(Mux2);

	peripheralsController->board_gpio_pins_vector.push_back(GPIO_Pins::PC11);
	peripheralsController->multiplexer_pin.push_back(MUTLIPLEXER_PINS::Y2);
	peripheralsController->board_gpio_ports_vector.push_back(GPIO_PINS_PORTS::portC);
	peripheralsController->connected_multiplexer_gpio.push_back(Mux5);

	peripheralsController->board_gpio_pins_vector.push_back(GPIO_Pins::PD2);
	peripheralsController->multiplexer_pin.push_back(MUTLIPLEXER_PINS::Y1);
	peripheralsController->board_gpio_ports_vector.push_back(GPIO_PINS_PORTS::portD);
	peripheralsController->connected_multiplexer_gpio.push_back(Mux5);

	peripheralsController->board_gpio_pins_vector.push_back(GPIO_Pins::PA0);
	peripheralsController->multiplexer_pin.push_back(MUTLIPLEXER_PINS::Y9);
	peripheralsController->board_gpio_ports_vector.push_back(GPIO_PINS_PORTS::portA);
	peripheralsController->connected_multiplexer_gpio.push_back(Mux4);

	peripheralsController->board_gpio_pins_vector.push_back(GPIO_Pins::PA1);
	peripheralsController->multiplexer_pin.push_back(MUTLIPLEXER_PINS::Y8);
	peripheralsController->board_gpio_ports_vector.push_back(GPIO_PINS_PORTS::portA);
	peripheralsController->connected_multiplexer_gpio.push_back(Mux4);

	peripheralsController->board_gpio_pins_vector.push_back(GPIO_Pins::PA4);
	peripheralsController->multiplexer_pin.push_back(MUTLIPLEXER_PINS::Y7);
	peripheralsController->board_gpio_ports_vector.push_back(GPIO_PINS_PORTS::portA);
	peripheralsController->connected_multiplexer_gpio.push_back(Mux4);

	peripheralsController->board_gpio_pins_vector.push_back(GPIO_Pins::PB0);
	peripheralsController->multiplexer_pin.push_back(MUTLIPLEXER_PINS::Y6);
	peripheralsController->board_gpio_ports_vector.push_back(GPIO_PINS_PORTS::portB);
	peripheralsController->connected_multiplexer_gpio.push_back(Mux4);

	peripheralsController->board_gpio_pins_vector.push_back(GPIO_Pins::PC1);
	peripheralsController->multiplexer_pin.push_back(MUTLIPLEXER_PINS::Y5);
	peripheralsController->board_gpio_ports_vector.push_back(GPIO_PINS_PORTS::portC);
	peripheralsController->connected_multiplexer_gpio.push_back(Mux4);

	peripheralsController->board_gpio_pins_vector.push_back(GPIO_Pins::PC0);
	peripheralsController->multiplexer_pin.push_back(MUTLIPLEXER_PINS::Y4);
	peripheralsController->board_gpio_ports_vector.push_back(GPIO_PINS_PORTS::portC);
	peripheralsController->connected_multiplexer_gpio.push_back(Mux4);

	peripheralsController->board_gpio_pins_vector.push_back(GPIO_Pins::PC3);
	peripheralsController->multiplexer_pin.push_back(MUTLIPLEXER_PINS::Y3);
	peripheralsController->board_gpio_ports_vector.push_back(GPIO_PINS_PORTS::portC);
	peripheralsController->connected_multiplexer_gpio.push_back(Mux4);

	peripheralsController->board_gpio_pins_vector.push_back(GPIO_Pins::PC2);
	peripheralsController->multiplexer_pin.push_back(MUTLIPLEXER_PINS::Y2);
	peripheralsController->board_gpio_ports_vector.push_back(GPIO_PINS_PORTS::portC);
	peripheralsController->connected_multiplexer_gpio.push_back(Mux4);

	peripheralsController->board_gpio_pins_vector.push_back(GPIO_Pins::PF1);
	peripheralsController->multiplexer_pin.push_back(MUTLIPLEXER_PINS::Y0);
	peripheralsController->board_gpio_ports_vector.push_back(GPIO_PINS_PORTS::portF);
	peripheralsController->connected_multiplexer_gpio.push_back(Mux4);

	peripheralsController->board_gpio_pins_vector.push_back(GPIO_Pins::PF0);
	peripheralsController->multiplexer_pin.push_back(MUTLIPLEXER_PINS::Y4);
	peripheralsController->board_gpio_ports_vector.push_back(GPIO_PINS_PORTS::portF);
	peripheralsController->connected_multiplexer_gpio.push_back(Mux3);

	peripheralsController->board_gpio_pins_vector.push_back(GPIO_Pins::PC15);
	peripheralsController->multiplexer_pin.push_back(MUTLIPLEXER_PINS::Y3);
	peripheralsController->board_gpio_ports_vector.push_back(GPIO_PINS_PORTS::portC);
	peripheralsController->connected_multiplexer_gpio.push_back(Mux3);

	peripheralsController->board_gpio_pins_vector.push_back(GPIO_Pins::PC14);
	peripheralsController->multiplexer_pin.push_back(MUTLIPLEXER_PINS::Y2);
	peripheralsController->board_gpio_ports_vector.push_back(GPIO_PINS_PORTS::portC);
	peripheralsController->connected_multiplexer_gpio.push_back(Mux3);

	peripheralsController->board_gpio_pins_vector.push_back(GPIO_Pins::PC13);
	peripheralsController->multiplexer_pin.push_back(MUTLIPLEXER_PINS::Y1);
	peripheralsController->board_gpio_ports_vector.push_back(GPIO_PINS_PORTS::portC);
	peripheralsController->connected_multiplexer_gpio.push_back(Mux3);

	peripheralsController->board_gpio_pins_vector.push_back(GPIO_Pins::PB7);
	peripheralsController->multiplexer_pin.push_back(MUTLIPLEXER_PINS::Y0);
	peripheralsController->board_gpio_ports_vector.push_back(GPIO_PINS_PORTS::portB);
	peripheralsController->connected_multiplexer_gpio.push_back(Mux3);

	peripheralsController->board_gpio_pins_vector.push_back(GPIO_Pins::PA15);
	peripheralsController->multiplexer_pin.push_back(MUTLIPLEXER_PINS::Y14);
	peripheralsController->board_gpio_ports_vector.push_back(GPIO_PINS_PORTS::portA);
	peripheralsController->connected_multiplexer_gpio.push_back(Mux3);

	peripheralsController->board_gpio_pins_vector.push_back(GPIO_Pins::PA14);
	peripheralsController->multiplexer_pin.push_back(MUTLIPLEXER_PINS::Y13);
	peripheralsController->board_gpio_ports_vector.push_back(GPIO_PINS_PORTS::portA);
	peripheralsController->connected_multiplexer_gpio.push_back(Mux3);

	peripheralsController->board_gpio_pins_vector.push_back(GPIO_Pins::PA13);
	peripheralsController->multiplexer_pin.push_back(MUTLIPLEXER_PINS::Y12);
	peripheralsController->board_gpio_ports_vector.push_back(GPIO_PINS_PORTS::portA);
	peripheralsController->connected_multiplexer_gpio.push_back(Mux3);

	peripheralsController->board_gpio_pins_vector.push_back(GPIO_Pins::PC12);
	peripheralsController->multiplexer_pin.push_back(MUTLIPLEXER_PINS::Y9);
	peripheralsController->board_gpio_ports_vector.push_back(GPIO_PINS_PORTS::portC);
	peripheralsController->connected_multiplexer_gpio.push_back(Mux3);

	peripheralsController->board_gpio_pins_vector.push_back(GPIO_Pins::PC10);
	peripheralsController->multiplexer_pin.push_back(MUTLIPLEXER_PINS::Y8);
	peripheralsController->board_gpio_ports_vector.push_back(GPIO_PINS_PORTS::portC);
	peripheralsController->connected_multiplexer_gpio.push_back(Mux3);


	//ANALOG
	peripheralsController->board_analog_pins_vector.push_back(Analog_Pins::PC5);
	peripheralsController->analog_ports_vector.push_back(GPIO_PINS_PORTS::portC);
	peripheralsController->analog_multiplexer_pins.push_back(MUTLIPLEXER_PINS::Y5); //Mux1
	peripheralsController->connected_multiplexer_analog.push_back(Mux1);

	peripheralsController->board_analog_pins_vector.push_back(Analog_Pins::PB12);
	peripheralsController->analog_ports_vector.push_back(GPIO_PINS_PORTS::portB);
	peripheralsController->analog_multiplexer_pins.push_back(MUTLIPLEXER_PINS::Y1); //Mux1
	peripheralsController->connected_multiplexer_analog.push_back(Mux1);

	peripheralsController->board_analog_pins_vector.push_back(Analog_Pins::PB11);
	peripheralsController->analog_ports_vector.push_back(GPIO_PINS_PORTS::portB);
	peripheralsController->analog_multiplexer_pins.push_back(MUTLIPLEXER_PINS::Y0); //Mux1
	peripheralsController->connected_multiplexer_analog.push_back(Mux1);

	peripheralsController->board_analog_pins_vector.push_back(Analog_Pins::PB2);
	peripheralsController->analog_ports_vector.push_back(GPIO_PINS_PORTS::portB);
	peripheralsController->analog_multiplexer_pins.push_back(MUTLIPLEXER_PINS::Y14); //Mux1
	peripheralsController->connected_multiplexer_analog.push_back(Mux1);

	peripheralsController->board_analog_pins_vector.push_back(Analog_Pins::PB1);
	peripheralsController->analog_ports_vector.push_back(GPIO_PINS_PORTS::portB);
	peripheralsController->analog_multiplexer_pins.push_back(MUTLIPLEXER_PINS::Y13); //Mux1
	peripheralsController->connected_multiplexer_analog.push_back(Mux1);

	peripheralsController->board_analog_pins_vector.push_back(Analog_Pins::PB15);
	peripheralsController->analog_ports_vector.push_back(GPIO_PINS_PORTS::portB);
	peripheralsController->analog_multiplexer_pins.push_back(MUTLIPLEXER_PINS::Y12); //Mux1
	peripheralsController->connected_multiplexer_analog.push_back(Mux1);

	peripheralsController->board_analog_pins_vector.push_back(Analog_Pins::PB14);
	peripheralsController->analog_ports_vector.push_back(GPIO_PINS_PORTS::portB);
	peripheralsController->analog_multiplexer_pins.push_back(MUTLIPLEXER_PINS::Y11); //Mux1
	peripheralsController->connected_multiplexer_analog.push_back(Mux1);

	peripheralsController->board_analog_pins_vector.push_back(Analog_Pins::PB13);
	peripheralsController->analog_ports_vector.push_back(GPIO_PINS_PORTS::portB);
	peripheralsController->analog_multiplexer_pins.push_back(MUTLIPLEXER_PINS::Y10); //Mux1
	peripheralsController->connected_multiplexer_analog.push_back(Mux1);

	peripheralsController->board_analog_pins_vector.push_back(Analog_Pins::PC4);
	peripheralsController->analog_ports_vector.push_back(GPIO_PINS_PORTS::portC);
	peripheralsController->analog_multiplexer_pins.push_back(MUTLIPLEXER_PINS::Y8); //Mux1
	peripheralsController->connected_multiplexer_analog.push_back(Mux1);

	peripheralsController->board_analog_pins_vector.push_back(Analog_Pins::PA7);
	peripheralsController->analog_ports_vector.push_back(GPIO_PINS_PORTS::portA);
	peripheralsController->analog_multiplexer_pins.push_back(MUTLIPLEXER_PINS::Y15); //Mux2
	peripheralsController->connected_multiplexer_analog.push_back(Mux2);

	peripheralsController->board_analog_pins_vector.push_back(Analog_Pins::PA6);
	peripheralsController->analog_ports_vector.push_back(GPIO_PINS_PORTS::portA);
	peripheralsController->analog_multiplexer_pins.push_back(MUTLIPLEXER_PINS::Y14); //Mux2
	peripheralsController->connected_multiplexer_analog.push_back(Mux2);

	peripheralsController->board_analog_pins_vector.push_back(Analog_Pins::PA5);
	peripheralsController->analog_ports_vector.push_back(GPIO_PINS_PORTS::portA);
	peripheralsController->analog_multiplexer_pins.push_back(MUTLIPLEXER_PINS::Y13); //Mux2
	peripheralsController->connected_multiplexer_analog.push_back(Mux2);

	peripheralsController->board_analog_pins_vector.push_back(Analog_Pins::PC0);
	peripheralsController->analog_ports_vector.push_back(GPIO_PINS_PORTS::portC);
	peripheralsController->analog_multiplexer_pins.push_back(MUTLIPLEXER_PINS::Y4); //Mux4
	peripheralsController->connected_multiplexer_analog.push_back(Mux4);

	peripheralsController->board_analog_pins_vector.push_back(Analog_Pins::PC1);
	peripheralsController->analog_ports_vector.push_back(GPIO_PINS_PORTS::portC);
	peripheralsController->analog_multiplexer_pins.push_back(MUTLIPLEXER_PINS::Y5); //Mux4
	peripheralsController->connected_multiplexer_analog.push_back(Mux4);

	peripheralsController->board_analog_pins_vector.push_back(Analog_Pins::PB0);
	peripheralsController->analog_ports_vector.push_back(GPIO_PINS_PORTS::portB);
	peripheralsController->analog_multiplexer_pins.push_back(MUTLIPLEXER_PINS::Y6); //Mux4
	peripheralsController->connected_multiplexer_analog.push_back(Mux4);

	peripheralsController->board_analog_pins_vector.push_back(Analog_Pins::PA4);
	peripheralsController->analog_ports_vector.push_back(GPIO_PINS_PORTS::portA);
	peripheralsController->analog_multiplexer_pins.push_back(MUTLIPLEXER_PINS::Y7); //Mux4
	peripheralsController->connected_multiplexer_analog.push_back(Mux4);

	peripheralsController->board_analog_pins_vector.push_back(Analog_Pins::PA1);
	peripheralsController->analog_ports_vector.push_back(GPIO_PINS_PORTS::portA);
	peripheralsController->analog_multiplexer_pins.push_back(MUTLIPLEXER_PINS::Y8); //Mux4
	peripheralsController->connected_multiplexer_analog.push_back(Mux4);

	peripheralsController->board_analog_pins_vector.push_back(Analog_Pins::PA0);
	peripheralsController->analog_ports_vector.push_back(GPIO_PINS_PORTS::portA);
	peripheralsController->analog_multiplexer_pins.push_back(MUTLIPLEXER_PINS::Y9); //Mux4
	peripheralsController->connected_multiplexer_analog.push_back(Mux4);

	peripheralsController->board_analog_pins_vector.push_back(Analog_Pins::PC3);
	peripheralsController->analog_ports_vector.push_back(GPIO_PINS_PORTS::portC);
	peripheralsController->analog_multiplexer_pins.push_back(MUTLIPLEXER_PINS::Y3); //Mux4
	peripheralsController->connected_multiplexer_analog.push_back(Mux4);

	peripheralsController->board_analog_pins_vector.push_back(Analog_Pins::PC2);
	peripheralsController->analog_ports_vector.push_back(GPIO_PINS_PORTS::portC);
	peripheralsController->analog_multiplexer_pins.push_back(MUTLIPLEXER_PINS::Y2); //Mux4
	peripheralsController->connected_multiplexer_analog.push_back(Mux4);

}


/* initialize Power Pins Vectors
 *  * Parameter:
 *  powerPinsVectors : structure that contains Power and GND Pins Test Vectors
 */
void Initializer::initialPowerGndPinsTestVectors(){
	peripheralsController->powerGndMuxConnectionVector.clear();
	peripheralsController->powerGndMuxPinVectors.clear();
	peripheralsController->powerGndPinsStringVector.clear();
	peripheralsController->powerGndPinsType.clear();

	peripheralsController->powerGndMuxConnectionVector.push_back(Connected_Multiplexer::MUX1);
	peripheralsController->powerGndMuxPinVectors.push_back(Power_GND_Connected_Multiplexer_Pin::U5V);
	peripheralsController->powerGndPinsStringVector.push_back("U5V");
	peripheralsController->powerGndPinsType.push_back(POWER_PIN_TYPE::POWER5V);

	peripheralsController->powerGndMuxConnectionVector.push_back(Connected_Multiplexer::MUX1);
	peripheralsController->powerGndMuxPinVectors.push_back(Power_GND_Connected_Multiplexer_Pin::GND1);
	peripheralsController->powerGndPinsStringVector.push_back("GND1");
	peripheralsController->powerGndPinsType.push_back(POWER_PIN_TYPE::GND);

	peripheralsController->powerGndMuxConnectionVector.push_back(Connected_Multiplexer::MUX1);
	peripheralsController->powerGndMuxPinVectors.push_back(Power_GND_Connected_Multiplexer_Pin::AGND);
	peripheralsController->powerGndPinsStringVector.push_back("AGND");
	peripheralsController->powerGndPinsType.push_back(POWER_PIN_TYPE::GND);

	peripheralsController->powerGndMuxConnectionVector.push_back(Connected_Multiplexer::MUX2);
	peripheralsController->powerGndMuxPinVectors.push_back(Power_GND_Connected_Multiplexer_Pin::AVDD);
	peripheralsController->powerGndPinsStringVector.push_back("AVDD");
	peripheralsController->powerGndPinsType.push_back(POWER_PIN_TYPE::POWER3V);

	peripheralsController->powerGndMuxConnectionVector.push_back(Connected_Multiplexer::MUX2);
	peripheralsController->powerGndMuxPinVectors.push_back(Power_GND_Connected_Multiplexer_Pin::GND2);
	peripheralsController->powerGndPinsStringVector.push_back("GND2");
	peripheralsController->powerGndPinsType.push_back(POWER_PIN_TYPE::GND);

	peripheralsController->powerGndMuxConnectionVector.push_back(Connected_Multiplexer::MUX3);
	peripheralsController->powerGndMuxPinVectors.push_back(Power_GND_Connected_Multiplexer_Pin::VDD);
	peripheralsController->powerGndPinsStringVector.push_back("VDD");
	peripheralsController->powerGndPinsType.push_back(POWER_PIN_TYPE::POWER3V);

	peripheralsController->powerGndMuxConnectionVector.push_back(Connected_Multiplexer::MUX3);
	peripheralsController->powerGndMuxPinVectors.push_back(Power_GND_Connected_Multiplexer_Pin::GND3);
	peripheralsController->powerGndPinsStringVector.push_back("GND3");
	peripheralsController->powerGndPinsType.push_back(POWER_PIN_TYPE::GND);

	peripheralsController->powerGndMuxConnectionVector.push_back(Connected_Multiplexer::MUX4);
	peripheralsController->powerGndMuxPinVectors.push_back(Power_GND_Connected_Multiplexer_Pin::VBAT);
	peripheralsController->powerGndPinsStringVector.push_back("VBAT");
	peripheralsController->powerGndPinsType.push_back(POWER_PIN_TYPE::POWER3V);

	peripheralsController->powerGndMuxConnectionVector.push_back(Connected_Multiplexer::MUX4);
	peripheralsController->powerGndMuxPinVectors.push_back(Power_GND_Connected_Multiplexer_Pin::VIN);
	peripheralsController->powerGndPinsStringVector.push_back("VIN");
	peripheralsController->powerGndPinsType.push_back(POWER_PIN_TYPE::VIN);

	peripheralsController->powerGndMuxConnectionVector.push_back(Connected_Multiplexer::MUX4);
	peripheralsController->powerGndMuxPinVectors.push_back(Power_GND_Connected_Multiplexer_Pin::GND6);
	peripheralsController->powerGndPinsStringVector.push_back("GND6");
	peripheralsController->powerGndPinsType.push_back(POWER_PIN_TYPE::GND);

	peripheralsController->powerGndMuxConnectionVector.push_back(Connected_Multiplexer::MUX4);
	peripheralsController->powerGndMuxPinVectors.push_back(Power_GND_Connected_Multiplexer_Pin::GND5);
	peripheralsController->powerGndPinsStringVector.push_back("GND5");
	peripheralsController->powerGndPinsType.push_back(POWER_PIN_TYPE::GND);

	peripheralsController->powerGndMuxConnectionVector.push_back(Connected_Multiplexer::MUX4);
	peripheralsController->powerGndMuxPinVectors.push_back(Power_GND_Connected_Multiplexer_Pin::P5V);
	peripheralsController->powerGndPinsStringVector.push_back("5V");
	peripheralsController->powerGndPinsType.push_back(POWER_PIN_TYPE::POWER5V);

	peripheralsController->powerGndMuxConnectionVector.push_back(Connected_Multiplexer::MUX4);
	peripheralsController->powerGndMuxPinVectors.push_back(Power_GND_Connected_Multiplexer_Pin::P3V3);
	peripheralsController->powerGndPinsStringVector.push_back("3.3V");
	peripheralsController->powerGndPinsType.push_back(POWER_PIN_TYPE::POWER3V);

	peripheralsController->powerGndMuxConnectionVector.push_back(Connected_Multiplexer::MUX5);
	peripheralsController->powerGndMuxPinVectors.push_back(Power_GND_Connected_Multiplexer_Pin::E5V);
	peripheralsController->powerGndPinsStringVector.push_back("E5V");
	peripheralsController->powerGndPinsType.push_back(POWER_PIN_TYPE::E5V);

	peripheralsController->powerGndMuxConnectionVector.push_back(Connected_Multiplexer::MUX5);
	peripheralsController->powerGndMuxPinVectors.push_back(Power_GND_Connected_Multiplexer_Pin::GND4);
	peripheralsController->powerGndPinsStringVector.push_back("GND4");
	peripheralsController->powerGndPinsType.push_back(POWER_PIN_TYPE::GND);

	peripheralsController->powerGndMuxConnectionVector.push_back(Connected_Multiplexer::MUX5);
	peripheralsController->powerGndMuxPinVectors.push_back(Power_GND_Connected_Multiplexer_Pin::IOREF);
	peripheralsController->powerGndPinsStringVector.push_back("IOREF");
	peripheralsController->powerGndPinsType.push_back(POWER_PIN_TYPE::POWER3V);
}
