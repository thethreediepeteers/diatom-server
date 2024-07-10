CXX := g++
LIB_FLAGS := -lusockets -lz

server: src/main.cpp
	$(CXX) -std=c++20 $(LIB_FLAGS) src/main.cpp src/modules/*.cpp src/components/*.cpp -o server

clean:
	rm server
