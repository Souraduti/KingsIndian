#ifndef ZOBRIST
#define ZOBRIST


#include "board.h"
#include "move.h"

long long int compute_hash(const Board * board);
void update_hash(Board * board,Move * move,int);

#endif