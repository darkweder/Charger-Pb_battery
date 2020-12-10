#include "stm32f10x.h"                  // Device header
#include "delay_ms.h"
#include "lcd.h"
#include "i2c_user.h"
#include <stdio.h>
#include "pwm.h"
#include "adc.h"
#include "encoder.h"
#include "math.h"

//voids*********************************************************************************************************

//voids*********************************************************************************************************


//константы*********************************************************************************************************
	uint8_t Ah;
  char str2[10];
	char str3[10];
  
//константы*********************************************************************************************************


int main(){
//порты*********************************************************************************************************
	RCC->APB2ENR |= RCC_APB2ENR_IOPCEN; // тактирование порта С
	GPIOC->CRH |= GPIO_CRH_MODE13_1 ; //2МГц скорость порта на PC13
	CLEAR_BIT(GPIOC->CRH, GPIO_CRH_CNF13); //обнуление битов CNF13
	SET_BIT(GPIOC->BSRR,GPIO_BSRR_BR13); 
//порты*********************************************************************************************************
		I2C_Init();
		LCD_ini();
		ADC_init();
		Encoder_init();
		ADC1->CR2 |=ADC_CR2_JSWSTART;     //запустить процес преобразования ADC	
		
    //Приветствие с указанием прошивки и типа акб
	LCD_SetPos(1,0);
  LCD_String("Charger 12V Pb ");  //тип акб
	LCD_SetPos(1,1);
  LCD_String("V.01 darkweder");  // версия
  delay_ms(3000);  
	LCD_Clear();
	
	while (READ_BIT(GPIOA->IDR, GPIO_IDR_IDR5))
			{
				//LCD_Clear();
				TIM3->ARR = 83 ;
				Ah=(TIM3->CNT)/4*5;
				LCD_SetPos(3,0);
				sprintf(str2,"Select %dAh", Ah);
				LCD_String(str2);
				delay_ms(30);
			}
	while(1) {
	          LCD_Clear();
						//проверка акб перед запуском зарядки
	          if(307<(ADC1->DR) & (ADC1->DR)<3686) //НРЦ в пределах от 1,2В до 14,4В
						{					 
						 
						 LCD_SetPos(0,0);
             LCD_String("Charging");
						 while(307<(ADC1->DR) & (ADC1->DR)<3686) //входим в цикл при котором выводим напряжение и следим, чтобы не выйти за 14,4В и не ниже 5В
						 {
						 int Ic, Id;
						 if(!(READ_BIT(TIM2->CR1, TIM_CR1_CEN)))
						 {
						 PWM_init();
						 }
						 if((ADC1->DR)>3455 & (ADC1->DR)<3686) // V<14,4
						 {
						 
						  Ic=CYCLE*Ah*8/975;
							Id=CYCLE*Ah/7800;
						  WRITE_REG (TIM2->CCR1, (Ic)); //[[x=(Ah/10*2*P)/(Ic*(P-1)),y=Ah/10*2/(Id*(P-1))]] P=40, Ic=25, Id=40
							WRITE_REG (TIM2->CCR2, (Id));	
						 }
						 else{
						 if((ADC1->DR)>3327 &(ADC1->DR)<3455) //V<13,5
						 {
						 Ic=CYCLE*Ah/120;
						 Id=round(CYCLE*Ah/4800);
						 WRITE_REG (TIM2->CCR1, Ic); //[[x=(Ah/10*2*P)/(Ic*(P-1)),y=Ah/10*2/(Id*(P-1))]] P=25, Ic=25, Id=40
						 WRITE_REG (TIM2->CCR2, Id);
						 }
						 else{
						 if((ADC1->DR)>3199 &(ADC1->DR)<3327) //V<13
						 {
						 Ic=CYCLE*Ah*4/475;
						 Id=CYCLE*Ah/3800;
						 WRITE_REG (TIM2->CCR1, (Ic)); //[[x=(Ah/10*2*P)/(Ic*(P-1)),y=Ah/10*2/(Id*(P-1))]] P=20, Ic=25, Id=40
							WRITE_REG (TIM2->CCR2, (Id));
						 }
						 else{
						 if((ADC1->DR)>3071 &(ADC1->DR)<3199) //V<12,5
						 {
						 Ic=CYCLE*Ah*3/350;
						 Id=CYCLE*Ah/2800;
						 WRITE_REG (TIM2->CCR1, (Ic)); //[[x=(Ah/10*2*P)/(Ic*(P-1)),y=Ah/10*2/(Id*(P-1))]] P=15, Ic=25, Id=40
							WRITE_REG (TIM2->CCR2, (Id));
						 }
						 else{
						 if((ADC1->DR)<3071) //V<12
						 {
						 Ic=CYCLE*Ah*2/225;
						 Id=CYCLE*Ah/1800;
						 WRITE_REG (TIM2->CCR1, (Ic)); //[[x=(Ah/10*2*P)/(Ic*(P-1)),y=Ah/10*2/(Id*(P-1))]] P=10, Ic=25, Id=40
							WRITE_REG (TIM2->CCR2, (Id));
						 }
						 else{}
						 }
						 }
						 }
						 }
						  if(!(READ_BIT(TIM2->CR1, TIM_CR1_CEN)))
						 {
						  Start_PWM;
						 }
						 	LCD_SetPos(9,0);
							sprintf(str2,"%4.2fV", (ADC1->DR/(float)4095*16)); //выводим значение В АКБ
							LCD_String(str2);
							LCD_SetPos(0,1);
							sprintf(str3,"%d %d", (TIM2->CCR2), (TIM2->CCR1)); // ток в цепи в А
							LCD_String(str3);
							delay_ms(100);
						 }
						 }
						else{
						if((ADC1->DR)<307) //напряжение упало ниже нормы, всё останавливаем
						{
						 Stop_PWM;
						 PWM_deinit();
						 LCD_SetPos(0,0);
             LCD_String("Not battery");
						}
						 else{
						 if((ADC1->DR)>3790)
						 {
						  Stop_PWM;
							PWM_deinit();
						  LCD_SetPos(0,0);
              LCD_String("wrong battery");
							}
						       } 
						      }
						
							
						delay_ms(185);
	         }
					 }

//прерывания*********************************************************************************************************

 void TIM2_IRQHandler(void)
{  
   TIM2->SR &=~TIM_SR_CC1IF;
	// ADC1->CR2    |=  ADC_CR2_JSWSTART;     //запустить процес преобразования
	 GPIOC->ODR ^=GPIO_ODR_ODR13;
	 
}

 void ADC1_2_IRQHandler(void)
{
  	{
	 ADC1->SR &=~ADC_SR_JEOC;
	 GPIOC->ODR ^=GPIO_ODR_ODR13;

	// ADC1->CR2 &=~ADC_CR2_JSWSTART;     //остановить процес преобразования
	}
}

//прерывания********************************************************************************************************
