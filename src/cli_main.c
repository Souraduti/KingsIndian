#include <stdio.h>
#include <time.h>

#include "board.h"
#include "move.h"
#include "take_input.h"
#include "chess_rule.h"
#include "search.h"


/*
    This function is for playing game on Console
 */
int main()
{

    int choice = 0,first_move=1,count=0;
    time_t t,start,end;
    double elapsed,total=0;
    srand((unsigned)time(&t));
    Board board;
    Move move;
    Game_End_State game_state = ON;

    set_board(&board);
    display(&board);
    do{
        printf("Play as \n1.White\n2.Black\n3.Random\n");
        fflush(stdin);
        scanf("%d",&choice);
    }while(choice!=1&&choice!=2&&choice!=3);
    if(choice==3){
        choice = rand()%2+1;
    }
    Turn player = choice==1?White:Black;

    while (game_state==0)
    { 
        Move move;
        move.mv = 0;
        int depth;
        if(board.move_number<=10){
            depth = 3;
        }else if(board.move_number<=60){
            depth = 4;
        }else{
            depth = 5;
        }
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
        system("cls");
        display(&board);
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
    printf("Avarage time spend by Computer  : %lf\n",total/count);
    fflush(stdin);
    getchar();
    return 0;
}
