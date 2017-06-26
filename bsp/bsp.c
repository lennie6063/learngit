//****************BSP����ʵ�ֿ�����������豸��ʼ��******************//
#include "bsp.h"


//__IO uint16_t ad[1];

//****************BSP����ʵ��******************//



void BSP_Config(void)
{
	 //  LED_Config();
      BEEP_Config();
	    HY_Config();
   //  KEY_Config();
	    JDQ_Config();
     USART_Config();
	
	   EXTI_Config();
  //   ADC_Config();
  	 NVIC_Config();
}
//****************BEEP������ʼ��***************//
void BEEP_Config(void)
{

  GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_2MHz;
  GPIO_Init(GPIOA,&GPIO_InitStructure);	
  BEEP_OFF; 
	

}

void HY_Config(void)
{

  GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(HY_CLK,ENABLE);
	GPIO_InitStructure.GPIO_Pin=HY_PIN;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_2MHz;
  GPIO_Init(HY_PORT,&GPIO_InitStructure);	


}
//****************LED������ʼ��***************//
void LED_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(LED_CLK, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin=LED1_PIN|LED2_PIN;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	LED1_OFF(); LED2_OFF();
	GPIO_Init(LED_PORT,&GPIO_InitStructure);
 
  
}
//****************LED��ת����ʵ��***************//
void LED1_Flip(void)
{
   if(LED1_FLAG ==0)
	 {
	   LED1_OFF();
	 }
   else
	 {
     LED1_ON();
	 }
    	      
}
void LED2_Flip(void)
{
   if(LED2_FLAG ==0)
	 {
	   LED2_OFF();
	 }
   else
	 {
     LED2_ON(); 
	 }		 
}
//****************KEY������ʼ��***************//
void KEY_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(KEY_CLK, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin=KEY1_PIN|KEY2_PIN;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	GPIO_Init(KEY_PORT,&GPIO_InitStructure);
	GPIO_SetBits(KEY_PORT ,KEY1_PIN);
	GPIO_SetBits(KEY_PORT ,KEY2_PIN);
}
//****************JDQ������ʼ��***************//
void JDQ_Config()
{
  GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(JDQ_CLK ,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin=JDQ1_PIN|JDQ2_PIN;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_2MHz;
  GPIO_Init(JDQ_PORT,&GPIO_InitStructure);
	JDQ1_OFF;
	JDQ2_OFF;
  	 
}
//****************USART������ʼ���Լ���������***************//
void USART_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA,&GPIO_InitStructure);

	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
		
	
/**************************COM1����****************************************/	
	USART_InitStructure.USART_BaudRate=38400;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1, &USART_InitStructure);
	USART_Cmd(USART1,ENABLE);
//	
//	/**************************COM2����****************************************/	
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO,ENABLE);
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
//	
//	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_2;
//	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
//	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
//	GPIO_Init(GPIOA,&GPIO_InitStructure);

//	
//	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_3;
//	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
//	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;
//	GPIO_Init(GPIOA,&GPIO_InitStructure);
//	
//	USART_InitStructure.USART_BaudRate=115200;
//	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
//	USART_InitStructure.USART_StopBits = USART_StopBits_1;
//	USART_InitStructure.USART_Parity = USART_Parity_No;
//	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
//	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
//	USART_Init(USART2, &USART_InitStructure);
//	USART_Cmd(USART2,ENABLE);
	
	
//	/**************************COM3����****************************************/	
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO,ENABLE);//
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);

//	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_10;
//	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
//	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
//	GPIO_Init(GPIOB,&GPIO_InitStructure);

//	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_11;
//	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
//	GPIO_InitStructure.GPIO_Mode= GPIO_Mode_IN_FLOATING;
//	GPIO_Init(GPIOB,&GPIO_InitStructure);
//	
//	USART_InitStructure.USART_BaudRate=38400;
//	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
//	USART_InitStructure.USART_StopBits = USART_StopBits_1;
//	USART_InitStructure.USART_Parity = USART_Parity_No;
//	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
//	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
//	USART_Init(USART3, &USART_InitStructure);
//	USART_Cmd(USART3,ENABLE);
	
	

	
	
}    
//************************NVIC������ʼ��***************************//
void NVIC_Config(void)
{
    NVIC_InitTypeDef  NVIC_InitStructure;		
  	USART_ITConfig(USART1, USART_IT_RXNE , ENABLE);	
	  NVIC_InitStructure.NVIC_IRQChannel=USART1_IRQn ;  
	  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;
	  NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;
	  NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	  NVIC_Init(&NVIC_InitStructure);
	
//  	USART_ITConfig(USART3, USART_IT_RXNE , ENABLE);	
//	  NVIC_InitStructure.NVIC_IRQChannel=USART3_IRQn ;   
//	  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;
//	  NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;
//	  NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
//	  NVIC_Init(&NVIC_InitStructure);
	
//	 	USART_ITConfig(USART2, USART_IT_RXNE , ENABLE);	
//	  NVIC_InitStructure.NVIC_IRQChannel=USART2_IRQn ;   // ��stm32f10x_it.h�ļ���186 ö�ٱ���
//	  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;
//	  NVIC_InitStructure.NVIC_IRQChannelSubPriority=2;
//	  NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
//	  NVIC_Init(&NVIC_InitStructure);
//		
		
	
	  NVIC_InitStructure.NVIC_IRQChannel=EXTI9_5_IRQn ;   // ��stm32f10x_it.h�ļ���186 ö�ٱ���
	  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x07;
	  NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x07;
	  NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	  NVIC_Init(&NVIC_InitStructure);
	
//	  NVIC_InitStructure.NVIC_IRQChannel=EXTI9_5_IRQn ;
//	  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x07;
//	  NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x07;
//	  NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
//	  NVIC_Init(&NVIC_InitStructure);
	
	

}
//**********printf���øú���*****************************************//
int fputc(int ch, FILE *f)
{      
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==0);
	USART_SendData(USART1,ch);
	return ch;
}

//void DMA_Config(void)
//{
//	

//	 DMA_InitTypeDef DMA_InitStructure; 
//   RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
// 
//  /* DMA1 channel1 configuration ----------------------------------------------*/
//  DMA_DeInit(DMA1_Channel1);
//  DMA_InitStructure.DMA_PeripheralBaseAddr =(uint32_t)&ADC1->DR ;    //DMA��Ӧ���������ַ
//  DMA_InitStructure.DMA_MemoryBaseAddr =(uint32_t)&ad[0];   //�ڴ�洢����ַ
//  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;	//DMA��ת��ģʽΪSRCģʽ����������Ƶ��ڴ�
//  DMA_InitStructure.DMA_BufferSize = 1;		   //DMA�����С��1��,��λΪDMA_MemoryDataSize
//  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;	//����һ�����ݺ��豸��ַ��ֹ����
//  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable;	//�رս���һ�����ݺ�Ŀ���ڴ��ַ����
//  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;  //�����������ݿ��Ϊ16λ
//  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;  //DMA�����ݳߴ磬HalfWord����Ϊ16λ
//  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;   //ת��ģʽ��ѭ������ģʽ��
//  DMA_InitStructure.DMA_Priority = DMA_Priority_High;	//DMA���ȼ���
//  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;		  //M2Mģʽ����
//  DMA_Init(DMA1_Channel1, &DMA_InitStructure);          
//  
//  /* Enable DMA1 channel1 */
//  DMA_Cmd(DMA1_Channel1, ENABLE);
//	
//	
//	

//}


void ADC_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	ADC_InitTypeDef ADC_InitStructure; 

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1|RCC_APB2Periph_AFIO  ,ENABLE);
	
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);   //����ADC��Ƶ����6 72M/6=12,ADCʱ�Ӳ��ܳ���14M

	/* PC4 ��Ϊģ��ͨ����������   */                      
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		//ģ����������
	GPIO_Init(GPIOC, &GPIO_InitStructure);	

	ADC_DeInit(ADC1);  //��λADC1,������ ADC1 ��ȫ���Ĵ�������Ϊȱʡֵ

	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	//ADC����ģʽ:ADC1�����ڶ���ģʽ(������������������adcģ�����)
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;	//ģ��ת�������ڵ�ͨ��ģʽ��ɨ��ģʽ������
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;	//ģ��ת�������ڵ���ת��ģʽ
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//ת��������������ⲿ��������
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	//ADC�����Ҷ���
	ADC_InitStructure.ADC_NbrOfChannel = 1;	//˳����й���ת����ADCͨ��������
	ADC_Init(ADC1, &ADC_InitStructure);	//����ADC_InitStruct��ָ���Ĳ�����ʼ������ADCx�ļĴ���   

  
	ADC_Cmd(ADC1, ENABLE);	//ʹ��ָ����ADC1
	
	ADC_ResetCalibration(ADC1);	//ʹ�ܸ�λУ׼  	 
	while(ADC_GetResetCalibrationStatus(ADC1));	//�ȴ���λУ׼����
	
	ADC_StartCalibration(ADC1);	 //����ADУ׼
	while(ADC_GetCalibrationStatus(ADC1));	 //�ȴ�У׼���� 

}

u16 GetAdc(void)   
{
  /*����ָ��ADC�Ĺ�����ͨ����һ�����У�����ʱ��239.5������*/
	ADC_RegularChannelConfig(ADC1, ADC_Channel_14, 1, ADC_SampleTime_239Cycles5 );		  			    
  
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//ʹ��ָ����ADC1�����ת������	
	 
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));//�ȴ�ת������

	return ADC_GetConversionValue(ADC1);	//�������һ��ADC1�������ת�����
}

void EXTI_Config(void)
{
  EXTI_InitTypeDef  EXTI_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	
 // GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource8);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource5);
	
//	EXTI_InitStructure.EXTI_Line=EXTI_Line8;
//	EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;
//	EXTI_InitStructure.EXTI_LineCmd=ENABLE;
//  EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Falling;
//	EXTI_Init(&EXTI_InitStructure);
	
	EXTI_InitStructure.EXTI_Line=EXTI_Line5;
	EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_LineCmd=ENABLE;
  EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Falling;
	EXTI_Init(&EXTI_InitStructure);
}




