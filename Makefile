CXX = g++

CXXFLAGS = -Wall -std=c++17

SRC = $(wildcard src/*.cpp)

BIN = grapevine

INCLUDES = "./libs/"

LIBS = SDL2 SDL2_image SDL2_ttf SDL2_mixer lua

LINKFLAGS = $(addprefix -l,$(LIBS))

build: $(SRC)
	$(CXX) $(CXXFLAGS) -I$(INCLUDES) -o $(BIN) $(SRC) $(LINKFLAGS)

run:
	./$(BIN)

clean:
	rm $(BIN)
