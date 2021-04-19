#include <Text.h>
#include <fdc2214.h>
#include <oled.h>
#include "delay.h"
#include <SortArr.h>
#include "usart.h"
//#include <math.h>
//#include <stdlib.h>
extern unsigned char EntryFigerTimes;//录进次数
extern unsigned char EntryFistTimes ;//录进次数
extern unsigned char Train_Figer_Entry_Flg;//训练模式标志位
extern unsigned char Train_Fist_Entry_Flg; //训练模式标志位

extern float ArrayData[10];
extern float ArrayA[8];
extern float ArrayB[8];
//范围变量
float FMax,FMin;
float EMax,EMin;
float DMax,DMin;
float CMax,CMin;
float BMax,BMin;
float AMax,AMin;
//范围变量
float DDMax,DDMin;
float CCMax,CCMin;
float BBMax,BBMin;
float AAMax,AAMin;
//四路电容总值
extern float Total_Value;
extern float temp0;
extern float temp1;
extern float temp2;
extern float temp3;
void TextDelay()
{
    delay_ms(10);
}
/*
函数功能：录入手势
*/
void EntryFiger()
{   
	ArrayData[0]=Filtering();//滤波

	switch(EntryFigerTimes)
   {
     case 0: ArrayA[0]=ArrayData[0];OLED_ShowCN(12,4,40);data_tran(0,0x30);break;
     case 1: ArrayA[1]=ArrayData[0];OLED_ShowCN(28,4,38);data_tran(0,0x31);break;
     case 2: ArrayA[2]=ArrayData[0];OLED_ShowCN(44,4,38);data_tran(0,0x32);break;
     case 3: ArrayA[3]=ArrayData[0];OLED_ShowCN(60,4,38);data_tran(0,0x33);break;
     case 4: ArrayA[4]=ArrayData[0];OLED_ShowCN(76,4,38);data_tran(0,0x34);break;
     case 5: ArrayA[5]=ArrayData[0];OLED_ShowCN(92,4,38);data_tran(0,0x35);break;
     default: break;
   }

  EntryFigerTimes++;

  if(EntryFigerTimes>=6)
  {
     Train_Figer_Entry_Flg=0;
     EntryFigerTimes=0;
  }
}

/*
函数功能：录入手势
*/
void EntryFist()
{   
	ArrayData[0]=Filtering();//滤波

	switch(EntryFistTimes)
   {
     case 0: ArrayB[0]=ArrayData[0];OLED_ShowCN(26,4,40);;break;
     case 1: ArrayB[1]=ArrayData[0];OLED_ShowCN(42,4,38);data_tran(0,0x31);break;  //石头
     case 2: ArrayB[2]=ArrayData[0];OLED_ShowCN(58,4,38);data_tran(0,0x32);break;  //剪刀
     case 3: ArrayB[3]=ArrayData[0];OLED_ShowCN(74,4,38);data_tran(0,0x33);break;  //布
     default: break;
   }

  EntryFistTimes++;

  if(EntryFistTimes>=4)
   {
     Train_Fist_Entry_Flg=0;
     EntryFistTimes=0;
   }
}
/*
函数功能：对不同手势，确定不同范围
*/
void  JudgmentFigerRange()
{
    float Diff=0;//可不用声明Diff  这里只是为了便于理解
    float Ftemp=FMin,Etemp=EMin,Dtemp=DMin,Ctemp=CMin,Btemp=BMin;
 
     FMax=ArrayA[5]+5;  FMin=ArrayA[5]-1;
     EMax=ArrayA[4]+1;  EMin=ArrayA[4]-1;
     DMax=ArrayA[3]+1;  DMin=ArrayA[3]-1;
     CMax=ArrayA[2]+1;  CMin=ArrayA[2]-1;
     BMax=ArrayA[1]+1;  BMin=ArrayA[1]-1;
     AMax=ArrayA[0]+1;  AMin=ArrayA[0]-5;

    if((FMin-EMax)<0)
    {
        Diff=(FMin-EMax)/2;
        FMin=EMax+Diff;
        EMax=Ftemp-Diff;
    }
    else
    {
        Diff=(FMin-EMax)/2;
        FMin=FMin-Diff;
        EMax=EMax+Diff;
    }


    if((EMin-DMax)<0)
    {
        Diff=(EMin-DMax)/2;
        EMin=DMax+Diff;
        DMax=Etemp-Diff;
    }
    else
    {
        Diff=(EMin-DMax)/2;
        EMin=EMin-Diff;
        DMax=DMax+Diff;
    }

    if((DMin-CMax)<0)
    {
        Diff=(DMin-CMax)/2;
        DMin=CMax+Diff;
        CMax=Dtemp-Diff;
    }
    else
    {
        Diff=(DMin-CMax)/2;
        DMin=DMin-Diff;
        CMax=CMax+Diff;
    }

    if((CMin-BMax)<0)
    {
        Diff=(CMin-BMax)/2;
        CMin=BMax+Diff;
        BMax=Ctemp-Diff;
    }
    else
    {
        Diff=(CMin-BMax)/2;
        CMin=CMin-Diff;
        BMax=BMax+Diff;
    }

    if((BMin-AMax)<0)
    {
        Diff=(BMin-AMax)/2;
        BMin=AMax+Diff;
        AMax=Btemp-Diff;
    }
    else
    {
        Diff=(BMin-AMax)/2;
        BMin=BMin-Diff;
        AMax=AMax+Diff;
    }
}

/*
函数功能：对不同手势，确定不同范围
*/
void  JudgmentFistRange()
{
    float Diff=0;//可不用声明Diff  这里只是为了便于理解
    float DDtemp=DDMin,CCtemp=CCMin,BBtemp=BBMin;
    DDMax=ArrayB[3]+5;  DDMin=ArrayB[3]-1;
    CCMax=ArrayB[2]+1;  CCMin=ArrayB[2]-1;
    BBMax=ArrayB[1]+1;  BBMin=ArrayB[1]-1;
    AAMax=ArrayB[0]+1;  AAMin=ArrayB[0]-5;


    if((DDMin-CCMax)<0)
    {
        Diff=(DDMin-CCMax)/2;
        DDMin=CCMax+Diff;
        CCMax=DDtemp-Diff;
    }
    else
    {
        Diff=(DDMin-CCMax)/2;
        DDMin=DDMin-Diff;
        CCMax=CCMax+Diff;
    }

    if((CCMin-BBMax)<0)
    {
        Diff=(CCMin-BBMax)/2;
        CCMin=BBMax+Diff;
        BBMax=CCtemp-Diff;
    }
    else
    {
        Diff=(CCMin-BBMax)/2;
        CCMin=CCMin-Diff;
        BBMax=BBMax+Diff;
    }

    if((BBMin-AAMax)<0)
    {
        Diff=(BBMin-AAMax)/2;
        BBMin=AAMax+Diff;
        AAMax=BBtemp-Diff;
    }
    else
    {
        Diff=(BBMin-AAMax)/2;
        BBMin=BBMin-Diff;
        AAMax=AAMax+Diff;
    }
}
/*
函数功能：确定手势
*/
unsigned char JudgmentFiger()
{
    if(Total_Value>=FMin)
       {
           return 0;
       }
    if((Total_Value>=EMin)&&(Total_Value<EMax))
       {
           return 1;
       }

    if((Total_Value>=DMin)&&(Total_Value<DMax))
       {
            return 2;
       }

     if((Total_Value>=CMin)&&(Total_Value<CMax))
       {
            return 3;
       }

      if((Total_Value>=BMin)&&(Total_Value<BMax))
       {
            return 4;
       }

      if(Total_Value<BMin)
        {
            return 5;
        }

      else return 0;
}


/*
函数功能：确定手势
*/
unsigned char JudgmentFist()
{

    if(Total_Value>=DDMin)
       {
            return 0;
       }

     if((Total_Value>=CCMin)&&(Total_Value<CCMax))
       {
            return 1;
       }

      if((Total_Value>=BBMin)&&(Total_Value<BBMax))
       {
            return 2;
       }

      if(Total_Value<BBMin)
        {
            return 3;
        }

      else return 0;
}

/**
**简单滤波函数
**/
float Filtering()
{
	unsigned char i=0;
	for(i=0;i<10;i++)
	{
		ArrayData[i]=Total_Value;//采集10次分别取4通道平均值
    delay_ms(100);
	}
	SortArr(ArrayData,10);//冒泡排序
	
	ArrayData[0]=0;
	for(i=1;i<9;i++)
  {ArrayData[0]=ArrayData[0]+ArrayData[i];}
	ArrayData[0]=ArrayData[0]/8;
	
  return ArrayData[0];
}

/**
**数据传输函数
**/
void data_tran(int mode, char num)
{
	unsigned char i=0;
	switch(mode)
	{
		
     case 0:
			 Usart_SendByte(USART1,num);
		   Usart_SendByte(USART1,0x0D); //换行符
			 //Usart_SendByte(USART1,0x0A);
		   for(i=0;i<10;i++)
		 {
			  printf("%d ", (unsigned long int)temp0/10000); 
			  printf("%d ", (unsigned long int)temp1/10000);
			  printf("%d ", (unsigned long int)temp2/10000);
			  printf("%d ", (unsigned long int)temp3/10000);
		    //printf("%d %d %d %d ", (unsigned long int)temp0/10000, (unsigned long int)temp1/10000, (unsigned long int)temp2/10000, (unsigned long int)temp3/10000);
			  Usart_SendByte(USART1,0x0D); //换行符
			  //Usart_SendByte(USART1,0x0A);
        Filtering();
	   }
		    Usart_SendByte(USART1,0x44); //终止标记
		 break;
		 
		 case 1:
			 //无数量编号
		   for(i=0;i<10;i++)
		 {
		    printf("%d ", (unsigned long int)temp0/10000); 
			  printf("%d ", (unsigned long int)temp1/10000);
			  printf("%d ", (unsigned long int)temp2/10000);
			  printf("%d ", (unsigned long int)temp3/10000);
			  Usart_SendByte(USART1,0x0D); //换行符
			  //Usart_SendByte(USART1,0x0A);
        Filtering();
	   }
		    Usart_SendByte(USART1,0x44); //终止标记
		 break;
     default: break;
		 
   }
}
