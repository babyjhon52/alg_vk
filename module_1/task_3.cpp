/* Реализовать дек с динамическим зацикленным буфером.
Обрабатывать команды push * и pop *. */

#include <iostream>

class BufffDeque 
{

public:
    BufffDeque() 
    {
        arr = new int[5];
        head = 0;
        tail = -1;
        count = 0;
        SIZE = 5;
    }

    ~BufffDeque() { delete[] arr; }

    BufffDeque( const BufffDeque &other ) = delete;
    BufffDeque& operator=( const BufffDeque &other ) = delete;

    void pushBack( int n ) 
    {
        if ( SIZE == count ) resize();
        if ( isEmpty() ) 
        {
            head = 0;
            tail = 0;
        } else 
        {
            tail = ( tail + 1 ) % SIZE;
        }

        arr[tail] = n;
        count++;
    }

    void pushFront( int n ) 
    {
        if ( SIZE == count ) resize();
        if ( isEmpty() ) 
        {
            head = 0;
            tail = 0;
        } else 
        {
            head = ( head - 1 + SIZE ) % SIZE;
        }

        arr[head] = n;
        count++;
    }

    bool popFront( int n ) {
        if ( isEmpty() ) return n == -1;
        
        if ( arr[head] == n)  
        {
            if ( head == tail ) 
            {
                head = 0;
                tail = -1;
            } else 
            {
                head = ( head + 1 ) % SIZE;
            }

            count--;
            return true;
        }

        return false;
    }

    bool popBack( int n ) 
    {
        if ( isEmpty() ) return n == -1;
        
        if ( arr[tail] == n ) 
        {
            if ( head == tail ) 
            {
                head = 0;
                tail = -1;
            } else 
            {
                tail = ( tail - 1 + SIZE ) % SIZE;
            }

            count--;
            return true;
        }

        return false;
    }

    bool isEmpty() { return count == 0; }

    void resize() 
    {
        int new_size = SIZE * 2;
        int *new_arr = new int[new_size];
        
        for ( int i = 0; i < count; ++i ) 
        {
            new_arr[i] = arr[( head + i ) % SIZE];
        }
        
        delete[] arr;
        arr = new_arr;

        head = 0;
        tail = count - 1;
        SIZE = new_size;
    }

private:
    int head, tail, count, SIZE;
    int * arr;
};

void run( std::istream &input, std::ostream &output ) 
{
    BufffDeque deque;
    
    int n;
    input >> n;
    
    for ( int i = 0; i < n; ++i ) 
    {
        int op, val;
        input >> op >> val;
        
        switch(op) 
        {
            case 1: deque.pushFront( val ); break;
            case 2: if ( !deque.popFront(val) ) { output << "NO"; return; } break;
            case 3: deque.pushBack(val); break;
            case 4: if ( !deque.popBack(val) ) { output << "NO"; return; } break;
        }
    }
    
    output << "YES";
}

int main() 
{
    run( std::cin, std::cout );
    return 0;
}
