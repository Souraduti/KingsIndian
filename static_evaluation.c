#include "static_evaluation.h"

/*Activity tables for pieces */
int _king[8][8] 
    = {
        {-30,-40,-40,-50,-50,-40,-40,-30},
        {-30,-40,-40,-50,-50,-40,-40,-30},
        {-30,-40,-40,-50,-50,-40,-40,-30},
        {-30,-40,-40,-50,-50,-40,-40,-30},
        {-20,-30,-30,-40,-40,-30,-30,-20},
        {-10,-20,-20,-20,-20,-20,-20,-10},
        { 20, 20,  0,  0,  0,  0, 20, 20},
        { 20, 30, 10,  0,  0, 10, 30, 20}
    };
int _king_endgame[8][8] 
    = {
        {-30,-10,-10,-10,-10,-10,-10,-30},
        {-10, 20, 30, 30, 30, 30, 20,-10},
        {-10, 40, 60, 60, 60, 60, 40,-10},
        {-10, 20, 40, 50, 50, 40, 20,-10},
        {-10, 20, 40, 40, 40, 40, 20,-10},
        {-10, 20, 20, 20, 20, 20, 20,-10},
        {-20,-10,  0,  0,  0,  0,-10,-20},
        {-30,-20,-10,-10,-10,-10,-20,-30}
    };
const int _queen[8][8] 
    = {
        {-20,-10,-10, -5, -5,-10,-10,-20},
        {-10,  0,  0,  0,  0,  0,  0,-10},
        {-10,  0,  5,  5,  5,  5,  0,-10},
        { -5,  0,  5,  5,  5,  5,  0, -5},
        {  0,  0,  5,  5,  5,  5,  0, -5},
        {-10,  5,  5,  5,  5,  5,  0,-10},
        {-10,  0,  5,  0,  0,  0,  0,-10},
        {-20,-10,-10, -5, -5,-10,-10,-20}
    };
const int _rook[8][8] 
    = {
        { 0,  0,  0,  0,  0,  0,  0,  0},
        { 5, 10, 10, 10, 10, 10, 10,  5},
        {-5,  0,  0,  0,  0,  0,  0, -5},
        {-5,  0,  0,  0,  0,  0,  0, -5},
        {-5,  0,  0,  0,  0,  0,  0, -5},
        {-5,  0,  0,  0,  0,  0,  0, -5},
        {-5,  0,  0,  0,  0,  0,  0, -5},
        { 0,  0,  5, 10, 10,  5,  0,  0}
    };
const int _bishop[8][8] 
    = {
        {-20,-10,-10,-10,-10,-10,-10,-20},
        {-10,  0,  0,  0,  0,  0,  0,-10},
        {-10,  0,  5, 10, 10,  5,  0,-10},
        {-10,  5,  5, 10, 10,  5,  5,-10},
        {-10,  0, 10, 10, 10, 10,  0,-10},
        {-10, 10, 10, 10, 10, 10, 10,-10},
        {-10,  5,  0,  0,  0,  0,  5,-10},
        {-20,-10,-10,-10,-10,-10,-10,-20}
    };
const int _night[8][8] 
    = {
        {-50,-40,-30,-30,-30,-30,-40,-50},
        {-40,-20,  0,  0,  0,  0,-20,-40},
        {-30,  0, 10, 15, 15, 10,  0,-30},
        {-30,  5, 15, 20, 20, 15,  5,-30},
        {-30,  0, 15, 20, 20, 15,  0,-30},
        {-30,  5, 10, 15, 15, 10,  5,-30},
        {-40,-20,  0,  5,  5,  0,-20,-40},
        {-50,-40,-30,-30,-30,-30,-40, -5}
    };
const int _pawn[8][8] 
    = {
        { 0,  0,  0,  0,  0,  0,  0,  0},
        {60, 50, 50, 50, 50, 50, 50, 60},
        {10, 10, 20, 30, 30, 20, 10, 10},
        { 5,  5, 10, 25, 25, 10,  5,  5},
        { 0,  0,  0, 20, 20,  0,  0,  0},
        { 5, -5,-10,  0,  0,-10, -5,  5},
        { 5, 10, 10,-20,-20, 10, 10,  5},
        { 0,  0,  0,  0,  0,  0,  0,  0}
    };

int get_piece_value(int8 p_code){
    int sign = 1,val;
    if(p_code<0){
        sign=-1;
        p_code = -p_code;
    }
    switch (p_code){
        case 0:val=0;break;
        case 1:val = 100;break;
        case 2:val = 320;break;
        case 3:val = 330;break;
        case 4:val = 500;break;
        case 5:val = 900;break;
        case 6:val = 100000;break;
    }
    return sign*val;
}
//if there is queens on the board it is not considered endgame
int is_endgame(const Board * board){
    int i,e = 0,q_flag = 0;
    for(i=0;i<64;i++){
        int p = board->brd[i];
        if(p<0) p=-p;
        if(p==6) continue;
        if(p==5) q_flag = 1;
        e+= get_piece_value(p);
    }
    if(q_flag==0&&e<3000) return 1;
    if(q_flag==1&&e<1600) return 1;
    return 0;
}

int piece_count(const Board * board){
    int i,e = 0;
    for(i=0;i<64;i++){
        int p = board->brd[i];
        if(p<0) p=-p;
        if(p==6) continue;
        e+= get_piece_value(p);
    }
    return e;
}
int activity(const Board * board,int endgame){
    int i,r,c,eval = 0;
    for(i=0;i<64;i++){
        int8 p = board->brd[i];
        if(p==0) continue;
        /* row is actully 7-r which is adjusted while accessing the tabe*/
        r = (i>>3); 
        c = i&7;
        if(p<0){
            switch(-p){
                case 1: eval-=_pawn[r][c];break;
                case 2: eval-=_night[r][c];break;
                case 3: eval-=_bishop[r][c];break;
                case 4: eval-=_rook[r][c];break;
                case 5: eval-=_queen[r][c];break;
                case 6: eval-=(endgame==0)?_king[r][c]:_king_endgame[r][c];break;
            }
        }else{
            switch(p){
                case 1: eval+=_pawn[7-r][c];break;
                case 2: eval+=_night[7-r][c];break;
                case 3: eval+=_bishop[7-r][c];break;
                case 4: eval+=_rook[7-r][c];break;
                case 5: eval+=_queen[7-r][c];break;
                case 6: eval+=(endgame==0)?_king[7-r][c]:_king_endgame[7-r][c];break;
            }
        }
    }
    return eval;
}


int material(const Board *board){
    int i,eval=0;
    for(i=0;i<64;i++){
        eval += get_piece_value(board->brd[i]);
    }
    return eval;
}

int static_eval(const Board * board){
    return material(board)+activity(board,is_endgame(board));
}
/*int get_position_evaluated(){
    int temp  = s;
    s = 0;
    return temp;
}*/

