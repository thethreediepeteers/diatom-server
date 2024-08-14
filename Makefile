CXX := g++
CXXFLAGS := -std=c++20 -g -fsanitize=address -Wall -Werror
LIBFLAGS := -Llib/ -luSockets -lz
INCLUDES := -Iinclude/ -Isrc/
SRC_DIR := src
TARGET := server

SRC_FILES := ${wildcard ${SRC_DIR}/main.cpp ${SRC_DIR}/modules/*.cpp ${SRC_DIR}/components/*.cpp}
OBJ_FILES := ${SRC_FILES:.cpp=.o}

all: ${TARGET}

${TARGET}: ${OBJ_FILES}
	${CXX} ${CXXFLAGS} ${OBJ_FILES} -o ${TARGET} ${LIBFLAGS}

%.o: %.cpp
	${CXX} ${CXXFLAGS} ${INCLUDES} -c $< -o $@

clean:
	rm -rf ${TARGET} ${OBJ_FILES}
