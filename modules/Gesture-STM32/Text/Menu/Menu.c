#include <oled.h>
unsigned char Key_Menu_Page_Pos =0;//�ڲ˵������л�ʱ ��������λ��
//��Ӧ��־λ�������������н���
extern unsigned char Judge_Figer_Flg;
extern unsigned char Judge_Fist_Flg;
extern unsigned char Train_Figer_Entry_Flg;
extern unsigned char Train_Fist_Entry_Flg;
extern long int Loding_Times;//������ ��Ӧ�жϴ���
unsigned char Page_Flag=0;  //ҳ���л���־λ
extern int Page_New;
//����λ�� ����ǰλ�����ϴ�λ�ã�
int key_pos=0;
int KeyPos=0;
void OLED_Page_Show(int page,int KeyJudgeUpDown,int Confirm_Key)
{
    Page_New=Page_New+page;
    key_pos=key_pos+KeyJudgeUpDown;

    if(Page_New<0)
        Page_New=0;
    if(Page_New>2)
        Page_New=2;

    if(key_pos>-1)
      key_pos=-4;
    if(key_pos<-4)
      key_pos=-1;

//����0 :��ʾ��·ͨ��ֵ
    if(Page_New == 0)
    {
        key_pos=-1;
        Page_Flag=1;
        OLED_Fill(0x00);
        UI_TILTLE();
        OLED_ShowStr(0,3,"CH0: ",1);
        OLED_ShowStr(0,4,"CH1: ",1);
        OLED_ShowStr(0,5,"CH2: ",1);
        OLED_ShowStr(0,6,"CH3: ",1);
        OLED_ShowStr(104,7,"1/2",1);
    }
//����1 :����ģʽѡ��
    if((Page_New == 1)&&(Confirm_Key==0))
    {
        Judge_Figer_Flg=0;
        Judge_Fist_Flg =0;
        Judge_Figer_Flg=0;
        Judge_Fist_Flg=0;

        if(Page_Flag==1)//�����ظ�ˢ��
        {OLED_Fill(0x00);}
        Page_Flag=0;
        if((-KeyPos)!=key_pos)
        {OLED_ShowOneNum(78,KeyPos+2,-16,1);}

        UI_TILTLE();
        OLED_ShowStr(0,3,"1.JudgeNum  ",1);//�о�ģʽ
        OLED_ShowStr(0,4,"2.JudgeFist ",1);
        OLED_ShowStr(0,5,"3.TrainNum  ",1);//ѵ��ģʽ
        OLED_ShowStr(0,6,"4.TrainFist ",1);
        OLED_ShowStr(104,7,"2/2",1);  //ҳ����

        switch(key_pos)
            {
               case -1: OLED_ShowOneNum(78,3,76,1);KeyPos=1; break;
               case -2: OLED_ShowOneNum(78,4,76,1);KeyPos=2; break;
               case -3: OLED_ShowOneNum(78,5,76,1);KeyPos=3; break;
               case -4: OLED_ShowOneNum(78,6,76,1);KeyPos=4; break;

              default: break;
            }

        Key_Menu_Page_Pos=KeyPos;
    }
//����1 ����1:�о� ��������һ
    if((Key_Menu_Page_Pos==1)&&(Confirm_Key==1))
    {
        Page_New=2;
        Page_Flag=1;
        key_pos=-Key_Menu_Page_Pos;
        Judge_Figer_Flg=1;
        Judge_Fist_Flg =0;

        OLED_Fill(0x00);
        OLED_ShowStr(0,0,"1.JudgeNum   ",2);
    }
//����1 ����2:�о� ʯͷ ���� ��
    if((Key_Menu_Page_Pos==2)&&(Confirm_Key==1))
    {
        Page_New=2;
        Page_Flag=1;
        key_pos=-Key_Menu_Page_Pos;
        Judge_Figer_Flg=0;
        Judge_Fist_Flg =1;

        OLED_Fill(0x00);
        OLED_ShowStr(0,0,"2.JudgeFist ",2);
    }
//����1 ����3:ѵ�� ��������һ
    if((Key_Menu_Page_Pos==3)&&(Confirm_Key==1))
    {
       Page_New=2;
       Page_Flag=1;
       key_pos=-Key_Menu_Page_Pos;
       Train_Figer_Entry_Flg=1;
       Train_Fist_Entry_Flg =0;

       OLED_Fill(0x00);
       //UI_TILTLE();
       OLED_ShowStr(0,0,"3.TrainNum   ",2);
       OLED_ShowCN(12,4,39);
       OLED_ShowCN(28,4,37);
       OLED_ShowCN(44,4,37);
       OLED_ShowCN(60,4,37);
       OLED_ShowCN(76,4,37);
       OLED_ShowCN(92,4,37);
    }
//����1 ����4:ѵ�� ʯͷ ���� ��
    if((Key_Menu_Page_Pos==4)&&(Confirm_Key==1))
    {
       Page_New=2;
       Page_Flag=1;
       key_pos=-Key_Menu_Page_Pos;
       Train_Figer_Entry_Flg=0;
       Train_Fist_Entry_Flg =1;

       OLED_Fill(0x00);
       //UI_TILTLE();
       OLED_ShowStr(0,0,"4.TrainFist  ",2);
       OLED_ShowCN(26,4,39);
       OLED_ShowCN(42,4,37);
       OLED_ShowCN(58,4,37);
       OLED_ShowCN(74,4,37);
    }

}

/**
**��������ʾ����
**Ч��Ϊ. .. ...�ֻ���ʾ
**/
void Loading()
{
    Loding_Times++;
   if(Loding_Times>=300)
       Loding_Times=0;
   if(Loding_Times<100)
       OLED_ShowStr(92,1,".  ",1);
   if((Loding_Times>=100)&&(Loding_Times<200))
       OLED_ShowStr(92,1,".. ",1);
   if(Loding_Times>=200)
       OLED_ShowStr(92,1,"...",1);
}

