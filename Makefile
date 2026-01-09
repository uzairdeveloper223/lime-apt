CC = clang
CFLAGS = -Wall -Wextra -O2

SRC_DIR = src
BIN_DIR = bin
OBJ_DIR = obj

TARGET = $(BIN_DIR)/lime-apt
SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS))

.PHONY: all clean install

all: $(TARGET)

$(TARGET): $(OBJS) | $(BIN_DIR)
	$(CC) $(OBJS) -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BIN_DIR) $(OBJ_DIR):
	mkdir -p $@

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)/lime-apt

install: $(TARGET)
	install -m 755 $(TARGET) /usr/local/bin/lime-apt
