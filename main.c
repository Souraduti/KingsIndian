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

    while (game_state==ON)
    {
        //white move 
        move.mv = 0;
        if(choice==1){
            while(move.mv==0){
                move = user_input(&board,White);
            }
            move_on_board(&board,&move);
            system("cls");
            display(&board);
        }else if(choice==2){
            start = clock();
            if(first_move==1){
                move = random_move(&board,White);
                first_move = 0;
            }else{
                move = computer_move(&board,White);
            }
            end = clock();
            elapsed = (end-start)*1000/CLOCKS_PER_SEC;
            total+=elapsed;
            move_on_board(&board,&move);
            system("cls");
            display(&board);
            display_move(&move);
            printf("Time : %0.3lf ms\n",elapsed);
        }
        game_state = get_game_state(&board,Black);
        if(game_state!=ON) break;
        //Black move
        move.mv = 0;
        if(choice==1){
            start = clock();
            if(first_move==1){
                move = random_move(&board,Black);
                first_move = 0;
            }else{
                move = computer_move(&board,Black);
            }
            end = clock();
            elapsed = (end-start)*1000/CLOCKS_PER_SEC;
            total+=elapsed;
            move_on_board(&board,&move);
            system("cls");
            display(&board);
            display_move(&move);
            printf("Time : %0.3lf ms\n",elapsed);
        }else if (choice==2){
            while(move.mv==0){
                move = user_input(&board,Black);
            }
            move_on_board(&board,&move);
            system("cls");
            display(&board);
        }
        game_state = get_game_state(&board,White);
        if(game_state!=ON) break;  
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
