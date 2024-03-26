
#ifndef BOARD 
#define BOARD

#include <stdio.h>

/*
  Pieces Representation
  0 -> Empty Square
  1 -> pawn
  2 -> Night
  3 -> Bishop
  4 -> Rook 
  5 -> Queen
  6 -> King
  -ve values are used Black pieces
  +ve values are for White Pieces
*/
typedef enum Pieces{
    Empty = 0,
    Pawn = 1,
    Night = 2,
    Bishop = 3 ,
    Rook =4,
    Queen = 5,
    King = 6
}Pices;

typedef enum Turn{
    White = 1,
    Black = -1
}Turn;


/*
    Board Representation :
    brd  : Array of 8 bit charachters of size 64 stores all the pieces in board
           0->a1  63 ->h8
           
    flag : lowest 4 bits indicate which of the 16 pawns has moved 2 square in last move
           stored for En Passent 
           next 4 bit stores castling rights  (w-short,w-long,b-short,b-long)
           next 6 bit will store position of white king
           next 6 bit will store position of black king
           20th bit will be set if pawn has moved 2 Square
*/
typedef char int8;
typedef struct Board
{
    int8 brd[64];
    int flag;
}Board;

/* sets the pieces on the board in initial configuration */
void set_board(Board * board){

    board->brd[0] = Rook;
    board->brd[1] = Night;
    board->brd[2] = Bishop;
    board->brd[3] = Queen;
    board->brd[4] = King;
    board->brd[5] = Bishop;
    board->brd[6] = Night;
    board->brd[7] = Pawn;

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
}

/* sets an Empty Board */
void set_empty_board(Board * board){
    int i;
    for(i=0;i<64;i++){
        board->brd[i] = 0;
    }
    board->flag = 0;
}

char get_piece_from_code(int8 p_code){
    char mapper[] = {'k','q','r','b','n','p','.','P','N','B','R','Q','K'};
    return mapper[p_code+6];
}
int8 get_pcode(char p){
    int8 pcode = 0;
    switch (p)
    {
      case 'k': pcode = -6;break;
      case 'q': pcode = -5;break;
      case 'r': pcode = -4;break;
      case 'b': pcode = -3;break;
      case 'n': pcode = -2;break;    
      case 'p': pcode = -1;break;

      case 'K': pcode = 6;break;
      case 'Q': pcode = 5;break;
      case 'R': pcode = 4;break;
      case 'B': pcode = 3;break;
      case 'N': pcode = 2;break;    
      case 'P': pcode = 1;break;
    }
    return pcode;
}

/* Displays the chess board */
void * display(const Board * board){
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

//63 => 00111111
int8 get_king_pos(const Board * board,Turn turn){
    int8 sq;
    int off = (turn==White)?8:14;
    sq = (board->flag>>off)&63;
    return sq;
}
void set_king_pos(Board * board,Turn turn,int8 sq){
    int off = (turn==White)?8:14;
    board->flag = ~(~(board->flag)|(63<<off));
    board->flag = board->flag|((sq&63)<<off);
}
int get_pawn_jump(const Board * board,int turn){
    int last = board->flag&15;
    if((board->flag&(1<<20))==0) return -1;
    if(turn==0) return last;
    if(turn==-1&&(last&(1<<3))==0) return -1;
    if(turn==1&&(last&(1<<3))!=0) return -1;
    return last&7;
}
void set_pawn_jump(Board * board,int file,Turn turn){
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
/*
    side = 1 for short
    side = 0 for long
*/
int get_castling_right(const Board * board ,Turn turn,int side){
    int off = 5+turn+side;
    if((board->flag&(1<<off))==0) return 0;
    return 1;
}
void set_castling_right(Board * board ,Turn turn,int side,int v){
    int off = 5+turn+side;;
    if(v==0){
        board->flag = ~(~(board->flag)|(1<<off));
    }else if(v==1){
        board->flag|=(1<<off);
    }
}

#endif

