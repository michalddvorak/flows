CXXFLAGS = -Wall -pedantic -std=c++17 -Ofast -O3
PROGS = main

MAIN_FILES = $(shell grep -lr "int main" "./src/" | grep ".cpp$$")
OTHER_FILES = $(shell grep -Lr "int main" "./src/" | grep ".cpp$$")
OBJS = $(OTHER_FILES:./src/%.cpp=./build/%.o)

all: $(PROGS)

./build/%.o: ./src/%.cpp
	@mkdir -p $$(dirname $@)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

main : src/main.cpp $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@ -lgurobi_c++ -lgurobi95

clean:
	rm -f $(PROGS) $(OBJS)
