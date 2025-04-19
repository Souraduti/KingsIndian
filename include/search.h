#ifndef SEARCH
#define SEARCH

#include "board.h"
#include "move.h"
#include "chess_rule.h"
#include "static_evaluation.h"

#define MAXDEPTH 5
#define INF 500000

/*
    evaluate function uses minimax algorithm with alpha-beta pruning
*/
int evaluate(Board * board,Turn turn,int depth,int* legal,int alpha,int beta);
Move computer_move(Board * board,Turn turn,int depth);
Move random_move(Board * board,int turn);

#endif