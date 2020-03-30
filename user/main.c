#include "stm32f10x.h"                  // Device header
#include "delay_ms.h"
#include "PWM_A0_A1.h"

int main(){
	
	RCC->APB2ENR |= RCC_APB2ENR_IOPCEN; // тактирование порта С
	GPIOC->CRH |= GPIO_CRH_MODE13_1 ; //2МГц скорость порта на PC13
	CLEAR_BIT(GPIOC->CRH, GPIO_CRH_CNF13); //обнуление битов CNF13
	CLEAR_BIT(GPIOC->CRH,GPIO_ODR_ODR13); //подтяжка вывода к +
	
	PWM_init ();

	
	while(1) {
		Start_PWM;
		
		GPIOC->BSRR |= GPIO_BSRR_BS13;
		delay_ms(50);
		GPIOC->BSRR |= GPIO_BSRR_BR13;
		delay_ms(50);
		

	         }
          }
