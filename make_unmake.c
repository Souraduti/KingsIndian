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
    Turn turn = get_turn(move);

    board->brd[dest] = p;
    board->brd[src] = 0;
    
    store_board_flag(move,board);

    //castling
    if(p==turn*King && is_castling(move,1)==1){
        board->brd[dest-1] = turn*Rook; 
        board->brd[dest+1] = 0;
    }else if(p==turn*King && is_castling(move,0)==1){
        board->brd[dest+1] = turn*Rook; 
        board->brd[dest-2] = 0;
    }

    //promotion
    int prom = get_promotion(move);
    if(prom!=0&&p==turn){
        switch(prom){
            //Queen
            case 1:board->brd[dest] = turn*Queen;break;
            //Knight
            case 2:board->brd[dest] = turn*Night;break;
            //Rook
            case 4:board->brd[dest] = turn*Rook;break;
            //Bishop
            case 8:board->brd[dest] = turn*Bishop;break;
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
    if(p==King||p==-King){
        set_king_pos(board,turn,dest);
        set_castling_right(board,turn,Short,0);
        set_castling_right(board,turn,Long,0);
    }
    if((p==4&&src==7)||(p==-4&&src==63)){
        set_castling_right(board,turn,Short,0);
    }
    if((p==4&&src==0)||(p==-4&&src==56)){
        set_castling_right(board,turn,Long,0);
    }

}

/*
    Undo the effects of move_on_board function 
*/
void unmove_on_board(Board * board,Move* move){
    int8 p = get_piece(move);
    int8 dest = get_destination(move); 
    int8 src = get_source(move);
    Turn turn = get_turn(move);

    board->brd[src] = p;
    board->brd[dest] = get_captured_piece(move);

    restore_board_flag(move,board);

    //castling
    if(p==King*turn&&is_castling(move,Short)==1){
        board->brd[dest-1] = 0; 
        board->brd[dest+1] = Rook*turn;
    }else if(p==King*turn&&is_castling(move,Long)==1){
        board->brd[dest-2] = Rook*turn;
        board->brd[dest+1] = 0;
    }

    //promotion
    int prom = get_promotion(move);
    if(prom!=0&&p==turn){
        board->brd[src] = turn*Pawn;
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
    if(p==King*turn){
        set_king_pos(board,turn,src);
    }

}

#endif
