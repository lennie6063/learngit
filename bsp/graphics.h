#ifndef _GRAPHICS_H_ 
#define _GRAPHICS_H_

#include	"includes.h"
#include "qc12864.h"



#define  VK_UP      (1<<1)
#define  VK_DOWN    (1<<2)
#define  VK_OK      (1<<3)
#define  VK_ESC     (1<<0)

/***********************接口函数*******************/
#define  ocmLine(a,b,c,d,e)        Qc12864_DrawHLine(a,b,c,e)    
#define  ocmFillRect(a,b,c,d,e)    Qc12864_Draw_Rect(a,b,c,d,e)
#define  graphDrawChnStr(a,b,c)    Qc12864_Draw_GB12(a,b,c)
#define  graphDrawASCNum(a,b,c,d)    Qc12864_Draw_ASC8(a,b,c,d)

/**********************所需数据*******************/
//Temperature格式为带符号整数，其余为整数
//值均为真实数值的百倍(两位小数点)
extern u32 tem,hum,adc;
#define  Temperature     tem*100         //123
#define  Humidity        hum*100
#define  Luminosity      adc*5

#define  DEST_IP1             192
#define  DEST_IP2             168
#define  DEST_IP3             110
#define  DEST_IP4             104
#define  DEST_PORT            7092

#define  SELF_IP1             192
#define  SELF_IP2             168
#define  SELF_IP3             0
#define  SELF_IP4             94
#define  SELF_PORT            10

#define  COM_ECHO             1
//1:开启  0 关闭
#define  COM_BAUD             115200
  //最多支持六位数字
#define  DATA_INTER           15
//单位 s 最多支持四位数字



void graphMain(void);
void graphOnKeyPressed(u8 key);
void graphOnTick(void);

#endif
