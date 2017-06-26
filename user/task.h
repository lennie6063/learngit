#ifndef __DH_TASK_H
#define __DH_TASK_H
#include	"includes.h"
/*************************   �����ջ�ߴ�   *****************************/
#define		START_STK_SIZE	64
#define   LCD_STK_SIZE  128
#define   USER2_STK_SIZE  128
#define   HSE_STK_SIZE  128
#define   USER1_STK_SIZE  128

/*************************   �������ȼ�   *****************************/
#define START_TASK_PRIO      			9     //��ʼ��������ȼ�����Ϊ���
#define LCD_TASK_PRIO            10
#define USER2_TASK_PRIO            11
#define HSE_TASK_PRIO            12
#define USER1_TASK_PRIO            13

/*************************   ��������  *****************************/
void start_task(void *p_arg);
void lcd_task(void *p_arg);
void user2_task(void *p_arg);
void hse_task(void *p_arg);
void user1_task(void *p_arg);

/*************************   �����ջ   *****************************/
extern OS_STK START_TASK_STK[START_STK_SIZE];
extern OS_STK LCD_TASK_STK[LCD_STK_SIZE];
extern OS_STK USER2_TASK_STK[USER2_STK_SIZE];
extern OS_STK HSE_TASK_STK[HSE_STK_SIZE];
extern OS_STK USER1_TASK_STK[USER1_STK_SIZE];

/*************************   ���ú���  *****************************/


#endif


