CXX := g++
LIB_FLAGS := -lusockets -lz

build: src/main.cpp
	$(CXX) src/main.cpp src/modules/*.cpp src/components/*.cpp -o server -std=c++20 $(LIB_FLAGS)

chase: src/main.cpp
	$(CXX) src/main.cpp src/modules/*.cpp src/components/*.cpp -o server -std=c++20 -luSockets -lz

clean:
	rm server
