#include	"stm32f10x.h"
#include  "stdio.h"
//#include "usart1.h"
#include "usart2.h"
#include  "delay.h"
#include  "task.h"

//void mDelay(uint32_t i)
//{
//		uint32_t j=0;
//		for(;i>0;i--)
//		{
//				for(j=0;j<12000;j++);
//		}
//}

int main(void)
{
		//SystemInit();
	  BSP_Config();
   	LED_Init();    //LED????????
		//KEY_Init();	   //???????
	//	USART1_Init(); //USART1???????
		USART2_Init(); //USART2???????
	  delay_init();
		Qc12864_Init();	
		Hal_I2C_Init();			//I2C???,??????????
	  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
 // bsp_InitCOM1(115200);
	
  OSInit();   
 	OSTaskCreate(start_task,(void *)0,
		  (OS_STK *)&START_TASK_STK[START_STK_SIZE-1],
	    START_TASK_PRIO );//创建起始任务
	OSStart();
}
