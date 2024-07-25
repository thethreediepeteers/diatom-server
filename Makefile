CXX := g++
CXXFLAGS := -std=c++20 -g
LIBFLAGS := -lusockets -lz -ljsoncpp #-lboost_system -lssl -lcrypto
INCLUDES := -Isrc/ #-L/usr/include/websocketpp -I/usr/include/boost
SRC_DIR := src
TARGET := server

SRC_FILES := ${wildcard ${SRC_DIR}/main.cpp ${SRC_DIR}/modules/*.cpp ${SRC_DIR}/components/*.cpp}
OBJ_FILES := ${SRC_FILES:.cpp=.o}

all: ${TARGET}

${TARGET}: ${OBJ_FILES}
	${CXX} ${CXXFLAGS} ${OBJ_FILES} -o ${TARGET} ${LIBFLAGS}

${OBJ_FILES}: src/config.h

%.o: %.cpp
	${CXX} ${CXXFLAGS} -c $< -o $@

clean:
	rm -rf ${TARGET} ${OBJ_FILES}
