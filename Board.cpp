#include "board.h"

Board::Board()
{
    int i, x, y;
    std::ifstream input_file;
    char ch;

    x = y = 1;
    turn = 'W';
    in_check[0] = in_check[1] = false;
    king_has_moved[0] = king_has_moved[1] = false;
    valid_moves.clear();
    eaten.clear();
    past_moves.clear();

    input_file.open("pieces.dat");

    for (i = 0; i < 16; i++)
    {
        input_file >> ch;
        white_pieces[i].set(x, y, 'W', ch, false);
        if (x == 8)
        {
            x = 1;
            y = 2;
        }
        else
        {
            x++;
        }
    }

    input_file.seekg(0);
    x = 1;
    y = 8;

    for (i = 0; i < 16; i++)
    {
        input_file >> ch;
        black_pieces[i].set(x, y, 'b', tolower(ch), false);
        if (x == 8)
        {
            x = 1;
            y = 7;
        }
        else
        {
            x++;
        }
    }

    input_file.close();
}

inline piece *Board::eat(piece *p)
{
    p->eaten = true;
    eaten.push_back(p);
    return p;
}

inline piece *Board::undo_eat(piece *p)
{
    p->eaten = false;
    eaten.pop_back();
    return p;
}

void Board::input()
{
    pos p1, p2;
    piece *p;
    int t;
    if (turn == 'W')
        t = 0;
    else
        t = 1;

    do
    {
        do
        {
            std::cout << "Piece to move(x,y):\n(-1 -1 to undo):";
            std::cin >> p1.x >> p1.y;
            if (p1.x == -1)
            {
                undo();
                undo();
                show();
                continue;
            }
            if (p = check_pos(p1))
            {
                if (p->color == turn)
                    break;
            }
        } while (1);
        generate_moves(p1);
    } while (!valid_moves.size());

    show_valid_moves();

    do
    {
        std::cout << "Move to(x,y):";
        std::cin >> p2.x >> p2.y;
        if (valid(p2))
            break;
    } while (1);

    move(p1, p2);

    if (is_in_check())
    {
        if (turn == 'W')
            in_check[0] = true;
        if (turn == 'b')
            in_check[1] = true;
    }
}

bool Board::valid(pos new_pos)
{
    for (int i = 0; i < valid_moves.size(); i++)
    {
        if (new_pos == valid_moves[i])
            return true;
    }
    return false;
}

void Board::move(pos old_p, pos new_p)
{
    moves m;
    m.old_pos = old_p;
    m.new_pos = new_p;
    //checking if we're eating
    piece *p = check_pos(new_p);
    if (p)
    {
        m.occupied_by = p;
        p = eat(p);
    }
    else
    {
        m.occupied_by = NULL;
    }
    past_moves.push_back(m);

    //actually moving the piece
    p = check_pos(old_p);
    *p = new_p;

    if ((p->type == 'p' && new_p.y == 1) || (p->type == 'P' && new_p.y == 8))
        promote(p);
}

void Board::undo()
{
    if (past_moves.size())
    {
        moves m = past_moves[past_moves.size() - 1];
        piece *p = check_pos(m.new_pos);
        *p = m.old_pos;
        if (m.occupied_by)
        {
            p = eaten[eaten.size() - 1];
            p = undo_eat(p);
        }
        past_moves.pop_back();
    }
}

piece *Board::check_pos(pos current)
{
    int i;
    if (in_bounds(current))
    {
        for (i = 0; i <= 15; i++)
        {
            if (white_pieces[i] == current && !white_pieces[i].eaten)
            {
                return &white_pieces[i];
            }
            else if (black_pieces[i] == current && !black_pieces[i].eaten)
            {
                return &black_pieces[i];
            }
        }
    }
    return NULL;
}

void Board::show()
{
    system("CLS");
    pos p;
    piece *pi;
    std::cout << turn << "'s turn\n";

    for (int i = 1; i < 9; ++i)
        std::cout << i << "|";
    std::cout << "\n";

    for (int i = 1; i < 9; i++)
    {
        for (int j = 1; j < 9; j++)
        {
            p.set(j, i);
            pi = check_pos(p);
            if (pi)
            {
                std::cout << pi->type;
            }
            else
            {
                std::cout << '.';
            }
            std::cout << "|";
        }
        std::cout << i << "\n";
    }
    std::cout << "eaten:\n";
    for (int i = 0; i < eaten.size(); i++)
    {
        std::cout << eaten[i]->color << eaten[i]->type << "\n";
    }
}

void Board::generate_moves(pos current)
{
    piece *p = check_pos(current);
    pos xy;

    valid_moves.clear();

    if (p)
    {
        //checking black pawn
        if (p->type == 'P')
        {
            //non eating checks
            xy = *p;
            xy.y++;
            if (in_bounds(xy) && check_pos(xy) == NULL)
            {
                valid_moves.push_back(xy);
                if (xy.y == 3)
                {
                    xy.y++;
                    if (in_bounds(xy) && check_pos(xy) == NULL)
                        valid_moves.push_back(xy);
                }
            }

            //eating checks
            xy = *p;
            xy.y++;
            xy.x += 1;
            if (in_bounds(xy) && check_pos(xy))
                if (check_pos(xy)->color == 'b')
                {
                    valid_moves.push_back(xy);
                }

            xy.x -= 2;
            if (in_bounds(xy) && check_pos(xy))
                if (check_pos(xy)->color == 'b')
                    valid_moves.push_back(xy);
        }

        //checking white pawn
        else if (p->type == 'p')
        {
            //non eating checks
            xy = *p;
            xy.y--;
            if (in_bounds(xy) && check_pos(xy) == NULL)
            {
                valid_moves.push_back(xy);
                if (xy.y == 6)
                {
                    xy.y--;
                    if (in_bounds(xy) && check_pos(xy) == NULL)
                        valid_moves.push_back(xy);
                }
            }

            //eating checks
            xy = *p;
            xy.y--;
            xy.x += 1;
            if (check_pos(xy))
                if (check_pos(xy)->color == 'W')
                    valid_moves.push_back(xy);

            xy.x -= 2;
            if (check_pos(xy))
                if (check_pos(xy)->color == 'W')
                    valid_moves.push_back(xy);
        }

        //checking horses
        else if (tolower(p->type) == 'n')
            check_l(*p);

        //checking bishop
        else if (tolower(p->type) == 'b')
            check_diagonal(*p);

        //checking rook
        else if (tolower(p->type) == 'r')
            check_horver(*p);

        //checking queen
        else if (tolower(p->type) == 'q')
        {
            check_diagonal(*p);
            check_horver(*p);
        }

        else
        {
            check_king();
        }
    }
}

void Board::generate_moves(piece p)
{
    pos current;
    current = p;
    generate_moves(current);
}

void Board::check_linear(pos xy, int x, int y)
{
    pos offset;
    offset.set(x, y);
    xy = xy + offset;
    while (in_bounds(xy))
    {
        if (check_pos(xy))
        {
            if (check_pos(xy)->color != turn)
                valid_moves.push_back(xy);
            return;
        }
        valid_moves.push_back(xy);
        xy = xy + offset;
    }
}

inline void Board::check_diagonal(piece p)
{
    pos xy;
    xy = p;
    check_linear(xy, 1, 1);
    check_linear(xy, 1, -1);
    check_linear(xy, -1, 1);
    check_linear(xy, -1, -1);
}

inline void Board::check_horver(piece p)
{
    pos xy;
    xy = p;
    check_linear(xy, 1, 0);
    check_linear(xy, -1, 0);
    check_linear(xy, 0, 1);
    check_linear(xy, 0, -1);
}

void Board::check_l(piece p)
{
    pos xy;
    int a = 2, b = 1;

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 2; j++)
        {
            xy = p;
            xy.x += a;
            xy.y += b;
            if (in_bounds(xy))
            {
                if (check_pos(xy))
                {
                    if (check_pos(xy)->color != p.color)
                    {
                        valid_moves.push_back(xy);
                    }
                }
                else
                    valid_moves.push_back(xy);
            }
            b = -b;
        }
        a = -a;
        std::swap(a, b);
    }
}

void Board::check_king()
{
    std::vector<pos> m;
    pos xy, king;
    piece *p;
    int i, j;
    i = j = -1;

    if (turn == 'W')
    {
        king.x = white_pieces[4].x;
        king.y = white_pieces[4].y;
    }
    else
    {
        king.x = black_pieces[4].x;
        king.y = black_pieces[4].y;
    }

    while (i < 2)
    {
        xy.y = king.y + (i++);
        j = -1;
        while (j < 2)
        {
            xy.x = king.x + (j++);
            if (in_bounds(xy))
            {
                p = check_pos(xy);
                if (p == NULL)
                {
                    change_turn();
                    move(king, xy);
                    if (!is_in_check())
                        m.push_back(xy);
                    undo();
                    change_turn();
                }
                else if (p->color != turn)
                {
                    move(king, xy);
                    change_turn();
                    if (!is_in_check())
                        m.push_back(xy);
                    undo();
                    change_turn();
                }
            }
        }
    }
    valid_moves.clear();
    valid_moves = m;
}

bool Board::is_in_check()
{
    pos current;
    if (turn == 'W')
    {
        current.x = black_pieces[4].x;
        current.y = black_pieces[4].y;
        for (int i = 0; i < 16; i++)
        {
            if (!white_pieces[i].eaten && i != 4)
            {
                generate_moves(white_pieces[i]);
                for (int j = 0; j < valid_moves.size(); j++)
                {
                    if (current == valid_moves[j])
                    {
                        return true;
                    }
                }
            }
        }
    }
    else
    {
        current.x = white_pieces[4].x;
        current.y = white_pieces[4].y;
        for (int i = 0; i < 16; ++i)
        {
            if (!black_pieces[i].eaten && i != 4)
            {
                generate_moves(black_pieces[i]);
                for (int j = 0; j < valid_moves.size(); j++)
                {
                    if (current == valid_moves[j])
                        return true;
                }
            }
        }
    }
    return false;
}

inline void Board::show_valid_moves()
{
    std::cout << "Valid moves(x,y):\n";
    for (int i = 0; i < valid_moves.size(); i++)
    {
        std::cout << valid_moves[i].x << " " << valid_moves[i].y << "\n";
    }
}

inline void Board::promote(piece *p)
{
    char t;
    std::cout << "new type?";
    std::cin >> p->type;
}

void Board::castle(char side)
{
    piece *p;

    pos castle_old, castle_new, king_old, king_new;
    king_old.x = 5;

    if (turn == 'W')
        castle_old.y = king_old.y = castle_new.y = king_new.y = 1;
    else
        castle_old.y = king_old.y = castle_new.y = king_new.y = 8;

    if (side == 'l')
    {
        castle_old.x = 1;
        king_new.x = 3;
        castle_new.x = 4;
    }
    else
    {
        castle_old.x = 8;
        king_new.y = 7;
        castle_new.y = 6;
    }

    if (allow_castle(king_old, castle_old, side))
    {
        move(king_old, king_new);
        move(castle_old, castle_new);
    }
}

bool Board::allow_castle(pos king, pos castle, char side)
{
    int t;
    if (turn == 'W')
        t = 0;
    else
        t = 1;

    if (king_has_moved[t])
        return false;

    pos p;
    p = king;
    if (side == 'l')
    {
        t = -1;
    }
    else
    {
        t = 1;
    }
    p.x += t;

    while (p.x != castle.x)
    {
        if (check_pos(p))
            return false;
        p.x += t;
    }
    return true;
}

void Board::change_turn()
{
    if (turn == 'W')
        turn = 'b';
    else
        turn = 'W';
}

bool Board::win()
{
    if (black_pieces[4].eaten)
    {
        std::cout << "w wins\n";
        return true;
    }
    else if (white_pieces[4].eaten)
    {
        std::cout << "b wins\n";
        return true;
    }
    return false;
}

inline void Board::kill_all()
{
    for (int i = 0; i < 16; i++)
    {
        white_pieces[i].eaten = true;
        black_pieces[i].eaten = true;
    }
}

inline void Board::castle_ready()
{
    white_pieces[0].eaten = white_pieces[4].eaten = false;
    black_pieces[0].eaten = black_pieces[4].eaten = false;
}

void Board::win_ready()
{
    white_pieces[0].eaten = white_pieces[4].eaten = white_pieces[7].eaten = false;
    black_pieces[4].eaten = false;
    show();
    char ch;
    pos p1, p2;
    p1 = white_pieces[0];
    p2.x = 1;
    p2.y = 7;
    std::cin >> ch;
    move(p1, p2);
    show();
    p1 = black_pieces[4];
    p2.x = 4;
    p2.y = 8;
    std::cin >> ch;
    move(p1, p2);
    show();
    p1 = white_pieces[7];
    p2.x = 8;
    p2.y = 8;
    std::cin >> ch;
    move(p1, p2);
    show();
    change_turn();
    check_king();
    if (valid_moves.size() == 0)
    {
        change_turn();
        std::cout << turn << " wins";
    }
}