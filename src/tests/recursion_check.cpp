#include <iostream>

int some_function(int c, int v)
{
    if (v==15)
    {
        return v;
    }
    if (v<15)
    {
        v += c;
    }
    some_function(c, v);
}

int main()
{
    int b = 10;
    int u = 1;
    int y = some_function(u, b);
    std::cout<< y<< std::endl;
    return 0;
}
