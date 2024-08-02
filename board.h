
#ifndef BOARD 
#define BOARD

#include <stdio.h>

/*
  Pieces Representation
  -ve values are used Black pieces
  +ve values are for White Pieces
*/
typedef enum Pieces{
    Empty = 0,
    Pawn = 1,
    Night = 2,
    Bishop = 3 ,
    Rook = 4,
    Queen = 5,
    King = 6
}Pices;

typedef enum Turn{
    White = 1,
    Black = -1
}Turn;

typedef enum Castling_side{
    Short = 1,
    Long = 0
}Castling_side;


/*
    Board Representation :
    brd  : Array of 8 bit charachters of size 64 stores all the pieces in board
           0->a1  63 ->h8
           
    flag : lowest 4 bits indicate which of the 16 pawns has moved 2 square in last move
           stored for En Passent 
           next 4 bit stores castling rights  (w-short,w-long,b-short,b-long)
           next 6 bit will store position of white king
           next 6 bit will store position of black king
           20th bit will be set if pawn has moved 2 Square
*/

typedef char int8;
typedef struct Board
{
    int8 brd[64];
    int flag;
}Board;

void set_board(Board *);
void set_empty_board(Board *);
char get_piece_from_code(int8);
int8 get_pcode(char p);
void  display(const Board *);
int8 get_king_pos(const Board *,Turn);
void set_king_pos(Board *,Turn,int8);
int get_pawn_jump(const Board *,Turn);
void set_pawn_jump(Board * board,int file,Turn);
int get_castling_right(const Board * board ,Turn,Castling_side);
void set_castling_right(Board * board ,Turn turn,Castling_side,int);

#endif