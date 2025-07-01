CXX = clang++
CXXFLAGS = -Wall -Wextra -std=c++17 -g3

SRC = main.cpp ListGraph.cpp SetGraph.cpp MatrixGraph.cpp ArcGraph.cpp Algs.cpp
OBJ = $(SRC:.cpp=.o)
OUT = output/main

all: $(OUT)

$(OUT): $(OBJ)
	@mkdir -p output
	$(CXX) $(CXXFLAGS) -o $(OUT) $(OBJ)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f *.o $(OUT)

.PHONY: all clean
