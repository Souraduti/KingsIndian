@echo off
for %%i in (main.c main_gui.c board.c move.c piece_movement.c chess_rule.c  make_unmake.c search.c static_evaluation.c take_input.c) do (
     obj.bat %%i   
)


