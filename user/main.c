#include "stm32f10x.h"                  // Device header
#include "delay_ms.h"
#include "lcd.h"
#include "i2c_user.h"
#include <stdio.h>

uint16_t ADC_A2, ADC_A3;

//voids*********************************************************************************************************
#define Start_PWM SET_BIT(TIM2->CR1,TIM_CR1_CEN)
#define PRESCALER 179
#define CYCLE     200
#define DUTY1     99
#define DUTY2     189

void PWM_init (void);
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


void ADC_init(void);
void ADC_Init(void)
{  
   NVIC->ISER[0] |=NVIC_ISER_SETENA_18;  // включаем прерывание от ацп1_2
	 
   RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;   //разрешаем тактирование порта А
	 RCC->APB2ENR |= RCC_APB2ENR_ADC1EN ;  //разрешаем тактирование АЦП
	 RCC->CFGR |= RCC_CFGR_ADCPRE_DIV8;    //так как тактовая частота АЦП не должна превышать 14MHz  72MHz/8=9MHz
	 
   GPIOA->CRL &= ~GPIO_CRL_CNF2;         //настраиваем вывод A2 для работы АЦП в режим аналогового входа 
   GPIOA->CRL &= ~GPIO_CRL_CNF3;         //настраиваем вывод A3 для работы АЦП в режим аналогового входа 
	 
   ADC1->CR1 =0;
	 ADC1->CR2 |=ADC_CR2_CAL;              //запуск калибровки, 1 — запустить калибровку, 
	 while (!(ADC1->CR2 & ADC_CR2_CAL));      //0 — устанавливается аппаратно по завершении калибровки 
	 
	
	 ADC1->CR2 |=ADC_CR2_CONT;             //режим преобразования, 0 — однократный, 1 — непрерывный.
	 ADC1->CR2 |=ADC_CR2_JEXTSEL;          //источника запуска инжектированных каналов JSWSTART
	 ADC1->CR2 |=  ADC_CR2_JEXTTRIG;       //разр. внешний запуск инжектированной группы
	 
	 ADC1->CR1 |= ADC_CR1_JEOCIE;          //разрешение прерывания по окончании инжектированного преобразования	 
	 ADC1->CR1 |= ADC_CR1_JAUTO;           //непрерывное преобразование инжектированных каналов
	 ADC1->CR1 |= ADC_CR1_SCAN;            //режим сканирования (т.е. несколько каналов)
	 
	 ADC1->SMPR2 |=ADC_SMPR2_SMP2;       //время между выборками 235,5циклов А2
	 ADC1->SMPR2 |=ADC_SMPR2_SMP3;       //время между выборками 239,5циклов А3
	 
	 ADC1->JSQR |=ADC_JSQR_JL_0;           //длина  последовательности 2 преобразования
	 ADC1->JSQR |=ADC_JSQR_JSQ3_1;         //Номер секции JSQx определяет номер в группе инжектированных преобразований,
                                         //а число записанное в секцию определяет номер канала. 
   ADC1->JSQR |=ADC_JSQR_JSQ4_1|ADC_JSQR_JSQ4_0;
	 
	 ADC1->CR2    |=  ADC_CR2_ADON;         //включить АЦП
   ADC1->CR2    |=  ADC_CR2_JSWSTART;     //запустить процес преобразования
	 
	 }
//voids*********************************************************************************************************

//прерывания*********************************************************************************************************

 void TIM2_IRQHandler(void)
{  
   TIM2->SR &=~TIM_SR_CC1IF;
	 //ADC1->CR2    |=  ADC_CR2_JSWSTART;     //запустить процес преобразования
	 //GPIOC->ODR |=GPIO_ODR_ODR13;
	 //ADC1->CR2 |=ADC_CR2_ADON;
}

void ADC1_2_IRQHandler(void)
{
  	{
	 ADC1->SR &=~ADC_SR_JEOC;
	 GPIOC->ODR ^=GPIO_ODR_ODR13;
	 ADC_A2 = ADC1->JDR1;
	 ADC_A3 = ADC1->JDR2;
	 //ADC1->CR2 &=~ADC_CR2_ADON;
	}
}


//прерывания*********************************************************************************************************

int main(){
//порты*********************************************************************************************************
	
	RCC->APB2ENR |= RCC_APB2ENR_IOPCEN; // тактирование порта С
	GPIOC->CRH |= GPIO_CRH_MODE13_1 ; //2МГц скорость порта на PC13
	CLEAR_BIT(GPIOC->CRH, GPIO_CRH_CNF13); //обнуление битов CNF13
	SET_BIT(GPIOC->BSRR,GPIO_BSRR_BR13); 
//порты*********************************************************************************************************
  
		I2C_Init();
		LCD_ini();
		ADC_Init();
		PWM_init();
		Start_PWM;
    ADC1->CR2 |=ADC_CR2_JSWSTART;

	
	
//константы*********************************************************************************************************
  
  char str1[10];
  
//константы*********************************************************************************************************
	

	while(1) {
	
  LCD_SetPos(0,0);
  LCD_String("String 1");
  LCD_SetPos(0,1);
  
    sprintf(str1,"%3.2f %3.2f", ADC_A3/(float)4095*3.305, ADC_A2/(float)4095*3.305);
    LCD_String(str1);
		LCD_SetPos(8,1);
    delay_ms(40);
   
	
	         }

          }

