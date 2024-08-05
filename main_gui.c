#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#include "board.h"
#include "move.h"
#include "take_input.h"
#include "chess_rule.h"
#include "search.h"

//void set_board_from2D(Board * board);
int main(int argc,char ** argv)
{
    int i;
    char input[8],res[8];
    Board board;
    Movelist moves;
    int first_move = 1;
    set_board(&board);
    //display(&board);
    time_t t;
    srand((unsigned)time(&t));
    //int count=1;
    Turn player = argv[1][0]=='w'?White:Black;
    while (1)
    {
        Move move;
        //white move 
        if(is_checkmate(&board,White)==1){
            fprintf(stdout,"Black Won\n");
            fflush(stdout);
            break;
        }else if(is_stalemate(&board,White)){
            fprintf(stdout,"Draw by Stalemate\n");
            fflush(stdout);
            break;
        }
        move.mv = 0;
        if(player==White){
            while(move.mv==0){
                scanf("%s",input);
                move = user_input_from_GUI(&board,White,input);
                if(move.mv==0){
                    fprintf(stdout,"invalid\n");
                    fflush(stdout);
                }else{
                    fprintf(stdout,"valid\n");
                    fflush(stdout);
                }
            }
            move_on_board(&board,&move);
            for(i=0;i<8;i++) input[i] = '\0';
        }else{
            if(first_move==1){
                move = random_move(&board,White);
                first_move = 0;
            }else{
                move = computer_move(&board,White);
            }

            for(i=0;i<8;i++) res[i] = '\0';
            move_to_string(&move,res);
            fprintf(stdout,res);
            fflush(stdout);
            move_on_board(&board,&move);  
        }
        
        //Black move
        if(is_checkmate(&board,Black)==1){
            fprintf(stdout,"White Won\n");
            fflush(stdout);
            break;
        }else if(is_stalemate(&board,Black)){
            fprintf(stdout,"Draw by Stalemate\n");
            fflush(stdout);
            break;
        }
        move.mv = 0;
        if(player==Black){
            while(move.mv==0){
                scanf("%s",input);
                move = user_input_from_GUI(&board,Black,input);
                if(move.mv==0){
                    fprintf(stdout,"invalid\n");
                    fflush(stdout);
                }else{
                    fprintf(stdout,"valid\n");
                    fflush(stdout);
                }
            }
            move_on_board(&board,&move);
            for(i=0;i<8;i++) input[i] = '\0';
        }else{
            if(first_move==1){
                move = random_move(&board,Black);
                first_move = 0;
            }else{
                move = computer_move(&board,Black);
            }
            for(i=0;i<8;i++) res[i] = '\0';
            move_to_string(&move,res);
            printf("%s",res);
            fflush(stdout);
            move_on_board(&board,&move);  
        }
    }
    return 0;
}
/*

void set_board_from2D(Board * board){
    char b[8][8] = {
                    {'.','.','.','n','k','.','.','.'},
                    {'R','.','.','.','.','.','.','.'},
                    {'.','.','.','.','.','.','.','.'},
                    {'.','.','.','.','.','.','.','.'},
                    {'.','.','.','.','.','.','.','P'},
                    {'.','.','.','K','.','.','.','.'},
                    {'.','.','.','p','.','.','.','.'},
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
*/
