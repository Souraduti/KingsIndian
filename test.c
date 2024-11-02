#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#include "board.h"
#include "move.h"
#include "take_input.h"
#include "chess_rule.h"
#include "search.h"

void show_bits(int a);
void set_board_from2D(Board * board);

int main(int argc,char ** argv)
{
    int choice = 0,first_move=0,count=0;
    time_t t,start,end;
    double elapsed,total=0;
    srand((unsigned)time(&t));
    Board board;
    Move move;
    Movelist moves;
    Game_End_State game_state = ON;
    Turn player = White;
    set_board_fen(&board,"8/8/8/k7/8/2q5/1R6/1K6",'w');
    display(&board);
    while (game_state==0)
    { 
        Move move;
        move.mv = 0;
        int depth = 4;
        // if(board.move_number<=10){
        //     depth = 3;
        // }else if(board.move_number<=60){
        //     depth = 4;
        // }else{
        //     depth = 5;
        // }
        if(player==board.turn){
            while(move.mv==0){
                move = user_input(&board,board.turn);
            }
            move_on_board(&board,&move);
        }else{
            if(first_move==1){
                move = random_move(&board,get_next_turn(&board));
                first_move = 0;
            }else{
                move = computer_move(&board,get_next_turn(&board),depth);
            }
            display_move(&move);
            move_on_board(&board,&move);  
        }
        game_state = get_game_state(&board,get_next_turn(&board));
        // system("cls");
        display(&board);
        printf("hash %X\n",board.hash[board.move_number]);
    }
    char * str;
    switch(game_state){
        case ON:str = "game-on";break;
        case White_Won:str = "White Won";break;
        case Black_Won:str = "Black Won";break;
        case Draw_Stalemate:str = "Draw by Stalemate";break;
        case Draw_Repetition:str = "Draw by Repitition";break;
        case Draw_Insufficient:str = "Draw by Insufficient Material";break;
    }
    display(&board);
    printf("Game Result : %s\n",str);
    
    // display(&board);
    return 0;
}

void set_board_from2D(Board * board){
    char b[8][8] = {
                    {'.','.','.','.','.','.','K','.'},
                    {'.','.','.','.','.','.','.','.'},
                    {'.','.','.','.','.','.','.','.'},
                    {'.','.','k','.','.','.','.','.'},
                    {'.','.','.','.','.','.','.','.'},
                    {'.','.','.','.','.','.','.','.'},
                    {'.','.','.','.','.','.','.','.'},
                    {'.','.','.','.','.','.','.','.'},
                    };
    set_empty_board(board);
    int i,j;
    board->flag = 0b000000000000011110000010011110000;
    for(i=0;i<8;i++){
        for(j=0;j<8;j++){
            board->brd[8*(7-i)+j] = get_pcode(b[i][j]);
            if(b[i][j]=='k'){
                set_king_pos(board,-1,8*(7-i)+j);
            }else if(b[i][j]=='K'){
                set_king_pos(board,1,8*(7-i)+j);
            }
        }
    }
}
void show_bits(int a){
    int i;
    for(i=31;i>=0;i--){
        int k = a&(1<<i);
        if(k==0){
            printf("0");
        }else{
            printf("1");
        }
    }
    printf("\n");
}
