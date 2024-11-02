#include "board.h"



/* sets the pieces on the board in initial configuration */
void set_board(Board * board)
{
    board->move_number = 0;
    board->hash[0] =  compute_hash(board);
    board->brd[0] = Rook;
    board->brd[1] = Night;
    board->brd[2] = Bishop;
    board->brd[3] = Queen;
    board->brd[4] = King;
    board->brd[5] = Bishop;
    board->brd[6] = Night;
    board->brd[7] = Rook;

    board->brd[56] = -Rook;
    board->brd[57] = -Night;
    board->brd[58] = -Bishop;
    board->brd[59] = -Queen;
    board->brd[60] = -King;
    board->brd[61] = -Bishop;
    board->brd[62] = -Night;
    board->brd[63] = -Rook;

    int i;
    for(i=8;i<16;i++){
        board->brd[i] = Pawn;
    }

    for(i=48;i<56;i++){
        board->brd[i] = -Pawn;
    }
   

    for(i = 16;i<48;i++){
        board->brd[i] = Empty;
    }
    /* 984304 in binary => 000000000000011110000010011110000*/
    board->flag = 984304;
    board->turn = White;
}

/* sets an Empty Board */
void set_empty_board(Board * board)
{
    int i;
    for(i=0;i<64;i++){
        board->brd[i] = 0;
    }
    board->flag = 0;
    board->turn = White;
}

int validate_fen(const char * fen){
    int i;
    int king_count[2] = {0,0};
    int slash_count = 0;
    int first_pawn = 72;
    int last_pawn = -1;
    int first_slash = -1;
    int last_slash = -1; 
    for(i=0;fen[i]!='\0'&&i<=72;i++){
        char ch = fen[i];
        if(ch!='/'&&ch!=' '&&!('1'<=ch&&ch<='8')&&get_pcode(ch)==Empty) return 0;
        if(ch==' ') continue;
        if(ch=='K') 
            king_count[0]++;
        else if(ch=='k') 
            king_count[1]++;
        else if (ch=='/'){
            slash_count++;
            if(first_slash==-1) first_slash = i;
            last_slash = i;
        }
        else if(ch=='p'||ch=='P'){
            if(first_pawn==72) first_pawn = i;
            last_pawn = i;
        } 
    }
    //all the ranks not given
    if(slash_count!=7) return 0 ;

    // both side must have one and only one kings
    if(king_count[0]!=1||king_count[1]!=1) return 0;

    //pawn on 8th Rank
    if(first_pawn<=first_slash) return 0;
    //pwan on 1st rank
    if(last_pawn>=last_slash) return 0;

    return 1;
}
void set_board_fen(Board * board,const char * fen,char t){
    int i,j,k=0;
    //if fen is invalid set up the standerd pieces
    if(validate_fen(fen)==0){
        set_board(board);
        // printf("Invalid FEN\n");
        return;
    }
    set_empty_board(board);
    board->turn = (t=='b')?Black:White;
    for(i=56;i>=0;i-=8){
        j = 0;
        while(!(i==0&&j>=8)){
            char ch = fen[k++];
            if(ch=='/'){
                break;
            }

            if('1'<=ch&&ch<='8'){
                j+= ch-'0';
            }else {
                board->brd[i+j] = get_pcode(ch);
                j++;
                if(ch=='K') set_king_pos(board,White,i+j);
                if(ch=='k') set_king_pos(board,Black,i+j);
            }
        }
    }
    board->move_number = 0;
    board->hash[0] =  compute_hash(board);
    set_castling_right(board,White,Short,1);
    set_castling_right(board,White,Long,1);
    set_castling_right(board,Black,Short,1);
    set_castling_right(board,Black,Long,1);
}

char get_piece_from_code(int8 p_code){
    char mapper[] = {'k','q','r','b','n','p','.','P','N','B','R','Q','K'};
    return mapper[p_code+6];
}
int8 get_pcode(char p){
    int8 pcode = Empty;
    switch (p)
    {
      case 'k': pcode = -King;break;
      case 'q': pcode = -Queen;break;
      case 'r': pcode = -Rook;break;
      case 'b': pcode = -Bishop;break;
      case 'n': pcode = -Night;break;    
      case 'p': pcode = -Pawn;break;

      case 'K': pcode = King;break;
      case 'Q': pcode = Queen;break;
      case 'R': pcode = Rook;break;
      case 'B': pcode = Bishop;break;
      case 'N': pcode = Night;break;    
      case 'P': pcode = Pawn;break;
    }
    return pcode;
}

/* Displays the chess board */
void  display(const Board * board){
    int i,j;
    
    printf("\n\n");
    printf("       a b c d e f g h\n");
    for(i=7;i>=0;i--){
        printf("     %d ",(i+1));
        for(j=0;j<8;j++){
            printf("%c ",get_piece_from_code(board->brd[8*i+j]));
        }
        printf("\n");
    }
    printf("\n");
}

int8 get_king_pos(const Board * board,Turn turn)
{
    int8 sq;
    int off = (turn==White)?8:14;
    //63 => 00111111
    sq = (board->flag>>off)&63;
    return sq;
}
void set_king_pos(Board * board,Turn turn,int8 sq)
{
    int off = (turn==White)?8:14;
    board->flag = ~(~(board->flag)|(63<<off));
    board->flag = board->flag|((sq&63)<<off);
}
int get_pawn_jump(const Board * board,Turn turn)
{
    int last = board->flag&15;
    if((board->flag&(1<<20))==0) return -1;
    if(turn==0) return last;
    if(turn==-1&&(last&(1<<3))==0) return -1;
    if(turn==1&&(last&(1<<3))!=0) return -1;
    return last&7;
}
void set_pawn_jump(Board * board,int file,Turn turn)
{
    if(file==-1){
        board->flag = ~((~board->flag)|(1<<20));
        board->flag = ~((~board->flag)|15);
        return;
    }
    if(turn==Black) file+=8;
    board->flag|= 1<<20;
    board->flag = board->flag&(~15);
    board->flag|= file;
}
int get_castling_right(const Board * board ,Turn turn,Castling_side side)
{
    int off = 5+turn+side;
    if((board->flag&(1<<off))==0) return 0;
    return 1;
}
void set_castling_right(Board * board ,Turn turn,Castling_side side,int v)
{
    int off = 5+turn+side;;
    if(v==0){
        board->flag = ~(~(board->flag)|(1<<off));
    }else if(v==1){
        board->flag|=(1<<off);
    }
}

Turn get_next_turn(Board * board){
    return board->turn;
}



