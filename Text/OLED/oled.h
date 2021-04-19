#ifndef  __OLED_H
#define  __OLED_H

#include "sys.h"

#define SCL PAout(1)
#define SDA PAout(0) 

#define high 1
#define low 0
 
#define X_WIDTH 	128
#define Y_WIDTH 	64

#define Brightness 200

void IIC_Start(void);// -- ����I2C����
void IIC_Stop(void);// -- �ر�I2C����
void Write_IIC_Byte(unsigned char IIC_Byte);// -- ͨ��I2C����дһ��byte������
void OLED_WrDat(unsigned char dat);// -- ��OLED��д����
void OLED_WrCmd(unsigned char cmd);// -- ��OLED��д����
void OLED_Set_Pos(unsigned char x, unsigned char y);// -- ������ʾ����
void OLED_Fill(unsigned char bmp_dat);// -- ȫ����ʾ(��ʾBMPͼƬʱ�Ż��õ��˹���)
void OLED_CLS(void);// -- ��λ/����
void OLED_ON(void);
void OLED_OFF(void);
void Oled_Init(void);// -- OLED����ʼ�����򣬴˺���Ӧ�ڲ�����Ļ֮ǰ���ȵ���
void OLED_P6x8Str(unsigned char x,unsigned char y,unsigned char ch[]);// -- 6x8������������ʾASCII�����С���У���̫����
void OLED_P8x16Str(unsigned char x,unsigned char y,unsigned char ch[]);// -- 8x16������������ʾASCII�룬�ǳ�����
void OLED_Show8x16(unsigned char x,unsigned char y,unsigned N);
void OLED_Show6x8(unsigned char x,unsigned char y,unsigned N);
void OLED_keyboard(unsigned char x,unsigned char y,unsigned N);

void OLED_Show16x16(unsigned char x,unsigned char y,unsigned int N);// -- 16x16������������ʾ���ֵ���С���У������ø������塢�Ӵ֡���б���»��ߵ�
void Draw_BMP(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,unsigned char BMP[]);// -- ��128x64���ص�BMPλͼ��ȡ�����������ֱ�Ȼ���Ƶ�codetab�У��˺������ü���
void Oled_Show_X(unsigned char x,unsigned y,unsigned int temp,unsigned char TextSize);
void Oled_Show_3(unsigned char x,unsigned y,unsigned int temp,unsigned char TextSize);
void LCD_ShowxNum( char x,char y,float  num);
void OLED_ShowNum(unsigned char x, unsigned char y, u16 num, unsigned char TextSize);
void OLED_ShowOneNum(unsigned char x, unsigned char y, int num, unsigned char TextSize);

void OLED_ShowStr(unsigned char x, unsigned char y, unsigned char ch[], unsigned char TextSize);
void OLED_ShowCN(unsigned char x, unsigned char y, unsigned char N);
void OLED_ShowOneNum(unsigned char x, unsigned char y, int num, unsigned char TextSize);
void OLED_ShowNum(unsigned char x, unsigned char y, u16 num, unsigned char TextSize);
void Oled_Printf_Float(unsigned char x,unsigned char y,float num,unsigned char TextSize);
void Oled_Printf_U32(unsigned char x,unsigned char y,unsigned long int num,unsigned char TextSize);
void Show(u8 mark,u8 mark1);
void UI_TILTLE(void);
void UI_1(void);
void UI_2(void);

#endif
