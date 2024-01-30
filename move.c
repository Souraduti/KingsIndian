#ifndef MOVE
#define MOVE

#include "board.c"

/*
    Move representation : 
    structure of mv
    00000000000 0000  0000   000000 000000
    |  flag |  |capt||piece| |dest| |source|

    flag ->   000   0          00       0000       00
                               LS       BRNQ       LS
               En-passant prev-castle promotion  castling
*/
typedef struct Move{
    int mv;
}Move;

typedef struct Movelist{
    Move list[128];
    int size;
}Movelist;

int add_move(Movelist * movelist,Move move){
    if(movelist->size==100){
        return -1;
    }
    movelist->list[movelist->size].mv = move.mv;
    movelist->size+=1;
    return movelist->size;
}

void clear_movelist(Movelist * movelist){
    int i = 0;
    for(i =0;i<100;i++){
        movelist->list[i].mv =0;
    }
    movelist->size = 0;
}

int8 get_source(Move * move){
    int flag = 0b00000000000000000000000000111111;
    int off = 0;
    int8 sq_code = (move->mv&flag)>>off;
    return sq_code;
}

int8 get_destination(Move * move){
    int flag = 0b00000000000000000000111111000000;
    int off = 6;
    int8 sq_code = (move->mv&flag)>>off;
    return sq_code;
}

int8 get_piece(Move * move){
    int flag = 0b00000000000000001111000000000000;
    int off = 12;
    int8 p_code = (move->mv&flag)>>off;
    if(p_code&(1<<3)) p_code =  p_code-16;
    return p_code;
}
int8 get_captured_piece(Move * move){
    int flag = 0b00000000000011110000000000000000;
    int off = 16;
    int8 p_code = (move->mv&flag)>>off;
    if(p_code&(1<<3)) p_code =  p_code-16;
    return p_code;
}

void set_source(Move * move,int8 source){
   int off = 0;
   int flag = 0b00000000000000000000000000111111;
   move->mv = (move->mv)&(~flag);
   move->mv = (move->mv)|((source<<off)&flag);
}

void set_destination(Move * move,int8 dest){
    int off = 6;
    int flag = 0b00000000000000000000111111000000;
    move->mv = (move->mv)&(~flag);
    move->mv = (move->mv)|((dest<<off)&flag);
}

void set_piece(Move * move,int8 piece){
    int off = 12;
    int flag = 0b00000000000000001111000000000000;
    move->mv = (move->mv)&(~flag);
    move->mv = (move->mv)|((piece<<off)&flag);
}

void set_captured_piece(Move * move,int8 piece){
    int off = 16;
    int flag = 0b00000000000011110000000000000000;
    move->mv = (move->mv)&(~flag);
    move->mv = (move->mv)|((piece<<off)&flag);
}

void display_move(Move * move){
    
    char p,cpt;
    int8 src,dst;
    int prom = 0;

    p = get_piece_from_code(get_piece(move));
    cpt = get_piece_from_code(get_captured_piece(move));
    src = get_source(move);
    dst = get_destination(move);

    char from[3],to[3];
    from[0] = src%8+'a';
    from[1] = src/8+'1';
    from[2] = '\0';
    to[0] = dst%8+'a';
    to[1] = dst/8+'1';
    to[2] = '\0';

    printf("%c from %s to %s ",p,from,to);
    if(cpt!='.'){
        printf("capturing %c ",cpt);
    }
    prom = (move->mv>>22)&15;
    if(prom!=0){
        printf(" promoting ");
        if(prom==1){
            printf("Queen");
        }else if(prom==2){
            printf("Night");
        }else if(prom==4){
            printf("Rook");
        }else if(prom==8){
            printf("Bishop");
        }
    }
    printf("\n");
}

#endif