#ifndef TAKE_INPUT 
#define TAKE_INPUT


#include "board.c"
#include "move.c"
#include "chess_rule.c"


/*
    turn = -1 -> black
    turn = 1  -> white
*/

/* accepts all input in lower case */
Move user_input(Board * board, int turn){
    int i; 
    int8 src,dest;
    char p; 
    char valid_pieces[6] = {'p','n','b','r','q','k'};
    char from[2],to[2];
    Move user_move;
    Movelist movelist;

    //0 represents null move
    user_move.mv = 0; 
    if(in_check(board,turn)==1){
        printf("-----Chechk-----\n");
    }
    if(turn==1){
        printf("White's turn : \n");
    }else if(turn==-1){
        printf("Black's turn : \n");
    }else{
        return user_move;
    }
    printf("Piece :");
    fflush(stdin);
    scanf("%c",&p);
    if(p=='0') exit(1);
    /*if(p=='x'){
        Movelist moves;
        genarate_all(board,&moves,-turn,1);
        for(int i =0;i<moves.size;i++){
            display_move(&moves.list[i]);
        }
        return user_move;
    }*/
    for(i=0;i<=6;i++){
        if(i==6){
            printf("Please Enter a valid Piece\n");
            return user_move;
        }
        if(p==valid_pieces[i]) break;
    }
    printf("from  :");
    fflush(stdin);
    scanf("%s",from);
    if(from[0]<'a'||from[0]>'h'||from[1]<'0'||from[1]>'8'){
        printf("Invalid start square\n");
        return user_move;
    }else{
        src = (from[0]-'a')+(from[1]-'1')*8;
    }
    printf("to    :");
    fflush(stdin);
    scanf("%s",to);
    if(to[0]<'a'||to[0]>'h'||to[1]<'0'||to[1]>'8'){
        printf("Invalid end square\n");
        return user_move;
    }else{
        dest = (to[0]-'a')+(to[1]-'1')*8;
    }
    if(turn==1){
        p = p+'A'-'a';
    }
    if(board->brd[src]!=get_pcode(p)){
        printf("Piece is not in that square \n");
        return user_move;
    }
    if((p=='K'&&src==4&&dest==6)||(p=='k'&&src==60&&dest==62)){
        //short castle
        user_move.mv = user_move.mv|(1<<20); 
    }else if((p=='K'&&src==4&&dest==2)||(p=='k'&&src==60&&dest==58)){
        //long castle
        user_move.mv = user_move.mv|(1<<21); 
    }

    if((p=='P'||p=='p')&&((src-dest)%8!=0)&&board->brd[dest]==0){
        //En-passant
        /*Movelist movelist;
        genarate_all(board,&movelist,-1,0);
        for(int i=0;i<movelist.size;i++){
            display_move(&movelist.list[i]);
        }*/
        user_move.mv|=(1<<28);
    }
    if((p=='P'&&to[1]=='8')||(p=='p'&&to[1]=='1')){
        int ch = 0;
        do{
            printf("Promote to :\n");
            printf("1 for Queen\n");
            printf("2 for Night\n");
            printf("3 for Rook\n");
            printf("4 for Bishop\n");
            scanf("%d",&ch);
        }while(ch<=0||ch>4);
        //for setting promotion flag 
        ch+=21;
        user_move.mv = user_move.mv|(1<<ch);
    }
    set_piece(&user_move,get_pcode(p));
    set_source(&user_move,src);
    set_destination(&user_move,dest);
    set_captured_piece(&user_move,board->brd[dest]);

    /* genrate all possible legal moves in the position
       compares if user_move is present in movelist
    */
    genarate_all(board,&movelist,turn,1);
    for(i=0;i<=movelist.size;i++){
        if(i==movelist.size){
            user_move.mv = 0;
            printf("Invaid Move\n");
            break;
        }
        if(user_move.mv == movelist.list[i].mv) break;
    }
    return user_move;
}

#endif