#include "stm32f10x.h"

void PWM_init (void);

#define Start_PWM SET_BIT(TIM2->CR1,TIM_CR1_CEN)
#define PRESCALER 36000
#define CYCLE     2000	
#define DUTY1     500
#define DUTY2     1500



