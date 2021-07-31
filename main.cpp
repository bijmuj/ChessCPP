#include "Board.cpp"

#define CASTLE_DEMO 0
#define MAIN 1
#define WINNING 0

#if MAIN
int main()
{
    pos p1, p2;
    Board b;
    int t = 1;
    b.show();
    while (!b.win())
    {
        b.input();
        if (b.in_check[t])
        {
            std::cout << b.turn << " is in check\n";
            b.check_king();
            if (b.valid_moves.size() == 0)
                break;
            b.undo();
        }
        else
        {
            b.change_turn();
            if (t)
                t = 0;
            else
                t = 1;
        }
        b.show();
    }
    b.show();
    if (b.turn == 'W')
        std::cout << "White won";
    else
    {
        std::cout << "Black won";
    }
}
#endif

#if CASTLE_DEMO
int main()
{
    char ch;
    Board b;
    b.kill_all();
    b.castle_ready();
    b.show();
    std::cout << "Press any key:";
    std::cin >> ch;
    system("CLS");
    b.castle('l');
    b.show();
    std::cout << "Press any key:";
    std::cin >> ch;
    b.change_turn();
    system("CLS");
    b.castle('l');
    b.show();
    std::cout << "Press any key:";
    std::cin >> ch;
}
#endif

#if WINNING
int main()
{
    Board b;
    b.kill_all();
    b.win_ready();

    char ch;
    std::cin >> ch;
}

#endif