/*Напишите программу, которая использует кучу для слияния K отсортированных массивов суммарной длиной N.

Требования:
Время работы O(N * logK)
Куча должна быть реализована в виде шаблонного класса.
Решение должно поддерживать передачу функции сравнения снаружи.
Куча должна быть динамической.*/

#include <iostream>

struct myArray 
{
    int* data;
    int size;
    int nowPos;

    myArray() : data(nullptr), size(0), nowPos(0) {}

    myArray(int n) : size(n), nowPos(0) {data = new int[n];}

    myArray(const myArray &other) : size(other.size), nowPos(other.nowPos) 
    {
        data = new int[size];
        for (int i = 0; i < size; i++) 
        {
            data[i] = other.data[i];
        }
    }

    myArray& operator=(const myArray &other) 
    {
        if (this == &other)
            return * this;

        delete[] data;
        size = other.size;
        nowPos = other.nowPos;
        data = new int[size];
        for (int i = 0; i < size; i++) 
        {
            data[i] = other.data[i];
        }

        return * this;
    }

    ~myArray() {delete[] data;}
};

std::istream& operator>>(std::istream &in, myArray &arr) 
{
    for (int i = 0; i < arr.size; i++) 
    {
        in >> arr.data[i];
    }

    return in;
}

bool operator<(const myArray &l, const myArray &r) 
{
    return l.data[l.nowPos] < r.data[r.nowPos];
}

template<typename T>
class defaultComp
{
public:
    bool operator()(const T &l, const T &r)
    {
        return l < r;
    }
};

template <typename T, typename Comparator = defaultComp<T> >
class Heap 
{
public:
    T * arr;
    int capacity;
    int size;
    Comparator cmp;

    Heap(Comparator c = Comparator()) : capacity(8), size(0), cmp(c) {arr = new T[capacity];}

    ~Heap() {delete[] arr;}

    bool empty() {return size == 0;}

    void add(const T &value) 
    {
        if (size >= capacity) resize();

        arr[size] = value;
        siftUp(size);
        size++;
    }

    T extract() 
    {
        T result = arr[0];

        arr[0] = arr[size - 1];
        size--;
        siftDown(0);
        return result;
    }

    T top() {return arr[0];}

    void siftUp(int index) 
    {
        int i = index;
        while (i > 0) {
            int parent = (i - 1) / 2;
            if (cmp(arr[i], arr[parent])) 
            {
                std::swap(arr[i], arr[parent]);
                i = parent;
            } else break;
        }
    }

    void siftDown(int index) 
    {
        int i = index;
        while (true) 
        {
            int left = 2 * i + 1;
            int right = 2 * i + 2;
            int smallest = i;

            if (left < size && cmp(arr[left], arr[smallest])) smallest = left;
            if (right < size && cmp(arr[right], arr[smallest])) smallest = right;
            if (smallest == i) break;

            std::swap(arr[i], arr[smallest]);
            i = smallest;
        }
    }

    void resize() 
    {
        int newCapacity = capacity * 2;
        T * newArr = new T[newCapacity];

        for (int i = 0; i < size; i++)
        {
            newArr[i] = arr[i];
        }

        delete[] arr;
        arr = newArr;
        capacity = newCapacity;
    }
};

int main() 
{
    int k;
    std::cin >> k;
    Heap<myArray> heap;

    for (int i = 0; i < k; i++) 
    {
        int m;
        std::cin >> m;
        myArray arr(m);
        std::cin >> arr;
        heap.add(arr);
    }

    while (!heap.empty()) 
    {
        myArray cur = heap.extract();
        std::cout << cur.data[cur.nowPos] << " ";
        cur.nowPos++;
        if (cur.nowPos < cur.size) heap.add(cur);
    }
    return 0;
}
