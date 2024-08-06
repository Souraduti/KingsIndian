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

int main()
{

    int choice = 0,first_move=0,count=0;
    time_t t,start,end;
    double elapsed,total=0;
    srand((unsigned)time(&t));
    Board board;
    Move move;
    Movelist moves;

    set_board_from2D(&board);
    display(&board);
    //generate_all(&board,&moves,Black,1);
    //show_all_moves(&moves);
    move = computer_move(&board,White);
    move_on_board(&board,&move);
    display(&board);
    display_move(&move);
    generate_all(&board,&moves,Black,0);
    for(int i=0;i<moves.size;i++){
        display_move(&moves.list[i]);
        show_bits(moves.list[i].mv);
    }
    printf("Enter Your move");
    char input[8];
    scanf("%s",input);
    move = user_input_from_GUI(&board,Black,input);
    move_on_board(&board,&move);
    display(&board);
    display_move(&move);
    /*do{
        printf("Play as \n1.White\n2.Black\n3.Random\n");
        fflush(stdin);
        scanf("%d",&choice);
    }while(choice!=1&&choice!=2&&choice!=3);
    if(choice==3){
        choice = rand()%2+1;
    }*/
    choice = 1;

    /*while (1)
    {
        //white move 
        if(is_checkmate(&board,White)==1){
            printf("Black Won\n");
            break;
        }else if(is_stalemate(&board,White)){
            printf("Draw by Stalemate\n");
            break;
        }
        move.mv = 0;
        if(choice==1){
            while(move.mv==0){
                move = user_input(&board,White);
            }
            move_on_board(&board,&move);
            //system("cls");
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
            //system("cls");
            display(&board);
            display_move(&move);
            //printf("Position Evaluated %d\n",get_position_evaluated());
            printf("Time : %0.3lf ms\n",elapsed);
        }
        show_bits(move.mv);
        //Black move
        if(is_checkmate(&board,Black)==1){
            printf("White Won\n");
            break;
        }else if(is_stalemate(&board,Black)){
            printf("Draw by Stalemate\n");
            break;
        }
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
            //system("cls");
            display(&board);
            display_move(&move);
            //printf("Position Evaluated %d\n",get_position_evaluated());
            printf("Time : %0.3lf ms\n",elapsed);
        }else if (choice==2){
            while(move.mv==0){
                move = user_input(&board,Black);
            }
            move_on_board(&board,&move);
            //system("cls");
            display(&board);
        } 
        show_bits(move.mv);
		count++;   
    }*/
    
    display(&board);
    return 0;
}

void set_board_from2D(Board * board){
    char b[8][8] = {
                    {'.','.','.','.','.','.','K','.'},
                    {'.','p','.','r','.','.','.','.'},
                    {'p','.','.','.','.','.','n','.'},
                    {'P','r','k','.','.','.','.','n'},
                    {'.','.','p','.','.','.','.','.'},
                    {'.','.','.','.','.','.','.','.'},
                    {'.','P','.','.','.','.','.','.'},
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
