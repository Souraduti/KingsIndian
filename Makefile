# Compiler
CC = gcc

# Directories
SRCDIR = src
INCDIR = include
OBJDIR = obj
BINDIR = bin

# Source files
COMMON_SRC = $(SRCDIR)/board.c $(SRCDIR)/piece_movement.c $(SRCDIR)/take_input.c $(SRCDIR)/chess_rule.c $(SRCDIR)/make_unmake.c $(SRCDIR)/search.c $(SRCDIR)/zobrist.c $(SRCDIR)/move.c $(SRCDIR)/static_evaluation.c
CLI_SRC = $(SRCDIR)/cli_main.c $(COMMON_SRC)
GUI_SRC = $(SRCDIR)/gui_main.c $(COMMON_SRC)

# Object files
CLI_OBJ = $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(CLI_SRC))
GUI_OBJ = $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(GUI_SRC))

# Targets
CLI_TARGET = $(BINDIR)/cli_app
GUI_TARGET = $(BINDIR)/gui_app

# Compiler flags
CFLAGS = -I$(INCDIR) -Wall -Wextra -std=c11

# Default target builds both executables
all: $(CLI_TARGET) $(GUI_TARGET)

# CLI executable
$(CLI_TARGET): $(CLI_OBJ)
	@mkdir -p $(BINDIR)
	$(CC) $^ -o $@

# GUI executable
$(GUI_TARGET): $(GUI_OBJ)
	@mkdir -p $(BINDIR)
	$(CC) $^ -o $@

# Pattern rule to compile all .c files to .o
$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean
clean:
	rm -rf $(OBJDIR) $(BINDIR)

.PHONY: all clean
