CPP          = g++
WARNINGS     = -Wall -Wextra -Wno-sign-compare
SFML_LIBS    = -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -lsfml-network
SFML_INCLUDE = -I/usr/local/include -Iheader
SFML_LIB     = -L/usr/local/lib
COMPILEARG   = -c -O2 -g -fPIC $(SFML_INCLUDE)
TARGET       = main
SRC_FILES    = main.cpp Bullet.cpp Enemy.cpp Collider.cpp Player.cpp Tile.cpp World.cpp
OBJ_FILES    = $(SRC_FILES:.cpp=.o)

all: clean $(TARGET)

$(TARGET): $(OBJ_FILES)
	$(CPP) -o $@ $^ $(SFML_LIB) $(SFML_LIBS)

%.o: %.cpp
	$(CPP) $(WARNINGS) $(COMPILEARG) -c $< -o $@

clean:
	rm -f $(OBJ_FILES) $(TARGET)