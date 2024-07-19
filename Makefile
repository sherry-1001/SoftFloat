# SOURCE_DIR ?= .
# DELETE = rm -f
# C_INCLUDES = -I. -I$(SOURCE_DIR)/include
# COMPILE_C = gcc -c -Werror-implicit-function-declaration -DSOFTFLOAT_FAST_INT64 $(C_INCLUDES) -O2 -o $@
# MAKELIB = ar crs $@

# OBJ = .o
# LIB = .a

# .PHONY: all
# all: softfloat$(LIB)

# OBJS_ALL = \
#   fp32_add${OBJ}

# # 生成对象文件规则
# $(OBJS_ALL): %$(OBJ): $(SOURCE_DIR)/%.c  $(SOURCE_DIR)/include/softfloat.h
# 	$(COMPILE_C) $(SOURCE_DIR)/$*.c

# # 生成静态库规则
# softfloat$(LIB): $(OBJS_ALL)
# 	$(DELETE) $@
# 	$(MAKELIB) $^

# .PHONY: clean
# clean:
# 	$(DELETE) $(OBJS_ALL) softfloat$(LIB)


# Variables
SOURCE_DIR = src
INCLUDE_DIR = include
BUILD_DIR = build
TEST_DIR = test

DELETE = rm -f
C_INCLUDES = -I$(INCLUDE_DIR)
CFLAGS = -Werror-implicit-function-declaration -DSOFTFLOAT_FAST_INT64 -O2

# List of source files
SRC_FILES = $(wildcard $(SOURCE_DIR)/*.c)
OBJ_FILES = $(patsubst $(SOURCE_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRC_FILES))

# List of test files
TEST_SRC_FILES = $(wildcard $(TEST_DIR)/*.c)
TEST_OBJ_FILES = $(patsubst $(TEST_DIR)/%.c,$(BUILD_DIR)/%.o,$(TEST_SRC_FILES))
TEST_EXEC = $(BUILD_DIR)/test_exec

.PHONY: all clean test

# Default target
all: softfloat.a

# Build static library
softfloat.a: $(OBJ_FILES)
	$(DELETE) $@
	ar crs $@ $^

# Compile source files
$(BUILD_DIR)/%.o: $(SOURCE_DIR)/%.c
	$(CC) -c $(CFLAGS) $(C_INCLUDES) -o $@ $<

# Compile test files
$(BUILD_DIR)/%.o: $(TEST_DIR)/%.c
	$(CC) -c $(CFLAGS) $(C_INCLUDES) -o $@ $<

# Link test executable
$(TEST_EXEC): $(OBJ_FILES) $(TEST_OBJ_FILES)
	$(CC) -o $@ $^

# Run tests
test: $(TEST_EXEC)
	./$(TEST_EXEC)

# Clean build directory
clean:
	$(DELETE) $(OBJ_FILES) $(TEST_OBJ_FILES) softfloat.a $(TEST_EXEC)