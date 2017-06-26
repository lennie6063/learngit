#ifndef __DH_TASK_H
#define __DH_TASK_H
#include	"includes.h"
/*************************   任务堆栈尺寸   *****************************/
#define		START_STK_SIZE	64
#define   LCD_STK_SIZE  128
#define   USER2_STK_SIZE  128
#define   HSE_STK_SIZE  128
#define   USER1_STK_SIZE  128

/*************************   任务优先级   *****************************/
#define START_TASK_PRIO      			9     //开始任务的优先级设置为最低
#define LCD_TASK_PRIO            10
#define USER2_TASK_PRIO            11
#define HSE_TASK_PRIO            12
#define USER1_TASK_PRIO            13

/*************************   任务声明  *****************************/
void start_task(void *p_arg);
void lcd_task(void *p_arg);
void user2_task(void *p_arg);
void hse_task(void *p_arg);
void user1_task(void *p_arg);

/*************************   任务堆栈   *****************************/
extern OS_STK START_TASK_STK[START_STK_SIZE];
extern OS_STK LCD_TASK_STK[LCD_STK_SIZE];
extern OS_STK USER2_TASK_STK[USER2_STK_SIZE];
extern OS_STK HSE_TASK_STK[HSE_STK_SIZE];
extern OS_STK USER1_TASK_STK[USER1_STK_SIZE];

/*************************   配置函数  *****************************/


#endif


