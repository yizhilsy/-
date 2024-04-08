#include <iostream>
#include <chrono>
#include <cstring>
using namespace std;
typedef long long LL;
const int MAXN = 1e9;
LL* fibArray = nullptr;
int main()
{
    
    fibArray = new LL[MAXN];
    memset(fibArray,0,sizeof(fibArray));
    fibArray[0] = 1;fibArray[1] = 1;
    LL num1 = 1;LL num2 = 1;
    
    cout << "数据相关程度高的情况" << endl;
    auto start = std::chrono::high_resolution_clock::now();
    for(int i=2;i<MAXN;i++)
    {
        fibArray[i] = fibArray[i-1]+fibArray[i-2];
    }
    auto finish = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = finish - start;
    cout << "Num: "<< fibArray[MAXN-1] <<" Execution time: " << elapsed.count() << " seconds" << endl;

    cout << "数据相关程度低的情况" << endl;
    start = std::chrono::high_resolution_clock::now();
    for(int i=2;i<MAXN;i++)
    {
        num2 = num1 + num2;
        num1 = num2 - num1;
    }
    finish = std::chrono::high_resolution_clock::now();
    elapsed = finish - start;
    cout << "Num: "<< num2 <<" Execution time: " << elapsed.count() << " seconds" << endl;
    return 0;
}