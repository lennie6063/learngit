#include "usart.h"
#include "stdio.h"

//////////////////////////////////////////////////////////////////
//֧��printf������ ��ʹ��use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
                
struct __FILE 
{ 
	int handle; 
	/* Whatever you require here. If the only file you are using is */ 
	/* standard output using printf() for debugging, no file handling */ 
	/* is required. */ 
}; 
/* FILE is typedef� d in stdio.h. */ 
FILE __stdout;       
//����_sys_exit()����ʹ�ð�����ģʽ   
void _sys_exit(int x) 
{ 
	x = x; 
} 
/*    �ض���fputc����    */
int fputc(int ch, FILE *f)
{      
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==0);
	USART_SendData(USART1,ch);
	return ch;
}
#endif 
/** ��ʼ��USART1  **/
void bsp_InitCOM1(u32 baud)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
#ifdef EN_USART1_RXNE
	NVIC_InitTypeDef NVIC_InitStructure;
#endif
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
  //�������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
  //��������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	USART_InitStructure.USART_BaudRate = baud;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1, &USART_InitStructure);

#ifdef EN_USART1_RXNE
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
	USART_ITConfig(USART1,USART1_IT_RXNE,ENABLE);
#endif

	USART_Cmd(USART1, ENABLE);
	USART_ClearFlag(USART1, USART_FLAG_TC);    
}

#ifdef EN_USART_RXNE
COM_PARAMx COM1;
/***  ����1�жϴ�����  ***/
void USART1_IRQHandler(void)
{
  if(USART_GetFlagStatus(USART1,USART_FLAG_RXNE))
  {
  }
}

#endif
/*****  ��ָ������д��һ���ֽ�  ***/
void ComPutChar(USART_TypeDef* USARTx,u8 ch)
{
	while(USART_GetFlagStatus(USARTx,USART_FLAG_TC)==0);
	USART_SendData(USARTx,ch);	
}
/******  ��ָ������д��ָ�����ȵ�����******/
void ComPutStr(USART_TypeDef* USARTx,u8* ptr,u16 len)
{
	u16 i;
	for(i=0;i<len;i++) 
	{
		ComPutChar(USARTx,*ptr);
		ptr++;
	}
}
/*******  ��ָ������д���ַ���  *******/	
void ComPutCStr(USART_TypeDef* USARTx,const char *ptr)
{
	while((*ptr)!=0)
	{
		ComPutChar(USARTx,*ptr);
		ptr++;
	}		
}

