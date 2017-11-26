
CXX=g++ -std=c++11 -Wall -I/home/frank/git/glfw/include
LD=${CXX}

OBJ=main.o

all : myProg

myProg : ${OBJ}
	${LD} `pkg-config --cflags glfw3 gl` -o myProg ${OBJ} `pkg-config --libs glfw3 gl`

clean :
	rm -f myProg ${OBJ}
