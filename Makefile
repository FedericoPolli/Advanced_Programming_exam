EXE = main.x
BENCH = benchmark.x
CXX = c++
CXXFLAGS = -Wall -Wextra -std=c++14 -I include 
SRC = main.cpp
OBJ = $(SRC.cpp=.o)
INC = include/bst.hpp

all: $(EXE) $(BENCH)

%.o: %.cpp
	$(CXX) -c $< -o $@ $(CXXFLAGS)

%.x: %.o
	$(CXX) -o $@ $<

main.o: $(INC)

benchmark.o: $(INC)

clean:
	rm -f $(EXE) $(BENCH) *.o *~

.PHONY: all clean

