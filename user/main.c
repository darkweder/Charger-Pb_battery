#include "stm32f10x.h"                  // Device header
#include "delay_ms.h"
#include "PWM_A0_A1.h"
//прерывания*********************************************************************************************************

//прерывания*********************************************************************************************************

int main(){
	
	RCC->APB2ENR |= RCC_APB2ENR_IOPCEN; // тактирование порта С
	GPIOC->CRH |= GPIO_CRH_MODE13_1 ; //2МГц скорость порта на PC13
	CLEAR_BIT(GPIOC->CRH, GPIO_CRH_CNF13); //обнуление битов CNF13
	SET_BIT(GPIOC->BSRR,GPIO_BSRR_BS13); //подтяжка вывода к +
	
	
	//прерывания
	__enable_irq ();
		
	TIM2->DIER |= TIM_DIER_UIE; //активировали прерывание от таймера2
	TIM2->DIER |=TIM_DIER_CC1IE;
	TIM2->EGR |= TIM_EGR_CC1G;
	//TIM2->SR|=TIM_SR_CC1IF;
	
	//
	
//	int i, u;



	
	while(1) {
//	SET_BIT(GPIOC->BSRR, GPIO_BSRR_BS13);
//	delay_ms (1000);
//	SET_BIT(GPIOC->BSRR, GPIO_BSRR_BR13);
	delay_ms(500);
	PWM_init ();
		Start_PWM;
	TIM2->PSC  = 3600;
	WRITE_REG (TIM2->CCR1, 10000);
	WRITE_REG (TIM2->CCR2, 19000);
//		WRITE_REG (TIM2->CCR1, 12000);
//		WRITE_REG (TIM2->CCR2, 17000);
		//delay_ms(100);
		
		
		
//		GPIOC->BSRR |= GPIO_BSRR_BS13;
//		delay_ms(5000);
//		GPIOC->BSRR |= GPIO_BSRR_BR13;
//		delay_ms(5000);
		

	         }

          }
 void TIM2_IRQHandler(void)
{  SET_BIT(GPIOC->BSRR,GPIO_BSRR_BR13);

   TIM2->SR   = 0;
  
}

