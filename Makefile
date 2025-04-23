CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -g -Iinclude -Idependencies/GLEW/include -Idependencies/GLFW/include
LDFLAGS := -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo

SRC_DIR := src
OBJ_DIR := obj
INC_DIR := include

SRCS := $(wildcard $(SRC_DIR)/*.cpp)
OBJS := $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRCS))

TARGET := app

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $@ $(LDFLAGS) dependencies/GLEW/lib/libGLEW.a -Ldependencies/GLFW/lib-arm64 -lglfw3

# Rule to compile .cpp to .o into obj/
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Ensure obj directory exists
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

clean:
	rm -rf $(OBJ_DIR) $(TARGET)
