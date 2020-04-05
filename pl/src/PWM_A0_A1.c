#include "stm32f10x.h" // Device header
#include "PWM_A0_A1.h"
void PWM_init(void);

	uint16_t Negative=1;
	uint16_t Positive=19999;

void PWM_init (void)
{
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN; //ВКЛ ТАКТИРОВАНИЕ ПОРТА А 
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN; //ВКЛ ТАКТИРОВАНИЕ ТАЙМЕРА2
//настраивем порты А0 и А1
	GPIOA->CRL &=~GPIO_CRL_CNF0 | GPIO_CRL_CNF1; //ОБНУЛЯЕМ ЧТОБЫ ВЫСТАВИТЬ 1:0
	GPIOA->CRL |=GPIO_CRL_CNF0_1|GPIO_CRL_CNF1_1; //Alternate function output Push-pull
  GPIOA->CRL |=GPIO_CRL_MODE0|GPIO_CRL_MODE1; //Output mode, max speed 50 MHz	
//настраиваем таймер
	TIM2->PSC  = PRESCALER;
	TIM2->ARR  = CYCLE;
//	TIM2->CCR1 =Positive;
//	TIM2->CCR2 =Negative;
//настройки ШИМа
  TIM2->CR1 |=TIM_CR1_CMS_1; //ВЫРАВНИВАНИЕ ПО ЦЕНТРУ,
  TIM2->CCMR1 |=TIM_CCMR1_OC1M_1|TIM_CCMR1_OC1M_2; // [110] ЭТО ПРЯМОЙ ШИМ НА КХАНАЛЕ А0
	TIM2->CCMR1 |=TIM_CCMR1_OC2M; //[111] ЭТО ИНВЕРСНЫЙ ШИМ НА КХАНАЛЕ А1
	TIM2->CCER |=TIM_CCER_CC1E|TIM_CCER_CC2E; //формируется выход с 1 и 2го канала
}
