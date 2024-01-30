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
    board->brd[dest] = p;
    board->brd[src] = 0;
    
    if(p==6){
        set_king_pos(board,1,dest);
        //updating castling flags for king move
        if((board->flag&(1<<7))!=0){
            move->mv|=(1<<26);
            board->flag = ~((~board->flag)|(1<<7));
        }
        if((board->flag&(1<<6))!=0){
            move->mv|=(1<<27);
            board->flag = ~((~board->flag)|(1<<6));
        }     
    }else if(p==-6){
        set_king_pos(board,-1,dest);
        if((board->flag&(1<<5))!=0){
            move->mv|=(1<<26);
            board->flag = ~((~board->flag)|(1<<5));
        }
        if((board->flag&(1<<4))!=0){
            move->mv|=(1<<27);
            board->flag = ~((~board->flag)|(1<<4));
        }
    }

    //updating castling flag for rook move
    if(p==4&&src==7){
        if((board->flag&(1<<7))!=0){
            move->mv|=(1<<26);
            board->flag = ~((~board->flag)|(1<<7));
        }
    }else if(p==-4&&src==63){
        if((board->flag&(1<<5))!=0){
            move->mv|=(1<<26);
            board->flag = ~((~board->flag)|(1<<5));
        }
    }else if(p==4&&src==0){
        if((board->flag&(1<<6))!=0){
            move->mv|=(1<<27);
            board->flag = ~((~board->flag)|(1<<6));
        }
    }else if(p==-4&&src==56){
        if((board->flag&(1<<4))!=0){
            move->mv|=(1<<27);
            board->flag = ~((~board->flag)|(1<<4));
        }
    }

    //castling
    if((move->mv&(1<<20))!=0){
        board->brd[dest-1] = p*2/3; //(6*2/3 == 4)
        board->brd[dest+1] = 0;
    }else if((move->mv&(1<<21))!=0){
        board->brd[dest+1] = p*2/3; //(6*2/3 == 4)
        board->brd[dest-2] = 0;
    }

    //cheking for pwan promotion
    //shifting mv 22 times gives the promotion flags,15 = 1111
    int prom = (move->mv>>22)&15;
    if(prom!=0){
        int sign =(p>0)?1:-1;
        switch(prom){
            //Queen
            case 1:board->brd[dest] = sign*5;break;
            //Knight
            case 2:board->brd[dest] = sign*2;break;
            //Rook
            case 4:board->brd[dest] = sign*4;break;
            //Bishop
            case 8:board->brd[dest] = sign*3;break;
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
    if((move->mv&(1<<28))!=0&&(p==1||p==-1)){
        if((dest-src)==9||(dest-src)==-7){
            board->brd[src+1] = 0;
        }else if((dest-src)==-9||(dest-src)==7){
            board->brd[src-1] = 0;
        } 
    } 
}

/*
    Undo the effects of move_on_board function 
    except for en-passant flags
*/
void unmove_on_board(Board * board,Move* move){
    int8 p = get_piece(move);
    int8 dest = get_destination(move); 
    int8 src = get_source(move);

    board->brd[src] = p;
    board->brd[dest] = get_captured_piece(move);

    if(p==6){
        set_king_pos(board,1,src);
        if((board->flag&(1<<7))==0&&(move->mv&(1<<26))!=0){
            board->flag|=(1<<7);
            move->mv = ~((~move->mv)|(1<<26));
        }
        if((board->flag&(1<<6))==0&&(move->mv&(1<<27))!=0){
            board->flag|=(1<<6);
            move->mv = ~((~move->mv)|(1<<27));
        }
        
    }else if(p==-6){
        set_king_pos(board,-1,src);
        if((board->flag&(1<<5))==0&&(move->mv&(1<<26))!=0){
            board->flag|=(1<<5);
            move->mv = ~((~move->mv)|(1<<26));
        }
        if((board->flag&(1<<4))==0&&(move->mv&(1<<27))!=0){
            board->flag|=(1<<4);
            move->mv = ~((~move->mv)|(1<<27));
        }
    }

    //updating castling flag for rook move
    if(p==4&&src==7){
        if((board->flag&(1<<7))==0&&(move->mv&(1<<26))!=0){
            board->flag|=(1<<7);
            move->mv = ~((~move->mv)|(1<<26));
        }
    }else if(p==-4&&src==63){
        if((board->flag&(1<<5))==0&&(move->mv&(1<<26))!=0){
            board->flag|=(1<<5);
            move->mv = ~((~move->mv)|(1<<26));
        }
    }else if(p==4&&src==0){
        if((board->flag&(1<<6))==0&&(move->mv&(1<<27))!=0){
            board->flag|=(1<<6);
            move->mv = ~((~move->mv)|(1<<27));
        }
    }else if(p==-4&&src==56){
        if((board->flag&(1<<4))==0&&(move->mv&(1<<27))!=0){
            board->flag|=(1<<4);
            move->mv = ~((~move->mv)|(1<<27));
        }
    }

    //castling
    if((move->mv&(1<<20))!=0){
        board->brd[dest-1] = 0; //(6*2/3 == 4)
        board->brd[dest+1] = p*2/3;
    }else if((move->mv&(1<<21))!=0){
        board->brd[dest-2] = p*2/3; //(6*2/3 == 4)
        board->brd[dest+1] = 0;
    }
    //cheking for pwan promotion
    //shifting mv 22 times gives the promotion flags,15 = 1111
    int prom = (move->mv>>22)&15;
    if(prom!=0){
        int sign =(p>0)?1:-1;
        board->brd[src] = sign;
    }

    //En-passant
    if((move->mv&(1<<28))!=0){
        if((dest-src)==9||(dest-src)==-7){
            board->brd[src+1] = -p;
        }else if((dest-src)==-9||(dest-src)==7){
            board->brd[src-1] = -p;
        } 
    } 
}

#endif
