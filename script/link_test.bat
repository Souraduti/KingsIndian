@echo off

gcc ../target/test.o ../target/board.o ../target/zobrist.o ../target/move.o ../target/piece_movement.o ../target/chess_rule.o  ../target/make_unmake.o ../target/search.o ../target/static_evaluation.o ../target/take_input.o -o ../test
IF %ERRORLEVEL% NEQ 0 (
    echo Linking failed.
    exit /b 1
)

@echo Linking successful for Testing.
@echo Run test