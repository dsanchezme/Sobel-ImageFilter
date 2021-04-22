CFLAGS = `pkg-config --cflags opencv`
LIBS = `pkg-config --libs opencv`

all: practica

practica: practica.cpp
	gcc practica.cpp -o practica -std=c++11 $(CFLAGS) $(LIBS) -lstdc++ -lm

practicaHilos: practicaHilos.cpp
	gcc practicaHilos.cpp -o practicaHilos -std=c++11 $(CFLAGS) $(LIBS) -lstdc++ -lm -pthread
