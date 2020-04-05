#include "stm32f10x.h"                  // Device header
#include "delay_ms.h"
#include "PWM_A0_A1.h"
//прерывания*********************************************************************************************************

 void TIM2_IRQHandler(void)
{  
   TIM2->SR &=~TIM_SR_CC1IF;
	 //TIM2->SR &=~TIM_SR_UIF;
	 GPIOC->ODR ^=GPIO_ODR_ODR13;  
}

//прерывания*********************************************************************************************************

int main(){
	
	RCC->APB2ENR |= RCC_APB2ENR_IOPCEN; // тактирование порта С
	GPIOC->CRH |= GPIO_CRH_MODE13_1 ; //2МГц скорость порта на PC13
	CLEAR_BIT(GPIOC->CRH, GPIO_CRH_CNF13); //обнуление битов CNF13
	SET_BIT(GPIOC->BSRR,GPIO_BSRR_BR13); 
	
	PWM_init ();
	Start_PWM;
	TIM2->PSC  = 3600;
	WRITE_REG (TIM2->CCR1, 10000);
	WRITE_REG (TIM2->CCR2, 19000);	
	//прерывания
	
	//NVIC_SetPriority (TIM2_IRQn, 5);
	NVIC_EnableIRQ(TIM2_IRQn);	
  
	TIM2->DIER |= TIM_DIER_CC1IE; //активировали прерывание от таймера2 по 
	TIM2->SR &=~TIM_SR_CC1IF;
	TIM2->SR  &=~TIM_SR_UIF;
	

	while(1) {
	
	delay_ms(500);
	
	         }

          }

