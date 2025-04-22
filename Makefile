# Compiler and flags
CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra \
						-DGLEW_STATIC \
            -Idependencies/GLFW/include \
            -Idependencies/GLEW/include

# Direct static linking to libGLEW.a
LDFLAGS := dependencies/GLEW/lib/libGLEW.a \
           -Ldependencies/GLFW/lib-arm64 -lglfw3 \
           -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo

# Sources and targets
SRC := $(wildcard src/*.cpp)
OBJ := $(SRC:.cpp=.o)
TARGET := app

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(OBJ) -o $@ $(LDFLAGS)

src/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f src/*.o $(TARGET)
