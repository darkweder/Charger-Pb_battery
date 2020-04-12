#include "i2c_user.h"
//------------------------------------------------
#define I2C_REQUEST_WRITE                       0x00
#define I2C_REQUEST_READ                        0x01
#define I2C_OWNADDRESS1_7BIT             0x00004000U
#define I2C_MODE_I2C                     0x00000000U
//------------------------------------------------
__IO uint32_t tmpreg1;
//------------------------------------------------
void I2C_SendByteByADDR(I2C_TypeDef * i2c, uint8_t c,uint8_t addr)
{
  //Disable Pos
  CLEAR_BIT(i2c->CR1, I2C_CR1_POS);
  MODIFY_REG(i2c->CR1, I2C_CR1_ACK, I2C_CR1_ACK);
  SET_BIT(i2c->CR1, I2C_CR1_START);
  while (!READ_BIT(i2c->SR1, I2C_SR1_SB)){};            
  (void) i2c->SR1;
  //I2C_Write_Byte(addr);
  MODIFY_REG(i2c->DR, I2C_DR_DR, addr | I2C_REQUEST_WRITE);
  while (!READ_BIT(i2c->SR1, I2C_SR1_ADDR)){};
  (void) i2c->SR1;
  (void) i2c->SR2;
  //I2C_Write_Byte(c);
  MODIFY_REG(i2c->DR, I2C_DR_DR,c);
  while (!READ_BIT(i2c->SR1, I2C_SR1_TXE)){};
  //I2C_StopCondition();
  SET_BIT(i2c->CR1, I2C_CR1_STOP);
}
//------------------------------------------------
void I2C_Init(void)
{
  //I2C1 GPIO
  SET_BIT(RCC->APB2ENR, RCC_APB2ENR_IOPBEN);
  SET_BIT(GPIOB->CRL, GPIO_CRL_CNF7_1 | GPIO_CRL_CNF6_1 | GPIO_CRL_CNF7_0 | GPIO_CRL_CNF6_0 |\
                  GPIO_CRL_MODE7_1 | GPIO_CRL_MODE6_1 | GPIO_CRL_MODE7_0 | GPIO_CRL_MODE6_0);
  //RCC peripheral clock enabling
  SET_BIT(RCC->APB1ENR, RCC_APB1ENR_I2C1EN);
  //Delay after an RCC peripheral clock enabling
  tmpreg1 = READ_BIT(RCC->APB1ENR, RCC_APB1ENR_I2C1EN);
  //Disable  acknowledge on Own Address2 match address
  CLEAR_BIT(I2C1->OAR2, I2C_OAR2_ENDUAL);
  //Disable General Call
  CLEAR_BIT(I2C1->CR1, I2C_CR1_ENGC);
  //Enable Clock stretching
  CLEAR_BIT(I2C1->CR1, I2C_CR1_NOSTRETCH);
  //Disable I2C peripheral
  CLEAR_BIT(I2C1->CR1, I2C_CR1_PE);
  //ClockSpeed
  MODIFY_REG(I2C1->CR2, I2C_CR2_FREQ, 36);
  //MODIFY_REG(I2C1->TRISE, I2C_TRISE_TRISE, 36 + 1); //100
  MODIFY_REG(I2C1->TRISE, I2C_TRISE_TRISE, 11); //400
  //MODIFY_REG(I2C1->CCR, (I2C_CCR_FS | I2C_CCR_DUTY | I2C_CCR_CCR), 180); //100
  MODIFY_REG(I2C1->CCR, (I2C_CCR_FS | I2C_CCR_DUTY | I2C_CCR_CCR), 0x0000801E); //400
  //Set the Own Address1
  MODIFY_REG(I2C1->OAR1, I2C_OAR1_ADD0 | I2C_OAR1_ADD1_7 | I2C_OAR1_ADD8_9 | I2C_OAR1_ADDMODE, I2C_OWNADDRESS1_7BIT);
  //Configure I2C1 peripheral mode with parameter : I2C
  MODIFY_REG(I2C1->CR1, I2C_CR1_SMBUS | I2C_CR1_SMBTYPE | I2C_CR1_ENARP, I2C_MODE_I2C);
  //Enable I2C peripheral
  SET_BIT(I2C1->CR1, I2C_CR1_PE);
  //TypeAcknowledge
  MODIFY_REG(I2C1->CR1, I2C_CR1_ACK, I2C_CR1_ACK);
  //Set the 7bits Own Address2
  MODIFY_REG(I2C1->OAR2, I2C_OAR2_ADD2, 0);
}
//----------------------------------------------------------
