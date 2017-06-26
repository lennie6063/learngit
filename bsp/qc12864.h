#ifndef   __QC12864_H__
#define   __QC12864_H__
#include "stm32f10x.h"

void Qc12864_Init(void);
void Qc12864_Display_Str(u8 line,char *str);
void Qc12864_DrawHLine(u8 x0,u8 y,u8 x1,u8 white);
void Qc12864_Draw_Rect(u8 x,u8 y,u8 xsize,u8 ysize,u8 en);
void Qc12864_Draw_GB12(u8 row,u8 colu,const u8 *ptr);//colu范围从1-8
void Qc12864_Draw_ASC8(u8 row,u8 colu,const u8 *ptr,u8 diff);//字节单位，故colu范围从1-16
void Qc12864_Draw_Byte(u8 x,u8 y,u8 byte);
void Qc12864_Draw_nWord(u8 x,u8 y,u16* str,u8 len);



#endif
