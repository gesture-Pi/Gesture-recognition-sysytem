#include "KNN.h"

extern int Mode;

int classsify_KNN(int* INX, int** DataSet, int DataSetlen, int* label, int k) // 限定数据规模 
{
    int* b = new int[DataSetlen];  //存放距离数组
    //设置INX是4列的，DataSet也是 
    for (int i = 0; i < DataSetlen; ++i) {
        double sum = 0;
        for (int j = 0; j < 4; ++j) {
            sum += (DataSet[i][j] - INX[j]) * (DataSet[i][j] - INX[j]);
            //cout << DataSet[i][j];
        }
        b[i] = sqrt(sum);
        //cout << "  distance = " << b[i] << "  label = " << label[i] << endl;
    }
    //===  接下来用改写选择排序进行减枝，降低时间复杂度 
    for (int time = 0; time < k; ++time)
    {
        int num_index = time;
        for (int j = time + 1; j < DataSetlen; ++j)   //距离升序排序
        {
            if (b[j] < b[num_index])
            {
                num_index = j;
            }
        }
        if (time != num_index) //确认交换
        {
            int tn = b[num_index];
            int tc = label[num_index];
            b[num_index] = b[time];
            label[num_index] = label[time];
            b[time] = tn;
            label[time] = tc;
        }
    }
    for (int i = 0; i < k; ++i)
    {
        cout << "label = " << label[i] << "  distance = " << b[i] << endl;
    }
    int* sc = new int[k];
    int* sc_num = new int[k];
    memset(sc_num, 0, sizeof(sc_num));
    memset(sc, '0', sizeof(sc));
    int len = 0;
    for (int i = 0; i < k; ++i) {
        bool p = true;
        for (int j = 0; j < len; ++j) {
            if (sc[j] == label[i]) {
                sc_num[j] ++;
                p = false;
            }
        }
        if (p) {
            sc[len] = label[i];
            sc_num[len++] = 1;
        }
    }

    int ans;
    int time = 0;
    for (int i = 0; i < k; ++i)
    {
        //cout << "The answer is " << sc[i] << ", which appears " << sc_num[i] << " times!" << endl;
        if (time < sc_num[i] && sc_num[i]<= k)
        {
            ans = sc[i];
            time = sc_num[i];
        }
    }
    if (Mode == 1)
    {
        cout << "The final answer is " << ans << ", which appears " << time << " times!" << endl;
    }
    else
    {
        if( ans == 1) cout << "The final answer is Rock, which appears " << time << " times!" << endl;
        if (ans == 2) cout << "The final answer is Scissors, which appears " << time << " times!" << endl;
        if (ans == 3) cout << "The final answer is Paper, which appears " << time << " times!" << endl;
    }
    delete[]b;
    delete[]sc;
    delete[]sc_num;

    return ans;
}
