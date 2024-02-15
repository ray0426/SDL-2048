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

# Specific source files for the task
SRC_MAIN := main.cpp \
			src/LTexture.cpp \
			src/LWindow.cpp \
			src/game_controller.cpp \
			src/game_2048_standard.cpp \
			src/game_2048_change.cpp
SRC_MAIN_2048 := 	main-2048.cpp \
					src/game_2048_standard.cpp


all: main main-2048

main: $(EXEC_MAIN) copy-assets

main-2048: $(EXEC_MAIN_2048)

$(EXEC_MAIN): $(SRC_MAIN)
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS_MAIN) -o $@ $(SRC_MAIN) $(LDFLAGS_MAIN)

$(EXEC_MAIN_2048): $(SRC_MAIN_2048)
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS_MAIN_2048) -o $@ $(SRC_MAIN_2048) $(LDFLAGS_MAIN_2048)

copy-assets:
	cp -r assets $(BUILD_DIR)/
	cp -r $(DLL_DIR)/*.dll $(BUILD_DIR)/

clean:
	rm -rf $(BUILD_DIR)

.PHONY: all clean copy-assets main main-2048