@echo off

REM Check if a filename was provided as an argument
IF "%1"=="" (
    echo Usage: %~nx0 filename.c
    exit /b 1
)

REM Extract the file name without extension
SET filename=%~n1

REM Compile the C program
gcc -c ../%1 -o ../target/%filename%.o

REM Check if compilation was successful
IF %ERRORLEVEL% NEQ 0 (
    echo Compilation failed.
    exit /b 1
)

REM Print success message
@REM echo Compilation successful. Object file created: %filename%.o



