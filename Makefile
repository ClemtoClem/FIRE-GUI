PROJECT_NAME := FIRE_GUI
CC        := gcc
SRCDIR    := src
BUILDDIR  := build
BINDIR    := bin
TARGET    := $(PROJECT_NAME)
SOURCES   := $(shell find $(SRCDIR) -name '*.c')
OBJECTS   := $(patsubst $(SRCDIR)/%.c,$(BUILDDIR)/%.o,$(SOURCES))
DEPS      := $(patsubst $(SRCDIR)/%.c,$(BUILDDIR)/%.d,$(SOURCES))
CFLAGS    := -Wall -g
LIB       := -lSDL2 -lSDL2_ttf -lSDL2_image -lSDL2_mixer

# Platform-specific settings
ifeq ($(OS),Windows_NT)
    PLATFORM := windows
    LIB += -lgdi32
else
    PLATFORM := linux
    LIB += -lX11 -lXext
endif

GREEN=`tput setaf 2`
RESET=`tput sgr0`

define print_green
	@echo "$(GREEN)$(1)$(RESET)"
endef

all: $(TARGET)

clean:
	rm -rf $(BUILDDIR) $(TARGET)

$(TARGET): $(BUILDDIR) $(OBJECTS)
	$(call print_green,"Linking object files...")
	@$(CC) $(OBJECTS) -o $(TARGET) $(LIB)
	$(call print_green,"$(TARGET) has been created!")

$(BUILDDIR) :
	mkdir -p $(BUILDDIR)
	
$(BUILDDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c -o $@ $<
	@$(CC) $(CFLAGS) -M $< -MT $@ > $(@:.o=.td)
	@cp $(@:.o=.td) $(@:.o=.d); 
	@sed -e 's/#.*//' -e 's/^[^:]*: *//' -e 's/ *\\$$//' \
	-e '/^$$/ d' -e 's/$$/ :/' < $(@:.o=.td) >> $(@:.o=.d); 
	@rm -f $(@:.o=.td)

-include $(DEPS)

.PHONY: clean all
