#include <iostream>
#include <stdio.h>
#include <vector>        //提供向量头文件
#include <algorithm>     // 算法头文件，提供迭代器
#include <fstream>       //提供文件头文件
#include <iomanip>       //C++输出精度控制需要
#include<string>
#include <sstream>


int FileRemove(const char* fname);
int Data_recv(int n);
void Train_to_Dataset(int Mode);
int Get_num_for_file_1();
int Get_num_for_file_2();
void prepare_list();
int** createDataSet_Type1(int* Testarray);
int createDataSet_Type2(int* Testarray);

