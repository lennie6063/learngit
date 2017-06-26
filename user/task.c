#include	"includes.h"
#include  "task.h"

int flag_12864=0,flag_gprs_shuju=0;
int flag_yuyin=0;
int flag_yy=0;

uint16_t temp, humi;    //???
u32 tem=0;
u32 hum=0;
u32 adc;
extern int flag_hy;
extern uint16_t Receive[5];

char yuyin01[11]={"play,006,$"};
char yunwait[11]={"play,007,$"};
char yunready[11]={"play,008,$"};

void Putchar_String(char* ch);
void LED1_Config(void);
void LED2_Config(void);
void YUYIN(void);


/*************************   任务堆栈   *****************************/
OS_STK START_TASK_STK[START_STK_SIZE];
OS_STK LCD_TASK_STK[LCD_STK_SIZE];
OS_STK USER2_TASK_STK[USER2_STK_SIZE];
OS_STK HSE_TASK_STK[HSE_STK_SIZE];
OS_STK USER1_TASK_STK[USER1_STK_SIZE];


/**** 开始任务 ****/
void start_task(void *pdata)
{
  OS_CPU_SR cpu_sr=0;
	pdata = pdata;        
	OSStatInit();					//初始化统计任务.这里会延时1秒钟左右	
 	OS_ENTER_CRITICAL();			//进入临界区(无法被中断打断)  
	Putchar_String("play,006,$");  
	Receive[0]='0';Receive[1]='0';Receive[2]='0';Receive[3]='0';Receive[4]='0';
	
 	OSTaskCreate(lcd_task,  (void *)0,(OS_STK*)&LCD_TASK_STK[LCD_STK_SIZE-1],    LCD_TASK_PRIO);			
  OSTaskCreate(hse_task,  (void *)0,(OS_STK*)&HSE_TASK_STK[HSE_STK_SIZE-1],    HSE_TASK_PRIO);			
// 	OSTaskCreate(user2_task,(void *)0,(OS_STK*)&USER2_TASK_STK[USER2_STK_SIZE-1],USER2_TASK_PRIO);	 
  OSTaskCreate(user1_task,(void *)0,(OS_STK*)&USER1_TASK_STK[USER1_STK_SIZE-1],USER1_TASK_PRIO);	
	
	OSTaskSuspend(START_TASK_PRIO);	//挂起起始任务.
	OS_EXIT_CRITICAL();				//退出临界区(可以被中断打断)
}
 
void lcd_task(void *pdata)
{

	while(1)
	{
     graphMain();//12864
		 SHT2x_MeasureHM(SHT20_Measurement_T_HM, &temp);
     OSTimeDlyHMSM (0,0,0,500);
     SHT2x_MeasureHM(SHT20_Measurement_RH_HM, &humi);
		 OSTimeDlyHMSM (0,0,0,500);
		 tem=temp;
		 hum=humi;
  	 OSTimeDlyHMSM (0,0,0,500);	

		
	}
}

//void user2_task(void *pdata)
//{

//    	while(1)
//		{		
//			//	YUYIN();
//		}
//	
//}

void hse_task(void *pdata)
{
	while(1)
	{
       
		   if(HY_FLAG ==0)
	     {
				BEEP_ON;
			 		 
			 }
			 else
		   BEEP_OFF;
			 YUYIN();			
	     OSTimeDlyHMSM (0,0,0,100);
 }
}

void user1_task(void *pdata)
{

    	while(1)
		{		
				LED_Switch(LED_ON,LED_R|LED_G|LED_Y|LED_B);
				ESP8266_Init();    //ESP8266???
				ESP8266_DevLink(DEVICEID,APIKEY,20);    
				LED_Switch(LED_OFF,LED_R|LED_G|LED_Y|LED_B); 
			  flag_12864=1;
				while(1)
				{		
					 					
						if(!(ESP8266_CheckStatus(30)))    //??ESP8266??????
						{
								ESP8266_SendDat();     //???????
//						  graphMain();	delay_ms(200);

						}
						else
						{
								break;
						}
				}
		}	
		
	
}
void YUYIN(void)
{
	    if((Receive[0]==0xFF)&&(Receive[1]==0XAA)&&(Receive[2]==0XAA)&&(Receive[3]==0XEE)&&(Receive[4]==0X05))//你好
			{
				flag_yuyin=1;
				Receive[0]='0';Receive[1]='0';Receive[2]='0';Receive[3]='0';Receive[4]='0';	
				BEEP_ON;
			  delay_ms(100);
			  BEEP_OFF;
			}
		
		if(flag_yuyin==1)
		{	
			
			if((Receive[0]==0xFF)&&(Receive[1]==0XAA)&&(Receive[2]==0XAA)&&(Receive[3]==0XEE)&&(Receive[4]==0X00))// LED1
			{
				 
				 Receive[0]='0';Receive[1]='0';Receive[2]='0';Receive[3]='0';Receive[4]='0';	
				 JDQ1_OFF;
				 BEEP_ON;
				 delay_ms(100);
				 BEEP_OFF;
				
				
			}
			if((Receive[0]==0XFF)&&(Receive[1]==0XAA)&&(Receive[2]==0XAA)&&(Receive[3]==0XEE)&&(Receive[4]==0X01))//CLL1
			{
				
				 Receive[0]='0';Receive[1]='0';Receive[2]='0';Receive[3]='0';Receive[4]='0';	
				 JDQ1_ON;
				 BEEP_ON;
				 delay_ms(100);
				 BEEP_OFF;
			}
			
					if((Receive[0]==0XFF)&&(Receive[1]==0XAA)&&(Receive[2]==0XAA)&&(Receive[3]==0XEE)&&(Receive[4]==0X02))
			{
				
							Receive[0]='0';Receive[1]='0';Receive[2]='0';Receive[3]='0';Receive[4]='0';	
				      JDQ2_OFF;
							BEEP_ON;
							delay_ms(100);
							BEEP_OFF;
			}
				if((Receive[0]==0XFF)&&(Receive[1]==0XAA)&&(Receive[2]==0XAA)&&(Receive[3]==0XEE)&&(Receive[4]==0X03))
			{
						
							Receive[0]='0';Receive[1]='0';Receive[2]='0';Receive[3]='0';Receive[4]='0';	
				      JDQ2_ON;
							BEEP_ON;
							delay_ms(100);
							BEEP_OFF;
			}	
			
		
		}
	
}

void Putchar_String(char*ch)//输出字符串函数
{    int i=0;
	   while(ch[i]!='\0')
     {
		  while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==0);
		  USART_SendData(USART1,ch[i]);	
	    i++;  
	   }	
	    
}  



