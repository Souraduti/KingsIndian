#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#include "board.c"
#include "move.c"
#include "take_input.c"
#include "chess_rule.c"
#include "search.c"



/*
    //for time taken by a move
    start = clock();
    move = computer_move(&board,-1);
    end = clock();
    elapsed = (end-start)*1000/CLOCKS_PER_SEC;
*/
int main(){

    int choice = 0;
    time_t t,start,end;
    double elapsed;
    srand((unsigned)time(&t));
    Board board;
    Move move;
    Movelist moves;

    set_board(&board);
    display(&board);
    do{
        printf("Play as \n1.White\n2.Black\n3.Random\n");
        scanf("%d",&choice);
    }while(choice!=1&&choice!=2&&choice!=3);
    if(choice==3){
        choice = rand()%2+1;
    }

    while (1)
    {
        //white move 
        if(is_checkmate(&board,1)==1){
            printf("Black Won\n");
            break;
        }else if(is_stalemate(&board,1)){
            printf("Draw by Stalemate\n");
            break;
        }
        move.mv = 0;
        if(choice==1){
            while(move.mv==0){
                move = user_input(&board,1);
            }
            move_on_board(&board,&move);
            system("cls");
            display(&board);
        }else if(choice==2){
            start = clock();
            move = computer_move(&board,1);
            end = clock();
            elapsed = (end-start)*1000/CLOCKS_PER_SEC;
            move_on_board(&board,&move);
            system("cls");
            display(&board);
            display_move(&move);
            printf("Position Evaluated %d\n",get_position_evaluated());
            printf("Time : %0.3lf ms\n",elapsed);

        }

        //Black move
        if(is_checkmate(&board,-1)==1){
            printf("White Won\n");
            break;
        }else if(is_stalemate(&board,-1)){
            printf("Draw by Stalemate\n");
            break;
        }
        move.mv = 0;
        if(choice==1){
           start = clock();
            move = computer_move(&board,-1);
            end = clock();
            elapsed = (end-start)*1000/CLOCKS_PER_SEC;
            move_on_board(&board,&move);
            system("cls");
            display(&board);
            display_move(&move);
            printf("Position Evaluated %d\n",get_position_evaluated());
            printf("Time : %0.3lf ms\n",elapsed);
        }else if (choice==2){
            while(move.mv==0){
                move = user_input(&board,-1);
            }
            move_on_board(&board,&move);
            system("cls");
            display(&board);
        }    
    }
    display(&board);
    return 0;
}