#include <iostream>
#include <stdio.h>
#include <vector>       
#include <algorithm>     
#include <fstream>       
#include <iomanip>      
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
