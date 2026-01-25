# Ref: gcc -o writer writer.c
# Ref: https://web.stanford.edu/class/archive/cs/cs107/cs107.1174/guide_make.html
# Ref: https://makefiletutorial.com/


# Generate application when CROSS_COMPILE not specificed on the make command line
# When CROSS_COMPLIE specified with aarch64-none-linux-gnu-, makefile should compile
#    successfully using cross complier specified in Step 1. 


# Application name
TARGET := writer


# Compiler & Linker
# If CROSS_COMPILE not inputed, CC & LD = gcc
CC := $(CROSS_COMPILE)gcc
LD := $(CROSS_COMPILE)gcc


# Compile flags
# CFLAGS := -g -Wall


# Source files (writer.c)
SRCS := writer.c


# Object (.o) files to create from .c
OBJS := $(SRCS:.c=.o)


# Default target
.PHONY: all
all: $(TARGET)


# Link .o to executable
$(TARGET): $(OBJS)
	$(LD) $(OBJS) -o $@

# Complile .c into .o
%.o: %.c
	$(CC) -c $< -o $@


# Clean up build files (remove "writer" application and all .o files)
.PHONY: clean
clean:
	rm -f $(TARGET) $(OBJS)


# Build natively   (BASH): make
# To cross-compile (BASH): make CROSS_COMPILE=aarch64-none-linux-gnu-
#                  (BASH): make clean
