#include "stm32f10x.h"                  // Device header
#include "handler.h"
//прерывания*********************************************************************************************************

 void TIM2_IRQHandler(void)
{  
   TIM2->SR &=~TIM_SR_CC1IF;
	 ADC1->CR2    |=  ADC_CR2_JSWSTART;     //запустить процес преобразования
	 GPIOC->ODR ^=GPIO_ODR_ODR13;
	 
}

 void ADC1_2_IRQHandler(void)
{
  	{
	 ADC1->SR &=~ADC_SR_JEOC;
	 GPIOC->ODR ^=GPIO_ODR_ODR13;
	 ADC_A2 = ADC1->JDR1;
	 ADC_A3 = ADC1->JDR2;
	 ADC1->CR2 &=~ADC_CR2_JSWSTART;     //остановить процес преобразования
	}
}

//прерывания********************************************************************************************************