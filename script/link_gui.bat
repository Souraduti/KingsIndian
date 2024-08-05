@echo off

gcc ../target/main_gui.o ../target/board.o ../target/move.o ../target/piece_movement.o ../target/chess_rule.o  ../target/make_unmake.o ../target/search.o ../target/static_evaluation.o ../target/take_input.o -o ../main_gui
IF %ERRORLEVEL% NEQ 0 (
    echo Linking failed.
    exit /b 1
)

@echo Linking successful for GUI.
@echo Run python gui.py for starting game