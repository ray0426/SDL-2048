# Compiler and flags
CC := g++
# Flags for main
CFLAGS_MAIN := -I src/include -L src/lib
LDFLAGS_MAIN := -lmingw32 -L/mingw64/bin -lSDL_bgi -lSDL2_ttf -lSDL2_mixer -lSDL2_image -lSDL2main -lSDL2

# Flags for main-2048 (without SDL libraries and include/lib paths)
CFLAGS_MAIN_2048 :=
LDFLAGS_MAIN_2048 := -lmingw32 -L/mingw64/bin

# Directories
BUILD_DIR := build
DLL_DIR := dll

# Executables
EXEC_MAIN := $(BUILD_DIR)/main
EXEC_MAIN_2048 := $(BUILD_DIR)/main-2048

all: main main-2048

main: $(EXEC_MAIN) copy-assets

main-2048: $(EXEC_MAIN_2048)

$(EXEC_MAIN): main.cpp
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS_MAIN) -o $@ main.cpp src/*.cpp $(LDFLAGS_MAIN)

$(EXEC_MAIN_2048): main-2048.cpp
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS_MAIN_2048) -o $@ main-2048.cpp src/*.cpp $(LDFLAGS_MAIN_2048)

copy-assets:
	cp -r assets $(BUILD_DIR)/
	cp -r $(DLL_DIR)/*.dll $(BUILD_DIR)/

clean:
	rm -rf $(BUILD_DIR)

.PHONY: all clean copy-assets main main-2048