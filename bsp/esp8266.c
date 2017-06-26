#include "includes.h"


EdpPacket* send_pkg;
char send_buf[MAX_SEND_BUF_LEN];
extern int flag_hy;

/**
  * @brief  ESP8266初始化
**/
void ESP8266_Init(void)
{
	//	printf("%s\r\n","[ESP8266_Init]ENTER AT.");
		SendCmd(AT,"OK",1000); 			
	//	printf("%s\r\n","[ESP8266_Init]EXIT AT.");
		
	//	printf("%s\r\n","[ESP8266_Init]ENTER CWMODE.");
		SendCmd(CWMODE,"OK",1000);		
	//	printf("%s\r\n","[ESP8266_Init]EXIT CWMODE.");
	
	//printf("%s\r\n","[ESP8266_Init]ENTER RST.");
		SendCmd(RST,"OK",2000);	
	//	printf("%s\r\n","[ESP8266_Init]EXIT RST.");
	
	//	printf("%s\r\n","[ESP8266_Init]ENTER CIFSR.");
		SendCmd(CIFSR,"OK",1000);	
	//	printf("%s\r\n","[ESP8266_Init]EXIT CIFSR.");
	
//		printf("%s\r\n","[ESP8266_Init]ENTER CWJAP.");
		SendCmd(CWJAP,"OK",2200);	
	//	printf("%s\r\n","[ESP8266_Init]EXIT CWJAP.");

	//	printf("%s\r\n","[ESP8266_Init]ENTER CIPSTART.");
		SendCmd(CIPSTART,"OK",2200);
//		printf("%s\r\n","[ESP8266_Init]EXIT CIPSART.");
		
//		printf("%s\r\n","[ESP8266_Init]ENTER CIPMODE.");
		SendCmd(CIPMODE,"OK",1000);
	//	printf("%s\r\n","[ESP8266_Init]EXIT CIPMODE.");
}

/**
  * @brief  生成LED当前状态的上传数据，分割字符串格式
**/
void GetSendBuf(void)
{
		char text[25] = {0};
		
		LED_GetValue();

		memset(send_buf,0,MAX_SEND_BUF_LEN);
		hy_value=flag_hy;
		strcat(send_buf, ",;");	
		strcat(send_buf, "temp,");
		sprintf(text,"%d",tem);
		strcat(send_buf, text);
		strcat(send_buf, ";");
		
		strcat(send_buf, "humi,");
		sprintf(text,"%d",hum);
		strcat(send_buf, text);
		strcat(send_buf, ";");

		strcat(send_buf, "jdq1_statu,");
		sprintf(text,"%d",jdq1_value);
		strcat(send_buf, text);
		strcat(send_buf, ";");
		
		strcat(send_buf, "jdq2_statu,");
		sprintf(text,"%d",jdq2_value);
		strcat(send_buf, text);
		strcat(send_buf, ";");
		
		strcat(send_buf, "hy_statu,");
		sprintf(text,"%d",hy_value);
		strcat(send_buf, text);
		strcat(send_buf, ";");
	
		strcat(send_buf, "beep_statu,");
		sprintf(text,"%d",beep_value);
		strcat(send_buf, text);
		strcat(send_buf, ";");	
		
		strcat(send_buf, "red_value,");
		sprintf(text,"%d",red_value);
		strcat(send_buf, text);
		strcat(send_buf, ";");	
		
		strcat(send_buf, "yellow_value,");
		sprintf(text,"%d",yellow_value);
		strcat(send_buf, text);
		strcat(send_buf, ";");	
		
		strcat(send_buf, "blue_value,");
		sprintf(text,"%d",blue_value);
		strcat(send_buf, text);
		strcat(send_buf, ";");	
		
		strcat(send_buf, "green_value,");
		sprintf(text,"%d",green_value);
		strcat(send_buf, text);
		strcat(send_buf, ";");	
		
		
		strcat(send_buf, "number,");
		sprintf(text,"%d",number);
		strcat(send_buf, text);
		strcat(send_buf, ";");	
}


/**
  * @brief  发送一条AT指令
**/
void SendCmd(char* cmd, char* result, int timeOut)
{
    while(1)
    {
        memset(usart2_rcv_buf,0,sizeof(usart2_rcv_buf));
				usart2_rcv_len=0;
        usart2_write(USART2, (unsigned char *)cmd, strlen((const char *)cmd));
        mDelay(timeOut);
       
        if((NULL != strstr((const char *)usart2_rcv_buf, result)))	//判断是否有预期的结果
        {
            break;
        }
        else
        {
            mDelay(100);
        }
    }
}

/**
  * @brief  和平台建立设备连接
**/
void ESP8266_DevLink(const char* devid, const char* auth_key, int timeOut)
{
		int32 count=0;
	
		memset(usart2_rcv_buf,0,strlen((const char *)usart2_rcv_buf));
		usart2_rcv_len=0;			
		
	//	printf("%s\r\n","[ESP8266_DevLink]ENTER device link...");
		usart2_write(USART2,CIPSEND,strlen(CIPSEND));  //向ESP8266发送数据透传指令
		for(count=0;count<timeOut;count++)
		{
				mDelay(100);
				if((NULL != strstr((const char *)usart2_rcv_buf,">")))
				{
						break;
				}
		}	

		send_pkg = PacketConnect1(devid,auth_key);
		mDelay(200);
		usart2_write(USART2,send_pkg->_data,send_pkg->_write_pos);  //发送设备连接请求数据
		mDelay(500);
		DeleteBuffer(&send_pkg);
		mDelay(200);
		usart2_write(USART2,"+++",3);  //向ESP8266发送+++结束透传，使ESP8266返回指令模式
		mDelay(50);
	//	printf("%s\r\n","[ESP8266_DevLink]EXIT device link...");
}

/**
  * @brief  检测ESP8266连接状态
**/
int ESP8266_CheckStatus(int timeOut)
{
		int32 res=0;
		int32 count=0;
	
		memset(usart2_rcv_buf,0,sizeof(usart2_rcv_buf));
		usart2_rcv_len=0;
		
	//	printf("%s\r\n","[ESP8266_CheckStatus]ENTER check status...");
		usart2_write(USART2,CIPSTATUS,strlen(CIPSTATUS));
		for(count=0;count<timeOut;count++)
		{
				mDelay(100);
				if((NULL != strstr((const char *)usart2_rcv_buf,"STATUS:4")))  //失去连接
				{
						res=-4;
						break;
				}
				else if((NULL != strstr((const char *)usart2_rcv_buf,"STATUS:3")))  //建立连接
				{
						res=0;	
						break;
				}
				else if((NULL != strstr((const char *)usart2_rcv_buf,"STATUS:2")))  //获得IP
				{
						res=-2;
						break;				
				}
				else if((NULL != strstr((const char *)usart2_rcv_buf,"STATUS:5")))  //物理掉线
				{
						res=-5;
						break;
				}
				else if((NULL != strstr((const char *)usart2_rcv_buf,"ERROR")))   
				{
						res=-1;
						break;
				}
				else
				{
						;
				}
		}	
	//	printf("%s\r\n","[ESP8266_CheckStatus]EXIT check status...");
		return res;	
}

/**
  * @brief  向平台上传LED当前状态数据
**/
void ESP8266_SendDat(void)
{		
		int32 count=0;

		memset(usart2_rcv_buf,0,sizeof(usart2_rcv_buf));
		usart2_rcv_len=0;			
	//	printf("%s\r\n","[ESP8266_SendDat]ENTER Senddata...");
		usart2_write(USART2,CIPSEND,strlen(CIPSEND));  //向ESP8266发送数据透传指令
		for(count=0;count<40;count++)
		{
				mDelay(100);
				if((NULL != strstr((const char *)usart2_rcv_buf,">")))
				{
						break;
				}
		}	
	
		GetSendBuf();		
		send_pkg = PacketSavedataSimpleString(DEVICEID,send_buf);   
		usart2_write(USART2,send_pkg->_data,send_pkg->_write_pos);	//向平台上传数据点
		DeleteBuffer(&send_pkg);
		mDelay(500);

		usart2_write(USART2,"+++",3);  //向ESP8266发送+++结束透传，使ESP8266返回指令模式
		mDelay(200);
//		printf("%s\r\n","[ESP8266_SendDat]EXIT Senddata...");
}









