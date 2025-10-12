# Compiler 
CC = clang
CFLAGS = -Wall -Wextra -g
TARGET = file_sorting
BUILD_DIR = build

SRCS = main.c 
OBJS = $(SRCS:%.c=$(BUILD_DIR)/%.o)

$(BUILD_DIR)/$(TARGET): $(OBJS) 
	$(CC) $(CFLAGS) $(OBJS) -o $(BUILD_DIR)/$(TARGET)

$(BUILD_DIR)/%.o: %.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

clean:
	rm -rf build

run: $(BUILD_DIR)/$(TARGET)
	$(BUILD_DIR)/$(TARGET)

.PHONY: clean run 
