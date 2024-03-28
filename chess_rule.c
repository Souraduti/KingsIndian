

#include "chess_rule.h"


int in_check(const Board * board,Turn turn){
    int8 sq = get_king_pos(board,turn);
    return is_opponent_controls(board,sq,turn);
}

/*
    checks if the move leaves king vulnarable to check.
    if a psudo-legal move is legal or not
*/
int is_playable(Board *board,Move * move,int turn){
    int ch = 1;
    move_on_board(board,move);
    if(in_check(board,turn)==1){
        ch = 0; 
    } 
    unmove_on_board(board,move);
    return ch;
}

int cmp(Move * m1,Move * m2){
    int8 c1,c2,p1,p2;
    c1  = get_captured_piece(m1);
    c2  = get_captured_piece(m2);
    if(c1<0) c1=-c1;
    if(c2<0) c2=-c2;
    if(c1>c2) return 1;
    if(c1<c2) return 0;
    if(c1==0&&c2==0) return rand()%2;
    p1 = get_piece(m1);
    p2 = get_piece(m2);
    if(p1<0) p1=-p1;
    if(p2<0) p2=-p2;
    if(p1>p2) return 0;
    if(p1<p2) return 1;
    return rand()%2;
}
void order_moves(Movelist * movelist){
    int i,j;
    Move key;
    for (i = 1; i < movelist->size; i++) {
        key = movelist->list[i];
        j = i - 1;
        while (j >= 0 && cmp(&movelist->list[j],&key)==0) {
            movelist->list[j + 1] = movelist->list[j];
            j = j - 1;
        }
        movelist->list[j + 1] = key;
    }
}

/*filters out illegal moves from pseudo-legal moves*/
void filter_move(Board *board,Movelist * all_move,int turn){
    int i,last;
    last = all_move->size-1;
    for(i=0;i<=last&&last>=0;i++){
        if(is_playable(board,&all_move->list[i],turn)==0){
            all_move->list[i] = all_move->list[last];
            i--;
            last--;
        }
    }
    all_move->size = last+1;
}
/*
    generate all possible moves in a position
    flag = 0 -> all pseudo-legal move
    flag = 1 -> only legal moves 
*/
Movelist generate_all(Board *board,Movelist * all_move,Turn turn,int filter_legal){
    all_move->size=0;
    int i;
    for(i=0;i<64;i++){
        generate_move(board,all_move,i,board->brd[i]*turn);
    }
    if(filter_legal==1) {
        filter_move(board,all_move,turn);
    }
    order_moves(all_move);
}

int is_checkmate(Board* board,Turn turn){
    if(in_check(board,turn)==0) return 0;
    Movelist moves;
    generate_all(board,&moves,turn,1);
    return (moves.size==0)?1:0;
}
int is_stalemate(Board * board,Turn turn){
    if(in_check(board,turn)==1) return 0;
    Movelist moves;
    generate_all(board,&moves,turn,1);
    return (moves.size==0)?1:0;
}

int is_gameover(Board * board,Turn turn){
    int cheked = in_check(board,turn);
    Movelist moves;
    generate_all(board,&moves,turn,1);
    if(moves.size!=0) return 0;
    //checkmate
    if(cheked==1) return -1; 
    //stalemate
    return 1; 
}
