CXX := g++
LIB_FLAGS := -luSockets -lz

build: src/main.cpp
	$(CXX) src/main.cpp src/modules/*.cpp src/components/*.cpp -o server.o -std=c++20 $(LIB_FLAGS)

clean:
	rm server
