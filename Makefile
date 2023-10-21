#==========================================
#           Makefile: makefile
#==========================================

CC := gcc

# Utiliser la commande de suppression propre à l'os
ifeq ($(OS), Windows_NT)
	RM := rm
else
	RM := rm -rf
endif

# Liste des repertoires contenant respectivement les fichiers :*.c,*.h,*.o, l'executable
SRCDIR  := src
HEADDIR := ../include/SDL2
LIBDIR := ../lib
#HEADDIR := C:/MinGW/x86_64-w64-mingw32/include/SDL2
#LIBDIR := C:/MinGW/x86_64-w64-mingw32/lib
BINDIR := bin

#CFLAGS := -I $(HEADDIR) -L $(LIBDIR) -g -Wall -pedantic -O3 -fno-tree-vectorize
CFLAGS := -I $(HEADDIR) -L $(LIBDIR) -g -Wall -fcommon
ifeq ($(OS), Windows_NT)
	GLLIBS := -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf -lSDL2_image -lSDL2_mixer
else
	GLLIBS := -lSDL2 -lSDL2_ttf -lSDL2_image -lSDL2_mixer
endif

# L'executable
BIN := $(BINDIR)/main

# Où trouver les differents sources *.c qu'il faudra compiler pour creer les objets correspondants
SRC := $(wildcard $(SRCDIR)/*.c) $(wildcard $(SRCDIR)/**/*.c)
OBJ := $(SRC:$(SRCDIR)/%.c=$(BINDIR)/%.o)

all: $(BIN)

# Creation de l'executable
$(BIN): $(OBJ)
	@echo "=== Checking $^ ==="
	$(CC) -o $@ $^ $(CFLAGS) $(GLLIBS)

# Creation des differents *.o à partir des /*.c
$(BINDIR)/%.o: $(SRCDIR)/%.c
	@echo "=== Creating object ==="
	$(CC) -o $@ -c $< $(CFLAGS)

# Nettoyage des objets (Tout sera recompiler)
clean:
	@echo "=== Cleaning project ==="
	$(RM) $(BINDIR)/main.exe $(BINDIR)/*.o $(BINDIR)/gui/*.o $(BINDIR)/core/*.o
.PHONY: clean
