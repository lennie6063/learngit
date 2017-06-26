#ifndef _BSP_H_
#define _BSP_H_

#include "includes.h"

//****************LED�궨��**********************//
#define LED_CLK      RCC_APB2Periph_GPIOB
#define LED_PORT     GPIOB
#define LED1_PIN     GPIO_Pin_0
#define LED2_PIN     GPIO_Pin_1

#define LED1_ON()    GPIO_ResetBits(LED_PORT,LED1_PIN )
#define LED1_OFF()   GPIO_SetBits(LED_PORT,LED1_PIN )
#define LED2_ON()    GPIO_ResetBits(LED_PORT,LED2_PIN)
#define LED2_OFF()   GPIO_SetBits(LED_PORT,LED2_PIN)

#define LED1_FLAG    GPIO_ReadOutputDataBit(LED_PORT,LED1_PIN)
#define LED2_FLAG    GPIO_ReadOutputDataBit(LED_PORT,LED2_PIN)

//****************�������궨��**********************//
#define BEEP_OFF      GPIO_ResetBits(GPIOA,GPIO_Pin_4)
#define BEEP_ON       GPIO_SetBits(GPIOA,GPIO_Pin_4)


//****************�����궨��**********************//
#define KEY_CLK      RCC_APB2Periph_GPIOB
#define KEY_PORT     GPIOB
#define KEY1_PIN     GPIO_Pin_5
#define KEY2_PIN     GPIO_Pin_6
#define KEY1_FLAG    GPIO_ReadInputDataBit(KEY_PORT,KEY1_PIN)
//********************************************************************//
#define HY_CLK      RCC_APB2Periph_GPIOB
#define HY_PORT     GPIOB
#define HY_PIN      GPIO_Pin_5
#define HY_FLAG     GPIO_ReadInputDataBit(HY_PORT,HY_PIN)


//****************�̵����궨��**********************//
#define JDQ_CLK       RCC_APB2Periph_GPIOA
#define JDQ_PORT      GPIOA
#define JDQ1_PIN      GPIO_Pin_5
#define JDQ2_PIN      GPIO_Pin_6
#define JDQ1_OFF      GPIO_ResetBits(JDQ_PORT ,JDQ1_PIN) 
#define JDQ1_ON       GPIO_SetBits(JDQ_PORT ,JDQ1_PIN)
#define JDQ2_OFF      GPIO_ResetBits(JDQ_PORT ,JDQ2_PIN) 
#define JDQ2_ON       GPIO_SetBits(JDQ_PORT ,JDQ2_PIN)

//****************BSP��������**********************//
void BSP_Config(void);


//****************LED��������**********************//
void LED_Config(void);
void LED1_Flip(void);
void LED2_Flip(void);


//****************��������������**********************//
void BEEP_Config(void);


void HY_Config(void);


//***************������������**********************//
void KEY_Config(void);

//***************�̵�����������**********************//
void JDQ_Config(void);

//***************USART��������**********************//
void USART_Config(void);


//***************NVIC��������**********************//
void NVIC_Config(void);


//void DMA_Config(void);
u16 GetAdc(void);

void ADC_Config(void);

void EXTI_Config(void);
//***************fputc��������**********************//
//int fputc(int ch , FILE *f);

#endif






