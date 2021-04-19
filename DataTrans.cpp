#include "SerialPort.h"
#include "DataTrans.h"
#include "KNN.h"

extern int K;
const int Train_num = 18; //ѵ������
extern int Trainning_Flag;  //ѵ����ɱ�־λ
extern int TestFilerecv_Flag;  //���Լ����ձ�־λ
int flagtime = 1;
const int type1file_start = 0;
const int type2file_start = 20000;//ѵ����������
int max_len = 20000;
int a_file_num = 10;
int train_filenum1; //ѵ���ļ�����
int train_filenum2; //ѵ���ļ�����
char train_path[1000];
int f1n = 0;
int f2n = 0;

int FileRemove(const char* fname)
{
    return remove(fname);
}

int Data_recv(int recv_num)
{
    
    if (recv_num == Train_num)
    {
        flagtime = 0;   //�����ļ�
        Trainning_Flag = 1;  //ѵ��������־λ
        cout << "End trainfile " << recv_num << endl;
        return recv_num;
    }
    else if (recv_num < Train_num)
    {
        flagtime = 0;   //�����ļ�
        cout << "End trainfile " << recv_num << endl;
        return 0;
    }
    else  //���յ����Լ� recv_num > Train_num
    {
        TestFilerecv_Flag = 1;
        cout << "End testfile " << recv_num - Train_num << endl;
    }
}

void Train_to_Dataset(int Mode)  //����ѵ������¼��
{
    if (Mode == 1)
    {
        for (int i = 1; i < Train_num + 1; i++)
        {
            sprintf(train_path, "%d.txt", type1file_start + i - 1);

            fstream file(train_path, ios::in);
            if (!file)
            {
                if (i % 6 > 0) { cout << "Please train the number of " << i % 6 - 1 << endl; }
                else if (i % 6 == 0) { cout << "Please train the number of 5" << endl; }
            }
            else
            {
                file.close();
                FileRemove(train_path);  //����ļ����ھ�ɾ������¼��
                if (i % 6 > 0) { cout << "Please train the number of " << i % 6 - 1 << endl; }
                else if (i % 6 == 0) { cout << "Please train the number of 5" << endl; }
            }
            while (flagtime);  //�ȴ������ļ�
            flagtime = 1;
        }
    }
    else if (Mode == 2)
    {
        for (int i = 1; i < Train_num + 1; i++)
        {
            sprintf(train_path, "%d.txt", type2file_start + i - 1);

            fstream file(train_path, ios::in);
            if (!file)
            {
                if (i % 3 == 1) { cout << "Please train the Rock " << endl; }
                else if (i % 3 == 2) { cout << "Please train the Scissors " << endl; }
                else if (i % 3 == 0) { cout << "Please train the Paper" << endl; }
            }
            else
            {
                file.close();
                FileRemove(train_path);  //����ļ����ھ�ɾ������¼��
                if (i % 3 == 1) { cout << "Please train the Rock " << endl; }
                else if (i % 3 == 2) { cout << "Please train the Scissors " << endl; }
                else if (i % 3 == 0) { cout << "Please train the Paper" << endl; }
            }
            while (flagtime);  //�ȴ������ļ�
            flagtime = 1;
        }
    }
    else
    {
        perror("Mode error!");
    }
    cout << "Train success !" << endl;
    
}

//ȡ��ѵ���ļ�����
int Get_num_for_file_1()
{
    train_filenum1 = max_len;
    int i;
    for (i = type1file_start; i < type1file_start + max_len; i++)
    {
        sprintf(train_path, "%d.txt", i);
        fstream _tranfile(train_path, ios::in);
        if (!_tranfile)
        {
            train_filenum1 = i - type1file_start;
            break;
        }
        _tranfile.close();
    }

    return train_filenum1;
}

int Get_num_for_file_2()
{
    train_filenum2 = max_len;
    int i;
    for (i = type2file_start; i < type2file_start + max_len; i++)
    {
        sprintf(train_path, "%d.txt", i);
        fstream _tranfile(train_path, ios::in);
        if (!_tranfile)
        {
            train_filenum2 = i - type2file_start;
            break;
        }
        _tranfile.close();
    }
    return train_filenum2;
}

void prepare_list()
{
    int f1n = Get_num_for_file_1();
    int f2n = Get_num_for_file_2();
    const int train_num1 = f1n * a_file_num;
    const int train_num2 = f2n * a_file_num;
    //������������
    int** train_X_list1 = new int* [train_num1];
    for (int i = 0; i < train_num1; i++)
    {
        train_X_list1[i] = new int[4];
    }
    int* train_Y_list1 = new int[train_num1];

    int** train_X_list2 = new int* [train_num2];
    for (int i = 0; i < train_num2; i++)
    {
        train_X_list2[i] = new int[4];
    }
    int* train_Y_list2 = new int[train_num1];

}

int** createDataSet_Type1(int* Testarray)
{
    int i, j, k;
    //int* label;
    //��ȡ����
    int f1n = Get_num_for_file_1();
    const int train_num1 = f1n * a_file_num;
    //������������
    int** train_X_list1 = new int* [train_num1];
    int* train_Y_Pre_list1 = new int[train_num1];
    int* train_Y_list1 = new int[train_num1];
    for (int i = 0; i < train_num1; i++)
    {
        train_X_list1[i] = new int[4];
    }
    for (i = 0; i < f1n; i++)
    {
        sprintf(train_path, "%d.txt", type1file_start + i);
        fstream _tranfile(train_path, ios::in);

        _tranfile >> train_Y_Pre_list1[i];

        for (j = 1; j < (a_file_num + 1); j++) //��
        {
            for (k = 0; k < 4; k++)  //��
            {
                _tranfile >> train_X_list1[j - 1 + i * a_file_num][k];
                //cout<< train_X_list1[j - 1 + i * a_file_num][k] <<endl;
            }
        }
        _tranfile.close();
    }
    for (i = 0; i < f1n; i++)
    {
        for (j = 0; j < a_file_num; j++)
        {
            train_Y_list1[j + i * a_file_num] = train_Y_Pre_list1[i];
        }
    }
    classsify_KNN(Testarray, train_X_list1, train_num1, train_Y_list1, K);
    return train_X_list1;
}

int createDataSet_Type2(int* Testarray)
{
    int i, j, k;
    int ans;
    //int* label;
    //��ȡ����
    int f2n = Get_num_for_file_2();
    const int train_num2 = f2n * a_file_num;
    //������������
    int** train_X_list2 = new int* [train_num2];
    int* train_Y_Pre_list2 = new int[train_num2];
    int* train_Y_list2 = new int[train_num2];
    for (int i = 0; i < train_num2; i++)
    {
        train_X_list2[i] = new int[4];
    }
    for (i = 0; i < f2n; i++)
    {
        sprintf(train_path, "%d.txt", type2file_start + i);
        fstream _tranfile(train_path, ios::in);

        _tranfile >> train_Y_Pre_list2[i];

        for (j = 1; j < (a_file_num + 1); j++) //��
        {
            for (k = 0; k < 4; k++)  //��
            {
                _tranfile >> train_X_list2[j - 1 + i * a_file_num][k];
                //cout<< train_X_list1[j - 1 + i * a_file_num][k] <<endl;
            }
        }
        _tranfile.close();
    }
    for (i = 0; i < f2n; i++)
    {
        for (j = 0; j < a_file_num; j++)
        {
            train_Y_list2[j + i * a_file_num] = train_Y_Pre_list2[i];
        }
    }
    ans = classsify_KNN(Testarray, train_X_list2, train_num2, train_Y_list2, K);
    return ans;
}



