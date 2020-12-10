#ifndef PWM_H_
#define PWM_H_
//------------------------------------------------
#include "stm32f10x.h"
//------------------------------------------------

#define Start_PWM SET_BIT(TIM2->CR1,TIM_CR1_CEN)
#define Stop_PWM CLEAR_BIT(TIM2->CR1,TIM_CR1_CEN)
#define PRESCALER 3599
#define CYCLE     65000
#define DUTY1     9999
#define DUTY2     18999
//------------------------------------------------
void PWM_init (void);
void PWM_deinit(void);
//------------------------------------------------
#endif /* PWM_H_ */
