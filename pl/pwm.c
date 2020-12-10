#include "pwm.h"
#include "stm32f10x.h"                  // Device header
//----------------------
void PWM_deinit(void)
{
  RCC->APB1ENR &= ~RCC_APB1ENR_TIM2EN; //ВыКЛ ТАКТИРОВАНИЕ ТАЙМЕРА2
	//Возвращаем стандартное состояние пинов
	GPIOA->CRL &=~GPIO_CRL_CNF0;
	GPIOA->CRL &=~GPIO_CRL_CNF1;
	GPIOA->BSRR |= GPIO_BSRR_BR0|GPIO_BSRR_BR1;
	
}
void PWM_init (void)
{
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN; //ВКЛ ТАКТИРОВАНИЕ ПОРТА А 
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN; //ВКЛ ТАКТИРОВАНИЕ ТАЙМЕРА2
//настраивем порты А0 и А1
	GPIOA->CRL &=~GPIO_CRL_CNF0 | GPIO_CRL_CNF1; //ОБНУЛЯЕМ ЧТОБЫ ВЫСТАВИТЬ 1:0
	GPIOA->CRL |=GPIO_CRL_CNF0_1|GPIO_CRL_CNF1_1; //Alternate function output Push-pull
  GPIOA->CRL |=GPIO_CRL_MODE0|GPIO_CRL_MODE1; //Output mode, max speed 50 MHz	

//настройки ШИМа
  TIM2->CR1 |=TIM_CR1_CMS_1; //ВЫРАВНИВАНИЕ ПО ЦЕНТРУ,
  TIM2->CCMR1 |=TIM_CCMR1_OC1M_1|TIM_CCMR1_OC1M_2; // [110] ЭТО ПРЯМОЙ ШИМ НА КХАНАЛЕ А0
	TIM2->CCMR1 |=TIM_CCMR1_OC2M; //[111] ЭТО ИНВЕРСНЫЙ ШИМ НА КХАНАЛЕ А1
	TIM2->CCER |=TIM_CCER_CC1E|TIM_CCER_CC2E; //формируется выход с 1 и 2го канала
	
	//прерывания
	NVIC_EnableIRQ(TIM2_IRQn);	
	TIM2->DIER |= TIM_DIER_CC1IE; //активировали прерывание от таймера2 по 
	TIM2->SR &=~TIM_SR_CC1IF;
	TIM2->SR  &=~TIM_SR_UIF;
	//TIM2->CR2 |=TIM_CR2_MMS_0|TIM_CR2_MMS_1; //прерывание по сигналу тим2А1 для перефирии
	//настраиваем таймер
	TIM2->PSC  = PRESCALER;
	TIM2->ARR  = CYCLE;
	WRITE_REG (TIM2->CCR1, DUTY1);
	WRITE_REG (TIM2->CCR2, DUTY2);	
}

