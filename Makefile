CC = g++

LANG_STD = -std=c++17

COMPILER_FLAGS = -Wall -Wfatal-errors

SRC = $(shell find ./src -name "*.cpp")

BIN = grapevine

INCLUDES = "./libs/"

LIBS = SDL2 SDL2_image SDL2_ttf SDL2_mixer lua

LINKFLAGS = $(addprefix -l,$(LIBS))

build: $(SRC)
	$(CC) $(COMPILER_FLAGS) $(LANG_STD) -I$(INCLUDES) -o $(BIN) $(SRC) $(LINKFLAGS)

run:
	./$(BIN)

br: build run

clean:
	rm $(BIN)
