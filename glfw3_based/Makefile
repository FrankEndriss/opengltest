
OBJ=main.o

all : myProg

myProg : ${OBJ}
	g++ `pkg-config --cflags glfw3` -o myProg ${OBJ} `pkg-config --libs glfw3` -lGL
