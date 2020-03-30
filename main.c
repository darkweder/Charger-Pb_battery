#include "stm32f10x.h"                  // Device header
#include "delay_ms.h"
#include "stdint.h"
#include "PWM_A0_A1.h"

#define Start_PWM SET_BIT(TIM2->CR1, TIM_CR1_CEN)
int main(){
	
	RCC->APB2ENR |= RCC_APB2ENR_IOPCEN; // тактирование порта С
	GPIOC->CRH |= GPIO_CRH_MODE13_1 ; //2МГц скорость порта на PC13
	CLEAR_BIT(GPIOC->CRH, GPIO_CRH_CNF13); //обнуление битов CNF13
	CLEAR_BIT(GPIOC->CRH,GPIO_ODR_ODR13); //подтяжка вывода к +
	
	PWM_init();
	//настройка uasrt1//
	//RCC->APB2ENR |=RCC_APB2ENR_USART1EN;
	
	while(1) {
		Start_PWM;
		
		GPIOC->BSRR |= GPIO_BSRR_BS13;
		delay_ms(50);
		GPIOC->BSRR |= GPIO_BSRR_BR13;
		delay_ms(50);
		

	         }
          }
