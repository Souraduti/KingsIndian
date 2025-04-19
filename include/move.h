#ifndef MOVE
#define MOVE

#include <stdio.h>

#include "board.h"
/*
    Move representation : 
    structure of mv
    00000000000 0000  0000   000000 000000
    |  flag |  |capt||piece| |dest| |source|

    flag ->   00000  00  0   0000
            prev-En  LS  En  BRNQ 
                               LS
                  prev-castle promotion&castling
*/

typedef struct Move{
    int mv;
}Move;


/*
    Highest number of moves possible is 218
    But for practical cases size 128 will suffice
*/
#define LIST_LENGTH 128

typedef struct Movelist{
    Move list[LIST_LENGTH];
    int size;
}Movelist;


Turn get_turn(const Move *);
int8 get_source(const Move *);
int8 get_destination(const Move *);
int8 get_piece(const Move *);
int8 get_captured_piece(const Move *);

void set_source(Move *,int8);
void set_destination(Move *,int8);
void set_piece(Move *,int8);
void set_captured_piece(Move *,int8);

//functions for manipulating flags 

int is_castling(const Move * move,Castling_side);
void set_castling(Move * move,Castling_side);
void clear_castling(Move * move,Castling_side);
int is_promotion(const Move * move);
int get_promotion(const Move *);
void set_promotion(Move *,int);
int clear_promotion(Move *);
int is_enpassant(const Move *);
void set_enpassant(Move *);
void clear_enpassant(Move *);

void store_board_flag(Move *,Board *);
void restore_board_flag(Move *,Board *);

void display_move(const Move * move);


int add_move(Movelist *,Move);
void clear_movelist(Movelist *);
void show_all_moves(const Movelist *moves);

void move_to_string(const  Move * move,char *);

#endif