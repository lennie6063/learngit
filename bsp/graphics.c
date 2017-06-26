#include "graphics.h"
#include "gb_12.h"
#include "stdlib.h"
#include "delay.h"


typedef struct tagMENUITEM                                      
{
  u8         PersistItemIndex;                                                                
  u8         SelectIndex;                                    
  u8         Tag;                                         
}MENUITEM;

typedef void (*LPPAINT)(MENUITEM* pMenu);
typedef void (*LPKEYPRESSED)(u8 key);

typedef struct tagPERSISTMENUITEM 
{
  u8         Caption[8];                                    
  u8         SubItemIndex[6];
  u8         SubItemCount;                                     /*子菜单*/                                 
  u8         ParentItemIndex;                               
  u8         RepaintInterval;                                  /* 重绘/刷新 */

  LPPAINT       pfnPaint;                                       /* 画图函数    */
  LPPAINT       pfnInvalidate;                                
  LPKEYPRESSED  pfnKeyPressed;                                  /* 按键时事件   */
}PERSISTMENUITEM;

typedef struct tagMENUSYSTEM
{
  u8         SelectMP;                                      
  u8         TickElapsed;                                    
  u8         IdleTick;
}MENUSYSTEM;

static void graphPaintMenu(MENUITEM* pMenu);
static void graphKeyDefault(u8 key);
static void graphPaintTemper(MENUITEM* pMenu);
static void graphPaintNetParam(MENUITEM* pMenu);
static void graphPaintSelfParam(MENUITEM* pMenu);
static void graphPaintCommParam(MENUITEM* pMenu);
static void graphPaintRelayMenu(MENUITEM* pMenu);


static void graphKeyControl(u8 key);

/*****************************************************************************************************************
                                                  
*****************************************************************************************************************/
const PERSISTMENUITEM _PersistMenus[] = 
{
  /* INDEX           |    Name                                |  Comment                                      */
  
  /* 0               | Root Menu Item                         |                                               */
  { { 0, 0, 0 }, { 1, 2, 3, 0, 0, 0  }, 3, 0, 0,
      graphPaintMenu, 0, graphKeyDefault },
	 /* 1               | 设备参数                               |                                           */
  { { GB_SHE, GB_BEI, GB_CAN, GB_SHU_1 }, { 4, 5, 6, 0, 0, 0 }, 3, 0, 0, 
      graphPaintMenu, 0, graphKeyDefault},
   /* 2               | 运行数据                               |                                               */
  { { GB_YUN, GB_XING, GB_SHU_1, GB_JU }, { 7, 0, 0, 0, 0, 0 }, 1, 0, 10,
      graphPaintTemper, graphPaintTemper, graphKeyDefault },
  /* 3              | 本地控制                                 |                                               */
  { { GB_BEN, GB_DI, GB_KONG, GB_ZHI }, { 8, 9, 10, 0, 0, 0 },   3, 0, 0,
      graphPaintMenu, 0, graphKeyDefault },                                                                                    
  /* 4              | 参数 -> 主站地址                         |                                               */
  { { GB_ZHU, GB_ZHAN, GB_DI, GB_ZHI_1 }, { 0, 0, 0, 0, 0, 0 },  0, 1, 0, 
      graphPaintNetParam, 0, graphKeyDefault },
  /* 5               | 参数 -> 设备地址                      |                                               */
  { { GB_SHE, GB_BEI, GB_DI, GB_ZHI_1 }, { 0, 0, 0, 0, 0, 0 }, 0, 1, 0, 
      graphPaintSelfParam, 0, graphKeyDefault },
	/* 6               | 参数 -> 通讯参数                       |                                               */
  { { GB_TONG, GB_XUN, GB_CAN, GB_SHU_1 }, { 0, 0, 0, 0, 0, 0 }, 0, 1, 0, 
      graphPaintCommParam, 0, graphKeyDefault },
	/* 7               | 运行数据->温湿度光度                   |                                               */
  { { GB_WEN, GB_SHI_1, GB_GUANG, GB_DU }, { 0, 0, 0, 0, 0, 0 }, 0, 2, 0, 
      0, 0, graphKeyDefault},
	/* 8               | 本地控制->继电器                       |                                               */
  { { GB_JI_1, GB_DIAN, GB_QI, ASC_NUM1}, { 0, 0, 0, 0, 0, 0 }, 0, 3, 0, 
      graphPaintRelayMenu, 0, graphKeyControl },
	/* 9               | 本地控制->继电器2                       |                                               */
  { { GB_JI_1, GB_DIAN, GB_QI, ASC_NUM2}, { 0, 0, 0, 0, 0, 0 }, 0, 3, 0, 
      graphPaintRelayMenu, 0, graphKeyControl },
	/* 10               | 本地控制->蜂鸣器1                       |                                               */
  { { GB_FENG, GB_MING, GB_QI,  ASC_NUM1}, { 0, 0, 0, 0, 0, 0 }, 0, 3, 0,
      graphPaintRelayMenu, 0, graphKeyControl },
};

MENUITEM  _Menus[] = 
{
  { 0,  0,  0 },
  { 1,  0,  0 },
  { 2,  0,  0 },
  { 3,  0,  0 },
  { 4,  0,  0 },
  { 5,  0,  0 },
  { 6,  0,  0 },
  { 7,  0,  0 },
  { 8,  0,  0 },
  { 9,  0,  0 },
	{ 10,  0,  0 },
};

u8                      _GraphCache[32];

MENUITEM               *_CurrentMenu,*_ParentMenu;                          
const PERSISTMENUITEM*  _PersistMenu;                           
MENUSYSTEM              _MenuSystem; 


/**************************加载菜单*************************************/
void graphLoadMenu(u8 index)
{
	_ParentMenu=_CurrentMenu;
  _CurrentMenu = &_Menus[index];
  _PersistMenu = &_PersistMenus[_CurrentMenu->PersistItemIndex];
  
  if (_PersistMenu->pfnPaint != 0)                              /* Paint screen                                 */
    _PersistMenu->pfnPaint(_CurrentMenu);
  if (_PersistMenu->pfnInvalidate != 0)                         /* Call pfnInvalidate to update data            */
    _PersistMenu->pfnInvalidate(_CurrentMenu);
}
/**************************开机页面****************************/
void graphMain(void)
{   
  graphLoadMenu(2);
}

//void OneNet_Wait(void)
//{   
//  graphLoadMenu(2);
//}

//void OneNet_Access(void)
//{   
//  graphLoadMenu(2);
//}


/**************************按键事件获取****************************/
void graphOnKeyPressed(u8 key)
{
  _MenuSystem.IdleTick = 0;
  
  _PersistMenu->pfnKeyPressed(key);//
}

/****************************刷新处理，未测试********************************/
void graphOnTick(void)
{
  _MenuSystem.IdleTick++;
  
  if (_PersistMenu->RepaintInterval != 0)               
  {
    _MenuSystem.TickElapsed++;  
    if (_MenuSystem.TickElapsed >= _PersistMenu->RepaintInterval)
    {
      _PersistMenu->pfnInvalidate(_CurrentMenu);
      _MenuSystem.TickElapsed = 0;
    }
  }
}

/**********************菜单绘制函数**********************************/
void cstrcpy(u8* a,u8* b)
{
	u8 i;
	for(i=0;i<4;i++) *a++=*b++;
}
static void graphPaintMenu(MENUITEM* pMenu)
{
  const PERSISTMENUITEM* pSubItem;
  u8 childcnt, i,j;
  u8 caption[8];
  u8 row, col,a;
  
  childcnt = _PersistMenu->SubItemCount;
  for(i = 0; i < childcnt; i++)                                 
  {  
		if(_CurrentMenu->SelectIndex>=childcnt) _CurrentMenu->SelectIndex=0;
    if(_CurrentMenu->SelectIndex == i)
		{
			graphDrawChnStr(i+1,1,&GB_12[ASC_MORE][0]);
		}
		else  graphDrawChnStr(i+1,1,&GB_12[ASC_SPA][0]);
		a=ASC_NUM_1+i;
		row=1+i;
		col=2;
		graphDrawChnStr(row, col,&GB_12[a][0]);
    pSubItem = &_PersistMenus[_PersistMenu->SubItemIndex[i]];
    cstrcpy(caption, (u8*)pSubItem->Caption);
    for(j=0;j<4;j++) 
		{
			a=caption[j];
			col=3+j;
			graphDrawChnStr(row, col,&GB_12[a][0]);
		}
		graphDrawChnStr(row, 7,&GB_12[0][0]);//clear
		graphDrawChnStr(row, 8,&GB_12[0][0]);
  }
}
/************************半字显示,即画ASC*********************************/
//row:行 col：列 diff ：决定所画部分在前半字还是后半字
//pos：点位置，0表示无点  div：除数(1000 ， 100 等)
static u8 graphDrawAsc(u8 row,u8 col,u32 temp,u32 div,u8 Pos,u8 diff)
{
	u8 i=0,j=0;
	while(1)
	{
		j++;
		if((temp/div)||(i!=0))
		{
			graphDrawASCNum(row,col,&GB_12[ASC_NUM0+(temp/div)][0],diff);
			diff+=8;	
			i++;
			if(diff>=16)
			{
				col++;
				diff=0;
			}
		}
		if(j==Pos) 
		{
			if(i==0)  //
			{
				graphDrawASCNum(row,col,&GB_12[ASC_NUM0][0],diff);
				diff+=8;			
				if(diff>=16)
				{
					col++;
					diff=0;
				}
			}
			graphDrawASCNum(row,col,&GB_12[ASC_POINT][0],diff);//.
			diff+=8;
			i++;			
			if(diff>=16)
			{
				col++;
				diff=0;
			}
		}
	  temp=temp%div;
		div=div/10;
		if(div==0) 
		{
			if((i==0)&&(Pos==0))
			{
				graphDrawASCNum(row,col,&GB_12[ASC_NUM0][0],diff);
				diff+=8;			
				if(diff>=16)
				{
					col++;
					diff=0;
				}
			}
			break;
		}
	}
	return (diff?(col|0x80):col);
}
/******************运行数据 菜单绘制***********************/
static void graphPaintTemper(MENUITEM* pMenu)
{
  extern int flag_12864;
	u8 diff,tmp,col;
  graphDrawChnStr(1,2,&GB_12[GB_ZHI1][0]);  //智能粮库管理系统
	graphDrawChnStr(1,3,&GB_12[GB_NENG1][0]);
	graphDrawChnStr(1,4,&GB_12[GB_LIANG1][0]);
	graphDrawChnStr(1,5,&GB_12[GB_KU1][0]);
	graphDrawChnStr(1,6,&GB_12[GB_GUAN1][0]);
	graphDrawChnStr(1,7,&GB_12[GB_LI1][0]);
	//graphDrawChnStr(1,8,&GB_12[GB_XI1][0]);
	//graphDrawChnStr(1,8,&GB_12[GB_TONG1][0]);
	
	if(flag_12864==0)                          //一号仓库
	{
		graphDrawChnStr(4,1,&GB_12[GB_YUN1][0]);
		graphDrawChnStr(4,2,&GB_12[GB_PING1][0]);
		graphDrawChnStr(4,3,&GB_12[GB_TAI1][0]);
		graphDrawChnStr(4,4,&GB_12[GB_LIAN][0]);
		graphDrawChnStr(4,5,&GB_12[GB_JIE][0]);
	  graphDrawChnStr(4,6,&GB_12[ASC_COLON][0]); //:
		graphDrawChnStr(4,7,&GB_12[GB_DENG][0]);
		graphDrawChnStr(4,8,&GB_12[GB_DAI][0]);
	}
	if(flag_12864==1)                          
	{
		graphDrawChnStr(4,1,&GB_12[GB_YUN1][0]);
		graphDrawChnStr(4,2,&GB_12[GB_PING1][0]);
		graphDrawChnStr(4,3,&GB_12[GB_TAI1][0]);
		graphDrawChnStr(4,4,&GB_12[GB_LIAN][0]);
		graphDrawChnStr(4,5,&GB_12[GB_JIE][0]);
	graphDrawChnStr(4,6,&GB_12[ASC_COLON][0]); //:
		graphDrawChnStr(4,7,&GB_12[GB_ZHENG][0]);
		graphDrawChnStr(4,8,&GB_12[GB_CHANG][0]);
	}
//	if(flag_12864!=0)   
	{
	graphDrawChnStr(2,2,&GB_12[GB_WEN][0]);  //温度
	graphDrawChnStr(2,3,&GB_12[GB_DU][0]);  
	graphDrawASCNum(2,4,&GB_12[ASC_COLON][0],0);//:
//  if(Temperature>0)  graphDrawASCNum(3,4,&GB_12[ASC_PLUS][0],8); //+
//	else graphDrawASCNum(2,4,&GB_12[ASC_MINUS][0],8);  //-
	tmp=graphDrawAsc(2,5,abs(Temperature),1000,2,0);
	col=tmp&0x7f;
	diff=(tmp&0x80)?8:0;
	if(diff) 
	{
		graphDrawASCNum(2,col,&GB_12[0][0],diff);
		col++;
	}
	for(;col<=6;col++) 	graphDrawChnStr(3,col,&GB_12[0][0]);
	graphDrawChnStr(2,8,&GB_12[ASC_TEMP][0]);//℃
	
	graphDrawChnStr(3,1,&GB_12[0][0]);   //  取消 >
	graphDrawChnStr(3,2,&GB_12[GB_SHI_1][0]);  //湿度
	graphDrawChnStr(3,3,&GB_12[GB_DU][0]);  
	graphDrawChnStr(3,4,&GB_12[ASC_COLON][0]); //:
	tmp=graphDrawAsc(3,5,Humidity,1000,2,0); 
	col=tmp&0x7f;
	diff=(tmp&0x80)?8:0;
	if(diff) 
	{
		graphDrawASCNum(3,col,&GB_12[0][0],diff);
		col++;
	}
	for(;col<=6;col++) 	graphDrawChnStr(3,col,&GB_12[0][0]);
	graphDrawChnStr(3,8,&GB_12[ASC_GRAM][0]);//g
	
//	graphDrawChnStr(4,2,&GB_12[GB_GUANG][0]);  //光度
//	graphDrawChnStr(4,3,&GB_12[GB_DU][0]);  
//	graphDrawChnStr(4,4,&GB_12[ASC_COLON][0]);//:
//	tmp=graphDrawAsc(4,5,Luminosity,1000,2,0);
//	col=tmp&0x7f;
//	diff=(tmp&0x80)?8:0;
//	if(diff) 
//	{
//		graphDrawASCNum(4,col,&GB_12[0][0],diff);
//		col++;
//	}
//	for(;col<=6;col++) 	graphDrawChnStr(4,col,&GB_12[0][0]);
//	graphDrawChnStr(4,8,&GB_12[ASC_PERCENT][0]);//%
}
	}
/********************主站参数************************/
static void graphPaintNetParam(MENUITEM* pMenu)
{
	u8 col=1,diff,tmp;
	graphDrawChnStr(1,1,&GB_12[GB_ZHU][0]);
	graphDrawChnStr(1,2,&GB_12[GB_ZHAN][0]);
	graphDrawChnStr(1,3,&GB_12[GB_DI][0]);
	graphDrawChnStr(1,4,&GB_12[GB_ZHI_1][0]);
	graphDrawChnStr(1,5,&GB_12[ASC_COLON][0]); //:	
	graphDrawChnStr(1,6,&GB_12[0][0]);
	
	tmp=graphDrawAsc(2,col,DEST_IP1,100,3,8);
	col=tmp&0x7f;
	diff=(tmp&0x80)?8:0;
	tmp=graphDrawAsc(2,col,DEST_IP2,100,3,diff);
	col=tmp&0x7f;
	diff=(tmp&0x80)?8:0;
	tmp=graphDrawAsc(2,col,DEST_IP3,100,3,diff);
	col=tmp&0x7f;
	diff=(tmp&0x80)?8:0;
	tmp=graphDrawAsc(2,col,DEST_IP4,100,0,diff);
	col=tmp&0x7f;
	diff=(tmp&0x80)?8:0;
	if(diff) {
		graphDrawASCNum(2,col,&GB_12[0][0],diff);  //半字显示，需清除缓存内容
		col++;
	}
  for(;col<=6;col++) 	graphDrawChnStr(2,col,&GB_12[0][0]);
	
	graphDrawChnStr(3,1,&GB_12[GB_DUAN][0]);
	graphDrawChnStr(3,2,&GB_12[GB_KOU][0]);
	graphDrawChnStr(3,3,&GB_12[GB_HAO][0]);
	graphDrawChnStr(3,4,&GB_12[ASC_COLON][0]); //:	
//	graphDrawChnStr(1,5,&GB_12[0][0]);
	tmp=graphDrawAsc(3,5,DEST_PORT,10000,0,0);
	col=tmp&0x7f;
	diff=(tmp&0x80)?8:0;
	if(diff) 
	{
		graphDrawASCNum(3,col,&GB_12[0][0],diff);
		col++;
	}
	for(;col<=6;col++) 	graphDrawChnStr(3,col,&GB_12[0][0]);
}
/*********************设备参数******************/
static void graphPaintSelfParam(MENUITEM* pMenu)
{
	u8 col=1,diff,tmp;
	graphDrawChnStr(1,1,&GB_12[GB_SHE][0]);
	graphDrawChnStr(1,2,&GB_12[GB_BEI][0]);
	graphDrawChnStr(1,3,&GB_12[GB_DI][0]);
	graphDrawChnStr(1,4,&GB_12[GB_ZHI_1][0]);
	graphDrawChnStr(1,5,&GB_12[ASC_COLON][0]); //:	
	graphDrawChnStr(1,6,&GB_12[0][0]);
	
	tmp=graphDrawAsc(2,col,SELF_IP1,100,3,8);
	col=tmp&0x7f;
	diff=(tmp&0x80)?8:0;
	tmp=graphDrawAsc(2,col,SELF_IP2,100,3,diff);
	col=tmp&0x7f;
	diff=(tmp&0x80)?8:0;
	tmp=graphDrawAsc(2,col,SELF_IP3,100,3,diff);
	col=tmp&0x7f;
	diff=(tmp&0x80)?8:0;
	tmp=graphDrawAsc(2,col,SELF_IP4,100,0,diff);
	col=tmp&0x7f;
	diff=(tmp&0x80)?8:0;
	if(diff) 
	{
		graphDrawASCNum(2,col,&GB_12[0][0],diff);
		col++;
	}
  for(;col<=6;col++) 	graphDrawChnStr(2,col,&GB_12[0][0]);

	graphDrawChnStr(3,1,&GB_12[GB_DUAN][0]);
	graphDrawChnStr(3,2,&GB_12[GB_KOU][0]);
	graphDrawChnStr(3,3,&GB_12[GB_HAO][0]);
	graphDrawChnStr(3,4,&GB_12[ASC_COLON][0]); //:	
//	graphDrawChnStr(1,5,&GB_12[0][0]);
	tmp=graphDrawAsc(3,5,SELF_PORT,10000,0,0);
	col=tmp&0x7f;
	diff=(tmp&0x80)?8:0;
	if(diff) 
	{
		graphDrawASCNum(3,col,&GB_12[0][0],diff);
		col++;
	}
	for(;col<=6;col++) 	graphDrawChnStr(3,col,&GB_12[0][0]);

}
/*********************通讯参数******************/
static void graphPaintCommParam(MENUITEM* pMenu)
{
	u8 col=1,diff,tmp;
	graphDrawChnStr(1,2,&GB_12[GB_CHUAN][0]);
	graphDrawChnStr(1,3,&GB_12[GB_KOU][0]);
	graphDrawChnStr(1,4,&GB_12[GB_HUI][0]);
	graphDrawChnStr(1,5,&GB_12[GB_XIAN_1][0]);
	graphDrawChnStr(1,6,&GB_12[ASC_COLON][0]); //:
  if(COM_ECHO)
	{		
	  graphDrawChnStr(1,7,&GB_12[GB_KAI][0]);
//		graphDrawChnStr(1,8,&GB_12[GB_QI_1][0]);
	}
	else 
	{
		graphDrawChnStr(1,7,&GB_12[GB_GUAN][0]);
//		graphDrawChnStr(1,8,&GB_12[GB_BI][0]);
	}
	
	graphDrawChnStr(2,2,&GB_12[GB_BO][0]);
	graphDrawChnStr(2,3,&GB_12[GB_TE][0]);
	graphDrawChnStr(2,4,&GB_12[GB_LV][0]);
	graphDrawChnStr(2,5,&GB_12[ASC_COLON][0]); //:
	tmp=graphDrawAsc(2,6,COM_BAUD,100000,0,0);
	col=tmp&0x7f;
	diff=(tmp&0x80)?8:0;
	if(diff) 
	{
		graphDrawASCNum(2,col,&GB_12[0][0],diff);
		col++;
	}
  
	graphDrawChnStr(3,1,&GB_12[0][0]); //>
	graphDrawChnStr(3,2,&GB_12[GB_SHU_1][0]);
	graphDrawChnStr(3,3,&GB_12[GB_JU][0]);
	graphDrawChnStr(3,4,&GB_12[GB_JIAN][0]);
	graphDrawChnStr(3,5,&GB_12[GB_GE][0]);
	graphDrawChnStr(3,6,&GB_12[ASC_COLON][0]); //:
	tmp=graphDrawAsc(3,7,DATA_INTER,1000,0,0);
	col=tmp&0x7f;
	diff=(tmp&0x80)?8:0;
	if(diff) 
	{
		graphDrawASCNum(3,col,&GB_12[0][0],diff);
		col++;
	}
}
/*********************控制菜单******************/
static void graphPaintRelayMenu(MENUITEM* pMenu)
{
//  u8 state=0xff;
	graphDrawChnStr(1,1,&GB_12[0][0]);
	graphDrawChnStr(1,2,&GB_12[0][0]);
//	graphDrawChnStr(1,6,&GB_12[ASC_COLON][0]); //:

	graphDrawChnStr(2,1,&GB_12[0][0]);
	graphDrawChnStr(2,2,&GB_12[GB_DANG][0]);
	graphDrawChnStr(2,3,&GB_12[GB_QIAN][0]);
	graphDrawChnStr(2,4,&GB_12[GB_ZHUANG][0]);
	graphDrawChnStr(2,5,&GB_12[GB_TAI][0]);
	graphDrawChnStr(2,6,&GB_12[ASC_COLON][0]); //:
	
	graphDrawChnStr(3,1,&GB_12[0][0]);
	graphDrawChnStr(3,2,&GB_12[0][0]);
	graphDrawChnStr(3,3,&GB_12[GB_SHI_2][0]);
	graphDrawChnStr(3,4,&GB_12[GB_FOU][0]);
	graphDrawChnStr(3,7,&GB_12[ASC_QUE][0]);

//	if(_ParentMenu->SelectIndex==0)  //继电器1
//	{
//		graphDrawChnStr(1,3,&GB_12[GB_JI_1][0]);
//		graphDrawChnStr(1,4,&GB_12[GB_DIAN][0]);
//		graphDrawChnStr(1,5,&GB_12[GB_QI][0]);
//	  graphDrawChnStr(1,6,&GB_12[ASC_NUM1][0]);
//		if(RELAY_IS_OFF())  state=1;
//		else state=0;
//  }
//	else if(_ParentMenu->SelectIndex==1)  //继电器2
//	{
//		graphDrawChnStr(1,3,&GB_12[GB_JI_1][0]);
//		graphDrawChnStr(1,4,&GB_12[GB_DIAN][0]);
//		graphDrawChnStr(1,5,&GB_12[GB_QI][0]);
//		graphDrawChnStr(1,6,&GB_12[ASC_NUM2][0]);
//		if(LED2_IS_OFF()) state=1;
//		else state=0;
//	}
//	else if(_ParentMenu->SelectIndex==2)  //蜂鸣器1
//	{
//		graphDrawChnStr(1,3,&GB_12[GB_FENG][0]);
//		graphDrawChnStr(1,4,&GB_12[GB_MING][0]);
//		graphDrawChnStr(1,5,&GB_12[GB_QI][0]);
//		graphDrawChnStr(1,6,&GB_12[ASC_NUM1][0]);
//		if(BEEP_IS_OFF()) state=1;
//		else state=0;
//	}
//	if(state==1)
//	{
////		graphDrawChnStr(2,7,&GB_12[GB_DUAN][0]);
//		graphDrawChnStr(2,7,&GB_12[GB_KAI][0]);
//		graphDrawChnStr(3,5,&GB_12[GB_BI][0]);
//		graphDrawChnStr(3,6,&GB_12[GB_HE][0]);
//	}
//	else if(state==0)
//	{
//		graphDrawChnStr(2,7,&GB_12[GB_HE][0]);
//		graphDrawChnStr(3,5,&GB_12[GB_DUAN_1][0]);
//		graphDrawChnStr(3,6,&GB_12[GB_KAI][0]);
//	}
}
//static void graphPaintRelayResult(u8 index,u8 result)
//{
//	u8 state=0xff;
////	graphDrawChnStr(2,1,&GB_12[0][0]);
////	graphDrawChnStr(2,2,&GB_12[GB_DANG][0]);
////	graphDrawChnStr(2,3,&GB_12[GB_QIAN][0]);
////	graphDrawChnStr(2,4,&GB_12[GB_ZHUANG][0]);
////	graphDrawChnStr(2,5,&GB_12[GB_TAI][0]);
////	graphDrawChnStr(2,6,&GB_12[ASC_COLON][0]); //:
//	graphDrawChnStr(3,4,&GB_12[ASC_POINT][0]);
//	graphDrawChnStr(3,5,&GB_12[GB_QING_1][0]);
//	switch(index)
//	{
//		case 0:
//			if(RELAY_IS_OFF())  state=1;
//		  else state=0;
//			break;
//		case 1:
//			if(LED2_IS_OFF())  state=1;
//		  else state=0;
//			break;
//		case 2:
//			if(BEEP_IS_OFF())  state=1;
//		  else state=0;
//			break;
//		default :
//			break;
//	}
//	if(state==1)
//	{
//		graphDrawChnStr(2,7,&GB_12[GB_KAI][0]);
//	}
//	else if(state==0)
//	{
//		graphDrawChnStr(2,7,&GB_12[GB_HE][0]);
//	}
////	if(result&0xf0) //操作之前的状态
////	{
////		graphDrawChnStr(3,1,&GB_12[GB_DUAN][0]);
////		graphDrawChnStr(3,2,&GB_12[GB_KAI][0]);
////	}
////	else 
////	{
////		graphDrawChnStr(3,1,&GB_12[GB_BI][0]);
////		graphDrawChnStr(3,2,&GB_12[GB_HE][0]);
////	}
//	if(result&0x0f)  //控制结果
//	{
//		graphDrawChnStr(3,2,&GB_12[GB_CHENG][0]);
//		graphDrawChnStr(3,3,&GB_12[GB_GONG_1][0]);
//		
//		graphDrawChnStr(3,6,&GB_12[GB_FAN][0]);
//		graphDrawChnStr(3,7,&GB_12[GB_HUI][0]);
//	}
//	else 
//	{
//		graphDrawChnStr(3,2,&GB_12[GB_SHI_4][0]);
//		graphDrawChnStr(3,3,&GB_12[GB_BAI][0]);
//		
//		graphDrawChnStr(3,6,&GB_12[GB_CHONG][0]);
//		graphDrawChnStr(3,7,&GB_12[GB_SHI_3][0]);
//	}
//}
/**********************按键处理************************/
static void graphKeyDefault(u8 key)
{
  switch(key)
  {
    case VK_DOWN:
      _CurrentMenu->SelectIndex++;
      if (_CurrentMenu->SelectIndex >= _PersistMenu->SubItemCount)
        _CurrentMenu->SelectIndex = 0;
      _PersistMenu->pfnPaint(_CurrentMenu);
      break;
    case VK_UP:
      if (_CurrentMenu->SelectIndex != 0)
      {
        _CurrentMenu->SelectIndex--;
      }
      else
      { 
        if (_PersistMenu->SubItemCount > 0)
          _CurrentMenu->SelectIndex = _PersistMenu->SubItemCount - 1;
      }
      _PersistMenu->pfnPaint(_CurrentMenu);
      break;
    case VK_OK:
      if (_PersistMenu->SubItemCount != 0)
        graphLoadMenu(_PersistMenu->SubItemIndex[_CurrentMenu->SelectIndex]);
      break;
    case VK_ESC:
      _CurrentMenu->SelectIndex = 0;
      _CurrentMenu->Tag = 0;
      graphLoadMenu(_PersistMenu->ParentItemIndex);
      break;
    default:
      break;
  }
}
static void graphKeyControl(u8 key)
{
//	u8 result;
  switch(key)
  {
    case VK_DOWN:
      _CurrentMenu->SelectIndex++;
      if (_CurrentMenu->SelectIndex >= _PersistMenu->SubItemCount)
        _CurrentMenu->SelectIndex = 0;
      _PersistMenu->pfnPaint(_CurrentMenu);
      break;
    case VK_UP:
      if (_CurrentMenu->SelectIndex != 0)
      {
        _CurrentMenu->SelectIndex--;
      }
      else
      { 
        if (_PersistMenu->SubItemCount > 0)
          _CurrentMenu->SelectIndex = _PersistMenu->SubItemCount - 1;
      }
      _PersistMenu->pfnPaint(_CurrentMenu);
      break;
//    case VK_OK:
//      if (_PersistMenu->SubItemCount != 0)
//        graphLoadMenu(_PersistMenu->SubItemIndex[_CurrentMenu->SelectIndex]);
//			if(_ParentMenu->SelectIndex==0)  //继电器1
//			{
//				if(RELAY_IS_OFF()) 
//				{
//					RELAY_ON();
//					result=!RELAY_IS_OFF();
//				}
//				else 
//				{
//					RELAY_OFF();
//					result=RELAY_IS_OFF()|0x10;
//				}
//			}
//			else if(_ParentMenu->SelectIndex==1)
//			{
//				if(LED2_IS_OFF()) 
//				{
//					LED2_ON();
//					result=!LED2_IS_OFF();
//				}
//				else 
//				{
//					LED2_OFF();
//					result=LED2_IS_OFF()|0x10;
//				}
//			}
//			else if(_ParentMenu->SelectIndex==2)
//			{
//				if(BEEP_IS_OFF()) 
//				{
//					BEEP_ON();
//					result=!BEEP_IS_OFF();
//				}
//				else 
//				{
//					BEEP_OFF();
//					result=BEEP_IS_OFF()|0x10;
//				}
//			}
////			graphPaintRelayResult(_ParentMenu->SelectIndex,result);
////			delay_ms(1000);
////			graphLoadMenu(_PersistMenu->ParentItemIndex);
//			_PersistMenu->pfnPaint(_CurrentMenu);//刷新
//      break;
//    case VK_ESC:
//      _CurrentMenu->SelectIndex = 0;
//      _CurrentMenu->Tag = 0;
//      graphLoadMenu(_PersistMenu->ParentItemIndex);
//      break;
    default:
      break;
  }
}

