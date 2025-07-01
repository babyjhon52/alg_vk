// Дан массив неотрицательных целых 64-битных чисел. Количество чисел не больше 1000000. 
// Отсортировать массив методом поразрядной сортировки LSD по байтам.

#include <iostream>


void lsdSort(unsigned long long *arr, int n)
{
    unsigned long long *buff = new unsigned long long[n]();
    for (int byte = 0; byte < 8; byte++)
    {
        int *countBuf = new int[256]();

        for (int i = 0; i < n; i++)
        {
            int val = (arr[i] >> (byte * 8)) & 0xFF;
            countBuf[val]++;
        }

        for (int i = 1; i < 256; i++)
        {
            countBuf[i] += countBuf[i - 1];
        }

        for (int i = n - 1; i >= 0; i--)
        {
            int val = (arr[i] >> (byte * 8)) & 0xFF;
            countBuf[val]--;
            buff[countBuf[val]] = arr[i];
        }

        for (int i = 0; i < n; i++)
        {
            arr[i] = buff[i];
        }

        delete[] countBuf;
    }

    delete[] buff;
}


int main()
{
    int n = 0;
    std::cin >> n;
    unsigned long long *arr = new unsigned long long[n];

    for (int i = 0; i < n; i++)
    {
        std::cin >> arr[i];
    }

    lsdSort(arr, n);

    for (int i = 0; i < n; i++)
    {
        std::cout << arr[i] << " ";
    }
    return 0;
}