#ifndef TAKE_INPUT 
#define TAKE_INPUT

#include <stdlib.h>
#include <stdio.h>

#include "board.h"
#include "move.h"
#include "chess_rule.h"

Move user_input(Board * board,const Turn turn);
Move user_input_from_GUI(Board * board,const Turn turn,char * move);

#endif