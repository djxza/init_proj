# The interpreter executable
INTERPRETER = ../../bin/proj_interpreter

# The project directory should be explicitly defined and not rely on external definitions
PROJDIR = .proj

# Fetch language, source, and binary directories
LANG   = $(shell $(INTERPRETER) $(PROJDIR)/handle.json lang)
SRCDIR = $(shell $(INTERPRETER) $(PROJDIR)/handle.json src)
BINDIR = $(shell $(INTERPRETER) $(PROJDIR)/handle.json bin)

# Ensure BINDIR and SRCDIR are valid
$(shell mkdir -p $(BINDIR) $(SRCDIR))

# Handle library directory
LIBDIR = ./lib
ifeq ($(shell $(INTERPRETER) $(PROJDIR)/handle.json libs), [])
       EXISTS_LIBDIR = 0
       PRINT_LIB = none
else 
       EXISTS_LIBDIR = 1
       PRINT_LIB = $(LIBDIR)
       HANDLE_LIBS = @mkdir -p $(LIBDIR)
endif

# Handle include directory
ifeq ($(shell $(INTERPRETER) $(PROJDIR)/handle.json incdir), false)
       EXISTS_INCDIR = 0
       INCDIR = $(SRCDIR)
       PRINT_INC = none
else 
       EXISTS_INCDIR = 1
       INCDIR = $(shell $(INTERPRETER) $(PROJDIR)/handle.json incdir)
       INCFLAGS = -I$(INCDIR)
       PRINT_INC = $(INCDIR)
endif

# Compiler settings based on language and version
ifeq ($(LANG), c++)
       EXT = .cpp

       IS23 = 0

       # Determine compiler
       CC = $(shell $(INTERPRETER) $(PROJDIR)/handle.json CC)++
       ifeq ($(CC),)
               # default to clang++ if not specified
               CC = clang++
       endif

       # Handle language version
       ifeq ($(shell $(INTERPRETER) $(PROJDIR)/handle.json langver), 23)
               IS23 = 1
               LANGVER_CODE = 2b
       endif

       ifeq ($(shell $(INTERPRETER) $(PROJDIR)/handle.json langver), 20)
               LANGVER_CODE = 2a
       else
               ifeq ($(IS23), 0)
                       LANGVER_CODE = $(shell $(INTERPRETER) $(PROJDIR)/handle.json langver)
               endif
       endif

else
       # C specific code
       EXT = .c
       LANGVER = c
       
       ifeq ($(shell $(INTERPRETER) $(PROJDIR)/handle.json langver), 20)
              LANGVER_CODE = 2x
       else
              LANGVER_CODE = $(shell $(INTERPRETER) $(PROJDIR)/handle.json langver)
       endif

       # Determine C compiler
       CC = $(shell $(INTERPRETER) $(PROJDIR)/handle.json CC)
       ifeq ($(CC),)
               # default to clang if not specified
               CC = clang
       endif
endif

# Construct language version flag
LANGVER = -std=$(LANG)$(LANGVER_CODE)

# Compiler flags
CCFLAGS  = $(LANGVER)
CCFLAGS += $(shell $(INTERPRETER) $(PROJDIR)/handle.json flags)
CCFLAGS += $(INCFLAGS)

# Include directories
INCFLAGS += -I./src

# Linker flags
LDFLAGS += -lm -lpthread -ldl

ifeq ($(LANG), c++)
       LDFLAGS += -lstdc++
endif

# Source files and corresponding object files
SRC = $(shell find $(SRCDIR) -name "*$(EXT)")
OBJ = $(SRC:$(SRCDIR)/%$(EXT)=$(BINDIR)/%.o)

default: all

all: build

# Info target to display project details
info:
       @echo "init_proj displays app info:"
       @echo "Project: $(shell $(INTERPRETER) $(PROJDIR)/handle.json name)"
       @echo "Author: $(shell $(INTERPRETER) $(PROJDIR)/handle.json author)"
       @echo "Version: $(shell $(INTERPRETER) $(PROJDIR)/handle.json version)"
       @echo "Repo: $(shell $(INTERPRETER) $(PROJDIR)/handle.json repo)"
       @echo "License: $(shell $(INTERPRETER) $(PROJDIR)/handle.json license)"
       @echo "Description: $(shell $(INTERPRETER) $(PROJDIR)/handle.json description)"
       @echo "Libraries: $(shell $(INTERPRETER) $(PROJDIR)/handle.json libs)"
       @echo "Source files: $(SRC)"
       @echo "CC: $(CC)"
       @echo "All flags: $(CCFLAGS)"
       @echo "SRCDIR: $(SRCDIR)"
       @echo "LIBDIR: $(LIBDIR)"
       @echo "INCDIR: $(PRINT_INC)"
       @echo "LIBDIR: $(PRINT_LIB)"
       @echo "INTERPRETER: $(INTERPRETER)"

# Object file rule
$(BINDIR)/%.o: $(SRCDIR)/%$(EXT)
	@mkdir -p $(dir $@)
	$(CC) $(CCFLAGS) -c $< -o $@

# Build target
build: $(OBJ)
	@echo "Building project..."
	$(CC) $(CCFLAGS) -o $(BINDIR)/$(shell $(INTERPRETER) $(PROJDIR)/handle.json executable) $(OBJ) $(LDFLAGS)
	@echo "Build complete! Executable is in $(BINDIR)"

# Clean target
clean:
	rm -f $(BINDIR)/*.o
	rm -f $(BINDIR)/$(shell $(INTERPRETER) $(PROJDIR)/handle.json executable)