/*
 * LinesRenderer.cpp
 */

#include "LinesRenderer.h"
#include <malloc.h>
#include "logging.h"
#include <iostream>
using namespace std;

// private implementation
class LinesRendererImpl {
public:
	GLfloat *vertices;

	LinesRendererImpl() {
		static GLfloat lV[] = {
			0.0,	0.8, 	0.0,
			-0.8,	-0.8,	0.0,
			0.8,	-0.8,	0.0
		};
		int numFloats=sizeof(lV)/sizeof(GLfloat);
		vertices=(GLfloat*)malloc(sizeof(GLfloat)*numFloats);

		for(int i=0; i<numFloats; i++)
			vertices[i]=lV[i];
	}

	~LinesRendererImpl() {
		free(vertices);
	}

	bool onceLog=false;
	uint renderCallCount=0;
};

LinesRenderer::LinesRenderer() : Renderer() {
	impl=new LinesRendererImpl();
#define IMPL ((LinesRendererImpl*)impl)
}

LinesRenderer::~LinesRenderer() {
	delete IMPL;
}

void LinesRenderer::bindAttribLocations(Program* program) {
#define ATTRIB_LOC_COORD3D (0)
	program->bindAttribLocation(ATTRIB_LOC_COORD3D, "coord3d");
}

void LinesRenderer::renderLogOnce() {
	IMPL->onceLog=true;
}

void LinesRenderer::render(GLFWwindow* window, Program* program) {
	IMPL->renderCallCount++;
	if(IMPL->onceLog) {
		INFO << "in LinesRenderer::render, calls: "<<IMPL->renderCallCount;
		IMPL->onceLog=false;
	}

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
