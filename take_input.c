
#include "take_input.h"

/*
    following two functions are for taking move input from user
    formating the move in the game representation
    and validating the moves are actually legal in that position or not
    the first funtion takes input from console and the second one for GUI 
*/


/* accepts all input in lower case */
Move user_input(Board * board,const Turn turn){
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
        printf("-----Check-----\n");
    }
    if(turn==White){
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
    if(from[0]<'a'||from[0]>'h'||from[1]<'1'||from[1]>'8'){
        printf("Invalid start square\n");
        return user_move;
    }else{
        src = (from[0]-'a')+(from[1]-'1')*8;
    }
    if(turn==White){
        p = p+'A'-'a';
    }
    if(board->brd[src]!=get_pcode(p)){
        printf("Piece is not in that square \n");
        return user_move;
    }
    printf("to    :");
    fflush(stdin);
    scanf("%s",to);
    if(to[0]<'a'||to[0]>'h'||to[1]<'1'||to[1]>'8'){
        printf("Invalid end square\n");
        return user_move;
    }else{
        dest = (to[0]-'a')+(to[1]-'1')*8;
    }
    
    if((p=='K'&&src==4&&dest==6)||(p=='k'&&src==60&&dest==62)){
        set_castling(&user_move,Short); 
    }else if((p=='K'&&src==4&&dest==2)||(p=='k'&&src==60&&dest==58)){
        set_castling(&user_move,Long); 
    }

    if((p=='P'||p=='p')&&((src-dest)%8!=0)&&board->brd[dest]==0){
        //En-passant
       set_enpassant(&user_move);
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
        set_promotion(&user_move,(1<<(ch-1)));
    }
    set_piece(&user_move,get_pcode(p));
    set_source(&user_move,src);
    set_destination(&user_move,dest);
    set_captured_piece(&user_move,board->brd[dest]);
    
    generate_all(board,&movelist,turn,1);
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



Move user_input_from_GUI(Board * board,const Turn turn,char * move){
    int i; 
    int8 src,dest;
    char p = move[0]; 
    char valid_pieces[6] = {'p','n','b','r','q','k'};
    char from[2],to[2];
    Move user_move;
    Movelist movelist;
    from[0] = move[1];
    from[1] = move[2];
    to[0] = move[3];
    to[1] = move[4];
    user_move.mv = 0; 
    
    //if(p=='0') exit(1);
    for(i=0;i<=6;i++){
        if(i==6){
            return user_move;
        }
        if(p==valid_pieces[i]) break;
    }
    
    if(from[0]<'a'||from[0]>'h'||from[1]<'1'||from[1]>'8'){
        return user_move;
    }else{
        src = (from[0]-'a')+(from[1]-'1')*8;
    }
    if(turn==White){
        p = p+'A'-'a';
    }
    if(board->brd[src]!=get_pcode(p)){
        return user_move;
    }
    if(to[0]<'a'||to[0]>'h'||to[1]<'1'||to[1]>'8'){
        return user_move;
    }else{
        dest = (to[0]-'a')+(to[1]-'1')*8;
    }
    
    if((p=='K'&&src==4&&dest==6)||(p=='k'&&src==60&&dest==62)){
        set_castling(&user_move,Short); 
    }else if((p=='K'&&src==4&&dest==2)||(p=='k'&&src==60&&dest==58)){
        set_castling(&user_move,Long); 
    }

    if((p=='P'||p=='p')&&((src-dest)%8!=0)&&board->brd[dest]==0){
        //En-passant
       set_enpassant(&user_move);
    }
    if((p=='P'&&to[1]=='8')||(p=='p'&&to[1]=='1')){
        int ch=0;
        switch (move[5]){
            case 'q':ch=1;break;
            case 'n':ch=2;break;
            case 'r':ch=3;break;
            case 'b':ch=4;break;
        }
        if(ch<=0||ch>4) {
            user_move.mv = 0;
            return user_move;
        }
        set_promotion(&user_move,(1<<(ch-1)));
    }
    set_piece(&user_move,get_pcode(p));
    set_source(&user_move,src);
    set_destination(&user_move,dest);
    set_captured_piece(&user_move,board->brd[dest]);
    
    generate_all(board,&movelist,turn,1);
    for(i=0;i<=movelist.size;i++){
        if(i==movelist.size){
            user_move.mv = 0;
            break;
        }
        if(user_move.mv == movelist.list[i].mv) break;
    }
    return user_move;
}




