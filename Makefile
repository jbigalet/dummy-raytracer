CXX=g++
CXXFLAGS=-g -std=c++11 -Wall -Ofast -march=native -flto

BIN=bin/raytracer

SRC=$(wildcard *.cpp)
OBJ=$(SRC:%.cpp=%.o)

all: $(OBJ)
	$(CXX) $(CXXFLAGS) -o $(BIN) $^

run: clean all
	$(BIN)

%.o: %.c
	$(CXX) $@ -c $<

clean:
	rm -f *.o
	rm -f $(BIN)
