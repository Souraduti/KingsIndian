#include <stdio.h>
#include <time.h>

#include "board.h"
#include "move.h"
#include "take_input.h"
#include "chess_rule.h"
#include "search.h"

/*
    This file is for communicating with the GUI program written in Python
    gui.py runs this as a sub-process and communicates through stdout and stdin
 */


int main(int argc,char ** argv)
{
    int i;
    int first_random_move = 1;
    char input[8],res[8];
    Board board;
    char* initial_position = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR"; 
    time_t time_;
    srand((unsigned)time(&time_));
    int game_state=0;
    Turn player = argv[1][0]=='w'?White:Black;
    char * fen = argv[2];
    char t = argv[3][0];
    set_board_fen(&board,fen,t);
    if(strcmp(fen,initial_position)!=0){
        first_random_move = 0;
    }

    while (game_state==0)
    { 
        Move move;
        move.mv = 0;
        int depth = 5;
        if(board.move_number<=10){
            depth = 3;
        }else if(board.move_number<=60){
            depth = 4;
        }else{
            depth = 5;
        }
        if(player==board.turn){
            while(move.mv==0){
                scanf("%s",input);
                move = user_input_from_GUI(&board,get_next_turn(&board),input);
                if(move.mv==0){
                    printf("invalid\n");
                    fflush(stdout);
                }else{
                    printf("valid\n");
                    fflush(stdout);
                }
            }
            for(i=0;i<8;i++) input[i] = '\0';
            move_on_board(&board,&move);
        }else{
            if(first_random_move==1){
                move = random_move(&board,get_next_turn(&board));
                first_random_move = 0;
            }else{
                move = computer_move(&board,get_next_turn(&board),depth);
            }

            for(i=0;i<8;i++) res[i] = '\0';
            move_to_string(&move,res);
            printf("%s",res);
            fflush(stdout);
            move_on_board(&board,&move);  
        }
        game_state = get_game_state(&board,get_next_turn(&board));
        printf("%d\n",game_state);
        fflush(stdout);
    }
    return 0;
}
