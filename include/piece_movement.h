#ifndef PIECE_MOVEMENT 
#define PIECE_MOVEMENT

#include "board.h"
#include "move.h"

void generate_move(const Board * board,Movelist * movelist,int8 sq,int8 piece);
int is_opponent_controls(const Board * board,int8 sq,Turn turn);

#endif