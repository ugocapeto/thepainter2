# Program name
PROGRAM = thepainter2

# Mode selection (debug or release)
# Usage: make MODE=release
# Default is debug
MODE ?= debug

# Compilers
CC = gcc
CXX = g++

# Mode-specific compiler flags
ifeq ($(MODE),debug)
  MODE_COMPILE_FLAGS = -g
  # Add any other debug-specific flags here, e.g., -DDEBUG_BUILD
else ifeq ($(MODE),release)
  MODE_COMPILE_FLAGS = -O2 -DNDEBUG # Optimization and disable asserts
  # Add any other release-specific flags here
else
  $(warning "Unknown MODE '$(MODE)'. Valid modes are 'debug' or 'release'. Defaulting to debug flags.")
  MODE_COMPILE_FLAGS = -g
endif

# Common static compiler flags (applied in all modes) and project includes
COMMON_STATIC_FLAGS = -Wall
PROJECT_INCLUDES = -I.. -I../../common

# C and C++ specific flags
CFLAGS = $(MODE_COMPILE_FLAGS) $(COMMON_STATIC_FLAGS) $(PROJECT_INCLUDES)
CXXFLAGS = $(MODE_COMPILE_FLAGS) $(COMMON_STATIC_FLAGS) $(PROJECT_INCLUDES) # Add C++ standard here if needed, e.g., -std=c++17

# Linker flags and libraries
LDFLAGS =
LDLIBS = -ltiff -lpng -ljpeg -lz -lm

# Object files managed by this Makefile
# If you add C++ source files (e.g., foo.cpp), add foo.o to main_objects.
main_objects = main.o

# Other object files (assumed to be pre-compiled or built by other Makefiles)
OTHER_OBJS = ../thepainter2.o \
             ../../common/io/io.o \
             ../../common/jpeg/jpeg.o \
             ../../common/math/math.o \
             ../../common/png/png.o \
             ../../common/tiff/tiff.o \
             ../../common/util/util.o

# Default target
all: $(PROGRAM)

# Linking the program
$(PROGRAM): $(main_objects) $(OTHER_OBJS)
	$(CXX) -o $@ $^ $(LDFLAGS) $(LDLIBS)

# Rule to compile C files
.c.o:
	$(CXX) -c $(CFLAGS) $< -o $@

# Rule to compile C++ files (e.g., .cpp)
.cpp.o:
	$(CXX) -c $(CXXFLAGS) $< -o $@

# Clean target
clean:
	rm -f $(PROGRAM) $(main_objects)
