#include "ld3461bs.h"

const u8 LED_0F[] = 
{// 0	   1	  2	   3	  4	   5	  6	   7	  8	   9	  .	   -	  C    d	  E    F    -
	0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90,0x7f,0xBF,0xC6,0xA1,0x86,0xFF,0xbf
};

#define LD_DIO_HIGH()      GPIO_SetBits(GPIOB , GPIO_Pin_5)
#define LD_DIO_LOW()       GPIO_ResetBits(GPIOB , GPIO_Pin_5)

#define LD_RCLK_HIGH()     GPIO_SetBits(GPIOB , GPIO_Pin_6)
#define LD_RCLK_LOW()      GPIO_ResetBits(GPIOB , GPIO_Pin_6)

#define LD_SCLK_HIGH()     GPIO_SetBits(GPIOB , GPIO_Pin_7)
#define LD_SCLK_LOW()      GPIO_ResetBits(GPIOB , GPIO_Pin_7)

void LD_OUT(u8 data)
{
	u8 i;
	for(i=8;i>=1;i--)
	{
		if (data&0x80) LD_DIO_HIGH(); 
		else LD_DIO_LOW();
		data<<=1;
		LD_SCLK_LOW();
		LD_SCLK_HIGH();
	}
}

/*********/
void LD_Display(u8 pos,u8 data)
{
	const u8 *led_table;          // ²é±íÖ¸Õë
	u8 c,bit;

	bit=(pos>4)?(pos-5):(pos-1);
	led_table = LED_0F + data;
	c = *led_table;
	
	if(pos>4)
	{
		LD_OUT(0);
	  LD_OUT(0);
	}
	LD_OUT(c);		
	LD_OUT(1<<bit);	
	
	if(pos<=4)
	{
	  LD_OUT(0);
	  LD_OUT(0);
	}
	LD_RCLK_LOW();
	LD_RCLK_HIGH();
}

/*********  LD Gpio Init PB5,6,7*********/
void LD_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin= GPIO_Pin_5 |GPIO_Pin_6| GPIO_Pin_7 ;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
}
