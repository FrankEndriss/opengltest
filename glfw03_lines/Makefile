
CXX=g++ -std=c++11 -Wall
LD=${CXX}

OBJ=main.o Renderer.o LinesRenderer.o Shader.o

all : lines

lines : ${OBJ}
	${LD} `pkg-config --cflags glfw3 gl` -o lines ${OBJ} `pkg-config --libs glfw3 gl`

clean :
	rm -f lines ${OBJ}
