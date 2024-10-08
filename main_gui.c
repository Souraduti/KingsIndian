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
    int first_move = 1;
    char input[8],res[8];
    Board board;
    
    time_t t;
    srand((unsigned)time(&t));
    int game_state=0;
    Turn player = argv[1][0]=='w'?White:Black;
    char * fen = argv[2];
    set_board_fen(&board,fen);
    while (game_state==0)
    {
        //white move 
        Move move;
        move.mv = 0;
        if(player==White){
            while(move.mv==0){
                scanf("%s",input);
                move = user_input_from_GUI(&board,White,input);
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
            if(first_move==1){
                move = random_move(&board,White);
                first_move = 0;
            }else{
                move = computer_move(&board,White);
            }

            for(i=0;i<8;i++) res[i] = '\0';
            move_to_string(&move,res);
            printf("%s",res);
            fflush(stdout);
            move_on_board(&board,&move);  
        }
        game_state = get_game_state(&board,Black);
        printf("%d\n",game_state);
        fflush(stdout);

        //Black move
        move.mv = 0;
        if(player==Black){
            while(move.mv==0){
                scanf("%s",input);
                move = user_input_from_GUI(&board,Black,input);
                if(move.mv==0){
                    printf("invalid\n");
                    fflush(stdout);
                }else{
                    printf("valid\n");
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
        game_state = get_game_state(&board,White);
        printf("%d\n",game_state);
        fflush(stdout);
    }
    return 0;
}
