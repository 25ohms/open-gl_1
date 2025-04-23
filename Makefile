CXX := g++
BUILD_MODE ?= release

ifeq ($(BUILD_MODE),debug)
	CXXFLAGS := -std=c++17 -Wall -Wextra -O0 -g -Iinclude -Idependencies/GLEW/include -Idependencies/GLFW/include
else
	CXXFLAGS := -std=c++17 -Wall -Wextra -O2 -Iinclude -Idependencies/GLEW/include -Idependencies/GLFW/include
endif

LDFLAGS := -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo

SRC_DIR := src
OBJ_DIR := obj
INC_DIR := include

SRCS := $(wildcard $(SRC_DIR)/*.cpp)
OBJS := $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRCS))

TARGET := app

.PHONY: all clean debug release

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $@ $(LDFLAGS) dependencies/GLEW/lib/libGLEW.a -Ldependencies/GLFW/lib-arm64 -lglfw3

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

debug:
	$(MAKE) BUILD_MODE=debug

clean:
	rm -rf $(OBJ_DIR) $(TARGET)
