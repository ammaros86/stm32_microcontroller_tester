/*
 * adcHandler.h
 *
 *  Created on: Jul 27, 2023
 *      Author: Astra
 */

#ifndef INC_ADC_HANDLER_H_
#define INC_ADC_HANDLER_H_
#include "stm32f3xx_hal.h"

#define ADC_INIT_ERROR 0x3000
#define ADC_SET_CHANNEL_ERROR 0x5000
#define ADC_START_ERROR 0x7000
#define ADC_POLL_CONVERSION_ERROR 0x9000
#define ADC_STOP_ERROR 0xB000
#define UNKNOWN_ADC 0xF000


int intialADC(ADC_HandleTypeDef * hadc, uint32_t channel);
uint16_t readAdc(ADC_HandleTypeDef * hadc);

#endif /* INC_ADC_HANDLER_H_ */
