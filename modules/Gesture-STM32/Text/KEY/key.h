#ifndef __KEY__H
#define __KEY__H
#define key           GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_12)
#define key_up        GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_15)
#define key_down      GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_5)
#define key_left      GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_7)
#define key_right     GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_6)
#define key_middle    GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_11)
#define key_press 1
#define key_up_press 2
#define key_down_press 3
#define key_left_press 4
#define key_right_press 5
#define key_middle_press 6

void Key_Init(void);
int Key_Scan(void);
void key_exti_test(void);
#endif

