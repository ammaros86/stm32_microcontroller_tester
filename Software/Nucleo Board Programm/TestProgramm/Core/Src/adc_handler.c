
#include <adc_handler.h>


/*
 * initialize ADC
 * Parameter:
 * hadc: ADC_HandleTypeDef to define which ADC
* channel: ADC Channel
  */
int intialADC(ADC_HandleTypeDef * hadc, uint32_t channel){
	  ADC_ChannelConfTypeDef sConfig = {0};
	  hadc->Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV1;
	  hadc->Init.Resolution = ADC_RESOLUTION_12B;
	  hadc->Init.ScanConvMode = ADC_SCAN_DISABLE;
	  hadc->Init.ContinuousConvMode = DISABLE;
	  hadc->Init.DiscontinuousConvMode = DISABLE;
	  hadc->Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
	  hadc->Init.ExternalTrigConv = ADC_SOFTWARE_START;
	  hadc->Init.DataAlign = ADC_DATAALIGN_RIGHT;
	  hadc->Init.NbrOfConversion = 1;
	  hadc->Init.DMAContinuousRequests = DISABLE;
	  hadc->Init.EOCSelection = ADC_EOC_SINGLE_CONV;
	  hadc->Init.LowPowerAutoWait = DISABLE;
	  hadc->Init.Overrun = ADC_OVR_DATA_OVERWRITTEN;
	  if (HAL_ADC_Init(hadc) != HAL_OK)
	  {
		  return ADC_INIT_ERROR;
	  }
	  sConfig.Channel = channel;
	  sConfig.Rank = ADC_REGULAR_RANK_1;
	  sConfig.SingleDiff = ADC_SINGLE_ENDED;
	  sConfig.SamplingTime = ADC_SAMPLETIME_181CYCLES_5;
	  sConfig.OffsetNumber = ADC_OFFSET_NONE;
	  sConfig.Offset = 0;
	  if (HAL_ADC_ConfigChannel(hadc, &sConfig) != HAL_OK)
	  {
	    return ADC_SET_CHANNEL_ERROR;
	  }
	  return 1;
}

/*
 * readAdc value
 * Parameter:
 * hadc: ADC_HandleTypeDef to define which ADC
  */
uint16_t readAdc(ADC_HandleTypeDef * hadc){
	uint16_t adcValue = 0;
	if(HAL_ADC_Start(hadc) != HAL_OK){
		return ADC_START_ERROR;
	}
	if(HAL_ADC_PollForConversion(hadc, 300) == HAL_OK){
	  adcValue = HAL_ADC_GetValue(hadc);
	}else{
		return ADC_POLL_CONVERSION_ERROR;
	}
	if(HAL_ADC_Stop(hadc) != HAL_OK){
		return ADC_STOP_ERROR;
	}
 	return adcValue;
}
