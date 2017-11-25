
OBJ=main.o

all : myProg

myProg : ${OBJ}
	g++ -std=c++11 `pkg-config --cflags glfw3` -o myProg ${OBJ} `pkg-config --libs glfw3` -lGL

clean :
	rm -f myProg ${OBJ}
