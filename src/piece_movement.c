#include "piece_movement.h"

/*
    logic for movement of all the pieces 
*/

int in_board(int8 sq,int dir,int step){
    if(sq<0||sq>=64) return 0;
    int r,c,i,j,sign=1;
    /*Anding with 7 accepts only the lowest 3 bits*/
    r = (sq>>3)&7;
    c = sq&7;
    
    if(dir<0){
        sign=-1;
        dir = -dir;
    }
    switch(dir){
        case 1:i=0;j=1;break;
        case 8:i=1;j=0;break;
        case 7:i=1;j=-1;break;
        case 9:i=1;j=1;break;
        case 6:i=1;j=-2;break;
        case 10:i=1;j=2;break;
        case 15:i=2;j=-1;break;
        case 17:i=2;j=1;break;
    }
    if(sign==-1){
        i = -i;
        j = -j; 
    }
    r+=i*step;
    c+=j*step;

    if(c<0||c>=8||r<0||r>=8) return 0;

    return 1;
}
int is_opponent(int8 p,int8 q){
    return (p*q<0)?1:0;
}
int is_sameside(int8 p,int8 q){
    return (p*q>0)?1:0;
}
int promotion(Movelist* movelist,Move * move){
    int8 p = get_piece(move);
    int8 dest = get_destination(move);
    if(p!=1&&p!=-1) return 0;
    if(p==1&&(dest>>3)!=7) return 0;
    if(p==-1&&(dest>>3)!=0) return 0;
    int i;
    for(i=0;i<=3;i++){
        set_promotion(move,(1<<i));
        add_move(movelist,*move); 
    }
    clear_promotion(move);
    return 1;
}
void enpassant(const Board * board,Movelist * movelist,const int8 sq){
    int c,side,rank,src_offset,dst_offset;
    Move move;
    move.mv = 0;
    if(board->brd[sq]==Pawn){
        side = -1;
        rank = 4;
        src_offset = 32;
        dst_offset = 40;
    }else{
        side = 1;
        rank = 3;
        src_offset = 24;
        dst_offset = 16;
    }
    c = get_pawn_jump(board,side);
    if(c==-1||(sq>>3)!=rank) return;
    if((src_offset+c-sq)==1||(src_offset+c-sq)==-1){
        set_piece(&move,-side*Pawn);
        set_source(&move,sq);
        set_destination(&move,dst_offset+c);
        /*Captured piece is not set as it would complecate unmaking move*/
        set_enpassant(&move);
        add_move(movelist,move);
    }

}
void pawn(const Board * board,Movelist * movelist,const int8 sq){
    Move move;
    move.mv = 0;
    int8 p = board->brd[sq];
    int8 dest;
    if(p!=Pawn&&p!=-Pawn) return;
    set_piece(&move,p);
    set_source(&move,sq);
    if(p==1){
        //white pawn
        // 1 square forward
        if(in_board(sq,8,1)&&board->brd[sq+8]==0){
            set_destination(&move,sq+8);
            set_captured_piece(&move,0);
            if(promotion(movelist,&move)==0){
                add_move(movelist,move);
            }
        }

        // 2 square forward
        if((sq/8==1)&&board->brd[sq+8]==0&&board->brd[sq+16]==0){
            set_destination(&move,sq+16);
            set_captured_piece(&move,0);
            add_move(movelist,move);
        }

        if(in_board(sq,7,1)&&is_opponent(1,board->brd[sq+7])==1){
            set_destination(&move,sq+7);
            set_captured_piece(&move,board->brd[sq+7]);
            if(promotion(movelist,&move)==0){
                add_move(movelist,move);
            }
        }
        if(in_board(sq,9,1)&&is_opponent(1,board->brd[sq+9])==1){
            set_destination(&move,sq+9);
            set_captured_piece(&move,board->brd[sq+9]);
            if(promotion(movelist,&move)==0){
                add_move(movelist,move);
            }
        }
        enpassant(board,movelist,sq);
    }else{
        //black pawn
        // 1 square forward
        if(in_board(sq,-8,1)&&board->brd[sq-8]==0){
            set_destination(&move,sq-8);
            set_captured_piece(&move,0);
            if(promotion(movelist,&move)==0){
                add_move(movelist,move);
            }
        }

        // 2 square forward
        if((sq/8==6)&&board->brd[sq-8]==0&&board->brd[sq-16]==0){
            set_destination(&move,sq-16);
            set_captured_piece(&move,0);
            add_move(movelist,move);
        }

        if(in_board(sq,-7,1)&&is_opponent(-1,board->brd[sq-7])==1){
            set_destination(&move,sq-7);
            set_captured_piece(&move,board->brd[sq-7]);
            if(promotion(movelist,&move)==0){
                add_move(movelist,move);
            }
        }
        if(in_board(sq,-9,1)&&is_opponent(-1,board->brd[sq-9])==1){
            set_destination(&move,sq-9);
            set_captured_piece(&move,board->brd[sq-9]);
            if(promotion(movelist,&move)==0){
                add_move(movelist,move);
            }
        }
        enpassant(board,movelist,sq);
    }
}
void night(const Board * board,Movelist * movelist,const int8 sq){
    const int8 offset[8] = {6,-6,15,-15,10,-10,17,-17};
    int i;
    Move move;
    move.mv = 0;
    int8 p = board->brd[sq];
    int8 dest;
    if(p!=Night&&p!=-Night) return;
    set_piece(&move,p);
    set_source(&move,sq);
    for(i=0;i<8;i++){
        dest = sq+offset[i];
        if(in_board(sq,offset[i],1)==0||is_sameside(p,board->brd[dest])==1) continue;
        set_destination(&move,dest);
        set_captured_piece(&move,board->brd[dest]);
        add_move(movelist,move);     
    } 
}
void bishop(const Board * board,Movelist * movelist,const int8 sq){
    const int8 offset[4] = {7,-7,9,-9};
    int i,j;
    Move move;
    move.mv = 0; 
    int8 p = board->brd[sq];
    int8 dest;
    if(p!=Bishop&&p!=-Bishop) return;
    set_piece(&move,p);
    set_source(&move,sq);
    for(i=0;i<4;i++){
        j=1;
        while(in_board(sq,offset[i],j)==1){
            dest = sq+j*offset[i];
            if(is_sameside(p,board->brd[dest])==1) break;
            set_destination(&move,dest);
            set_captured_piece(&move,board->brd[dest]); 
            add_move(movelist,move); 
            if(is_opponent(p,board->brd[dest])==1){
                break;             
            }
            j++;
        }
    }
}
void rook(const Board * board,Movelist * movelist,const int8 sq){
    const int8 offset[4] = {1,-1,8,-8};
    int i,j;
    Move move;
    move.mv = 0; 
    int8 p = board->brd[sq];
    int8 dest;
    if(p!=Rook&&p!=-Rook) return;
    set_piece(&move,p);
    set_source(&move,sq);
    for(i=0;i<4;i++){
        j=1;
        while(in_board(sq,offset[i],j)==1){
            dest = sq+j*offset[i];
            if(is_sameside(p,board->brd[dest])==1) break;
            set_captured_piece(&move,board->brd[dest]);                 
            set_destination(&move,dest);
            add_move(movelist,move); 
            if(is_opponent(p,board->brd[dest])==1){
                break;             
            }
            j++;
        }
    }
}
void queen(const Board * board,Movelist * movelist,int8 sq){
    const int8 offset[8] = {1,-1,8,-8,7,-7,9,-9};
    int i,j;
    Move move;
    move.mv = 0;
    int8 p = board->brd[sq];
    int8 dest;
    if(p!=Queen&&p!=-Queen) return;
    set_piece(&move,p);
    set_source(&move,sq);
    for(i=0;i<8;i++){
        j=1;
        while(in_board(sq,offset[i],j)==1){
            dest = sq+j*offset[i];
            if(is_sameside(p,board->brd[dest])==1) break;
            set_captured_piece(&move,board->brd[dest]);                 
            set_destination(&move,dest);
            add_move(movelist,move); 
            if(is_opponent(p,board->brd[dest])==1){
                break;             
            }
            j++;
        }
    }
}
void castling(const Board * board,Movelist * movelist,Turn turn){
    int k = (turn==White)?4:60;
    Move m;
    //short
    if(get_castling_right(board,turn,Short)==1){
        if(board->brd[k]!=turn*King) goto out1;
        if(board->brd[k+1]!=0) goto out1;
        if(board->brd[k+2]!=0) goto out1;
        if(board->brd[k+3]!=turn*Rook) goto out1;
        if(is_opponent_controls(board,k,turn)==1) goto out1;
        if(is_opponent_controls(board,k+1,turn)==1) goto out1;
        if(is_opponent_controls(board,k+2,turn)==1) goto out1;
        m.mv = 0;
        set_piece(&m,turn*King);
        set_source(&m,k);
        set_destination(&m,k+2);
        set_castling(&m,Short);
        add_move(movelist,m);
    }
    out1:
    //long
    if(get_castling_right(board,turn,Long)==1){
        if(board->brd[k]!=turn*King) goto out2;
        if(board->brd[k-1]!=Empty) goto out2;
        if(board->brd[k-2]!=Empty) goto out2;
        if(board->brd[k-3]!=Empty) goto out2;
        if(board->brd[k-4]!=turn*Rook) goto out2;
        if(is_opponent_controls(board,k,turn)==1) goto out2;
        if(is_opponent_controls(board,k-1,turn)==1) goto out2;
        if(is_opponent_controls(board,k-2,turn)==1) goto out2;
        m.mv = 0;
        set_piece(&m,turn*King);
        set_source(&m,k);
        set_destination(&m,k-2);
        set_castling(&m,Long);
        add_move(movelist,m);
    }
    out2:
    return;
}
void king(const Board * board,Movelist * movelist,int8 sq){
    const int8 offset[8] = {1,-1,8,-8,7,-7,9,-9};
    int i;
    Move move;
    move.mv = 0;
    int8 p = board->brd[sq];
    int8 dest;
    if(p!=King&&p!=-King) return;
    set_piece(&move,p);
    set_source(&move,sq);
    for(i=0;i<8;i++){
        dest = sq+offset[i];
        if(in_board(sq,offset[i],1)==0||is_sameside(p,board->brd[dest])==1) continue;
        set_destination(&move,dest);
        set_captured_piece(&move,board->brd[dest]);
        add_move(movelist,move);     
    }
    castling(board,movelist,p/King);
}

int is_opponent_controls(const Board * board,int8 sq,Turn turn){
    int kn_off[8] = {6,-6,15,-15,10,-10,17,-17};
    int offset[8] = {1,-1,8,-8,7,-7,9,-9};
    int i;
    for(i=0;i<8;i++){
        // for night 
        if(in_board(sq,kn_off[i],1)==1&&board->brd[sq+kn_off[i]]==-turn*Night) return 1;
        //for king
        if(in_board(sq,offset[i],1)==1&&board->brd[sq+offset[i]]==-turn*King) return 1;
    }

    //pawns
    if(turn==White){
       if(in_board(sq,9,1)==1&&board->brd[sq+9]==-Pawn) return 1; 
       if(in_board(sq,7,1)==1&&board->brd[sq+7]==-Pawn) return 1; 
    }else{
       if(in_board(sq,-9,1)==1&&board->brd[sq-9]==Pawn) return 1; 
       if(in_board(sq,-7,1)==1&&board->brd[sq-7]==Pawn) return 1; 
    }
    for(i=0;i<8;i++){
        int j = 1;
        while(in_board(sq,offset[i],j)==1){
            int8 dest = sq+j*offset[i];
            if(is_sameside(turn,board->brd[dest])==1) break;
            if(is_opponent(turn,board->brd[dest])==1){
                //queen
                if(board->brd[dest]==-turn*Queen) return 1;
                //rook
                if(board->brd[dest]==-turn*Rook && i<4) return 1;
                //bishop
                if(board->brd[dest]==-turn*Bishop && i>=4) return 1;
                //any other piece can not attack
                break;
            }
            j++;
        }
    }
    return 0;
}

void generate_move(const Board * board,Movelist * movelist,int8 sq,int8 piece){
    switch(piece){
        case Pawn   : pawn(board,movelist,sq);   break;
        case Night  : night(board,movelist,sq);  break;
        case Bishop : bishop(board,movelist,sq); break;
        case Rook   : rook(board,movelist,sq);   break;
        case Queen  : queen(board,movelist,sq);  break;
        case King   : king(board,movelist,sq);   break;
    }

}
