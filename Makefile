# Makefile pour le projet PENDU-GAME

# Compiler
CC = gcc
# Options de compilation
CFLAGS = -Wall -Wextra -I./include

# Fichiers source
SRC_DIR = src
SRCS = $(wildcard $(SRC_DIR)/*.c)

# Fichiers objet
OBJ_DIR = obj
OBJS = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS))

# Exécutable
EXECUTABLE = pendu_game

# Règles de construction
all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

# Nettoyage des fichiers objets et de l'exécutable
clean:
	rm -f $(OBJ_DIR)/*.o $(EXECUTABLE)

