#ifndef __USART_H__
#define __USART_H__

#include "stm32f10x.h"

//使用串口1接收中断
//#define EN_USART1_RXNE

#ifdef EN_USART1_RXNE
#define ComData_MaxLen  64
typedef struct COM_PARAM
{
	u8 RxBuffer[ComData_MaxLen];
//	u8 TxBuffer[ComData_MaxLen];
	u8 RxLen;
//	u8 TxLen;
}COM_PARAMx;
extern COM_PARAMx COM1;
#endif

void bsp_InitCOM1(u32 baud);
void ComPutCStr(USART_TypeDef* USARTx,const char *ptr);
void ComPutStr(USART_TypeDef* USARTx,u8* ptr,u16 len);
void ComPutChar(USART_TypeDef* USARTx,u8 ch);
#endif
