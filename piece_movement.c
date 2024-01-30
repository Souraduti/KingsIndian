#ifndef PIECE_MOVEMENT 
#define PIECE_MOVEMENT

#include "board.c"
#include "move.c"

/*
    logic for movement of all the pieces 
*/


int is_opponent_controls(Board * board,int8 sq,int turn);

int mod(int n){
    return (n>0)?n:-n;
}
int in_board(int8 sq,int dir,int step){
    if(sq<0||sq>=64) return 0;
    int r,c,i,j;
    /*
        56 = 00111000
        7  = 00000111
        using this two number the row and column can be calculated       
    */
    r = sq>>3;
    c = sq&7;
    switch(mod(dir)){
        case 1:i=0;j=1;break;
        case 8:i=1;j=0;break;
        case 7:i=1;j=-1;break;
        case 9:i=1;j=1;break;
        case 6:i=1;j=-2;break;
        case 10:i=1;j=2;break;
        case 15:i=2;j=-1;break;
        case 17:i=2;j=1;break;
    }
    if(dir<0){
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
    int i,m = move->mv;
    for(i=22;i<=25;i++){
        move->mv = m|(1<<i);
        add_move(movelist,*move); 
    }
    move->mv = ~((~move->mv)|(1<<25));
    return 1;
}
void pawn(Board * board,Movelist * movelist,const int8 sq){
    Move move;
    move.mv = 0;
    int8 p = 1;
    int8 dest;
    if(p!=board->brd[sq]) p=-p;
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
        set_captured_piece(&move,0);
        //En-passant
        int c = get_pawn_jump(board,1);
        if(c==-1||(sq>>3)!=4) return;
        if((32+c-sq)==1||(32+c-sq)==-1){
            set_destination(&move,40+c);
            move.mv|=(1<<28);
            add_move(movelist,move);
        }
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
        set_captured_piece(&move,0);
        //En-passant
        int c = get_pawn_jump(board,-1);
        if(c==-1||(sq>>3)!=3) return;
        if((24+c-sq)==1||(24+c-sq)==-1){
            set_destination(&move,16+c);
            move.mv|=(1<<28);
            add_move(movelist,move);
        }
    }
}
void night(Board * board,Movelist * movelist,const int8 sq){
    const int8 offset[8] = {6,-6,15,-15,10,-10,17,-17};
    int i;
    Move move;
    move.mv = 0;
    int8 p = 2;
    int8 dest;
    if(p!=board->brd[sq]) p=-p;
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
void bishop(Board * board,Movelist * movelist,const int8 sq){
    const int8 offset[4] = {7,-7,9,-9};
    int i,j;
    Move move;
    move.mv = 0; 
    int8 p = 3;
    int8 dest;
    if(p!=board->brd[sq]) p=-p;
    set_piece(&move,p);
    set_source(&move,sq);
    for(i=0;i<4;i++){
        j=1;
        while(in_board(sq,offset[i],j)==1){
            dest = sq+j*offset[i];
            if(is_sameside(p,board->brd[dest])==1) break;
            if(is_opponent(p,board->brd[dest])==1){             
                set_destination(&move,dest);
                set_captured_piece(&move,board->brd[dest]);
                add_move(movelist,move);
                break;
            }
            set_captured_piece(&move,board->brd[dest]);              
            set_destination(&move,dest);
            add_move(movelist,move); 
            j++;
        }
    }
}
void rook(Board * board,Movelist * movelist,const int8 sq){
    const int8 offset[4] = {1,-1,8,-8};
    int i,j;
    Move move;
    move.mv = 0; 
    int8 p = 4;
    int8 dest;
    if(p!=board->brd[sq]) p=-p;
    set_piece(&move,p);
    set_source(&move,sq);
    for(i=0;i<4;i++){
        j=1;
        while(in_board(sq,offset[i],j)==1){
            dest = sq+j*offset[i];
            if(is_sameside(p,board->brd[dest])==1) break;
            if(is_opponent(p,board->brd[dest])==1){               
                set_destination(&move,dest);
                set_captured_piece(&move,board->brd[dest]);
                add_move(movelist,move);
                break;
            }
            set_captured_piece(&move,board->brd[dest]);                 
            set_destination(&move,dest);
            add_move(movelist,move); 
            j++;
        }
    }
}
void queen(Board * board,Movelist * movelist,int8 sq){
    const int8 offset[8] = {1,-1,8,-8,7,-7,9,-9};
    int i,j;
    Move move;
    move.mv = 0;
    int8 p = 5;
    int8 dest;
    if(p!=board->brd[sq]) p=-p;
    set_piece(&move,p);
    set_source(&move,sq);
    for(i=0;i<8;i++){
        j=1;
        while(in_board(sq,offset[i],j)==1){
            dest = sq+j*offset[i];
            if(is_sameside(p,board->brd[dest])==1) break;
            if(is_opponent(p,board->brd[dest])==1){              
                set_destination(&move,dest);
                set_captured_piece(&move,board->brd[dest]);
                add_move(movelist,move);
                break;
            }
            set_captured_piece(&move,board->brd[dest]);                 
            set_destination(&move,dest);
            add_move(movelist,move); 
            j++;
        }
    }
}
void king(Board * board,Movelist * movelist,int8 sq){
    const int8 offset[8] = {1,-1,8,-8,7,-7,9,-9};
    int i;
    Move move;
    move.mv = 0;
    int8 p = 6;
    int8 dest;
    if(p!=board->brd[sq]) p=-p;
    set_piece(&move,p);
    set_source(&move,sq);
    for(i=0;i<8;i++){
        dest = sq+offset[i];
        if(in_board(sq,offset[i],1)==0||is_sameside(p,board->brd[dest])==1) continue;
        set_destination(&move,dest);
        set_captured_piece(&move,board->brd[dest]);
        add_move(movelist,move);     
    }
    set_captured_piece(&move,0);
    
    //Castling
    if(p==6){ 
        //short
        if((board->flag&(1<<7))!=0){
            if(board->brd[4]!=6||board->brd[5]!=0||board->brd[6]!=0||board->brd[7]!=4) goto out1;
            if(is_opponent_controls(board,4,1)==1) goto out1;
            if(is_opponent_controls(board,5,1)==1) goto out1;
            if(is_opponent_controls(board,6,1)==1) goto out1;
            set_destination(&move,6);
            move.mv = move.mv|(1<<20);
            add_move(movelist,move);
            move.mv = ~((~move.mv)|(1<<20));
        }
        out1:
        //long
        if((board->flag&(1<<6))!=0){
            if(board->brd[4]!=6||board->brd[3]!=0||board->brd[2]!=0||board->brd[1]!=0||board->brd[0]!=4) goto out2;
            if(is_opponent_controls(board,4,1)==1) goto out2;
            if(is_opponent_controls(board,3,1)==1) goto out2;
            if(is_opponent_controls(board,2,1)==1) goto out2;
            set_destination(&move,2);
            move.mv = move.mv|(1<<21);
            add_move(movelist,move);
            move.mv = ~((~move.mv)|(1<<21));            
        }
        out2:return;
    }else{
        //short
        if((board->flag&(1<<5))!=0){
            if(board->brd[60]!=-6||board->brd[61]!=0||board->brd[62]!=0||board->brd[63]!=-4) goto out3;
            if(is_opponent_controls(board,60,-1)==1) goto out3;
            if(is_opponent_controls(board,61,-1)==1) goto out3;
            if(is_opponent_controls(board,62,-1)==1) goto out3;
            set_destination(&move,62);
            move.mv = move.mv|(1<<20);
            add_move(movelist,move);
            move.mv = ~((~move.mv)|(1<<20));
        }
        out3:
        //long
        if((board->flag&(1<<4))!=0){
            if(board->brd[60]!=-6||board->brd[59]!=0||board->brd[58]!=0||board->brd[57]!=0||board->brd[56]!=-4) goto out4;
            if(is_opponent_controls(board,60,-1)==1) goto out4;
            if(is_opponent_controls(board,59,-1)==1) goto out4;
            if(is_opponent_controls(board,58,-1)==1) goto out4;
            set_destination(&move,58);
            move.mv = move.mv|(1<<21);
            add_move(movelist,move);
            move.mv = ~((~move.mv)|(1<<21));            
        }
        out4:return;
    }
}

int is_opponent_controls(Board * board,int8 sq,int turn){
    int kn_off[8] = {6,-6,15,-15,10,-10,17,-17};
    int offset[8] = {1,-1,8,-8,7,-7,9,-9};
    int i;
    for(i=0;i<8;i++){
        // for night 
        if(in_board(sq,kn_off[i],1)==1&&board->brd[sq+kn_off[i]]==-turn*2) return 1;
        //for king
        if(in_board(sq,offset[i],1)==1&&board->brd[sq+offset[i]]==-turn*6) return 1;
    }

    //pawns
    if(turn==1){
       if(in_board(sq,9,1)==1&&board->brd[sq+9]==-1) return 1; 
       if(in_board(sq,7,1)==1&&board->brd[sq+7]==-1) return 1; 
    }else{
       if(in_board(sq,-9,1)==1&&board->brd[sq-9]==1) return 1; 
       if(in_board(sq,-7,1)==1&&board->brd[sq-7]==1) return 1; 
    }
    for(i=0;i<8;i++){
        int j = 1;
        while(in_board(sq,offset[i],j)==1){
            int8 dest = sq+j*offset[i];
            if(is_sameside(turn,board->brd[dest])==1) break;
            if(is_opponent(turn,board->brd[dest])==1){
                //queen
                if(board->brd[dest]==-turn*5) return 1;
                //rook
                if(board->brd[dest]==-turn*4&&i<4) return 1;
                //bishop
                if(board->brd[dest]==-turn*3&&i>=4) return 1;
                //any other piece can not attack
                break;
            }
            j++;
        }
    }
    return 0;
}

#endif