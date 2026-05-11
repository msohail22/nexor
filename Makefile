CXX = g++
CXXFLAGS = -std=c++17

SRC = $(shell find src -name "*.cpp")
OUT = build

all:
	$(CXX) $(CXXFLAGS) $(SRC) -o $(OUT)

run: all
	./$(OUT)

clean:
	rm -f $(OUT)
