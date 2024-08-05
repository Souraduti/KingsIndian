import pygame
class Piece:
    images = {}
    def __init__(self,type,rank,file):
        self.type = type
        self.rank = rank
        self.file = file
        self.dragging = False
        self.last_moved = False
    def display(self,win,SQUARE_SIZE):
        # selected_white = (200,200,200)
        # selected_black = (70,70,70)
        cordinate = (self.file*SQUARE_SIZE,(7-self.rank)*SQUARE_SIZE)
        if not self.dragging:
            win.blit(Piece.images[self.type],cordinate)
    # Load images
    def load_images(SQUARE_SIZE):
        pieces = ['wp', 'wr', 'wn', 'wb', 'wq', 'wk', 'bp', 'br', 'bn', 'bb', 'bq', 'bk']
        for piece in pieces:
            Piece.images[piece] = pygame.transform.scale(pygame.image.load(f'images/{piece}.png').convert_alpha(), (SQUARE_SIZE,SQUARE_SIZE))
    def get_image(piece_name):
        return Piece.images[piece_name]