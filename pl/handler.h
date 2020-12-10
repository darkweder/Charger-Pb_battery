#ifndef handler_H_
#define handler_H_
//------------------------------------------------
#include "stm32f10x.h"
//------------------------------------------------
uint16_t ADC_A2, ADC_A3, ADC_A4;
 void TIM2_IRQHandler(void);
 void ADC1_2_IRQHandler(void);

//------------------------------------------------
#endif /* handler_H_ */