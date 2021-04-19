#include "SerialPort.h"
#include "DataTrans.h"
#include "socket.h"
#include <iostream>
#include <pthread.h>

using namespace std;

int K = 10;  //K值
unsigned  char buff[1024];
const char* dev = "/dev/ttyAMA0";  //固定串口号
int Trainning_Flag;//生成训练集标志
int TestFilerecv_Flag;  //测试集接收标志位
int Dataset_configFlag;
int Mode;
const int type1file_start = 0;
const int type2file_start = 20000;//训练集区分线
const int Train_num = 18; //训练次数
int test_num = 0;
int Filerecv_num = 0;
char str[1000];

void* ListenPort(void* avg)
{
    int nread;
    serialPort myserial;
    cout << "serialPort Test" << endl;
    cout << "······" << endl;
    myserial.OpenPort(dev);
    if (!myserial.setup(115200, 0, 8, 1, 'N')) 
    {
        cout << "initPort fail !" << endl;
    }
    else
    {
        cout << "initPort success !" << endl;
    }
    while (true)
    {
        if (nread = myserial.readBuffer(buff, 8, Train_num) == 1) 
        {
            continue;
        }
        else
        {
            cout << "Open listenthread fail !" << endl;
            sleep(400);
        }
    }
    return NULL;
}

void* ListenSocket(void* avg)
{
    socket_client myclient;
    int n;
    while (true)
    {
        n = myclient.readBuffer(buff);
        if (n == 1)  //开始串口监听，等待数据接收  
        {
            write(STDOUT_FILENO, buff, n);
            continue;
        }
        else
        {
            cout << "Open socket fail !" << endl;
            sleep(400);
        }
    }
    return NULL;
}

int main()
{

    pthread_t port;
    pthread_t Socket;
    socket_client myclient;

    while (Mode != 1 && Mode != 2)
    {
        cout << "Which mode do you want?  1 Finger or 2 Fist" << endl;
        cin >> Mode;
        cout << "······" << endl;
    } 
    
    cout << "Do you want to use the existing dataset?  0 not or 1 yes" << endl;
    cout << "······" << endl;
    cin >> Dataset_configFlag;

    pthread_create(&port, NULL, ListenPort, NULL);
    //pthread_create(&Socket, NULL, ListenSocket, NULL);

    if (Dataset_configFlag)
    {
        Dataset_configFlag = 0;
        fstream File_config;
        if (Mode == 1)
        {
            sprintf(str, "%d.txt", type1file_start + Train_num - 1);
        }
        else if(Mode == 2)
        {
            sprintf(str, "%d.txt", type2file_start + Train_num - 1);
        }
        else
        {
            perror("Mode error!");
        }
        File_config.open(str, ios::in);
        if (!File_config)   
        {
            cout << "Reading the dataset error!" << endl;
            cout << "Plese train the gesture as follow" << endl;
            
            Train_to_Dataset(Mode);
        }
        else    
        {
            File_config.close();
            Trainning_Flag = 1;
        }
    }
    else
    {
        cout << "Plese train the gesture as follow" << endl;  
        
        Train_to_Dataset(Mode);
    }

	while (true)
	{
        
		//nwrite = myserial.writeBuffer(buff, 8);
		
        if (Trainning_Flag == 1)
        {
            Trainning_Flag = 0;
            cout << "Train finished !" << endl;
            cout << "······" << endl;
            Filerecv_num = Train_num;  
            //mySerialPort.CloseListenTread();
         
            for (int i = 0; i < 30000; i++)
            {
                sprintf(str, "test%d.txt", i);
                FileRemove(str);
            }
            
            cout << "Plese input the test data!" << endl;
        }

        if (TestFilerecv_Flag == 1)
        {
            TestFilerecv_Flag = 0;
            cout << "Test file received successfully !" << endl;
            fstream testdata;
            if (Mode == 1)
            {
                sprintf(str, "test%d.txt", type1file_start + test_num);
            }
            else if (Mode == 2)
            {
                sprintf(str, "test%d.txt", type2file_start + test_num);
            }
            test_num++;
            testdata.open(str, ios::in);
          
            int Testarray[4];
            int** Test_list = new int* [10];
            for (int i = 0; i < 10; i++)
            {
                Test_list[i] = new int[4];
            }
            for (int j = 0; j < 10; j++) //行
            {
                for (int k = 0; k < 4; k++)  //列
                {
                    testdata >> Test_list[j][k];
                    //cout<< train_X_list1[j - 1 + i * a_file_num][k] <<endl;
                }
            }
         
            for (int i = 0; i < 4; i++)
            {
                int X = 0;
                for (int j = 0; j < 10; j++)
                {
                    X = X + Test_list[j][i] / 10;
                }
                Testarray[i] = X;
            }
            delete[]Test_list;
            testdata.close();
            
            if (Mode == 1)
            {
                createDataSet_Type1(Testarray);
            }
            else if (Mode == 2)
            {
                
                int int_ans;
                int_ans = createDataSet_Type2(Testarray);
                string char_ans;
                if (int_ans == 1)
                {
                    char_ans = 'r';
                    myclient.writeBuffer(char_ans);
                    cout << "successfully send" << endl;
                }
                if (int_ans == 2)
                {
                    char_ans = 's';
                    myclient.writeBuffer(char_ans);
                    cout << "successfully send" << endl;
                }
                if (int_ans == 3)
                {
                    char_ans = 'p';
                    myclient.writeBuffer(char_ans);
                    cout << "successfully send" << endl;
                }
                
            }

          
        }
    }

	return 0;

}

