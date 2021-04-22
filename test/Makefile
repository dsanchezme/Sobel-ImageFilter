CFLAGS = `pkg-config --cflags opencv`
LIBS = `pkg-config --libs opencv`

all: test

test: test.cpp
	gcc -o test test.cpp -std=c++11 $(CFLAGS) $(LIBS) -lstdc++