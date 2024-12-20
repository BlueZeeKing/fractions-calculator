CC=g++
STD=c++23

build/%.o: src/%.cpp
	@mkdir -p build
	@$(CC) -MM -MT $@ $< > build/$*.d
	$(CC) -c -g -o $@ $< -std=$(STD)

build/calculator: build/main.o build/Fraction.o build/FractionBuilder.o build/Expression.o
	$(CC) -o $@ $^ -g -std=$(STD) -lncursesw -ltinfo

build/test_fraction: build/test_fraction.o build/Fraction.o build/Expression.o
	$(CC) -o $@ $^ -g -std=$(STD) -lncursesw -ltinfo

-include build/*.d

.PHONY: test all clean

test: build/test_fraction
	./build/test_fraction

all: build/test_fraction build/main

clean:
	rm -rf build
