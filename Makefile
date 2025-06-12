# List of object files
objects=\
compute_difference_image_hertzmann.o\
compute_difference_image_shiraishi_2.o\
compute_difference_image_shiraishi_3.o\
compute_difference_image_shiraishi.o\
compute_equivalent_rectangle.o\
compute_image_moment.o\
compute_rectangle_points.o\
compute_reference_image.o\
free_texture_arr.o\
gaussian_blur_image_2.o\
gaussian_blur_image.o\
gaussian_blur_rgb_image.o\
get_cell_median_color.o\
get_cell_dominant_color.o\
get_rgb_image_colors.o\
initialize_canvas_bumpity_image.o\
initialize_canvas_rgb_image.o\
load_texture_arr.o\
make_brush_stroke.o\
paint_brush_stroke.o\
paint_canvas.o\
paint_canvas_layer.o\
rotate_image.o\
rotate_image_cubic.o\
rotate_image_linear.o\
rotate_image_nearest_neighbor.o\
scale_image.o\
scale_image_cubic.o\
scale_image_linear.o\
scale_image_nearest_neighbor.o\
scale_rgb_image.o

# The target relocatable object file
PROGRAM = thepainter2.o

# C++ Compiler and flags
# (Used for both .cpp and .c files as per user request)
CXX = g++

# Build mode configuration
# To build for release: make MODE=release
# To build for debug (default): make or make MODE=debug
MODE ?= debug

# Common flags applied to both C and C++ compilations
COMMON_FLAGS = -Wall -I../common

# Define the info messages as a variable to be expanded in a target's recipe.
# This defers the execution of $(info) until the recipe is run.
define PRINT_BUILD_MODE_MESSAGES
  $(if $(filter release,$(MODE)),$(info Building in RELEASE mode with $(OPTIMIZATION_FLAGS)))
  $(if $(filter debug,$(MODE)),$(info Building in DEBUG mode with $(OPTIMIZATION_FLAGS)))
endef

# Mode-specific optimization flags
ifeq ($(MODE),release)
  OPTIMIZATION_FLAGS = -O2 # Common optimization level for release
  # Info message is now handled by the .PRINT_BUILD_MODE_INFO target
else ifeq ($(MODE),debug)
  OPTIMIZATION_FLAGS = -g
  # Info message is now handled by the .PRINT_BUILD_MODE_INFO target
else
  # This warning should always appear if MODE is unknown, regardless of the target.
  $(warning Unknown MODE '$(MODE)'. Defaulting to DEBUG build with -g.)
  OPTIMIZATION_FLAGS = -g # Default to debug flags for unknown modes
endif

CXXFLAGS = $(OPTIMIZATION_FLAGS) $(COMMON_FLAGS)

# C Compiler and flags (defined for completeness, but CXX/CXXFLAGS are used for .c files below)
CC = gcc
CFLAGS = $(OPTIMIZATION_FLAGS) $(COMMON_FLAGS)

# Linker for creating relocatable objects
LD = ld
LDRFLAGS = -r

# Phony target to print build mode information.
# Its recipe expands PRINT_BUILD_MODE_MESSAGES, causing the $(info) calls to execute.
.PHONY: .PRINT_BUILD_MODE_INFO
.PRINT_BUILD_MODE_INFO:
	$(PRINT_BUILD_MODE_MESSAGES)

# Default target: build the relocatable object
# .PHONY declares 'all' as a target that doesn't produce a file named 'all'.
.PHONY: all
all: .PRINT_BUILD_MODE_INFO $(PROGRAM)

# Rule to create the combined relocatable object file
# $^ refers to all prerequisites (the object files)
$(PROGRAM): $(objects)
	$(LD) $(LDRFLAGS) -o $@ $^

# Pattern rule to compile C++ source files to object files
# $< is the first prerequisite (the .cpp file)
# $@ is the target (the .o file)
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Pattern rule to compile C source files to object files
# Using C++ compiler (CXX) and CXXFLAGS as requested,
# so .c files will be compiled as C++.
%.o: %.c
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Target to clean up build artifacts
.PHONY: clean
clean:
	@echo "Cleaning..."
	rm -f $(objects) $(PROGRAM)
