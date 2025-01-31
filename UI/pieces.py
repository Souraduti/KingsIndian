import pygame
class Piece:
    images = {}
    def __init__(self,type,rank,file):
        self.type = type
        self.rank = rank
        self.file = file
        self.dragging = False
    def display(self,win,SQUARE_SIZE,player_side):
        file = self.file*SQUARE_SIZE if player_side == 'w' else (7-self.file)*SQUARE_SIZE
        rank = (7-self.rank)*SQUARE_SIZE if player_side == 'w' else self.rank*SQUARE_SIZE
        cordinate = (file,rank)
        if not self.dragging:
            win.blit(Piece.images[self.type],cordinate)
    # Load images
    def load_images(SQUARE_SIZE):
        pieces = ['wp', 'wr', 'wn', 'wb', 'wq', 'wk', 'bp', 'br', 'bn', 'bb', 'bq', 'bk']
        for piece in pieces:
            Piece.images[piece] = pygame.transform.scale(pygame.image.load(f'E:/Git/KingsIndian/KingsIndian/UI/images/{piece}.png').convert_alpha(), (SQUARE_SIZE,SQUARE_SIZE))
    def get_image(piece_name):
        return Piece.images[piece_name]