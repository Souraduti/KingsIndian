
#include "chess_rule.h"


int in_check(const Board * board,Turn turn){
    int8 sq = get_king_pos(board,turn);
    return is_opponent_controls(board,sq,turn);
}

/*
    checks if the move leaves king vulnarable to check.
    if a psudo-legal move is legal or not
*/
int is_playable(Board *board,Move * move,Turn turn){
    int ch = 1;
    move_on_board(board,move);
    if(in_check(board,turn)==1){
        ch = 0; 
    } 
    unmove_on_board(board,move);
    return ch;
}

int mod(int a){
    return a<0?-a:a;
}
int priority(Move * m){
    int p = 0;
    if(is_enpassant(m)) p+=10;
    else if(is_promotion(m)==1){
        p+=10;
        switch(get_promotion(m)){
            case 1:p+=9;break;
            case 2:p+=3;break;
            case 4:p+=5;break;
            case 8:p+=4;break;
        }
    } 
    else if(is_castling(m,Short)==1||is_castling(m,Long)==1) p+=8;
    p += 2*mod(get_captured_piece(m)); 
    p -= mod(get_piece(m));
    return p;
}
int cmp(Move * m1,Move * m2){
    int p1 = priority(m1);
    int p2 = priority(m2);
    return p1>=p2?1:0;
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
void filter_move(Board *board,Movelist * all_move,Turn turn){
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
int is_insufficient(Board *board){
    int i;
    int minor_pieces[4] = {0,0,0,0};
    for(i=0;i<64;i++){
        /*If there is Pawn or Major Piecs on the board then it is not draw */
        if(board->brd[i]==Pawn||board->brd[i]==Rook||board->brd[i]==Queen) return 0;
        if(board->brd[i]==-Pawn||board->brd[i]==-Rook||board->brd[i]==-Queen) return 0;
        switch(board->brd[i]){
            case  Night  : minor_pieces[0]++;break;
            case -Night  : minor_pieces[1]++;break;
            case  Bishop : minor_pieces[2]++;break;
            case -Bishop : minor_pieces[3]++;break;
        }
    }
    // In case any side has 2 bishops
    if(minor_pieces[2]>=2||minor_pieces[3]>=2) return 0;
    if(minor_pieces[0]==0&&minor_pieces[2]==0){
        // White with lone king
        //2 or less nights and no bishop
        if(minor_pieces[1]<=2&&minor_pieces[3]==0) return 1;
        //total minor pieces at least 2 
        if((minor_pieces[1]+minor_pieces[3])>=2) return 0;
    }
    if(minor_pieces[1]==0&&minor_pieces[3]==0){
        // Black with lone king
        //2 or less nights and no bishop
        if(minor_pieces[0]<=2&&minor_pieces[2]==0) return 1;
        //total minor pieces at least 2 
        if((minor_pieces[0]+minor_pieces[2])>=2) return 0;
    }
    return 0;
}
Game_End_State get_game_state(Board * board,Turn turn){
    int cheked = in_check(board,turn);
    Movelist moves;
    generate_all(board,&moves,turn,1);
    if(moves.size==0&&cheked==0) return Draw_Stalemate;
    if(moves.size==0&&cheked==1) return turn==White?Black_Won:White_Won; 
    if(is_insufficient(board)==1) return Draw_Insufficient;
    return ON; 
}
