
#include <iostream>
using namespace std;

#include <GL/gl.h>
#include <GL/glext.h>

#include <GLFW/glfw3.h>
//#include <GL3/gl3w.h>
#include "vgl.h"
#include "LoadShaders.h"

enum VAO_IDs	{ Triangles,	NumVAOs };
enum Buffer_IDs	{ ArrayBuffer,	NumBuffers};
enum Attrib_IDs	{ vPosition=0 };

#define NumVertices 6

GLuint VAOs[NumVAOs];
GLuint Buffers[NumBuffers];

void init(void) {
	static const GLfloat vertices[NumVertices][2] = {
		{ -0.90, -0.90 }, // Tri 1
		{  0.85, -0.90 },
		{ -0.90,  0.85 },
		{  0.90, -0.85 }, // Tri 2
		{  0.90,  0.90 },
		{ -0.85,  0.90 }
	};

	glCreateVertexArrays(NumVAOs, VAOs);

	glCreateBuffers(NumBuffers, Buffers);
	glNamedBufferStorage(Buffers[ArrayBuffer], sizeof(vertices), vertices, 0);

	ShaderInfo shaders[] = {
		{ GL_VERTEX_SHADER, "triangels.vert" },
		{ GL_FRAGMENT_SHADER, "triangels.frag" },
		{ GL_NONE, NULL }
	};

	GLuint program = LoadShaders(shaders);
	glBindVertexArray(VAOs[Triangles]);
	glBindBuffer(GL_ARRAY_BUFFER, Buffers[ArrayBuffer]);
	glVertexAttribPointer(vPosition, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	glEnableVertexAttribArray(vPosition);
}

void display() {
	static const float black[]={ 0.0f, 0.0f, 0.0f, 0.0f };

	glClearBufferfv(GL_COLOR, 0, black);
	glBindVertexArray(VAOs[Triangles]);
	glDrawArrays(GL_TRIANGLES, 0, NumVertices);
}

int main(int argc, char** argv)  {
	glfwInit();

	GLFWwindow* window=glfwCreateWindow(640, 480, "Tri", NULL, NULL);

	glfwMakeContextCurrent(window);
	//gl3wInit();

	init();
	while(!glfwWindowShouldClose(window)) {
		display();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();
}
