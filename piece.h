#include "pos.h"

class piece : public pos
{
public:
    char color, type;
    bool eaten;

    void set(int x, int y, char c, char t, bool e)
    {
        pos::set(x, y);
        color = c;
        type = t;
        eaten = e;
    }

    void operator=(pos p2)
    {
        x = p2.x;
        y = p2.y;
    }

    void operator=(piece p)
    {
        x = p.x;
        y = p.y;
        color = p.color;
        type = p.type;
        eaten = p.eaten;
    }
};