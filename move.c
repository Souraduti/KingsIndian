
#include "move.h"
#include "zobrist.h"

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
int is_promotion(const Move * move){
    int8 p = get_piece(move);
    if(p!=1&&p!=-1) return 0;
    int v = get_promotion(move);
    return v==0?0:1;
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


/*
    the board flags indicating castling rights,pawn jumps
    are stored in move so that they can be retrived while unmaking the move
 */
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
/*
    returns the stored board flags to the board
*/
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
    if(is_promotion(move)){
        printf("promoting to ");
        switch(get_promotion(move)){
            case 1:printf("Queen");  break;
            case 2:printf("Knight"); break;
            case 4:printf("Rook");   break;
            case 8:printf("Bishop"); break;
        }
    }
    printf("\n");
}
void move_to_string(const  Move * move,char *str_move){
    char p;
    int8 src,dst;
    int prom = 0;
    p = get_piece_from_code(get_piece(move));
    p = 'A'<=p&&p<='Z'?p+'a'-'A':p;
    str_move[0] = p; 
    src = get_source(move);
    dst = get_destination(move);

    str_move[1] = src%8+'a';
    str_move[2] = src/8+'1';
    str_move[3] = dst%8+'a';
    str_move[4] = dst/8+'1';
    str_move[5]= ' ';
    if(p=='p'){
        switch(get_promotion(move)){
            case 1:str_move[5] = 'q';break;
            case 2:str_move[5] = 'n';break;
            case 4:str_move[5] = 'r';break;
            case 8:str_move[5] = 'b';break;
        }
    }
    
    if(is_enpassant(move)){
        str_move[5] = 'e';
    }
    str_move[6] = ' ';
    str_move[7] = '\n';
    
}
void show_all_moves(const Movelist *moves){
    int i;
    for(i=0;i<moves->size;i++){
        display_move(&(moves->list[i]));
    }
}




