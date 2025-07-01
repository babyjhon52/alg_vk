/* Если в числе содержится только один бит со значением 1, записать в выходной поток OK.
Иначе записать FAIL.
Необходимо использование битовых операций.
Использование арифметических операций запрещено. */

#include <iostream>

int main() 
{

    unsigned int number;
    std::cin >> number;

    if ( number == 0 )
    {
        std::cout << "FAIL";
    }
    else 
    {
        if ( ( number & (number - 1) ) == 0 ) std::cout << "OK";
        else std::cout << "FAIL";
    }
}