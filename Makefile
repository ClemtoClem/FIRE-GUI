#==========================================
#           Makefile: makefile
#==========================================

PROJECT_NAME := PolyAdventure
CC        := gcc
SRCDIR    := src
HEADERDIR := # include
ifeq ($(OS),Windows_NT)
BUILDDIR  := ./window/build
BINDIR    := ./window/bin
else
BUILDDIR  := ./linux/build
BINDIR    := ./linux/bin
endif
TARGET    := $(BINDIR)/$(PROJECT_NAME)
SOURCES   := $(shell find $(SRCDIR) -type f -name *.c*)
HEDEARS   := $(shell find $(HEADERDIR) -type f -name *.h*)
OBJECTS   := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(addsuffix .o,$(basename $(SOURCES))))
DEPS      := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(addsuffix .d,$(basename $(SOURCES))))
CFLAGS    := -Wall -D_GNU_SOURCE
LIB       := 
ifeq ($(OS),Windows_NT)
LIB       += -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer
else
LIB       += $(shell sdl2-config --libs) -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer
endif
INC       := -I src # -I include

# List of all files in the directory
ALL_FILES := $(wildcard $(BINDIR)/*) $(wildcard $(BUILDDIR)/*)
# List of .dll files
DLL_FILES := $(wildcard $(BINDIR)/*.dll)
# List of files to be deleted (excluding .dll files)
DELETE_FILES := $(filter-out $(DLL_FILES), $(ALL_FILES))

GREEN=`tput setaf 2`
RESET=`tput sgr0`

define print_green
	@echo "$(GREEN)$(1)$(RESET)"
endef

all: $(TARGET)

# Nettoyage des objets (Tout sera recompiler)
clean:
	@echo "=== Cleaning project ==="
	rm -f $(DELETE_FILES)

$(TARGET): $(BINDIR) $(BUILDDIR) $(OBJECTS)
	$(call print_green,"Linking object files...")
	@$(CC) $(OBJECTS) -o $(TARGET) $(LIB)
	$(call print_green,"$(TARGET) has been created!")

$(BUILDDIR) :
	mkdir $(BUILDDIR)

$(BINDIR):
	mkdir $(BINDIR)
	
$(BUILDDIR)/%.o: $(SRCDIR)/%.c*
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INC) -c -o $@ $<
	@$(CC) $(CFLAGS) $(INC) -M $< -MT $@ > $(@:.o=.td)
	@cp $(@:.o=.td) $(@:.o=.d); 
	@sed -e 's/#.*//' -e 's/^[^:]*: *//' -e 's/ *\\$$//' \
	-e '/^$$/ d' -e 's/$$/ :/' < $(@:.o=.td) >> $(@:.o=.d); 
	@rm -f $(@:.o=.td)


-include $(DEPS)

.PHONY: clean all
