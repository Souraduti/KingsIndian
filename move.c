
#include "move.h"

int add_move(Movelist * movelist,Move move){
    if(movelist->size==LIST_LENGTH-1){
        return -1;
    }
    movelist->list[movelist->size].mv = move.mv;
    movelist->size+=1;
    return movelist->size;
}

void clear_movelist(Movelist * movelist){
    int i;
    for(i = 0;i<LIST_LENGTH;i++){
        movelist->list[i].mv = 0;
    }
    movelist->size = 0;
}

int8 get_source(const Move * move){
    int flag = 0b00000000000000000000000000111111;
    int off = 0;
    int8 sq_code = (move->mv&flag)>>off;
    return sq_code;
}

int8 get_destination(const Move * move){
    int flag = 0b00000000000000000000111111000000;
    int off = 6;
    int8 sq_code = (move->mv&flag)>>off;
    return sq_code;
}

int8 get_piece(const Move * move){
    int flag = 0b00000000000000001111000000000000;
    int off = 12;
    int8 p_code = (move->mv&flag)>>off;
    if(p_code&(1<<3)) p_code =  p_code-16;
    return p_code;
}
int8 get_captured_piece(const Move * move){
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

Turn get_turn(const Move * move){
    return ((move->mv&(1<<15))==0)?White:Black;
}
int is_castling(const Move * move,Castling_side side){
    int off = 1-side+20;
    return ((move->mv&(1<<off))==0)?0:1;
}
void set_castling(Move * move,Castling_side side){
    int off = 1-side+20;
    move->mv = move->mv|(1<<off);
}
void clear_castling(Move * move,Castling_side side){
    int off = 1-side+20;
    move->mv = ~(~(move->mv)|(1<<off));
}
int get_promotion(const Move * move){
    return (move->mv>>20)&15;
}
int clear_promotion(Move * move){
    move->mv = ~(~(move->mv)|(15<<20));
}
void set_promotion(Move * move,int p){
    if(p!=1&&p!=2&p!=4&p!=8) return;
    move->mv = ~(~(move->mv)|(15<<20));
    move->mv|=((p&15)<<20);
}
int is_enpassant(const Move * move){
    return (move->mv&(1<<24))==0?0:1;
}
void set_enpassant(Move * move){
    move->mv|=(1<<24);
}
void clear_enpassant(Move * move){
    move->mv = ~(~(move->mv)|(1<<24));
}
void store_board_flag(Move * move,Board * board){
    Turn turn = get_turn(move);
    if(get_castling_right(board,turn,Short)==1){
        //short
        move->mv|=(1<<25);
    }
    if(get_castling_right(board,turn,Long)==1){
        //long
        move->mv|=(1<<26);
    }
    int jump = get_pawn_jump(board,0);
    if(jump!=-1&&jump<=15){
        move->mv = move->mv|(jump<<27);
        move->mv = move->mv|(1<<31);
    }
}
void restore_board_flag(Move * move,Board * board){
    Turn turn = get_turn(move);
    int file = (move->mv>>27)&15;
    if((move->mv&(1<<31))==0){
        set_pawn_jump(board,-1,turn);
    }else{
        set_pawn_jump(board,file,0);
    }

    set_castling_right(board,turn,Short,(move->mv>>25)&1);
    set_castling_right(board,turn,Long,(move->mv>>26)&1);

    move->mv = move->mv&((1<<25)-1);
}

void display_move(const Move * move){
    
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
    printf("\n");
}
void show_all_moves(const Movelist *moves){
    int i;
    for(i=0;i<moves->size;i++){
        display_move(&(moves->list[i]));
    }
}
int eval_cmp(Move * m1,Move * m2){
    // it is assumed that both move will be from same side
    if(get_turn(m1)==White){
        return m1->eval>=m2->eval?1:0;
    }else{
        return m1->eval<=m2->eval?1:0;
    }
}

void sort_moves_by_eval(Movelist * movelist){
    int i,j;
    Move key;
    for (i = 1; i < movelist->size; i++) {
        key = movelist->list[i];
        j = i - 1;
        while (j >= 0 && eval_cmp(&movelist->list[j],&key)==0) {
            movelist->list[j + 1] = movelist->list[j];
            j = j - 1;
        }
        movelist->list[j + 1] = key;
    }
}

void remove_illegal_moves(Movelist * moves){
    int i,j=0;
    for(i=0;i<moves->size;i++){
        if(moves->list[i].mv==0) continue;
        if(i!=j) moves->list[j] = moves->list[i];
        j++;
    }
    moves->size=j;
}