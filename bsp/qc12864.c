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
//12864 ��ֱ��ַ0-31��ˮƽ��ַ0-16��ˮƽ�����ֽ�Ϊ��λ
//ˮƽ0-7 �ϰ��� ˮƽ8-15 �°���
//��������64*8�洢������ucgui
u16 Lcd_Data_Cache[64][8]={
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
  {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
};

void Qc12864_Gpio_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOD,ENABLE);
	
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable , ENABLE);//����JTAG,������SW
	
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
/*************����һ����Ļ���****************/
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
/**************����һ���ֽڵĻ���*************************/
//xsize �ֽ�����Ч��λ�� diff:
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
/***********************��һ���ֽڵ�����*******************/
//������
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
  x=x/16;     //�ҵ����ص������ֵ�ˮƽλ��
	if(y>31)  //�Ƿ��°���
	{
		y-=32;
		x+=8;
	}
  Qc12864_WriteCmd(0x80+y);    //�趨��ֱ��ַ
  Qc12864_WriteCmd(0x80+x);

  Qc12864_WriteData((word>>8)&0xff); 	
  Qc12864_WriteData(word&0xff);
		
}
/**********************�������ֵ�����*******************/
//ע�⣬len���ֳ���
void Qc12864_Draw_nWord(u8 x,u8 y,u16* str,u8 len)
{
	u8 i;
	x=x/16;     //�ҵ����ص������ֵ�ˮƽλ��
	if(y>31)  //�Ƿ��°���
	{
		y-=32;
		x+=8;
	}
  Qc12864_WriteCmd(0x80+y);    //�趨��ֱ��ַ
  Qc12864_WriteCmd(0x80+x);
	
	for(i=0;i<len;i++)
	{
		Qc12864_WriteData((*str>>8)&0xff); 
		Qc12864_WriteData(*str&0xff);
		str++;
	}
}
/*********************���㺯��***************************/
//x:ˮƽ���� 0-127   y:��ֱ���� 0-64
//��ͼʱΪ��ˮƽ��ֱɨ�跽ʽ����ʵ��Ϊ16*64
//���з�ʽ���������ݣ����Դ�
void Qc12864_Draw_Point(u8 x,u8 y,u8 bit) 
{
	u16 word;	
	word=Refresh_Cache_Point(x,y,bit);
	Qc12864_Draw_nWord(x,y,&word,1);
}
/*********************�õ���ֵ***********************/
u8 Qc12864_Get_Point(u8 x,u8 y)
{
	u8 bit,tmp;
	tmp=Lcd_Data_Cache[y][x/16];
	if(tmp&(1<<(x%16))) bit=1;
	return bit;
}
/********************������************************/
void Qc12864_DrawHLine(u8 x0,u8 y,u8 x1,u8 white)
{
//	//���㻭�ߣ��ٶ���
//	for(;x0<x1;x0++) Qc12864_Draw_Point(x0,y,1);
	u16 word;
	u8 i=0,len;
  if((x1-x0)<(16-x0%16))  //����������һ��������
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
/*********************������*********************/
void Qc12864_Draw_VLine(u8 x,u8 y0,u8 y1,u8 white)
{
	for(;y0<y1;y0++) Qc12864_Draw_Point(x,y0,white);
}
/**********************�����ݻ�һ��*********************/
// x0,yΪ��ʼ���ص��ַ��xize:���س��� nbytes ��ÿ�������ֽڳ���
// diff=0
static void Qc12864_Draw_Linex(u8 x,u8 y,u8 xsize,u8 nbytes,u8* ptr)
{
	u16 tmp,cmp;
	u8 i=0,j=0,len,bit;
	
	if((x%16)==0)   //������λ
	{
		len=0;
		//������λ
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
/**********************��ͼ(��֧�����ţ�������)*****************************/
//xsize:�����ص���
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


/********************�����֣�����Ϊ12*16***********************/
void Qc12864_Draw_GB12_Linex(u8 x,u8 y,u8 xsize,const u8* ptr)
{
	u8 i;
	x=x/8;
	Qc12864_WriteCmd(0x80+y);    //�趨��ֱ��ַ
  Qc12864_WriteCmd(0x80+x);
	
	for(i=0;i<(xsize/8);i++)
	{ 
		Qc12864_WriteData(*ptr);
		ptr++;
	}
}
/**************************��ASC������12*8************************/
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
//row: �к�1-4
//colu:�к�1-8
//ptr: ָ���ֵ�ָ��
void Qc12864_Draw_GB12(u8 row,u8 colu,const u8 *ptr)
{
	u8 x,y,i;

	y=(row%2)?0:16;
	x=(row<3)?0:(8*8);
	x+=(colu-1)*8;
	y+=2;  //16*16��ʾ12*16���ַ����ʵ������м�
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
	for(i=0;i<32;i++) //i:��ֱ���ص�
	{
		Qc12864_WriteCmd(0x80+i);
	  Qc12864_WriteCmd(0x80);
		for(j=0;j<32;j++) Qc12864_WriteData(0);//
	}
}
void Qc12864_SoftInit(void) //LCD
{
	delay_ms(40);
  Qc12864_WriteCmd(0x30); //ѡ�����ָ�
	Qc12864_WriteCmd(0x0c); //����ʾ�����α꣬������
  Qc12864_WriteCmd(0x01); //������DDRAM��ַΪ0
	delay_ms(10);
  Qc12864_WriteCmd(0x06); //
	Qc12864_WriteCmd(0x80); //
	
	Qc12864_WriteCmd(0x34);//ѡ����չָ�
	Qc12864_WriteCmd(0x36);//�򿪻�ͼ����
}
void Qc12864_Init(void)
{
	Qc12864_Gpio_Config();
	Qc12864_SoftInit();
	Qc12864_Clear();
}

