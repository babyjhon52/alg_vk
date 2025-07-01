// Дано множество целых чисел из [0..109] размера n.
// Используя алгоритм поиска k-ой порядковой статистики, требуется найти следующие параметры множества:
// 10% перцентиль
// медиана
// 90% перцентиль
// Требования:
// К дополнительной памяти: O(n).
// Среднее время работы: O(n)
// Должна быть отдельно выделенная функция partition.
// Рекурсия запрещена.
// Решение должно поддерживать передачу функции сравнения снаружи.

#include <iostream>
#include <functional>
#include <algorithm>


template <typename T, typename Comparator = std::less<T>>
int pivot(T *arr, int a, int b, int c, Comparator cmp = Comparator())
{
    if ((cmp(arr[b], arr[a]) && cmp(arr[a], arr[c])) || (cmp(arr[a], arr[b]) && cmp(arr[c], arr[a])))
        return a;
    else if ((cmp(arr[a], arr[b]) && cmp(arr[b], arr[c])) || (cmp(arr[b], arr[a]) && cmp(arr[c], arr[b])))
        return b;
    else
        return c;
}

template <typename T, typename Comparator = std::less_equal<T>>
int partition(T *arr, int l, int r, Comparator cmp = Comparator())
{
    int pivot_pos = pivot(arr, l, (l + r) / 2, r);
    std::swap(arr[pivot_pos], arr[r]);

    int i = l, j = l;

    while (j < r)
    {
        if (cmp(arr[j], arr[r]))
        {
            std::swap(arr[i], arr[j]);
            i++;
        }
        j++;
    }

    std::swap(arr[i], arr[r]);
    return i;
}

template <typename T>
T kth_stat(T *arr, int k, int l, int r)
{
    while (l <= r)
    {
        int pivot_pos = partition(arr, l, r);

        if (pivot_pos == k)
            return arr[pivot_pos];
        else if (pivot_pos > k)
            r = pivot_pos - 1;
        else
            l = pivot_pos + 1;
    }
}


int main()
{
    int n = 0;
    std::cin >> n;
    int *arr = new int[n];

    for (int i = 0; i < n; i++)
    {
        std::cin >> arr[i];
    }

    int k_10_perc = n * 0.1;
    int k_median = n / 2;
    int k_90_perc = n * 0.9;

    int result_10_perc = kth_stat(arr, k_10_perc, 0, n - 1);
    int result_median = kth_stat(arr, k_median, 0, n - 1);
    int result_90_perc = kth_stat(arr, k_90_perc, 0, n - 1);

    std::cout << result_10_perc << std::endl;
    std::cout << result_median << std::endl;
    std::cout << result_90_perc << std::endl;

    delete[] arr;
    return 0;
}
