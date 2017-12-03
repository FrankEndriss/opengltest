/*
 * LinesRenderer.cpp
 */

#include "LinesRenderer.h"
#include <malloc.h>
#include <iostream>
using namespace std;

//int i=5;


// private implementation
class LinesRendererImpl {
public:
	GLfloat vertices[9];

	LinesRendererImpl() {
		static GLfloat lV[] = {
			0.0,	0.8, 	0.0,
			-0.8,	-0.8,	0.0,
			0.8,	-0.8,	0.0
		};
		cout<<"sizeof(vertices)=="<<sizeof(vertices)<<endl;
		cout<<"sizeof(lV)=="<<sizeof(lV)<<endl;
		for(uint i=0; i<sizeof(vertices)/sizeof(GLfloat); i++)
			vertices[i]=lV[i];
	}
};


LinesRenderer::LinesRenderer() : Renderer() {
	impl=new LinesRendererImpl();
#define IMPL ((LinesRendererImpl*)impl)
}

void LinesRenderer::bindAttribLocations(Program* program) {
#define ATTRIB_LOC_COORD3D (0)
	program->bindAttribLocation(ATTRIB_LOC_COORD3D, "coord3d");
}

LinesRenderer::~LinesRenderer() {
	delete IMPL;
}

void LinesRenderer::render(GLFWwindow* window, Program* program) {
	//cout << "in LinesRenderer::render" << endl;
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);
	glClearColor(0.0f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	program->use();

	glVertexAttribPointer(ATTRIB_LOC_COORD3D, 3, GL_FLOAT, GL_FALSE, 0, IMPL->vertices);
	glEnableVertexAttribArray(ATTRIB_LOC_COORD3D);
	glDrawArrays(GL_TRIANGLES,  0, 3);
}
