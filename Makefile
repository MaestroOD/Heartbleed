CPP          = g++
WARNINGS     = -Wall -Wextra -Wno-sign-compare
SFML_LIBS    = -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -lsfml-network
SFML_INCLUDE = -I/usr/local/include -Iheader
SFML_LIB     = -L/usr/local/lib
COMPILEARG   = -c -O2 -g -fPIC $(SFML_INCLUDE) $(INCLUDE_PATH)
TARGET       = main
SRC_FILES    = ./src/main.cpp ./src/Bullet.cpp ./src/Enemy.cpp ./src/Collider.cpp ./src/Player.cpp ./src/Tile.cpp ./src/Stage.cpp ./src/ScreenStates.cpp
OBJ_FILES    = $(SRC_FILES:.cpp=.o)
INCLUDE_PATH = -I./include

all: clean $(TARGET)

$(TARGET): $(OBJ_FILES)
	$(CPP) -o $@ $^ $(SFML_LIB) $(SFML_LIBS) $(INCLUDE_PATH)

%.o: %.cpp
	$(CPP) $(COMPILEARG) -c $< -o $@

clean:
	rm -f $(OBJ_FILES) $(TARGET)