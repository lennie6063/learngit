#include "qc12864.h"
#include "delay.h"

#define LCD_RS_CMD         0
#define LCD_RS_DATA        0x02
#define LCD_CS_High()      GPIO_SetBits(GPIOA,GPIO_Pin_15)
#define LCD_CS_Low()       GPIO_ResetBits(GPIOA,GPIO_Pin_15)
#define LCD_SID_High()     GPIO_SetBits(GPIOD,GPIO_Pin_2)
#define LCD_SID_Low()      GPIO_ResetBits(GPIOD,GPIO_Pin_2)
#define LCD_CLK_High()     GPIO_SetBits(GPIOB,GPIO_Pin_3)
#define LCD_CLK_Low()      GPIO_ResetBits(GPIOB,GPIO_Pin_3)

#define LCD_SID_STA()      GPIO_ReadOutputDataBit(GPIOD,GPIO_Pin_2)  
//12864 垂直地址0-31，水平地址0-16，水平以两字节为单位
//水平0-7 上半屏 水平8-15 下半屏
//缓存仍以64*8存储，方便ucgui
u16 Lcd_Data_Cache[64][8]={
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
  {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
};

void Qc12864_Gpio_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOD,ENABLE);
	
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable , ENABLE);//禁用JTAG,不禁用SW
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	LCD_CS_High();
	LCD_SID_High();
}
void Qc12864_Write(u8 data)
{
	u8 i;
	for(i=0;i<8;i++)
	{
		if(data&0x80) LCD_SID_High();
		else LCD_SID_Low();
		data<<=1;
		LCD_CLK_High();
		delay_us(1);
		LCD_CLK_Low();
		delay_us(5);
	}
}

void Qc12864_WriteCmd(u8 cmd)
{
	Qc12864_Write(0xf8|LCD_RS_CMD);
	Qc12864_Write(cmd&0xf0);
	Qc12864_Write((cmd<<4)&0xf0);
}
void Qc12864_WriteData(u8 data)
{
	Qc12864_Write(0xf8|LCD_RS_DATA);
	Qc12864_Write(data&0xf0);
	Qc12864_Write((data<<4)&0xf0);
}

void Qc12864_Display_Str(u8 line,char *str)
{
	u8 addr;
	line-=1;
	addr=0x80+0x10*(line%2)+0x08*(line/2);
	Qc12864_WriteCmd(addr);
	while(*str!=0)
	{
		Qc12864_WriteData(*str);
		str++;
	}
}
/*************更新一个点的缓存****************/
u16 Refresh_Cache_Point(u8 x,u8 y,u8 bit)
{
	u16 word;
	u8 tmp;
	word=Lcd_Data_Cache[y][x/16];//
	tmp=15-(x%16);
	if(bit) word|=(1<<tmp); 
	else    word&=~(1<<tmp);
	Lcd_Data_Cache[y][x/16]=word;
	return word;
}
/**************更新一个字节的缓存*************************/
//xsize 字节中有效的位数 diff:
void Refresh_Cache_Byte(u8 x,u8 y,u8 xsize,u8 diff,u8 data)
{
	u16 word;
	u8 i,bit;
	bit=(x%16);
	word=Lcd_Data_Cache[y][x/16];
	for(i=0;i<xsize;i++)
	{
		if(data&(0x80>>i))
		{
			word|=(0x0001<<(bit+i));
		}
		else word&=(~(0x0001<<(bit+i)));
	}
	Lcd_Data_Cache[y][x/16]=word;
}
/***********************画一个字节的数据*******************/
//测试用
void Qc12864_Draw_Byte(u8 x,u8 y,u8 byte)
{
	u16 word;
	word=Lcd_Data_Cache[y][x/16];
	if((x%16)<8)
	{
		word=(word&0x00ff)+((u16)byte<<8);
	  Lcd_Data_Cache[y][x/16]=word;
	}
	else 
	{
		word=(word&0xff00)+((u16)byte);
	  Lcd_Data_Cache[y][x/16]=word;
	}
  x=x/16;     //找到像素点所在字的水平位置
	if(y>31)  //是否下半屏
	{
		y-=32;
		x+=8;
	}
  Qc12864_WriteCmd(0x80+y);    //设定垂直地址
  Qc12864_WriteCmd(0x80+x);

  Qc12864_WriteData((word>>8)&0xff); 	
  Qc12864_WriteData(word&0xff);
		
}
/**********************画几个字的数据*******************/
//注意，len是字长度
void Qc12864_Draw_nWord(u8 x,u8 y,u16* str,u8 len)
{
	u8 i;
	x=x/16;     //找到像素点所在字的水平位置
	if(y>31)  //是否下半屏
	{
		y-=32;
		x+=8;
	}
  Qc12864_WriteCmd(0x80+y);    //设定垂直地址
  Qc12864_WriteCmd(0x80+x);
	
	for(i=0;i<len;i++)
	{
		Qc12864_WriteData((*str>>8)&0xff); 
		Qc12864_WriteData(*str&0xff);
		str++;
	}
}
/*********************画点函数***************************/
//x:水平坐标 0-127   y:垂直坐标 0-64
//画图时为先水平后垂直扫描方式，故实际为16*64
//串行方式读不出数据，建显存
void Qc12864_Draw_Point(u8 x,u8 y,u8 bit) 
{
	u16 word;	
	word=Refresh_Cache_Point(x,y,bit);
	Qc12864_Draw_nWord(x,y,&word,1);
}
/*********************得到点值***********************/
u8 Qc12864_Get_Point(u8 x,u8 y)
{
	u8 bit,tmp;
	tmp=Lcd_Data_Cache[y][x/16];
	if(tmp&(1<<(x%16))) bit=1;
	return bit;
}
/********************画横线************************/
void Qc12864_DrawHLine(u8 x0,u8 y,u8 x1,u8 white)
{
//	//按点画线，速度慢
//	for(;x0<x1;x0++) Qc12864_Draw_Point(x0,y,1);
	u16 word;
	u8 i=0,len;
  if((x1-x0)<(16-x0%16))  //所画像素在一个字以内
	{
		len=x1-x0+1;
		word=Lcd_Data_Cache[y][x0/16];
		for(i=0;i<len;i++)
		{
			if(white) word|=(0x8000>>(x0%16+i));
			else word&=(~(0x8000>>(x0%16+i)));
		}
		Qc12864_Draw_nWord(x0,y,&word,1);
		return;
	}
	if(x0%16)  
	{
		len=16-x0%16;
		word=Lcd_Data_Cache[y][x0/16];
		for(i=0;i<len;i++) 
		{
			if(white) word|=(1<<i);
			else word&=(~(1<<i));
		}
		Lcd_Data_Cache[y][x0/16]=word;
  }
	else 
	{
		if(white) Lcd_Data_Cache[y][x0/16]=0xffff;
		else Lcd_Data_Cache[y][x0/16]=0;
	}
  if((x1%16)!=15)
	{
		word=Lcd_Data_Cache[y][x1/16];
		for(i=0;i<=(x1%16);i++) 
		{
			if(white) word|=(0x8000>>i);
			else word&=(~(0x8000>>i));
		}
		Lcd_Data_Cache[y][x1/16]=word;
  }
	else 
	{
		if(white) Lcd_Data_Cache[y][x1/16]=0xffff;
		else Lcd_Data_Cache[y][x1/16]=0;
	}
	for(i=(x0/16+1);i<(x1/16);i++)
	{
		if(white) Lcd_Data_Cache[y][i]=0xffff;
		else Lcd_Data_Cache[y][i]=0;
	}
	len=(x1/16)-(x0/16)+1;
  Qc12864_Draw_nWord(x0,y,&Lcd_Data_Cache[y][x0/16],len);
}
/*********************画竖线*********************/
void Qc12864_Draw_VLine(u8 x,u8 y0,u8 y1,u8 white)
{
	for(;y0<y1;y0++) Qc12864_Draw_Point(x,y0,white);
}
/**********************按数据画一行*********************/
// x0,y为起始像素点地址，xize:像素长度 nbytes ：每行像素字节长度
// diff=0
static void Qc12864_Draw_Linex(u8 x,u8 y,u8 xsize,u8 nbytes,u8* ptr)
{
	u16 tmp,cmp;
	u8 i=0,j=0,len,bit;
	
	if((x%16)==0)   //不需移位
	{
		len=0;
		//不需移位
		for(i=0;i<(xsize/16);i++) 
		{
			tmp=((u16)(*ptr)<<8)+*(ptr+1);
			Lcd_Data_Cache[y][x/16+i]=tmp;
			ptr+=2;
		}
		bit=xsize%16;
		if(bit) 
		{
			tmp=Lcd_Data_Cache[y][x/16+i]&(0xffff>>bit);
			cmp=((u16)(*ptr)<<8)+*(ptr+1);
			Lcd_Data_Cache[y][x/16+i]=tmp|(cmp&(0xffff<<(16-bit)));
		}
		len=i+1;
		Qc12864_Draw_nWord(x,y,&Lcd_Data_Cache[y][x/16],len);
		return;
	}
	
	bit=x%16;
	len=16-(x%16);
	cmp=Lcd_Data_Cache[y][x/16]&(0xffff<<len);

  for(i=0;i<(xsize/16);i++)
	{
		Lcd_Data_Cache[y][x/16+i]=cmp;
		tmp=((u16)(*ptr)<<8)+*(ptr+1);
		cmp=tmp<<len;
		tmp=tmp>>bit;
		Lcd_Data_Cache[y][x/16+i]+=tmp;
		ptr+=2;
	}
	if(xsize%16)
	{
		tmp=Lcd_Data_Cache[y][x/16+i];
		for(j=0;j<(xsize%16);j++)
		{
			if(j>=8)
			{
				xsize-=8;
				j-=8;
				ptr++;
			}
			if((*ptr)&(0x80>>j))
			{
				tmp|=(0x0001<<(15-bit));
			}
			else 
			{
				tmp&=~(0x0001<<(15-bit));
			}
			if(++bit==16)
			{
				Lcd_Data_Cache[y][x/16+i]=tmp;
				i++;
				bit=0;			
				tmp=Lcd_Data_Cache[y][x/16+i];
			}
		}
		Lcd_Data_Cache[y][x/16+i]=tmp;
	}
  len=i+1;
  Qc12864_Draw_nWord(x,y,&Lcd_Data_Cache[y][x/16],len);
}
void Qc12864_Draw_Rect(u8 x,u8 y,u8 xsize,u8 ysize,u8 en)
{
	u8 i;
	for(i=0;i<ysize;i++) Qc12864_DrawHLine(x,y+i,x+xsize,en);
	for(i=0;i<ysize;i++) Qc12864_DrawHLine(x,y+i,x+xsize,en);
	Qc12864_Draw_VLine(x,y,y+ysize,en);
	Qc12864_Draw_VLine(x+xsize,y,y+ysize,en);
	
}
/**********************画图(不支持缩放，测试用)*****************************/
//xsize:行像素点数
void Qc12864_Draw_Bmp(u8 x,u8 y,u8 xsize,u8 ysize,u8 nbyte,u8* ptr,u8 color)
{
	u8 i,j,tmp[16];
	for(i=0;i<ysize;i++)
	{
		for(j=0;j<nbyte;j++) 
		{
			if(color==0) tmp[j]=*ptr++;
			else tmp[j]=~(*ptr++);
		}
	  Qc12864_Draw_Linex(x,y+i,xsize,nbyte,tmp);
	}
}


/********************画汉字，字体为12*16***********************/
void Qc12864_Draw_GB12_Linex(u8 x,u8 y,u8 xsize,const u8* ptr)
{
	u8 i;
	x=x/8;
	Qc12864_WriteCmd(0x80+y);    //设定垂直地址
  Qc12864_WriteCmd(0x80+x);
	
	for(i=0;i<(xsize/8);i++)
	{ 
		Qc12864_WriteData(*ptr);
		ptr++;
	}
}
/**************************画ASC，字体12*8************************/
void Qc12864_Draw_ASC8(u8 row,u8 colu,const u8 *ptr,u8 diff)
{
	u8 x,y,i;
	y=(row%2)?0:16;
	x=(row<3)?0:(8*16);
	x+=(colu-1)*16+diff;
	y+=2;
	for(i=0;i<12;i++)
	{
    Qc12864_Draw_Byte(x,y++,*ptr);
		ptr+=2;
	}
	for(i=0;i<2;i++)
	{
		Qc12864_WriteCmd(0x80+y++);
		Qc12864_WriteCmd(0x80+(x/8));
		Qc12864_WriteData(0);
		Qc12864_WriteData(0);
	}
}
//row: 行号1-4
//colu:列号1-8
//ptr: 指向汉字的指针
void Qc12864_Draw_GB12(u8 row,u8 colu,const u8 *ptr)
{
	u8 x,y,i;

	y=(row%2)?0:16;
	x=(row<3)?0:(8*8);
	x+=(colu-1)*8;
	y+=2;  //16*16显示12*16的字符，故调整到中间
	for(i=0;i<12;i++)
	{		
	  Qc12864_Draw_GB12_Linex(x,y++,16,ptr);
		ptr+=2;
	}
	for(i=0;i<2;i++)
	{
		Qc12864_WriteCmd(0x80+y++);
		Qc12864_WriteCmd(0x80+(x/8));
		Qc12864_WriteData(0);
		Qc12864_WriteData(0);
	}
}
void Qc12864_Clear(void)
{
	u8 i,j;	
	for(i=0;i<32;i++) //i:垂直像素点
	{
		Qc12864_WriteCmd(0x80+i);
	  Qc12864_WriteCmd(0x80);
		for(j=0;j<32;j++) Qc12864_WriteData(0);//
	}
}
void Qc12864_SoftInit(void) //LCD
{
	delay_ms(40);
  Qc12864_WriteCmd(0x30); //选择基本指令集
	Qc12864_WriteCmd(0x0c); //开显示，无游标，不反白
  Qc12864_WriteCmd(0x01); //清屏，DDRAM地址为0
	delay_ms(10);
  Qc12864_WriteCmd(0x06); //
	Qc12864_WriteCmd(0x80); //
	
	Qc12864_WriteCmd(0x34);//选择扩展指令集
	Qc12864_WriteCmd(0x36);//打开绘图开关
}
void Qc12864_Init(void)
{
	Qc12864_Gpio_Config();
	Qc12864_SoftInit();
	Qc12864_Clear();
}

