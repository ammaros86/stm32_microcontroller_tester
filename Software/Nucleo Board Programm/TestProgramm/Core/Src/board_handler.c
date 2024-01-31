#include "main.h"
#include "board_handler.h"


extern UART_HandleTypeDef huart2;

/*
 * handle UART Command
 * Parameter:
 * buffer: UART Buffer
  */
void handleUartCommand(uint16_t receivedValue){
 	if(receivedValue == COMMAND_TEST_FLASH_MEMORY){
		testFlashMemory();
 	}else if(receivedValue == COMMAND_READ_TEMPERATURE_SENSOR){
 		uint16_t temperatureSensorValue = getTemperatureSensorValue();
 		sendAdcValue(temperatureSensorValue);
	}else if(receivedValue & (1 << COMMAND_TYPE_INIT)){
		if((receivedValue & HANDLE_TYPE_MASK) == COMMAND_READ_ANALOG){
			readAnalogValue(receivedValue);
		}else{
			handleDigitalGpio(receivedValue);
		}
	}else{
		initialGpio(receivedValue);
	}
}

/*
 * checkEmptyFlashMemory: control if the page contain address is empty page
 * Parameter:
 * address: Flash-Memory Address */
uint32_t checkEmptyFlashMemory(uint32_t address) {
    uint32_t pageStartAddress = address & ~(FLASH_PAGE_SIZE - 1);
    for (int i = 0; i < FLASH_PAGE_SIZE / 4; i++) {
        uint32_t data = *(__IO uint32_t *)(address + i * 4);
        if (data != 0xFFFFFFFF) {
            return 0; // Page hat nicht den Wert 0xFFFFFFFF
        }
    }
    return pageStartAddress; // Page hat den Wert 0xFFFFFFFF, Rückgabe der Page-Startadresse
}

/*
 * send flash Test Result
 * Parameter:
 * testResult: Flash Result Structure */
void sendFlashMemoryTestResult(FLASH_TestResultTypeDef *testResult){
	uint8_t buffer[24];
 	if(testResult != NULL){
		size_t bufferOffset = 0;
		memcpy(buffer + bufferOffset, &testResult->testedPages, sizeof(uint16_t));
		bufferOffset += sizeof(uint16_t);
		memcpy(buffer + bufferOffset, &testResult->erasedPages, sizeof(uint16_t));
		bufferOffset += sizeof(uint16_t);
		memcpy(buffer + bufferOffset, &testResult->writedAddresses, sizeof(uint32_t));
		bufferOffset += sizeof(uint32_t);
		memcpy(buffer + bufferOffset, &testResult->writedAddressesError, sizeof(uint32_t));
		bufferOffset += sizeof(uint32_t);
		memcpy(buffer + bufferOffset, &testResult->readedAddresses, sizeof(uint32_t));
		bufferOffset += sizeof(uint32_t);
		memcpy(buffer + bufferOffset, &testResult->readedAddressesError, sizeof(uint32_t));
		bufferOffset += sizeof(uint32_t);
		memcpy(buffer + bufferOffset, &testResult->erasedPagesError, sizeof(uint32_t));
		HAL_UART_Transmit(&huart2, buffer, 24, 10);
	}else{
		buffer[0] = 0;
		HAL_UART_Transmit(&huart2, buffer, 24, 10);
	}
}

/*
 * test Flash-Memory
 *  */
void testFlashMemory(){
	FLASH_TestResultTypeDef testResult ={};
	uint32_t address ;
	uint32_t pageError;
	uint32_t pattern = 0xA1A1A1A1;
	for (address = FLASH_START_ADDR; address <= FLASH_END_ADDR; address += 0x800) {
		if (*(uint32_t*)address == 0xFFFFFFFF) {
			uint32_t pageStartAddress = checkEmptyFlashMemory(address);
			if(pageStartAddress != 0){
			  testResult.testedPages ++;
			  HAL_FLASH_Unlock();
				for (int offset = 0; offset < FLASH_PAGE_SIZE ; offset+=4) {
					HAL_StatusTypeDef writeStatus = HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, pageStartAddress + offset, pattern);
					if (writeStatus == HAL_OK) {
						testResult.writedAddresses++;
					}else{
						testResult.writedAddressesError ++;
					}
				}
				HAL_FLASH_Lock();
				for (int offset = 0; offset < FLASH_PAGE_SIZE ; offset+=4) {
					uint32_t data = *(__IO uint32_t *)(pageStartAddress + offset);
					testResult.readedAddresses ++;
					if (data != pattern) {
						testResult.readedAddressesError++;
					}
				}

				HAL_FLASH_Unlock();
				FLASH_EraseInitTypeDef flashEraseInitStruct;
				flashEraseInitStruct.TypeErase = FLASH_TYPEERASE_PAGES;
				flashEraseInitStruct.PageAddress = address;
				flashEraseInitStruct.NbPages = 1;
				HAL_StatusTypeDef status = HAL_FLASHEx_Erase(&flashEraseInitStruct, &pageError);
				if(status == HAL_OK){
					testResult.erasedPages ++;
				}else{
					testResult.erasedPagesError ++;
				}
				HAL_FLASH_Lock();
			}
		}
	}
	sendFlashMemoryTestResult(&testResult);
}

/*
 * initial GPIO
 * Parameter:
 * command: UART received Command */
void initialGpio(uint16_t command){
	resetGpiosRegisters();
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Mode = getInitialMode(command);
	GPIO_InitStruct.Pull = getInitialPullType(command);
	GPIO_InitStruct.Speed = getInitialSpeed(command);
	GPIO_InitStruct.Pin = getGpioNumber(command, COMMAND_INIT_GPIO_MASK, COMMAND_INIT_GPIO_SHIFT);
	GPIO_TypeDef * gpioPort = getGpioPort(command);
	if(gpioPort != NULL){
		HAL_GPIO_Init(gpioPort, &GPIO_InitStruct);
	}
	sendGpiosRegistersState(command);
}

/*
 * get GPIO Initial Mode
 * Parameter:
 * command: UART received Command */
uint32_t getInitialMode(uint16_t command){
 	if((command & COMMAND_INIT_MODE_MASK)  == COMMAND_Init_DIGITAL_INPUT){
		return GPIO_MODE_INPUT;
	}else if((command & COMMAND_INIT_MODE_MASK)  == COMMAND_Init_DIGITAL_OUTPUT_PP){
 		return GPIO_MODE_OUTPUT_PP;
	}else if((command & COMMAND_INIT_MODE_MASK) == COMMAND_Init_DIGITAL_OUTPUT_OD){
		return GPIO_MODE_OUTPUT_OD;
	}else if((command & COMMAND_INIT_MODE_MASK) == COMMAND_Init_DIGITAL_OUTPUT_AF_PP){
		return GPIO_MODE_AF_PP;
	}else if((command & COMMAND_INIT_MODE_MASK) == COMMAND_Init_DIGITAL_OUTPUT_AF_OD){
		return GPIO_MODE_AF_OD;
	}else if((command & COMMAND_INIT_MODE_MASK)  == COMMAND_Init_ANALOGE){
		return GPIO_MODE_ANALOG;
	}else{
		return GPIO_MODE_INPUT;
	}
}

/*
 * get GPIO Initial Pull Type
 * Parameter:
 * command: UART received Command */
uint32_t getInitialPullType(uint16_t command){
	if((command & COMMAND_Init_PULL_MASK)== COMMAND_Init_NO_PULL){
		return GPIO_NOPULL;
	}else if((command & COMMAND_Init_PULL_MASK) == COMMAND_Init_PULL_UP){
		return GPIO_PULLUP;
	}else if((command & COMMAND_Init_PULL_MASK) == COMMAND_Init_PULL_DOWN){
		return GPIO_PULLDOWN;
	}else{
		 return GPIO_NOPULL;
	}
}

/*
 * get GPIO Initial Speed
 * Parameter:
 * command: UART received Command */
uint32_t getInitialSpeed(uint16_t command){
	if((command & COMMAND_Init_GPIO_SPEED_FREQ_MASK)  == COMMAND_Init_GPIO_SPEED_FREQ_LOW){
		return GPIO_SPEED_FREQ_LOW;
	}else if((command & COMMAND_Init_GPIO_SPEED_FREQ_MASK) == COMMAND_Init_GPIO_SPEED_FREQ_MEDIUM){
		return GPIO_SPEED_FREQ_MEDIUM;
	}else if((command & COMMAND_Init_GPIO_SPEED_FREQ_MASK) == COMMAND_Init_GPIO_SPEED_FREQ_HIGH){
		return GPIO_SPEED_FREQ_HIGH;
	}else{
 		return GPIO_SPEED_FREQ_LOW;
	}
}

/*
 * get GPIO Port
 * Parameter:
 * command: UART received Command */
GPIO_TypeDef *getGpioPort(uint16_t command){
	if((command & COMMAND_PORT_MASK) == COMMAND_PORT_A){
		return GPIOA;
	}else if((command & COMMAND_PORT_MASK) == COMMAND_PORT_B){
		return GPIOB;
	}else if(((command & COMMAND_PORT_MASK) == COMMAND_PORT_C)){
		return GPIOC;
	}else if(((command & COMMAND_PORT_MASK) == COMMAND_PORT_D)){
		return GPIOD;
	}else if(((command & COMMAND_PORT_MASK) == COMMAND_PORT_E)){
		return GPIOE;
	}else if(((command & COMMAND_PORT_MASK) == COMMAND_PORT_F)){
		return GPIOF;
	}else{
		return NULL;
	}
}

/*
 * handle Digital Output / Input
 * Parameter:
 * command: UART received Command
 */
void handleDigitalGpio(uint16_t command){
	if(((command & HANDLE_TYPE_MASK) == COMMAND_WRITE_DIGITIAL_OUTPUT_LEVEL_HIGH) || ((command & HANDLE_TYPE_MASK) == COMMAND_WRITE_DIGITIAL_OUTPUT_LEVEL_LOW)){
		setDigitalGpioOutputLevelState(command);
	}else{
		readDigitalGpioLevelState(command);
	}
}

/*
 * set Digital Output Level
 * Parameter:
 * command: UART received Command
 */
void setDigitalGpioOutputLevelState(uint16_t command){
	GPIO_PinState gpioOutputState;
	if((command & HANDLE_TYPE_MASK) == COMMAND_WRITE_DIGITIAL_OUTPUT_LEVEL_HIGH){
		gpioOutputState = GPIO_PIN_SET;
	}else{
		gpioOutputState = GPIO_PIN_RESET;
	}
 	uint16_t  gpioNumber = getGpioNumber(command, COMMAND_HANDLE_GPIO_MASK, COMMAND_HANDLE_GPIO_SHIFT);
	GPIO_TypeDef *gpioPort = getGpioPort(command);
	if(gpioPort != NULL){
 		HAL_GPIO_WritePin(gpioPort, gpioNumber, gpioOutputState);
	}else{
		//Handle initial Failure
	}
	sendGpiosRegistersState(command);
}

/*
 * read GPIO Input
 * Parameter:
 * command: UART received Command
 */
void readDigitalGpioLevelState(uint16_t command){
	sendGpiosRegistersState(command);
}

/*
 * reset GPIOs Registers
  */
void resetGpiosRegisters(){ //TODO not testable pins not reset
	HAL_GPIO_DeInit(GPIOA, 0x9FF3);
	HAL_GPIO_DeInit(GPIOB, GPIO_PIN_All);
	HAL_GPIO_DeInit(GPIOC, 0xDFFF);
	HAL_GPIO_DeInit(GPIOD, GPIO_PIN_All);
	HAL_GPIO_DeInit(GPIOE, GPIO_PIN_All);
	HAL_GPIO_DeInit(GPIOF, GPIO_PIN_All);
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLDOWN;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct.Pin = 0x9FF3;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	GPIO_InitTypeDef GPIO_InitStruct2 = {0};
	GPIO_InitStruct2.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct2.Pull = GPIO_PULLDOWN;
	GPIO_InitStruct2.Speed = GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct2.Pin = GPIO_PIN_All;
	GPIO_InitTypeDef GPIO_InitStruct3 = {0};
	GPIO_InitStruct3.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct3.Pull = GPIO_PULLDOWN;
	GPIO_InitStruct3.Speed = GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct3.Pin = 0xDFFF;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct2);
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct3);
	HAL_GPIO_Init(GPIOD, &GPIO_InitStruct2);
	HAL_GPIO_Init(GPIOE, &GPIO_InitStruct2);
	HAL_GPIO_Init(GPIOF, &GPIO_InitStruct2);
}

/*
 * get GPIO Pin
 * Parameter:
 * command: UART received Command
 * mask: Command GPIOs Mask
 * shiftNum: number of Shift
 *   */
uint16_t getGpioNumber(uint16_t command, uint16_t mask, int shiftNum){
	if(((command & mask) >> shiftNum) == PIN0){
		return GPIO_PIN_0;
	}else if(((command & mask)>> shiftNum) == PIN1){
		return GPIO_PIN_1;
 	}else if(((command & mask)>> shiftNum) == PIN2){
 		return GPIO_PIN_2;
 	}else if(((command & mask)>> shiftNum) == PIN3){
 		return GPIO_PIN_3;
 	}else if(((command & mask)>> shiftNum) == PIN4){
 		return GPIO_PIN_4;
 	}else if(((command & mask)>> shiftNum) == PIN5){
 		return GPIO_PIN_5;
 	}else if(((command & mask)>> shiftNum) == PIN6){
 		return GPIO_PIN_6;
 	}else if(((command & mask)>> shiftNum) == PIN7){
 		return GPIO_PIN_7;
 	}else if(((command & mask)>> shiftNum) == PIN8){
 		return GPIO_PIN_8;
 	}else if(((command & mask)>> shiftNum) == PIN9){
 		return GPIO_PIN_9;
 	}else if(((command & mask)>> shiftNum) == PIN10){
 		return GPIO_PIN_10;
 	}else if(((command & mask)>> shiftNum) == PIN11){
 		return GPIO_PIN_11;
 	}else if(((command & mask)>> shiftNum) == PIN12){
 		return GPIO_PIN_12;
 	}else if(((command & mask)>> shiftNum) == PIN13){
 		return GPIO_PIN_13;
 	}else if(((command & mask)>> shiftNum) == PIN14){
 		return GPIO_PIN_14;
 	}else if(((command & mask)>> shiftNum) == PIN15){
 		return GPIO_PIN_15;
	}else{
		//unknown set No Pin selected
		return 0x00;
	}
}

/*
 * send GPIOs Register States over UART to Rasberry Pi
 * Parameter:
 * command: UART received Command */
void sendGpiosRegistersState(uint16_t command){
	uint8_t buffer[REGISTERS_STATES_BUFFER_SIZE];
	GPIO_TypeDef * port = getGpioPort(command);
	if(port != NULL){
		size_t bufferOffset = 0;
		uint32_t modeRgister =  (port->MODER);
		uint32_t oTypeRegister =  (port->OTYPER);
		uint32_t oSpeedRegister =  (port->OSPEEDR);
		uint32_t pupdRegister =  (port->PUPDR);
		uint32_t idRegister =  (port->IDR);
		uint32_t odRegister =  (port->ODR);
		memcpy(buffer, &modeRgister, sizeof(uint32_t));
		bufferOffset += sizeof(uint32_t);
		memcpy(buffer + bufferOffset, &oTypeRegister, sizeof(uint32_t));
		bufferOffset += sizeof(uint32_t);
		memcpy(buffer + bufferOffset, &oSpeedRegister, sizeof(uint32_t));
		bufferOffset += sizeof(uint32_t);
		memcpy(buffer + bufferOffset, &pupdRegister, sizeof(uint32_t));
		bufferOffset += sizeof(uint32_t);
		memcpy(buffer + bufferOffset, &idRegister, sizeof(uint32_t));
		bufferOffset += sizeof(uint32_t);
		memcpy(buffer + bufferOffset, &odRegister, sizeof(uint32_t));
		HAL_UART_Transmit(&huart2, buffer, REGISTERS_STATES_BUFFER_SIZE, 10);
	}else{
		buffer[0] = 0;
		HAL_UART_Transmit(&huart2, buffer, REGISTERS_STATES_BUFFER_SIZE, 10);
	}
}

/*
 * get Temperature Sensor Value
  */
uint16_t getTemperatureSensorValue(){
	ADC_HandleTypeDef hadc  = {};
	hadc.Instance = ADC1;
	intialADC(&hadc, ADC_CHANNEL_TEMPSENSOR);
	 return readAdc(&hadc);
}

/*
 * send ADC Value Value over USART 2 to Rasberry Pi
 * Parameter:
 * adcValue: ADC measured voltage
  */
void sendAdcValue(uint16_t adcValue){
	uint8_t sendBuffer[2];
   	memset(sendBuffer, 0, sizeof(sendBuffer));
	sendBuffer[0] = (uint8_t) (adcValue);
	sendBuffer[1] = (uint8_t) (adcValue>>8);
	HAL_UART_Transmit(&huart2, sendBuffer, 2, 10);
}

/*
 * read analog pin
 * Parameter:
 * command: UART received Command */
void readAnalogValue(uint16_t command){
	ADC_HandleTypeDef hadc  = {};
 	uint16_t  gpioNumber = getGpioNumber(command, COMMAND_HANDLE_GPIO_MASK, COMMAND_HANDLE_GPIO_SHIFT);
	GPIO_TypeDef *gpioPort = getGpioPort(command);
	uint16_t adcValue = -1;
	if(gpioPort != NULL){
		if((gpioNumber == GPIO_PIN_5) && (gpioPort == GPIOC)){ //ADC2_IN11
			hadc.Instance = ADC2;
			int adcAck = intialADC(&hadc, ADC_CHANNEL_11);
 			if(adcAck == 1){
				adcValue = readAdc(&hadc);
			}else{
				adcValue = adcAck;
			}
 		}else if((gpioNumber == GPIO_PIN_12) && (gpioPort == GPIOB)){ //ADC4_IN3
			hadc.Instance = ADC4;
			int adcAck = intialADC(&hadc, ADC_CHANNEL_3);
 			if(adcAck == 1){
				adcValue = readAdc(&hadc);
			}else{
				adcValue = adcAck;
			}
		}else if((gpioNumber == GPIO_PIN_11) && (gpioPort == GPIOB)){ //ADC1_IN14
			hadc.Instance = ADC1;
			int adcAck =intialADC(&hadc, ADC_CHANNEL_14);
			if(adcAck == 1){
				adcValue = readAdc(&hadc);
			}else{
				adcValue = adcAck;
			}
 		}else if((gpioNumber == GPIO_PIN_2) && (gpioPort == GPIOB)){ //ADC2_IN12
			hadc.Instance = ADC2;
			int adcAck =intialADC(&hadc, ADC_CHANNEL_12);
			if(adcAck == 1){
				adcValue = readAdc(&hadc);
			}else{
				adcValue = adcAck;
			}
 		}else if((gpioNumber == GPIO_PIN_1) && (gpioPort == GPIOB)){ //ADC3_IN1
			hadc.Instance = ADC3;
			int adcAck =intialADC(&hadc, ADC_CHANNEL_1);
			if(adcAck == 1){
				adcValue = readAdc(&hadc);
			}else{
				adcValue = adcAck;
			}
 		}else if((gpioNumber == GPIO_PIN_15) && (gpioPort == GPIOB)){ //ADC4_IN5
			hadc.Instance = ADC4;
			int adcAck =intialADC(&hadc, ADC_CHANNEL_5);
			if(adcAck == 1){
				adcValue = readAdc(&hadc);
			}else{
				adcValue = adcAck;
			}
 		}else if((gpioNumber == GPIO_PIN_14) && (gpioPort == GPIOB)){ //ADC4_IN4
			hadc.Instance = ADC4;
			int adcAck =intialADC(&hadc, ADC_CHANNEL_4);
			if(adcAck == 1){
				adcValue = readAdc(&hadc);
			}else{
				adcValue = adcAck;
			}
 		}else if((gpioNumber == GPIO_PIN_13) && (gpioPort == GPIOB)){ //ADC3_IN5
			hadc.Instance = ADC3;
			int adcAck =intialADC(&hadc, ADC_CHANNEL_5);
			if(adcAck == 1){
				adcValue = readAdc(&hadc);
			}else{
				adcValue = adcAck;
			}
 		}else if((gpioNumber == GPIO_PIN_4) && (gpioPort == GPIOC)){ //ADC2_IN5
			hadc.Instance = ADC2;
			int adcAck = intialADC(&hadc, ADC_CHANNEL_5);
			if(adcAck == 1){
				adcValue = readAdc(&hadc);
			}else{
				adcValue = adcAck;
			}
 		}else if((gpioNumber == GPIO_PIN_7) && (gpioPort == GPIOA)){ //ADC2_IN4
			hadc.Instance = ADC2;
			int adcAck = intialADC(&hadc, ADC_CHANNEL_4);
			if(adcAck == 1){
				adcValue = readAdc(&hadc);
			}else{
				adcValue = adcAck;
			}
 		}else if((gpioNumber == GPIO_PIN_6) && (gpioPort == GPIOA)){ //ADC2_IN3
			hadc.Instance = ADC2;
			int adcAck = intialADC(&hadc, ADC_CHANNEL_3);
			if(adcAck == 1){
				adcValue = readAdc(&hadc);
			}else{
				adcValue = adcAck;
			}
 		}else if((gpioNumber == GPIO_PIN_5) && (gpioPort == GPIOA)){ //ADC2_IN2
			hadc.Instance = ADC2;
			int adcAck = intialADC(&hadc, ADC_CHANNEL_2);
			if(adcAck == 1){
				adcValue = readAdc(&hadc);
			}else{
				adcValue = adcAck;
			}
 		}else if((gpioNumber == GPIO_PIN_0) && (gpioPort == GPIOC)){ //ADC1_IN6
			hadc.Instance = ADC1;
			int adcAck = intialADC(&hadc, ADC_CHANNEL_6);
			if(adcAck == 1){
				adcValue = readAdc(&hadc);
			}else{
				adcValue = adcAck;
			}
 		}else if((gpioNumber == GPIO_PIN_1) && (gpioPort == GPIOC)){ //ADC1_IN7
			hadc.Instance = ADC1;
			int adcAck = intialADC(&hadc, ADC_CHANNEL_7);
			if(adcAck == 1){
				adcValue = readAdc(&hadc);
			}else{
				adcValue = adcAck;
			}
 		}else if((gpioNumber == GPIO_PIN_0) && (gpioPort == GPIOB)){ //ADC3_IN12
			hadc.Instance = ADC3;
			int adcAck = intialADC(&hadc, ADC_CHANNEL_12);
			if(adcAck == 1){
				adcValue = readAdc(&hadc);
			}else{
				adcValue = adcAck;
			}
 		}else if((gpioNumber == GPIO_PIN_4) && (gpioPort == GPIOA)){ //ADC2_IN1
			hadc.Instance = ADC2;
			int adcAck = intialADC(&hadc, ADC_CHANNEL_1);
			if(adcAck == 1){
				adcValue = readAdc(&hadc);
			}else{
				adcValue = adcAck;
			}
 		}else if((gpioNumber == GPIO_PIN_1) && (gpioPort == GPIOA)){ //ADC1_IN2
			hadc.Instance = ADC1;
			int adcAck = intialADC(&hadc, ADC_CHANNEL_2);
			if(adcAck == 1){
				adcValue = readAdc(&hadc);
			}else{
				adcValue = adcAck;
			}
 		}else if((gpioNumber == GPIO_PIN_0) && (gpioPort == GPIOA)){ //ADC1_IN1
			hadc.Instance = ADC1;
			int adcAck = intialADC(&hadc, ADC_CHANNEL_1);
			if(adcAck == 1){
				adcValue = readAdc(&hadc);
			}else{
				adcValue = adcAck;
			}
 		}else if((gpioNumber == GPIO_PIN_3) && (gpioPort == GPIOC)){ //ADC1_IN9
			hadc.Instance = ADC1;
			int adcAck = intialADC(&hadc, ADC_CHANNEL_9);
			if(adcAck == 1){
				adcValue = readAdc(&hadc);
			}else{
				adcValue = adcAck;
			}
 		}else if((gpioNumber == GPIO_PIN_2) && (gpioPort == GPIOC)){ //ADC1_IN8
			hadc.Instance = ADC1;
			int adcAck = intialADC(&hadc, ADC_CHANNEL_8);
			if(adcAck == 1){
				adcValue = readAdc(&hadc);
			}else{
				adcValue = adcAck;
			}
		}else{
			adcValue = UNKNOWN_ADC;
		}
		sendAdcValue(adcValue);
	}else{
		sendAdcValue(UNKNOWN_ADC);
	}
}

