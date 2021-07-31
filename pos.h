#include <iostream>

class pos
{
public:
    int x; //1-8
    int y; //1-8

    virtual void set(int a, int b)
    {
        x = a;
        y = b;
    }

    pos operator+(pos p2)
    {
        pos temp;
        temp.x = x + p2.x;
        temp.y = y + p2.y;
        return temp;
    }

    virtual void operator=(pos p2)
    {
        x = p2.x;
        y = p2.y;
    }

    bool operator==(pos p2)
    {
        if (x == p2.x && y == p2.y)
            return true;
        return false;
    }
};