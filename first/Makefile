CC=gcc -DGL_GLEXT_PROTOTYPES -c
LD=g++
OBJECTS=first.o LoadShaders.o

all : first ${OBJECTS}
	${LD} -std=c++11 -o first ${OBJECTS} -lGLU -lGL -lglfw

clean : 
	rm -f first ${OBJECTS}

first.o : first.cpp LoadShaders.h
	${CC} first.cpp

LoadShaders.o : LoadShaders.cpp LoadShaders.h
	${CC} LoadShaders.cpp



