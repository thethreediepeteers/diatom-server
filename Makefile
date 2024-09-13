CC := gcc
CXX := g++
CXXFLAGS := -std=c++20 -g -Wall
LIBFLAGS := -lz lib/uSockets.a
INCLUDES := -Iinclude/ -Isrc/
SRC_DIR := src
RELEASE_TARGET := server
DEBUG_TARGET := debug_server

SRC_FILES := $(wildcard $(SRC_DIR)/main.cpp $(SRC_DIR)/modules/*.cpp $(SRC_DIR)/components/*.cpp)
OBJ_FILES := $(SRC_FILES:.cpp=.o)
C_FILES := include/hshg/hshg.c
C_OBJ_FILES := $(C_FILES:.c=.o)

all: $(RELEASE_TARGET) $(DEBUG_TARGET)

$(RELEASE_TARGET) $(DEBUG_TARGET): $(OBJ_FILES) $(C_OBJ_FILES)
	$(CXX) $(CXXFLAGS) -DNDEBUG -O3 $(OBJ_FILES) $(C_OBJ_FILES) -o $(RELEASE_TARGET) $(LIBFLAGS)
	$(CXX) $(CXXFLAGS) $(OBJ_FILES) $(C_OBJ_FILES) -o $(DEBUG_TARGET) $(LIBFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

%.o: %.c
	$(CC) $(INCLUDES) -c $< -o $@

clean:
	rm -rf $(TARGET) $(OBJ_FILES)
