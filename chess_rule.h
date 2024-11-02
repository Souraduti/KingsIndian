#ifndef CHESS_RULE
#define CHESS_RULE

#include <stdlib.h>

#include "board.h"
#include "move.h"
#include "piece_movement.h"
#include "make_unmake.h"

/*
    This functions generates all the pseode legal moves in a given position.
    if the filter_legal flag is set it filters out all moves that puts king 
    in  danger
*/
Movelist generate_all(Board * ,Movelist *,Turn,int filter);

/*
    1 implies true
    0 implies false
*/
int in_check(const Board *,Turn);
int is_checkmate(Board*,Turn);
int is_stalemate(Board *,Turn);
int is_insufficient(Board *board);
int is_repitation(Board * board);
Game_End_State get_game_state(Board *,Turn);

#endif