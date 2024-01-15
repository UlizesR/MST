CC = clang
SRC = $(wildcard *.c)
BUILD_DIR = build

build_ex:
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $(SRC) -o $(BUILD_DIR)/main
	echo "Build complete"

run:
	./$(BUILD_DIR)/main