/*На числовой прямой окрасили N отрезков.
Известны координаты левого и правого концов каждого отрезка [Li, Ri]. Найти сумму длин частей числовой прямой, окрашенных ровно в один слой.
N ≤ 10000. Li, Ri — целые числа в диапазоне [0, 109].*/

#include <iostream>

struct Segment 
{
    int x;
    int type;
    Segment(int x, int type) : x(x), type(type) {}
    Segment() = default;
};

bool operator<(const Segment &l, const Segment &r)
{
    return (l.x < r.x || (l.x == r.x && l.type < r.type));
}

template <typename T>
bool defaultLess(const T &l, const T &r)
{
    return l < r;
}

template <typename T>
void merge(T* arr, int left, int mid, int right, bool (*cmp)(const T &l, const T &r) = defaultLess) 
{
    int n1 = mid - left + 1;
    int n2 = right - mid;

    T* L = new T[n1];
    T* R = new T[n2];

    for (int i = 0; i < n1; i++) L[i] = arr[left + i];
    for (int j = 0; j < n2; j++) R[j] = arr[mid + 1 + j];

    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) 
    {
        if (cmp(L[i], R[j])) arr[k++] = L[i++];
        else arr[k++] = R[j++];
    }

    while (i < n1) arr[k++] = L[i++];
    while (j < n2)  arr[k++] = R[j++];

    delete[] L;
    delete[] R;
}

template <typename T>
void mergeSort(T* arr, int left, int right) 
{
    if (left >= right) return;
    
    int mid = left + (right - left) / 2;
    mergeSort(arr, left, mid);
    mergeSort(arr, mid + 1, right);
    merge(arr, left, mid, right);
}

int main() 
{
    int MAX_SIZE = 20000;
    Segment segments[MAX_SIZE];
    
    int x = 0, y = 0;
    int size = 0;
    int n = 0;
    std::cin >> n;
    for (int i = 0; i < n; i++)
    {
        std::cin >> x >> y;
        segments[size++] = Segment(x, 1);
        segments[size++] = Segment(y, -1);
    }

    mergeSort(segments, 0, size - 1);

    int count = 0, prevPos = 0, res = 0;

    for (int i = 0; i < size; i++) 
    {
        if (count == 1) 
        {
            res += segments[i].x - prevPos;
        }
        count += segments[i].type;
        prevPos = segments[i].x;
    }

    std::cout << res << std::endl;
    return 0;
}
