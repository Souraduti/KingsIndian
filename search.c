
#include "search.h"


/*gives a random legal move*/
Move random_move(Board * board,int turn){
    Movelist movelist;
    generate_all(board,&movelist,turn,1);
    if(movelist.size==0){
        Move move;
        move.mv = 0;
        return move;
    }
    int i = rand()%(movelist.size);
    return movelist.list[i];
}

/*
    the pointer to a varible is passed to successive recursive call
    if the last moved played was illegal (puts the king in danger)
    the value pointed is set to 0 so that higher level this is used to 
    check for Chekmate and Stalemate

    there is added bonus for chekmate corrosponding the depth
    to encourage it to choose fastest mate 

*/
int evaluate(Board * board,Turn turn,int depth,int* legal,int alpha,int beta){
    if(depth==0){
        return static_eval(board);
    }
    if(is_insufficient(board)==1){
        return 0;
    }
    if(is_repitation(board)==1){
        return 0;
    }
    int i,eval,best_eval,count=0;
    Movelist all_moves;
    generate_all(board,&all_moves,turn,0);
    best_eval = (-1)*turn*INF;
    for(i=0;i<all_moves.size;i++){
        if(get_captured_piece(&all_moves.list[i])==-King*turn){
            best_eval = turn*INF;
            *legal = 0;
            return best_eval;
        }
        int l = 1;
        if(all_moves.list[i].mv==0) continue;
        move_on_board(board,&all_moves.list[i]);
        eval = evaluate(board,-turn,depth-1,&l,alpha,beta);
        unmove_on_board(board,&all_moves.list[i]);
        if(l==0){
            //the move was illegal
            continue;
        }
        if(turn==White){
            if(best_eval<eval) best_eval = eval;
            if(alpha<eval) alpha = eval; 
        }else{
            if(best_eval>eval) best_eval = eval;
            if(beta>eval) beta = eval;
        }
        count++;
        if(beta<=alpha) break;
    }    
    if(count==0){
        if(in_check(board,turn)==0){
            //stalemate
            return 0;
        }else{
            //checkmate
            return (-1)*turn*(INF-(MAXDEPTH+1-depth)*1000);
        }
    }
    return best_eval;
}
Move computer_move(Board * board,Turn turn,int depth){
    Move move;
    move.mv = 0;
    if(get_game_state(board,turn)!=ON){
        return move;
    }
    int i,eval,e,legal=1;
    
    Movelist all_moves;
    generate_all(board,&all_moves,turn,1);
        
    if(turn==White){
        /*for White lowest possible value of eval */
        eval = -INF;
    }else{
        /*for Black highest possible value of eval */
        eval = INF;
    }
    for(i=0;i<all_moves.size;i++){
        move_on_board(board,&all_moves.list[i]);
        e = evaluate(board,-turn,depth,&legal,-INF,INF);
        unmove_on_board(board,&all_moves.list[i]);

        if(turn==White){
            /*White finds the maximum evaluation*/
            if(e>eval){
                eval = e;
                move = all_moves.list[i];
            }
        }else{
            /*Black finds the minimum evaluation*/
            if(e<eval){
                eval = e;
                move = all_moves.list[i];
            }
        }
    }  
    return move;
}

