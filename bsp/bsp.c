//****************BSP函数实现开发板上相关设备初始化******************//
#include "bsp.h"


//__IO uint16_t ad[1];

//****************BSP函数实现******************//



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
//****************BEEP函数初始化***************//
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
//****************LED函数初始化***************//
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
//****************LED翻转函数实现***************//
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
//****************KEY函数初始化***************//
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
//****************JDQ函数初始化***************//
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
//****************USART函数初始化以及串口配置***************//
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
		
	
/**************************COM1配置****************************************/	
	USART_InitStructure.USART_BaudRate=38400;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1, &USART_InitStructure);
	USART_Cmd(USART1,ENABLE);
//	
//	/**************************COM2配置****************************************/	
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
	
	
//	/**************************COM3配置****************************************/	
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
//************************NVIC函数初始化***************************//
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
//	  NVIC_InitStructure.NVIC_IRQChannel=USART2_IRQn ;   // 在stm32f10x_it.h文件中186 枚举变量
//	  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;
//	  NVIC_InitStructure.NVIC_IRQChannelSubPriority=2;
//	  NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
//	  NVIC_Init(&NVIC_InitStructure);
//		
		
	
	  NVIC_InitStructure.NVIC_IRQChannel=EXTI9_5_IRQn ;   // 在stm32f10x_it.h文件中186 枚举变量
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
//**********printf调用该函数*****************************************//
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
//  DMA_InitStructure.DMA_PeripheralBaseAddr =(uint32_t)&ADC1->DR ;    //DMA对应的外设基地址
//  DMA_InitStructure.DMA_MemoryBaseAddr =(uint32_t)&ad[0];   //内存存储基地址
//  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;	//DMA的转换模式为SRC模式，由外设搬移到内存
//  DMA_InitStructure.DMA_BufferSize = 1;		   //DMA缓存大小，1个,单位为DMA_MemoryDataSize
//  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;	//接收一次数据后，设备地址禁止后移
//  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable;	//关闭接收一次数据后，目标内存地址后移
//  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;  //定义外设数据宽度为16位
//  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;  //DMA搬数据尺寸，HalfWord就是为16位
//  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;   //转换模式，循环缓存模式。
//  DMA_InitStructure.DMA_Priority = DMA_Priority_High;	//DMA优先级高
//  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;		  //M2M模式禁用
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
	
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);   //设置ADC分频因子6 72M/6=12,ADC时钟不能超过14M

	/* PC4 作为模拟通道输入引脚   */                      
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		//模拟输入引脚
	GPIO_Init(GPIOC, &GPIO_InitStructure);	

	ADC_DeInit(ADC1);  //复位ADC1,将外设 ADC1 的全部寄存器重设为缺省值

	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	//ADC工作模式:ADC1工作在独立模式(单独工作，不与其他adc模块配合)
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;	//模数转换工作在单通道模式，扫描模式不开启
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;	//模数转换工作在单次转换模式
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//转换由软件而不是外部触发启动
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	//ADC数据右对齐
	ADC_InitStructure.ADC_NbrOfChannel = 1;	//顺序进行规则转换的ADC通道的数量
	ADC_Init(ADC1, &ADC_InitStructure);	//根据ADC_InitStruct中指定的参数初始化外设ADCx的寄存器   

  
	ADC_Cmd(ADC1, ENABLE);	//使能指定的ADC1
	
	ADC_ResetCalibration(ADC1);	//使能复位校准  	 
	while(ADC_GetResetCalibrationStatus(ADC1));	//等待复位校准结束
	
	ADC_StartCalibration(ADC1);	 //开启AD校准
	while(ADC_GetCalibrationStatus(ADC1));	 //等待校准结束 

}

u16 GetAdc(void)   
{
  /*设置指定ADC的规则组通道，一个序列，采样时间239.5个周期*/
	ADC_RegularChannelConfig(ADC1, ADC_Channel_14, 1, ADC_SampleTime_239Cycles5 );		  			    
  
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//使能指定的ADC1的软件转换启动	
	 
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));//等待转换结束

	return ADC_GetConversionValue(ADC1);	//返回最近一次ADC1规则组的转换结果
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




