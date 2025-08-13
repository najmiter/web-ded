CXX = g++
CXXFLAGS = -std=c++23 -Wall -Wextra \
    -I/opt/homebrew/Cellar/raylib/5.5/include \
    -I/opt/homebrew/include \
    -I./include/raylib-cpp

LDFLAGS = -L/opt/homebrew/Cellar/raylib/5.5/lib -lraylib \
    -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo

SRC_DIR = src
SRC = src/main.cpp $(wildcard $(SRC_DIR)/*.cpp)
OBJ = $(SRC:.cpp=.o)
TARGET = main

$(TARGET): $(OBJ)
	$(CXX) $(OBJ) -o $@ $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)

# WEB
WEB_CC = emcc
WEB_FLAGS = -std=c++23 -Wall -Wextra \
    -I./raylib/src \
    -I./include/raylib-cpp \
    -DPLATFORM_WEB \
    ./raylib/src/libraylib.web.a \
    -s USE_GLFW=3 \
    -s ASYNCIFY \
    -s WASM=1 \
    --preload-file assets

SRC = src/main.cpp

web:
	$(WEB_CC) $(SRC) $(WEB_FLAGS) -o web/index.html && cp -r assets web/assets