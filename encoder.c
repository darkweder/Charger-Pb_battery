#include "encoder.h"
#include "stm32f10x.h"                  // Device header

void Encoder_init(void){
//A6 & A7
RCC->APB2ENR |=RCC_APB2ENR_IOPAEN;
RCC->APB1ENR |=RCC_APB1ENR_TIM3EN;
RCC->APB2ENR |=RCC_APB2ENR_AFIOEN;
GPIOA->CRL |= (GPIO_CRL_CNF6_1|GPIO_CRL_CNF7_1); //[10] Input with pull-up / pull-down


TIM3->ARR = ENCODER ;
TIM3->CCMR1 |=TIM_CCMR1_CC1S_0;
TIM3->CCMR1 |=TIM_CCMR1_CC2S_1;

TIM3->CCMR1 |=(TIM_CCMR1_IC1F_0|TIM_CCMR1_IC1F_1|TIM_CCMR1_IC2F_0|TIM_CCMR1_IC2F_1);

TIM3->CCER &=~(TIM_CCER_CC1P | TIM_CCER_CC2P);
TIM3->SMCR |= (TIM_SMCR_SMS_0|TIM_SMCR_SMS_1);

TIM3->CR1 |=TIM_CR1_CEN;
//PA6 button
MODIFY_REG(GPIOA->CRL, GPIO_CRL_CNF5|GPIO_CRL_MODE5, GPIO_CRL_CNF5_1 );
CLEAR_BIT (GPIOA->ODR, GPIO_ODR_ODR5);
}