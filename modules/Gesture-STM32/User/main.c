#include "stm32f10x.h"
#include "sys.h"
#include "led.h"
#include "Beep.h"
#include "delay.h"
#include "stdio.h"
#include "usart.h"
#include "key.h"
#include <fdc2214.h>
#include <oled.h>
#include <niming.h>
#include <text.h>
#include <Menu.h>
#include <SortArr.h>
#include "time5.h"
unsigned char EntryFigerTimes=0;//录入54321时，所需记录的次数
unsigned char EntryFiger_Num = 0x30;//录入54321时，所需记录的次数
unsigned char EntryFistTimes =0;//录入石头剪刀布时，所需记录的次数
unsigned char EntryFist_Num = 0x30;//录入54321时，所需记录的次数
//相应标志位
unsigned char Interrupt_Flg=1,InterruptFlg=0;//按下按键 产生的相应标志位
unsigned char Train_Figer_Entry_Flg=0,Train_Fist_Entry_Flg=0;//进入猜拳训练 划拳训练 相应标志位
unsigned char Judge_Figer_Flg=0,Judge_Fist_Flg=0;//进入猜拳判决 划拳判决 相应标志位
//数组排序次数
unsigned char SortTimes=0;//录入结束后，对数组进行排序的次数
//电容值存放数据
float temp0=0,temp1=0,temp2=0,temp3=0;//四路通道值存放的变量
float Total_Value;//四路通道总值
//待存录入数组
float ArrayData[10]={0};//录入手势时，四路通道总值要存放的数组(在录入手势时采集了十次数据，并且采用滤波算法)
float ArrayA[8]={0};//录入54321时 存放54321数据的数组
float ArrayB[8]={0};//录入石头剪刀布时 存放石头剪刀布数据的数组
//按键数据
int Page_New=0;//OLED菜单页面
int ConfirmKey=0;//确认键(确认键按下: 1)
int KeyUpDownFlg=0;//按键上与下(两个按键采用一个变量 上键: 1 下键: -1)
int  KeyRightLeftFlg=0;//按键右与左(两个按键采用一个变量 右键: 1 左键: -1)
//加载中 次数
long int Loding_Times=0;//加载中 相应判断次数

void GetFouChVal(void);//读取四路通道值的函数
int main()
{
	  delay_init();	    	                             //延时函数初始化	
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //设置中断优先级分组2
    uart_init(115200);	                             //串口初始化为115200
    TIM2_init(79,7199);                              //10ms中断
	  Beep_Init();                                     //蜂鸣器初始化
    IIC_Start();                                     //IIC初始化
    Oled_Init();                                     //oled初始化
    LED_Init();	                                     //led初始化
    Key_Init();                                      //初始化五向按键和开关按键，可使用Key_Scan（）函数得到返回值
    key_exti_test();                                 //按键检测
	
	
	  while(FDC2214_Init());                           //fdc2214初始化 成功：0 失败：1
    Beep=0;delay_ms(10);Beep=1;                      //蜂鸣器提示
    delay_ms(100);
		UI_TILTLE();                                     //标题

	
	while(1)
{
		    Loding_Times=0;   //加载中函数的加载次数
 //在页面0 显示四路通道值
     if(Page_New==0)
     {
         Oled_Printf_U32(28,3,(unsigned long int)temp0,0);
         Oled_Printf_U32(28,4,(unsigned long int)temp1,0);
         Oled_Printf_U32(28,5,(unsigned long int)temp2,0);
         Oled_Printf_U32(28,6,(unsigned long int)temp3,0);
			   //printf("0");
			   //Usart_SendByte(USART1,0x0D); //换行符
			   //Usart_SendByte(USART1,0x0A);
			   //printf("%d", (unsigned long int)temp0); 
			   //printf("%d", (unsigned long int)temp1);
			   //printf("%d", (unsigned long int)temp2);
			   //printf("%d", (unsigned long int)temp3);
			   //printf("D");
			   //delay_ms(10);
			   //Usart_SendByte(USART1,0x0D); //换行符
			   //Usart_SendByte(USART1,0x0A);
			   
     }
/**
**在按键按下时 oled菜单界面进行相应的切换
**并且相应标志位被重新赋值
**/
    if(Interrupt_Flg==1)//按一次方向按键'Interrupt_Flg'变量会被置1
    {
				if(Page_New==0)
				{
					ConfirmKey=0;
				}
	
				OLED_Page_Show(KeyRightLeftFlg, KeyUpDownFlg,ConfirmKey);//此函数为oled菜单显示函数(参数分别为: 按键左右值 按键上下值 以及确认键)
			  ConfirmKey=0;
		  	Interrupt_Flg=0;
	  		InterruptFlg =0;  		
		  	KeyUpDownFlg =0;
		  	KeyRightLeftFlg=0;
    }

    InterruptFlg=0;//此标志位为 录入手势时所需的标志位

/**
** 在进入'Figer'训练界面时'Train_Figer_Entry_Flg'标志位被置1
** 在此训练期间,不会跳出此循环 除非强制返回或录入结束
** 相应标志位在此处被赋值
**/
		while(Train_Figer_Entry_Flg)
		{   SortTimes=0;
        Loading();
			
       if(InterruptFlg==1)
		    {
          EntryFiger();  //按一次确认录入一次！
          InterruptFlg=0;
        }
				
				ConfirmKey=0;
        KeyUpDownFlg=2;
        Interrupt_Flg=1;
				KeyRightLeftFlg=-1;
    }
/**
**'Figer'训练期间如果强制返回则代表录入失败
** 相应所存放数据的数组被清零
**/
   if(EntryFigerTimes!=0)
    {
				unsigned char i;
			
        for(i=0;i<6;i++)
        {
          ArrayA[i]=0;
        }
				
        EntryFigerTimes=0;//录入手势时的次数
				KeyUpDownFlg=0;
    }
/**
** 在进入'JudgeNum'判决界面时'Judge_Figer_Flg'标志位被置1
** 因为在录入手势时不需要按照顺序，所以此处用到冒泡排序算法
** oled显示相应结果
**/
	  if(Judge_Figer_Flg==1)
		{
        if(SortTimes==0)//录入一次数据只排序一次
        {
           SortArr(ArrayA,6);//冒泡排序算法
           JudgmentFigerRange();//对手势范围进行整定
        }
        switch(JudgmentFiger())
         {
           case 0: OLED_ShowCN(52,4,35);OLED_ShowCN(52,5,35); break;
           case 1: OLED_ShowStr(52,4,"1",2); break;
           case 2: OLED_ShowStr(52,4,"2",2); break;
				   case 3: OLED_ShowStr(52,4,"3",2); break;
					 case 4: OLED_ShowStr(52,4,"4",2); break;
           case 5: OLED_ShowStr(52,4,"5",2); break;
           default: break;
         }
				if(InterruptFlg==1)  //中断发送测试数据
		    {
          data_tran(1,0x30);
          InterruptFlg=0;
        }
        SortTimes=1;
    }

/**
** 在进入'Fist'训练界面时'Train_Fist_Entry_Flg'标志位被置1
** 在此训练期间,不会跳出此循环 除非强制返回或录入结束
** 相应标志位在此处被赋值
**/
	  while(Train_Fist_Entry_Flg)
    {
        Loading();
        SortTimes=0;
        if(InterruptFlg==1)
        {
          EntryFist();    //按一次录入一次！
          InterruptFlg=0;
        }
			 
        ConfirmKey=0;
        KeyUpDownFlg=2;
        Interrupt_Flg=1;
			  KeyRightLeftFlg=-1;
    }
/**
**'Fist'训练期间如果强制返回则代表录入失败
** 相应所存放数据的数组被清零
**/
     if(EntryFistTimes!=0)
    {
				unsigned char i;
				for(i=0;i<4;i++)
				{
            ArrayB[i]=0;
				}
				
        EntryFistTimes=0;//录入手势时的次数
				KeyUpDownFlg=0;
     }
/**
** 在进入'JudgeFist'判决界面时'Judge_Fist_Flg'标志位被置1
** 因为在录入手势时不需要按照顺序，所以此处用到冒泡排序算法
** oled显示相应结果
**/
    if(Judge_Fist_Flg==1)
     {
        if(SortTimes==0)//录入一次数据只排序一次
        {
           SortArr(ArrayB,4);
           JudgmentFistRange();
         }
         switch(JudgmentFist())
         {
           case 0: OLED_ShowCN(48,5,35); OLED_ShowCN(64,5,35);break;//空格
           case 1: OLED_ShowCN(48,5,30); OLED_ShowCN(64,5,31);break;//石头
           case 2: OLED_ShowCN(48,5,32); OLED_ShowCN(64,5,33);break;//剪刀
           case 3: OLED_ShowCN(48,5,34); OLED_ShowCN(64,5,35);break;//布
           default: break;
         }
				 if(InterruptFlg==1)  //中断发送测试数据
		    {
          data_tran(1,0x30);
          InterruptFlg=0;
        }
        SortTimes=1;
		}
		 
		
  }

	
}
/**
**可在此函数中直接采集四路通道值
**/

void GetFouChVal()
{
		temp0= Cap_Calculate(0);
    temp1= Cap_Calculate(1);
    temp2= Cap_Calculate(2);
    temp3= Cap_Calculate(3);
    Total_Value=(temp0+temp1+temp2+temp3)/10000;
}
/**
**定时器5中断函数 10ms
**在定时器中采集四路通道值，并计算总值
**/
void TIM2_IRQHandler(void)         
{
	 
	if(TIM2->SR&0X0001)//溢出中断
	{	
		temp0= Cap_Calculate(0);
    temp1= Cap_Calculate(1);
    temp2= Cap_Calculate(2);
    temp3= Cap_Calculate(3);
    Total_Value=(temp0+temp1+temp2+temp3)/10000;
		
	}						   
  TIM2->SR&=~(1<<0);
}



