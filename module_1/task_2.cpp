/* Даны два массива неповторяющихся целых чисел, упорядоченные по возрастанию. 
A[0..n-1] и B[0..m-1]. n » m. Найдите их пересечение. 
Требуемое время работы: O(m * log k), где k - позиция элементта B[m-1] в массиве A.. 
В процессе поиска очередного элемента B[i] в массиве A пользуйтесь результатом поиска элемента B[i-1]. n, k ≤ 10000. */

#include <iostream>

bool isExist(int arr[], int elem, int &left, int n) 
{
    if (left >= n) return false;

    int low = left;
    int high = left;

    if (arr[low] > elem) return false;
    
    while (high < n && arr[high] < elem) 
    {
        low = high;
        if (high == 0) high = 1;
        
        else high *= 2;

        if (high >= n) 
        {
            high = n;
            break;
        }
    }

    int l = low;
    int r = high;

    while (l <= r) 
    {
        int mid = (l + r) / 2;

        if (arr[mid] == elem) 
        {
            left = mid + 1;
            return true;

        } else if (arr[mid] < elem) 
        {
            l = mid + 1;
        } else 
        {
            r = mid - 1;
        }
    }

    left = l;
    return false;
}

int main() 
{
    int size_1, size_2;
    std::cin >> size_1 >> size_2;

    int arr1[size_1], arr2[size_2];
    int x;

    for (int i = 0; i < size_1; i++)
    {
        std::cin >> x;
        arr1[i] = x;
    }

    for (int j = 0; j < size_2; j++)
    {
        std::cin >> x;
        arr2[j] = x;
    }

    int left = 0;
    for (int l = 0; l < size_2; l++) 
    {
        if ((isExist(arr1, arr2[l], left, size_1)) == true) std::cout << arr2[l] << " ";
    }

    return 0;
}