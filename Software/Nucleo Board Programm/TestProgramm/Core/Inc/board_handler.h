/*
 * board_handler.h
 *
 *  Created on: Jun 25, 2023
 *      Author: Astra
 */

#ifndef INC_BOARD_HANDLER_H_
#define INC_BOARD_HANDLER_H_


#include <adc_handler.h>

#define REGISTERS_STATES_BUFFER_SIZE 24

#define FLASH_START_ADDR 0x08000000 // Anfangsadresse des Flash-Speichers
#define FLASH_END_ADDR   0x0807FFFF // Endadresse des Flash-Speichers

//COMMAND TYPE
#define COMMAND_READ_TEMPERATURE_SENSOR 0xFFFF
#define COMMAND_TEST_FLASH_MEMORY 0xFFF

#define COMMAND_TYPE_INIT 0x00
#define COMMAND_TYPE 0x01

//GPIO Port checked
#define COMMAND_PORT_A 0x02
#define COMMAND_PORT_B 0x04
#define COMMAND_PORT_C 0x06
#define  COMMAND_PORT_D 0x08
#define  COMMAND_PORT_E 0x0A
#define  COMMAND_PORT_F 0x0C
#define COMMAND_PORT_MASK 0x0E

//Initial GPIO: GPIO MODE
#define COMMAND_Init_DIGITAL_INPUT 0x10
#define COMMAND_Init_DIGITAL_OUTPUT_PP 0x20
#define COMMAND_Init_DIGITAL_OUTPUT_OD 0x30
#define COMMAND_Init_DIGITAL_OUTPUT_AF_PP 0x40
#define COMMAND_Init_DIGITAL_OUTPUT_AF_OD 0x50
#define COMMAND_Init_ANALOGE 0x60
#define COMMAND_INIT_MODE_MASK 0x70

// read/set GPIO State
#define COMMAND_WRITE_DIGITIAL_OUTPUT_LEVEL_HIGH 0x10
#define COMMAND_WRITE_DIGITIAL_OUTPUT_LEVEL_LOW 0x20
#define COMMAND_READ_DIGITAL_LEVEL_STATE 0x40
#define COMMAND_READ_ANALOG 0x30
#define HANDLE_TYPE_MASK 0x70

//GPIO PULL Type
#define COMMAND_Init_NO_PULL 0x00
#define COMMAND_Init_PULL_UP 0x80
#define COMMAND_Init_PULL_DOWN 0x100
#define COMMAND_Init_PULL_MASK 0x180

//Initial GPIO: GPIO SPEED
#define COMMAND_Init_GPIO_SPEED_FREQ_LOW 0x00
#define COMMAND_Init_GPIO_SPEED_FREQ_MEDIUM 0x200
#define COMMAND_Init_GPIO_SPEED_FREQ_HIGH 0x400
#define COMMAND_Init_GPIO_SPEED_FREQ_MASK 0x600

//Initial PIN SHIFT
#define COMMAND_INIT_GPIO_SHIFT 11
#define COMMAND_INIT_GPIO_MASK 0xF800

//HANDLE PIN SHIFT
#define COMMAND_HANDLE_GPIO_SHIFT 7
#define COMMAND_HANDLE_GPIO_MASK 0xF80

// GPIOs PINS
#define PIN0 0x01
#define PIN1 0x02
#define PIN2 0x03
#define PIN3 0x04
#define PIN4 0x05
#define PIN5 0x06
#define PIN6 0x07
#define PIN7 0x08
#define PIN8 0x09
#define PIN9 0x0A
#define PIN10 0x0B
#define PIN11 0x0C
#define PIN12 0x0D
#define PIN13 0x0E
#define PIN14 0x0F
#define PIN15 0x10

typedef struct
{
 	uint16_t testedPages ;
    uint16_t erasedPages ;
    uint32_t writedAddresses ;
    uint32_t writedAddressesError;
    uint32_t readedAddresses ;
    uint32_t readedAddressesError;
    uint32_t erasedPagesError ;

} FLASH_TestResultTypeDef;



void handleUartCommand(uint16_t receivedValue);

// Flash Memory Test
uint32_t checkEmptyFlashMemory(uint32_t address);
void sendFlashMemoryTestResult(FLASH_TestResultTypeDef *testResult);
void testFlashMemory();

//GPIOs
void initialGpio(uint16_t data);
uint32_t getInitialMode(uint16_t command);
uint32_t getInitialPullType(uint16_t command);
uint32_t getInitialSpeed(uint16_t command);
GPIO_TypeDef *getGpioPort(uint16_t command);
void handleDigitalGpio(uint16_t command);
void setDigitalGpioOutputLevelState(uint16_t command);
void readDigitalGpioLevelState(uint16_t command);
void resetGpiosRegisters();
uint16_t getGpioNumber(uint16_t command, uint16_t mask, int shiftNum);
void sendGpiosRegistersState(uint16_t command);

// Analog
uint16_t getTemperatureSensorValue();
void sendAdcValue(uint16_t adcValue);
void readAnalogValue(uint16_t command);

#endif /* INC_BOARD_HANDLER_H_ */
