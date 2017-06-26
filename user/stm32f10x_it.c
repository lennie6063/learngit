/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include "usart1.h"
#include "usart2.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "LED.h"
#include "bsp.h"

/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

///**
//* @brief  This function handles PendSVC exception.
//* @param  None
//* @retval None
//*/
//void PendSV_Handler(void)
//{
//}

///**
//  * @brief  This function handles SysTick Handler.
//  * @param  None
//  * @retval None
//  */
//void SysTick_Handler(void)
//{
//}
/*******************************************************************************
* Function Name  : EXTI15_10_IRQHandler
* Description    : This function handles External interrupt Line 0 request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
int flag_hy=0;
void EXTI9_5_IRQHandler (void)
{ 
    if(EXTI_GetFlagStatus(EXTI_Line5)==RESET)
	{
		   flag_hy=1;
	}	
	EXTI_ClearITPendingBit(EXTI_Line5);  	
}

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 
//void USART1_IRQHandler(void)
//{
//	#if 0
//		unsigned int data;

//    if(USART1->SR & 0x0F)
//    {
//        // See if we have some kind of error
//        // Clear interrupt (do nothing about it!)
//        data = USART1->DR;
//    }
//    else if(USART1->SR & USART_FLAG_RXNE)      //Receive Data Reg Full Flag
//    {		
//        data = USART1->DR;
//				//usart1_putrxchar(data);     //Insert received character into buffer                     
//    }
//		else
//		{;}
//			#endif
//}

/**
  * @brief  This function handles usart2 global interrupt request.
  * @param  None
  * @retval : None
  */
void LED_CmdCtl(void);
void USART2_IRQHandler(void)
{
		unsigned int data;

    if(USART2->SR & 0x0F)
    {
        // See if we have some kind of error
        // Clear interrupt (do nothing about it!)
        data = USART2->DR;
    }
		else if(USART2->SR & USART_FLAG_RXNE)   //Receive Data Reg Full Flag
    {		
        data = USART2->DR;
				usart2_rcv_buf[usart2_rcv_len++]=data;
				
				if(data=='{') //????????????'{'????,‘}’????????,?????????????????
				{
						rcv_cmd_start=1;
				}
				if(rcv_cmd_start==1)
				{
						usart2_cmd_buf[usart2_cmd_len++]=data;
						if((data=='}')||(usart2_cmd_len>=MAX_CMD_LEN-1))
						{
								rcv_cmd_start=0;
								LED_CmdCtl();
								memset(usart2_cmd_buf,0,usart2_cmd_len);
        				usart2_cmd_len=0;
						}
				}	  
    }
		else
		{
				;
		}
}
uint16_t Receive[5];
int j=0;
void USART1_IRQHandler(void)
{
    if(USART_GetFlagStatus(USART1,USART_FLAG_RXNE)==1)
		{
		      Receive[j]=USART_ReceiveData(USART1);
		    	//USART_SendData(USART1,Receive[j]);
			    j++;
		    	if((Receive[0]!=0xFF)&&(Receive[1]!=0XAA)&&(Receive[2]!=0XAA)&&(Receive[3]!=0XEE))
				  j=0;
			   	if(j==5)
					j=0;		 															 
		}
		
	USART_ClearITPendingBit(USART1, USART_FLAG_RXNE);
		

}

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
