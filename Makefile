CC := gcc
CXX := g++
CXXFLAGS := -std=c++20 -Wall -Wextra

ifeq ($(RELEASE), 1)
CXXFLAGS += -O3 -march=native -flto -DNDEBUG
else
CXXFLAGS += -g3 -ggdb
endif

LIBFLAGS := -lz -lssl -lcrypto /home/lib/uSockets.a
INCLUDES := -Iinclude/ -Isrc/
SRC_DIR := src
TARGET := server

SRC_FILES := $(wildcard $(SRC_DIR)/main.cpp $(SRC_DIR)/modules/*.cpp $(SRC_DIR)/components/*.cpp)
OBJ_FILES := $(SRC_FILES:.cpp=.o)
C_FILES := include/hshg/hshg.c
C_OBJ_FILES := $(C_FILES:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJ_FILES) $(C_OBJ_FILES)
	$(CXX) $(CXXFLAGS) $(OBJ_FILES) $(C_OBJ_FILES) -o $(TARGET) $(LIBFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

%.o: %.c
	$(CC) $(INCLUDES) -c $< -o $@

clean:
	rm -rf $(TARGET) $(OBJ_FILES)
