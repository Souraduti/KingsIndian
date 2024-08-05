from pieces import Piece
class Board:
    def __init__(self,SQ_SIZE):
        self.pieces = []
        self.SQ_SIZE = SQ_SIZE
        for f in range(0,8):
            self.pieces.append(Piece('wp',1,f))
            self.pieces.append(Piece('bp',6,f))
        self.pieces.append(Piece('wr',0,0))
        self.pieces.append(Piece('wn',0,1))
        self.pieces.append(Piece('wb',0,2))
        self.pieces.append(Piece('wq',0,3))
        self.pieces.append(Piece('wk',0,4))
        self.pieces.append(Piece('wb',0,5))
        self.pieces.append(Piece('wn',0,6))
        self.pieces.append(Piece('wr',0,7))

        self.pieces.append(Piece('br',7,0))
        self.pieces.append(Piece('bn',7,1))
        self.pieces.append(Piece('bb',7,2))
        self.pieces.append(Piece('bq',7,3))
        self.pieces.append(Piece('bk',7,4))
        self.pieces.append(Piece('bb',7,5))
        self.pieces.append(Piece('bn',7,6))
        self.pieces.append(Piece('br',7,7))

    def display(self,win):
        # draw_board(win)
        for piece in self.pieces:
            piece.display(win,self.SQ_SIZE)

    def piece_at(self,position):
        rank,file = position
        for piece in self.pieces:
            if piece.rank == rank and piece.file == file:
                return piece
        return None
    
    def to_string(self,sq):
        r,f = sq
        files = "abcdefgh"
        return files[f]+str(r+1)
    
    def string_to_sq(self,s):
        # print(s)
        file_map = {'a': 0, 'b': 1, 'c': 2, 'd': 3, 'e': 4, 'f': 5, 'g': 6, 'h': 7}
        f = file_map[s[0]]
        r = int(s[1])-1
        return (r,f)
    
    def get_piece(self,sq):
        for pieces in self.pieces:
            if pieces.rank == sq[0] and pieces.file == sq[1]:
                return pieces
        return None
    def remove_piece(self,sq):
        self.pieces = [ piece for piece in self.pieces if piece.rank != sq[0] or piece.file != sq[1] ]
    
    def move_to_string(self,src,dst):
        return self.get_piece(src).type[1]+self.to_string(src)+self.to_string(dst)
    
    def move_piece(self,src,dest):
        if src and dest:
            r1,f1 = src
            r2,f2 = dest
        self.pieces = [ piece for piece in self.pieces if piece.rank != r2 or piece.file != f2 ]
        for piece in self.pieces:
            piece.last_moved = False 
            if piece.rank == r1 and piece.file == f1:
                piece.rank = r2
                piece.file = f2
                piece.last_moved = True
    def castling(self,s):
        if s =='ke8g8':
            self.move_piece((7,7),(7,5))
        elif s=='ke8c8':
            self.move_piece((7,0),(7,3))
        elif s=='ke1g1':
            self.move_piece((0,7),(0,5))
        elif s=='ke1c1':
            self.move_piece((0,0),(0,3))
    def is_promotion(self,src,dest):
        p = self.get_piece(src)
        if p == None:
            return False
        if p.type[1]!='p':
            return False
        if p.type[0]=='w' and dest[0]==7:
            return True
        if p.type[0]=='b' and dest[0]==0:
            return True
        return False
    def is_enpassent(self,src,dest):
        p = self.get_piece(src)
        if p==None or p.type[1]!='p':
            return False
        if self.get_piece(dest)!=None:
            return False
        if src[0]==dest[0]:
            return False
        return True

    def promote(self,sq,promoted_piece):
        p = self.get_piece(sq)
        if p==None:
            return
        p.type = p.type[0]+promoted_piece 
    
    def move(self,src,dest,process,key):
        if src is None or dest is None:
           return False
        if src==dest: return False
        s = self.move_to_string(src,dest)
        promotion = self.is_promotion(src,dest)
        en_passent = self.is_enpassent(src,dest)
        if promotion:
            promotion = True
            if key not in ["q","r","b","n"]:
                return False
            s+= key
        print(s)
        process.stdin.write(f"{s}\n")
        process.stdin.flush()

        validity = process.stdout.readline().strip()
        print("Validity : "+validity)
        if validity == 'invalid':
            return False
        self.move_piece(src,dest)
        self.castling(s)
        if promotion:
            self.promote(dest,key)
        elif en_passent:
            self.remove_piece((src[0],dest[1]))
        return True
    
    def get_computer_move(self,process):
        print('Waiting')
        computer_move = process.stdout.readline().strip()
        print(f"Computer move: {computer_move}")
        start = self.string_to_sq(computer_move[1:3])
        end =  self.string_to_sq(computer_move[3:6])
        self.move_piece(start,end)
        self.castling(computer_move)
        if len(computer_move) == 6:
            if computer_move[5]=='e':
                self.remove_piece((start[0],end[1]))
            else:
                self.promote(end,computer_move[5])

        
