CXX = g++
CXXFLAGS = -std=c++23 -Wall -Wextra \
    -I/opt/homebrew/Cellar/raylib/5.5/include \
    -I./include/raylib-cpp

LDFLAGS = -L/opt/homebrew/Cellar/raylib/5.5/lib -lraylib \
    -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo

SRC_DIR = src
SRC = $(wildcard $(SRC_DIR)/*.cpp)
OBJ = $(SRC:.cpp=.o)
TARGET = main

$(TARGET): $(OBJ)
	$(CXX) $(OBJ) -o $@ $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)

# Web
WEB_CC = emcc
WEB_FLAGS = -std=c++23 -Wall -Wextra \
    -I./raylib/src \
    -I./include/raylib-cpp \
    -DPLATFORM_WEB \
    -s USE_GLFW=3 \
    -s ASYNCIFY \
    -s WASM=1 \
    -s TOTAL_MEMORY=67108864 \
    --preload-file assets \
    --shell-file ./template.html

WEB_SRC = $(wildcard $(SRC_DIR)/*.cpp)
RAYLIB_WEB_LIB = ./raylib/src/libraylib.web.a

web: $(RAYLIB_WEB_LIB)
	mkdir -p web
	$(WEB_CC) $(WEB_SRC) $(WEB_FLAGS) $(RAYLIB_WEB_LIB) -o web/index.html && cp -r assets web/assets

$(RAYLIB_WEB_LIB):
	cd ./raylib/src && emmake make PLATFORM=PLATFORM_WEB
