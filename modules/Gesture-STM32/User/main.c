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
unsigned char EntryFigerTimes=0;//¼��54321ʱ�������¼�Ĵ���
unsigned char EntryFiger_Num = 0x30;//¼��54321ʱ�������¼�Ĵ���
unsigned char EntryFistTimes =0;//¼��ʯͷ������ʱ�������¼�Ĵ���
unsigned char EntryFist_Num = 0x30;//¼��54321ʱ�������¼�Ĵ���
//��Ӧ��־λ
unsigned char Interrupt_Flg=1,InterruptFlg=0;//���°��� ��������Ӧ��־λ
unsigned char Train_Figer_Entry_Flg=0,Train_Fist_Entry_Flg=0;//�����ȭѵ�� ��ȭѵ�� ��Ӧ��־λ
unsigned char Judge_Figer_Flg=0,Judge_Fist_Flg=0;//�����ȭ�о� ��ȭ�о� ��Ӧ��־λ
//�����������
unsigned char SortTimes=0;//¼������󣬶������������Ĵ���
//����ֵ�������
float temp0=0,temp1=0,temp2=0,temp3=0;//��·ͨ��ֵ��ŵı���
float Total_Value;//��·ͨ����ֵ
//����¼������
float ArrayData[10]={0};//¼������ʱ����·ͨ����ֵҪ��ŵ�����(��¼������ʱ�ɼ���ʮ�����ݣ����Ҳ����˲��㷨)
float ArrayA[8]={0};//¼��54321ʱ ���54321���ݵ�����
float ArrayB[8]={0};//¼��ʯͷ������ʱ ���ʯͷ���������ݵ�����
//��������
int Page_New=0;//OLED�˵�ҳ��
int ConfirmKey=0;//ȷ�ϼ�(ȷ�ϼ�����: 1)
int KeyUpDownFlg=0;//����������(������������һ������ �ϼ�: 1 �¼�: -1)
int  KeyRightLeftFlg=0;//����������(������������һ������ �Ҽ�: 1 ���: -1)
//������ ����
long int Loding_Times=0;//������ ��Ӧ�жϴ���

void GetFouChVal(void);//��ȡ��·ͨ��ֵ�ĺ���
int main()
{
	  delay_init();	    	                             //��ʱ������ʼ��	
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //�����ж����ȼ�����2
    uart_init(115200);	                             //���ڳ�ʼ��Ϊ115200
    TIM2_init(79,7199);                              //10ms�ж�
	  Beep_Init();                                     //��������ʼ��
    IIC_Start();                                     //IIC��ʼ��
    Oled_Init();                                     //oled��ʼ��
    LED_Init();	                                     //led��ʼ��
    Key_Init();                                      //��ʼ�����򰴼��Ϳ��ذ�������ʹ��Key_Scan���������õ�����ֵ
    key_exti_test();                                 //�������
	
	
	  while(FDC2214_Init());                           //fdc2214��ʼ�� �ɹ���0 ʧ�ܣ�1
    Beep=0;delay_ms(10);Beep=1;                      //��������ʾ
    delay_ms(100);
		UI_TILTLE();                                     //����

	
	while(1)
{
		    Loding_Times=0;   //�����к����ļ��ش���
 //��ҳ��0 ��ʾ��·ͨ��ֵ
     if(Page_New==0)
     {
         Oled_Printf_U32(28,3,(unsigned long int)temp0,0);
         Oled_Printf_U32(28,4,(unsigned long int)temp1,0);
         Oled_Printf_U32(28,5,(unsigned long int)temp2,0);
         Oled_Printf_U32(28,6,(unsigned long int)temp3,0);
			   //printf("0");
			   //Usart_SendByte(USART1,0x0D); //���з�
			   //Usart_SendByte(USART1,0x0A);
			   //printf("%d", (unsigned long int)temp0); 
			   //printf("%d", (unsigned long int)temp1);
			   //printf("%d", (unsigned long int)temp2);
			   //printf("%d", (unsigned long int)temp3);
			   //printf("D");
			   //delay_ms(10);
			   //Usart_SendByte(USART1,0x0D); //���з�
			   //Usart_SendByte(USART1,0x0A);
			   
     }
/**
**�ڰ�������ʱ oled�˵����������Ӧ���л�
**������Ӧ��־λ�����¸�ֵ
**/
    if(Interrupt_Flg==1)//��һ�η��򰴼�'Interrupt_Flg'�����ᱻ��1
    {
				if(Page_New==0)
				{
					ConfirmKey=0;
				}
	
				OLED_Page_Show(KeyRightLeftFlg, KeyUpDownFlg,ConfirmKey);//�˺���Ϊoled�˵���ʾ����(�����ֱ�Ϊ: ��������ֵ ��������ֵ �Լ�ȷ�ϼ�)
			  ConfirmKey=0;
		  	Interrupt_Flg=0;
	  		InterruptFlg =0;  		
		  	KeyUpDownFlg =0;
		  	KeyRightLeftFlg=0;
    }

    InterruptFlg=0;//�˱�־λΪ ¼������ʱ����ı�־λ

/**
** �ڽ���'Figer'ѵ������ʱ'Train_Figer_Entry_Flg'��־λ����1
** �ڴ�ѵ���ڼ�,����������ѭ�� ����ǿ�Ʒ��ػ�¼�����
** ��Ӧ��־λ�ڴ˴�����ֵ
**/
		while(Train_Figer_Entry_Flg)
		{   SortTimes=0;
        Loading();
			
       if(InterruptFlg==1)
		    {
          EntryFiger();  //��һ��ȷ��¼��һ�Σ�
          InterruptFlg=0;
        }
				
				ConfirmKey=0;
        KeyUpDownFlg=2;
        Interrupt_Flg=1;
				KeyRightLeftFlg=-1;
    }
/**
**'Figer'ѵ���ڼ����ǿ�Ʒ��������¼��ʧ��
** ��Ӧ��������ݵ����鱻����
**/
   if(EntryFigerTimes!=0)
    {
				unsigned char i;
			
        for(i=0;i<6;i++)
        {
          ArrayA[i]=0;
        }
				
        EntryFigerTimes=0;//¼������ʱ�Ĵ���
				KeyUpDownFlg=0;
    }
/**
** �ڽ���'JudgeNum'�о�����ʱ'Judge_Figer_Flg'��־λ����1
** ��Ϊ��¼������ʱ����Ҫ����˳�����Դ˴��õ�ð�������㷨
** oled��ʾ��Ӧ���
**/
	  if(Judge_Figer_Flg==1)
		{
        if(SortTimes==0)//¼��һ������ֻ����һ��
        {
           SortArr(ArrayA,6);//ð�������㷨
           JudgmentFigerRange();//�����Ʒ�Χ��������
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
				if(InterruptFlg==1)  //�жϷ��Ͳ�������
		    {
          data_tran(1,0x30);
          InterruptFlg=0;
        }
        SortTimes=1;
    }

/**
** �ڽ���'Fist'ѵ������ʱ'Train_Fist_Entry_Flg'��־λ����1
** �ڴ�ѵ���ڼ�,����������ѭ�� ����ǿ�Ʒ��ػ�¼�����
** ��Ӧ��־λ�ڴ˴�����ֵ
**/
	  while(Train_Fist_Entry_Flg)
    {
        Loading();
        SortTimes=0;
        if(InterruptFlg==1)
        {
          EntryFist();    //��һ��¼��һ�Σ�
          InterruptFlg=0;
        }
			 
        ConfirmKey=0;
        KeyUpDownFlg=2;
        Interrupt_Flg=1;
			  KeyRightLeftFlg=-1;
    }
/**
**'Fist'ѵ���ڼ����ǿ�Ʒ��������¼��ʧ��
** ��Ӧ��������ݵ����鱻����
**/
     if(EntryFistTimes!=0)
    {
				unsigned char i;
				for(i=0;i<4;i++)
				{
            ArrayB[i]=0;
				}
				
        EntryFistTimes=0;//¼������ʱ�Ĵ���
				KeyUpDownFlg=0;
     }
/**
** �ڽ���'JudgeFist'�о�����ʱ'Judge_Fist_Flg'��־λ����1
** ��Ϊ��¼������ʱ����Ҫ����˳�����Դ˴��õ�ð�������㷨
** oled��ʾ��Ӧ���
**/
    if(Judge_Fist_Flg==1)
     {
        if(SortTimes==0)//¼��һ������ֻ����һ��
        {
           SortArr(ArrayB,4);
           JudgmentFistRange();
         }
         switch(JudgmentFist())
         {
           case 0: OLED_ShowCN(48,5,35); OLED_ShowCN(64,5,35);break;//�ո�
           case 1: OLED_ShowCN(48,5,30); OLED_ShowCN(64,5,31);break;//ʯͷ
           case 2: OLED_ShowCN(48,5,32); OLED_ShowCN(64,5,33);break;//����
           case 3: OLED_ShowCN(48,5,34); OLED_ShowCN(64,5,35);break;//��
           default: break;
         }
				 if(InterruptFlg==1)  //�жϷ��Ͳ�������
		    {
          data_tran(1,0x30);
          InterruptFlg=0;
        }
        SortTimes=1;
		}
		 
		
  }

	
}
/**
**���ڴ˺�����ֱ�Ӳɼ���·ͨ��ֵ
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
**��ʱ��5�жϺ��� 10ms
**�ڶ�ʱ���вɼ���·ͨ��ֵ����������ֵ
**/
void TIM2_IRQHandler(void)         
{
	 
	if(TIM2->SR&0X0001)//����ж�
	{	
		temp0= Cap_Calculate(0);
    temp1= Cap_Calculate(1);
    temp2= Cap_Calculate(2);
    temp3= Cap_Calculate(3);
    Total_Value=(temp0+temp1+temp2+temp3)/10000;
		
	}						   
  TIM2->SR&=~(1<<0);
}



