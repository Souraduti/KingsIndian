from pieces import Piece
class Board:
    WHITE = (180, 180, 180)
    BLACK = (50, 50, 50)
    RED = (200,0,0)
    GREEN = (0,200,0)
    BLUE = (0,0,200)
    Last_Move_White = (108,108,108)
    Last_Move_Black = (128,128,128)

    def __init__(self,SQ_SIZE,player_side,fen,sound):
        self.player_side = player_side
        self.pieces = []
        self.SQ_SIZE = SQ_SIZE
        self.game_end = 0
        self.last_move = [(-1,-1),(-1,-1)]
        self.parse_fen(fen) 
        self.sound = sound


    def parse_fen(self,fen):
        ranks = fen.split("/")
        i = 7
        for rank in ranks:
            j = 0
            for c in rank:
                if c.isnumeric():
                    j+=int(c)
                else:
                    self.pieces.append(Piece(Board.format_piece(c),i,j))
                    j+=1
            i -= 1
    def format_piece(c):
        if c in ['K','Q','R','B','N','P']:
            return 'w'+c.lower()
        else:
            c in ['k','q','r','b','n','p']
            return 'b'+c
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
        self.play_sound(self.get_piece(dest) != None)
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
    def is_square_valid(self,sq):
        if sq[0]<0 or sq[0]>=8:
            return False
        if sq[1]<0 or sq[1]>=8:
            return False
        return True
    
    def play_sound(self,is_capture):
        if self.sound == None:
            return
        root = "E:\\Git\\KingsIndian\\KingsIndian\\UI\\sound\\"
        if is_capture:
            self.sound.load(root+"capture.mp3")
        else:
            self.sound.load(root+"move.mp3")
        self.sound.play()


    def move(self,src,dest,process,key):
        if self.game_end!=0:
            return False
        if src is None or dest is None:
           return False
        if src==dest: return False
        if not self.is_square_valid(src):
            return False
        if not self.is_square_valid(dest):
            return False
        s = self.move_to_string(src,dest)
        promotion = self.is_promotion(src,dest)
        en_passent = self.is_enpassent(src,dest)
        
        if promotion:
            promotion = True
            if key not in ["q","r","b","n"]:
                key = "q"
            s+= key
        print(s)
        process.stdin.write(f"{s}\n")
        process.stdin.flush()

        validity = process.stdout.readline().strip()
        print("Validity : "+validity)
        if validity == 'invalid':
            return False
        self.move_piece(src,dest)
        self.last_move = [src,dest]
        self.castling(s)
        if promotion:
            self.promote(dest,key)
        elif en_passent:
            self.remove_piece((src[0],dest[1]))
        self.check_game_state(process)
        return True

    def get_computer_move(self,process):
        if self.game_end!=0:
            return
        print('Waiting')
        computer_move = process.stdout.readline().strip()
        print(f"Computer move: {computer_move}")
        start = self.string_to_sq(computer_move[1:3])
        end =  self.string_to_sq(computer_move[3:6])
        self.move_piece(start,end)
        self.last_move = [start,end]
        self.castling(computer_move)
        if len(computer_move) == 6:
            if computer_move[5]=='e':
                self.remove_piece((start[0],end[1]))
            else:
                self.promote(end,computer_move[5])
        self.check_game_state(process)
    
    def check_game_state(self,process):
        print("waiting for game state ")
        state = process.stdout.readline().strip()
        print("game state "+state)
        try:
            self.game_end = int(state)
        except:
            self.game_end = 0
        if self.game_end!=0 and self.sound != None:
            self.sound.load("E:\\Git\\KingsIndian\\KingsIndian\\UI\\sound\\gameover.mp3")
            self.sound.play()
    def adjust_sq(self,sq):
        if self.player_side == 'w':
            return (7-sq[0],sq[1])
        else:
            return (sq[0],7-sq[1])

    def get_color(self,row,col):
        sq = self.adjust_sq((row,col))
        p = self.piece_at(sq)
        if sq in self.last_move:
            color = Board.Last_Move_White if (row + col) % 2 == 0 else Board.Last_Move_Black
        else:
            color = Board.WHITE if (row + col) % 2 == 0 else Board.BLACK
        if self.game_end==0 or p==None:
            return color
        if self.game_end == 1:
            #White Won
            if p.type =='wk': 
                color = Board.GREEN
            elif p.type == 'bk':
                color = Board.RED
        elif self.game_end == 2:
            #Black Won
            if p.type =='bk': 
                color = Board.GREEN
            elif p.type == 'wk':
                color = Board.RED
        else:
            #Draw
            if p.type =='wk' or p.type =='bk':
                color = Board.BLUE
        return color
    
    def draw_board(self,win,pygame):
        win.fill(Board.WHITE)
        for row in range(8):
            for col in range(8):
                color = self.get_color(row,col)
                pygame.draw.rect(win, color, (col * self.SQ_SIZE, row * self.SQ_SIZE, self.SQ_SIZE, self.SQ_SIZE))
        for piece in self.pieces:
            piece.display(win,self.SQ_SIZE,self.player_side)

            
