/*
 * Constants.hpp
 *
 *  Created on: 13.08.2023
 *      Author: Astra
 */

#ifndef HELPERS_CONSTANTS_HPP_
#define HELPERS_CONSTANTS_HPP_

// Board Initialize Success Value
#define BOARD_INITIAL_OK 1

// Board Max Temperature Value
#define BOARD_MAX_TEMPERATURE 85  //Package STM32F303RET6 => "T" temperature: from -40° to 85°
#define TEMPERATURE_MEASURE_FAILED -1

//Raspberry Pi USB-PORT State
#define USB_PORT_SET_ON_SUCCESS 1
#define USB_PORT_SET_ON_FAILED -1

#define USB_PORT_OFF 0
#define USB_PORT_SET_OFF_FAILED -2


#define RESTART_USB_PORT_DONE 1
#define RESTART_USB_PORT_FAILED -1

//Raspberry Pi ON STATE
#define BOARD_TESTER_ON 2

//Raspberry Pi USB Interfaces Numbers
#define USB_SERIAL_PORT_TTYACM0_ACK 1
#define USB_SERIAL_PORT_TTYACM1_ACK 2

//MICROCONTROLLER Max Current Consumption
#define MAX_CURRENT_CONSUMPTION 63

//Bits
#define SHIFT_LEFT_BY(x) (1 << (x))
#define IS_BIT_SET(x,y) ((x) & (SHIFT_LEFT_BY(y)))
#define IS_BIT_CLEAR(x,y) !IS_BIT_SET((x), (y))

//LED GPIOs
#define LED_GREEN_PI_PIN 21
#define LED_RED_PI_PIN 20

// Buttons Raspberry Pi GPIOs
#define Yellow_Button_PIN 18
#define BLUE_Button_PIN 23
#define RED_Button_PIN 24
#define GREEN_Button_PIN 25

// CS0 and CS1 SPI
#define CS0 8
#define CS1 7
// MUX 5, 6, 7 Control GPIOS
#define S0_MUX6 6 //31
#define S1_MUX6 5 //29
#define S2_MUX6 22 //15
#define S0_MUX5 26 //37
#define S1_MUX5 19 //35
#define S2_MUX5 13 //33
#define S1_MUX7 12 //32
#define S2_MUX7 4  //7
#define S3_MUX7 17 //11

// Port Register
#define  PORT_A 0x02
#define  PORT_B 0x04
#define  PORT_C 0x06
#define  PORT_D 0x08
#define  PORT_E 0x0A
#define  PORT_F 0x0C

//Initial GPIO: GPIO MODE
#define COMMAND_DeInit 0x00
#define COMMAND_Init_DIGITAL_INPUT 0x10
#define COMMAND_Init_DIGITAL_OUTPUT_PP 0x20
#define COMMAND_Init_DIGITAL_OUTPUT_OD 0x30
#define COMMAND_Init_DIGITAL_OUTPUT_AF_PP 0x40
#define COMMAND_Init_DIGITAL_OUTPUT_AF_OD 0x50
#define COMMAND_Init_ANALOGE 0x60

// set / read
#define COMMAND_WRITE_DIGITIAL_OUTPUT_LEVEL_HIGH 0x10
#define COMMAND_WRITE_DIGITIAL_OUTPUT_LEVEL_LOW 0x20
#define COMMAND_READ_DIGITAL_LEVEL_STATE 0x40
#define COMMAND_READ_ANALOG 0x30

//Initial GPIO: GPIO PULL
#define COMMAND_Init_NO_PULL 0x00
#define COMMAND_Init_PULL_UP 0x80
#define COMMAND_Init_PULL_DOWN 0x100

//Initial GPIO: GPIO SPEED
#define COMMAND_Init_GPIO_SPEED_FREQ_LOW 0x00
#define COMMAND_Init_GPIO_SPEED_FREQ_MEDIUM 0x200
#define COMMAND_Init_GPIO_SPEED_FREQ_HIGH 0x400

// Port GPIO PINS
#define PIN0 0x00
#define PIN1 0x01
#define PIN2 0x02
#define PIN3 0x03
#define PIN4 0x04
#define PIN5 0x05
#define PIN6 0x06
#define PIN7 0x07
#define PIN8 0x08
#define PIN9 0x09
#define PIN10 0x0A
#define PIN11 0x0B
#define PIN12 0x0C
#define PIN13 0x0D
#define PIN14 0x0E
#define PIN15 0x0F

// Multiplexer PINS
#define MuxY0 0
#define MuxY1 1
#define MuxY2 2
#define MuxY3 3
#define MuxY4 4
#define MuxY5 5
#define MuxY6 6
#define MuxY7 7
#define MuxY8 8
#define MuxY9 9
#define MuxY10 10
#define MuxY11 11
#define MuxY12 12
#define MuxY13 13
#define MuxY14 14
#define MuxY15 15

// Multiplexers
#define Mux1 1
#define Mux2 2
#define Mux3 3
#define Mux4 4
#define Mux5 5

//Delays
#define _5MS_DELAY 5
#define _10MS_DELAY 10
#define _20MS_DELAY 20
#define _50MS_DELAY 50
#define _100MS_DELAY 100
#define _200MS_DELAY 200

#define _500MS_DELAY 500
#define _1S_DELAY 1000
#define _2S_DELAY 2000
#define _3S_DELAY 3000
#define _5S_DELAY 5000

// UART Buffer Size
#define UART_ACK_SIZE 24
#define UART_SEND_BUFFER_SIZE 2

// GPIO Input Test comment
#define OK "OK"
#define GPIO_TEST_ERROR "Error"

// GPIO Output Test comment
#define GPIO_OUTPUT_VOLTAGE_OK "OK"
#define GPIO_OUTPUT_VOLTAGE_ERROR "ERROR"

// GPIO Input connected to PC13 Push Button
#define GPIO_INCOMAPTIBLE_PULL_MODE "GPIO cannot be tested using this Pull-Mode"

#define PIPE_OPEN_FAILED_TEXT "failed to open pipe"

// NUCLEO BOARD CONNECTION
#define NUCLEO_CONNECTION_OK 1
#define NUCLEO_USB_DISCONNECTED 6

//ADC Resolution
#define ADC_RESOLUTION 4095

//Test Errors
#define MEASURE_VOLTAGE_ERROR -1
#define MEASURE_CURRENT_ERROR -2
#define DAC_SET_VALUE_ERROR -3
#define IO_EXPNADER_WRITE_ERROR -4
#define MUX7_CONTROLL_ERROR -5
#define MUX6_CONTROLL_ERROR -6
#define MUX5_CONTROLL_ERROR -7
#define UNKNOWN_MULTIPLEXER_SELECT -8
#define UNKNOWN_CONNECTED_MULTIPLEXER_TO_NUCLEO_PIN -9
#define UNKNOWN_IO_EXPANDER_COMMAND -10
#define TURN_USB_PORT_OFF_ERROR -11
#define GPIO_INPUT_HIGH_LEVEL_ERROR -15
#define GPIO_INPUT_LOW_LEVEL_ERROR -16
#define GPIO_FAILED_MULTIPLEXER_ERROR -17
#define GPIO_TEST_UART_FAILED -18
#define GPIO_FAILED_INITIAL_ERROR -19

//GPIO INPUT LEVELS State
#define GPIO_INPUT_LOW_LEVEL_OK 0
#define GPIO_INPUT_HIGH_LEVEL_OK 1

//GPIO RESET VALUE To Update Database
#define TEST_RESET_VALUE -1

//Read STM32-ADC Error
#define ADC_INIT_ERROR 0x3000
#define ADC_SET_CHANNEL_ERROR 0x5000
#define ADC_START_ERROR 0x7000
#define ADC_POLL_CONVERSION_ERROR 0x9000
#define ADC_STOP_ERROR 0xB000
#define UNKNOWN_ADC 0xF000

#endif /* HELPERS_CONSTANTS_HPP_ */
