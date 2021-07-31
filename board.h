#include <algorithm>
#include <vector>
#include <cctype>
#include <fstream>
#include "piece.h"

#define moves struct moves
moves
{
    pos old_pos;
    pos new_pos;
    piece *occupied_by;
};

class Board
{
private:
    //for both types of pieces 0-7 are powers and 8-15 are pawns
    //king is 3
    piece white_pieces[16]; //these are lower case atm
    piece black_pieces[16];
    std::vector<moves> past_moves;
    std::vector<piece *> eaten;
    bool king_has_moved[2]; //0 for white

public:
    std::vector<pos> valid_moves;
    char turn;
    bool in_check[2]; //0 for white

    Board();

    piece *check_pos(pos current); //only returns uneaten pieces that has this

    inline piece *eat(piece *p);
    inline piece *undo_eat(piece *p);

    void move(pos old_pos, pos new_pos);
    void undo();

    void generate_moves(pos current);
    void generate_moves(piece p);

    bool valid(pos new_pos);

    inline void check_diagonal(piece p);
    inline void check_horver(piece p);
    void check_l(piece p);
    void check_linear(pos xy, int x, int y);

    void check_king();

    inline void show_valid_moves();

    bool is_in_check(); //checks if this position is in check

    inline void promote(piece *p);
    void castle(char side);
    bool allow_castle(pos king, pos castle, char side);

    void show();

    void input();

    void change_turn();

    bool win();

    inline void kill_all();

    void castle_ready();

    void win_ready();
};

inline bool in_bounds(pos xy)
{
    if (xy.x > 0 && xy.x < 9 && xy.y > 0 && xy.y < 9)
        return true;
    return false;
}