import pygame
import subprocess
import sys
from pieces import Piece
from board import Board

WIDTH =  HEIGHT = 400
SQUARE_SIZE = WIDTH // 8
    

pygame.init()

WINDOW = pygame.display.set_mode((WIDTH, HEIGHT))
pygame.display.set_caption("Chess")

def draw_board(win,side,last_moved_square):
    # Colors
    WHITE = (180, 180, 180)
    BLACK = (50, 50, 50)
    win.fill(WHITE)
    pairity = 1 if side=='b' else 0
    for row in range(8):
        for col in range(8):
            color = WHITE if (row + col+pairity) % 2 == 0 else BLACK
            if (side=='w' and last_moved_square == (7-row,col)) or (side=='b' and last_moved_square == (row,7-col)):
                color = (128,128,128) if color==BLACK else (108,108,108)
            pygame.draw.rect(win, color, (col * SQUARE_SIZE, row * SQUARE_SIZE, SQUARE_SIZE, SQUARE_SIZE))

def get_square_under_mouse(side):
    mouse_pos = pygame.mouse.get_pos()
    file = mouse_pos[0] // SQUARE_SIZE if side == 'w' else (7-mouse_pos[0] // SQUARE_SIZE)
    rank = 7 - (mouse_pos[1] // SQUARE_SIZE) if side == 'w' else (mouse_pos[1] // SQUARE_SIZE)
    return (rank,file)

def main(process,side):
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


        last_moved_square = board.get_last_moved_square()
        if piece_dragged:
            piece_dragged.dragging = True
            draw_board(WINDOW,side,last_moved_square)
            board.display(WINDOW)
            piece_dragged.dragging = False
            x, y = pygame.mouse.get_pos()
            piece_image = Piece.get_image(piece_dragged.type)
            WINDOW.blit(piece_image, (x - SQUARE_SIZE // 2, y - SQUARE_SIZE // 2))
        else:
            draw_board(WINDOW,side,last_moved_square)                
            board.display(WINDOW)

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
