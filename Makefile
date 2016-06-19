CXX=g++
# CXX=clang++
# CXXFLAGS=-g -std=c++11 -lpthread -Wall -Ofast -march=native -flto
CXXFLAGS=-g -std=c++11 -lpthread -Wall -Ofast -march=native -flto -fomit-frame-pointer
# CXXFLAGS=-g -std=c++11 -lpthread -Wall -O2 -march=native
# CXXFLAGS=-g -std=c++11 -lpthread -Wall -O3
# CXXFLAGS=-g -std=c++11 -lpthread -Wall -O3 -march=native -flto
# CXXFLAGS=-g -lpthread -std=c++11 -Wall -O2

BIN=bin/raytracer

SRC=$(wildcard *.cpp)
OBJ=$(SRC:%.cpp=%.o)

all: $(OBJ)
	$(CXX) $(CXXFLAGS) -o $(BIN) $^

allfast: $(OBJ)
	$(CXX) $(CXXFLAGSFAST) -o $(BIN) $^

run: clean all
# run: all
	$(BIN)

# runfast: allfast
runfast: clean allfast
	$(BIN)

%.o: %.c
	$(CXX) $@ -c $<

clean:
	rm -f *.o
	rm -f $(BIN)
