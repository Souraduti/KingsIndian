#ifndef MAKE_UNMAKE
#define MAKE_UNMAKE

#include "board.c"
#include "move.c"

/*
    makes a move on board . moves the appropiate pieces to proper squares
*/
void move_on_board(Board * board,Move* move){

    int8 p = get_piece(move);
    int8 dest = get_destination(move); 
    int8 src = get_source(move);
    int turn = get_turn(move);

    board->brd[dest] = p;
    board->brd[src] = 0;
    
    store_board_flag(move,board);

    //castling
    if(p==turn*6&&is_castling(move,1)==1){
        board->brd[dest-1] = 4*turn; 
        board->brd[dest+1] = 0;
    }else if(p==turn*6&&is_castling(move,0)==1){
        board->brd[dest+1] = 4*turn; 
        board->brd[dest-2] = 0;
    }

    //promotion
    int prom = get_promotion(move);
    if(prom!=0&&p==turn){
        switch(prom){
            //Queen
            case 1:board->brd[dest] = turn*5;break;
            //Knight
            case 2:board->brd[dest] = turn*2;break;
            //Rook
            case 4:board->brd[dest] = turn*4;break;
            //Bishop
            case 8:board->brd[dest] = turn*3;break;
        }
    }
    //updating flags if pwan moves 2 square
    set_pawn_jump(board,-1,1);
    if(p==1&&(dest-src)==16){
        set_pawn_jump(board,src&7,1);
    }else if(p==-1&&(src-dest)==16){
        set_pawn_jump(board,src&7,-1);
    }

    //En-passant
    if(is_enpassant(move)==1){
        if((dest-src)==9||(dest-src)==-7){
            board->brd[src+1] = 0;
        }else if((dest-src)==-9||(dest-src)==7){
            board->brd[src-1] = 0;
        } 
    } 

    //updating king position and castling flag
    if(p==6||p==-6){
        set_king_pos(board,turn,dest);
        set_castling_right(board,turn,1,0);
        set_castling_right(board,turn,0,0);
    }
    if((p==4&&src==7)||(p==-4&&src==63)){
        set_castling_right(board,turn,1,0);
    }
    if((p==4&&src==0)||(p==-4&&src==56)){
        set_castling_right(board,turn,0,0);
    }

}

/*
    Undo the effects of move_on_board function 
*/
void unmove_on_board(Board * board,Move* move){
    int8 p = get_piece(move);
    int8 dest = get_destination(move); 
    int8 src = get_source(move);
    int turn = get_turn(move);

    board->brd[src] = p;
    board->brd[dest] = get_captured_piece(move);

    restore_board_flag(move,board);

    //castling
    if(p==6*turn&&is_castling(move,1)==1){
        board->brd[dest-1] = 0; 
        board->brd[dest+1] = 4*turn;
    }else if(p==6*turn&&is_castling(move,0)==1){
        board->brd[dest-2] = 4*turn;
        board->brd[dest+1] = 0;
    }

    //promotion
    int prom = get_promotion(move);
    if(prom!=0&&p==turn){
        board->brd[src] = turn;
    }

    //En-passant
    if(is_enpassant(move)==1){
        if((dest-src)==9||(dest-src)==-7){
            board->brd[src+1] = -p;
        }else if((dest-src)==-9||(dest-src)==7){
            board->brd[src-1] = -p;
        } 
    }
    //updating king position
    if(p==6*turn){
        set_king_pos(board,turn,src);
    }

}

#endif
