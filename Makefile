EXE = main.x
CXX = c++
CXXFLAGS = -Wall -Wextra -std=c++14 -I include
SRC = main.cpp
OBJ = $(SRC.cpp=.o)
INC = include/bst.hpp

all: $(EXE)

%.o: %.cpp
	$(CXX) -c $< -o $@ $(CXXFLAGS) -g

%.x: %.o
	$(CXX) -o $@ $<

main.o: $(INC)

clean:
	rm -f $(EXE) *.o *~

.PHONY: all clean

