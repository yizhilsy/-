#include <iostream>
#include <chrono>
#include <omp.h>

using namespace std;
typedef long long LL;
const int MAXN = 1e7;
int* numArray = nullptr;

void init();
void Merge(int* p, int low, int mid, int high);
void MergeSort(int* numArray, int length);
void ParallelMergeSort(int* numArray, int length);
bool IsSorted(int* numArray, int length);
void SerialTest();
void ParallelTest();
void WrongParallelTest();
void WrongParallelMergeSort(int* numArray, int length);

int main()
{
    init();
    SerialTest();
    ParallelTest();
    WrongParallelTest();
    return 0;
}

void init() {
    numArray = new int[MAXN];
    for (int i = 0; i < MAXN; i++) {
        numArray[i] = rand() % 1000;
    }
}

void Merge(int* p, int low, int mid, int high) {
    int* tempPtr = new int[high - low + 1];
    int i = low, j = mid + 1, k = 0;
    while (i <= mid && j <= high) {
        if (p[i] < p[j]) {
            tempPtr[k++] = p[i++];
        }
        else {
            tempPtr[k++] = p[j++];
        }
    }
    while (i <= mid) {
        tempPtr[k++] = p[i++];
    }
    while (j <= high) {
        tempPtr[k++] = p[j++];
    }
    for (int i = 0;i < k;i++) {
        p[low + i] = tempPtr[i];
    }
    delete[] tempPtr;
}

void MergeSort(int* numArray, int length) {
    int len = 1, i;
    while (len < length) {
        i = 0;
        while (i + 2 * len <= length) {
            Merge(numArray, i, i + len - 1, i + 2 * len - 1);
            i += 2 * len;
        }
        if(i + len < length) {
            Merge(numArray, i, i + len - 1, length - 1);
        }
        len <<= 1;
    }
}

void ParallelMergeSort(int* numArray, int length) {
    int len = 1;
    int OMP_THREAD_NUM = 4;
    while (len < length) {
        int groups = length / (2 * len);
        int GROUPS_PER_THREAD = (groups + OMP_THREAD_NUM - 1) / OMP_THREAD_NUM;
        if(GROUPS_PER_THREAD == 0) {
            OMP_THREAD_NUM = groups;
            GROUPS_PER_THREAD = 1;
        }
        
        #pragma omp parallel for num_threads(OMP_THREAD_NUM)
        for (int i = 0;i < OMP_THREAD_NUM;i++) {
            for (int j = i * GROUPS_PER_THREAD;j < (i + 1) * GROUPS_PER_THREAD && j < groups;j++) {
                int posStart = j * 2 * len;
                int posMid = j * 2 * len + len - 1;
                int posEnd = j * 2 * len + 2 * len - 1;
                Merge(numArray, posStart, posMid, posEnd);
            }
        }
        int remainStart = groups * 2 * len;
        if (remainStart + len < length) {
            Merge(numArray, remainStart, remainStart + len - 1, length - 1);
        }
        len <<= 1;
    }
}

bool IsSorted(int* numArray, int length) {
    for (int i = 0;i < length - 1;i++) {
        if (numArray[i] > numArray[i + 1]) {
            return false;
        }
    }
    return true;
}

void SerialTest() {
    init();
    auto start = chrono::high_resolution_clock::now();
    MergeSort(numArray, MAXN);
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed = end - start;
    cout << "Serial Execution time: " << elapsed.count() << " seconds" << endl;
    IsSorted(numArray, MAXN) ? cout << "Sorted" << endl : cout << "Unsorted" << endl;
    delete[] numArray;
}

void ParallelTest() {
    init();
    auto start = chrono::high_resolution_clock::now();
    ParallelMergeSort(numArray, MAXN);
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed = end - start;
    cout << "Parallel Execution time: " << elapsed.count() << " seconds" << endl;
    IsSorted(numArray, MAXN) ? cout << "Sorted" << endl : cout << "Unsorted" << endl;
    delete[] numArray;
}

void WrongParallelMergeSort(int* numArray, int length) {
    int len = 1;
    int OMP_THREAD_NUM = 4;

    while (len < length) {
        int groups = length / (2 * len);
        // int i = 0;
        int posStart, posMid, posEnd;
        #pragma omp parallel num_threads(OMP_THREAD_NUM)
        {
            int id = omp_get_thread_num();
            // #pragma omp for
            for (int i = id;i < groups;i += OMP_THREAD_NUM) {
                #pragma omp critical
                {
                    posStart = i * 2 * len;
                    posMid = i * 2 * len + len - 1;
                    posEnd = i * 2 * len + 2 * len - 1;
                    Merge(numArray, posStart, posMid, posEnd);
                }
            }
        }
        int remainStart = groups * 2 * len;
        if (remainStart + len < length) {
            Merge(numArray, remainStart, remainStart + len - 1, length - 1);
        }
        len <<= 1;
    }
}

void WrongParallelTest() {
    init();
    auto start = chrono::high_resolution_clock::now();
    WrongParallelMergeSort(numArray, MAXN);
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed = end - start;
    cout << "Data dependency Parallel Execution time: " << elapsed.count() << " seconds" << endl;
    IsSorted(numArray, MAXN) ? cout << "Sorted" << endl : cout << "Unsorted" << endl;
    delete[] numArray;
}