import pygame
import subprocess
import sys
from pieces import Piece
from board import Board

WIDTH =  HEIGHT = 512
SQUARE_SIZE = WIDTH // 8
    

def get_square_under_mouse(side):
    mouse_pos = pygame.mouse.get_pos()
    file = mouse_pos[0] // SQUARE_SIZE if side == 'w' else (7-mouse_pos[0] // SQUARE_SIZE)
    rank = 7 - (mouse_pos[1] // SQUARE_SIZE) if side == 'w' else (mouse_pos[1] // SQUARE_SIZE)
    return (rank,file)

def main(process,side):
    pygame.init()
    WINDOW = pygame.display.set_mode((WIDTH, HEIGHT))
    pygame.display.set_caption("Chess")
    Piece.load_images(SQUARE_SIZE)
    board = Board(SQUARE_SIZE,side)
    run = True
    selected_square = None
    piece_dragged = None
    if side=='b':
        user_moved = True
    else:
        user_moved = False
    last_pressed = '\n'
    while run:
        if user_moved:
            user_moved = False
            board.get_computer_move(process)
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                run = False
            if event.type == pygame.MOUSEBUTTONDOWN:
                selected_square = get_square_under_mouse(side)
                piece_dragged = board.piece_at(selected_square)
                if piece_dragged != None:
                    piece_dragged.dragging = True
            elif event.type == pygame.MOUSEBUTTONUP:
                if piece_dragged:
                    target_square = get_square_under_mouse(side)
                    piece_dragged.dragging = False
                    if board.move(selected_square,target_square,process,last_pressed) :
                        user_moved = True
                    selected_square = None
                    piece_dragged = None
            elif event.type == pygame.KEYDOWN:
                last_pressed = pygame.key.name(event.key)


        if piece_dragged:
            piece_dragged.dragging = True
            board.draw_board(WINDOW,pygame)
            # board.display(WINDOW)
            piece_dragged.dragging = False
            x, y = pygame.mouse.get_pos()
            piece_image = Piece.get_image(piece_dragged.type)
            WINDOW.blit(piece_image, (x - SQUARE_SIZE // 2, y - SQUARE_SIZE // 2))
        else:
            board.draw_board(WINDOW,pygame)                
            # board.display(WINDOW)

        pygame.display.update()
    pygame.quit()
    process.terminate()

if __name__ == "__main__":
    if len(sys.argv)<2:
        print("Choose Side ")
        sys.exit(1)
    side = sys.argv[1]
    if side not in ['w','b']:
        print('Enter w for playing white')
        print('Enter b for playing black')
        sys.exit(1)
    try:
        process = subprocess.Popen(['E:\Git\KingsIndian\KingsIndian\main_gui',side], stdin=subprocess.PIPE,stdout=subprocess.PIPE, text=True)       
        main(process,side)
    finally:
        process.terminate()
