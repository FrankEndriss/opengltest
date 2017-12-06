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

static GLint ATTRIB_LOC_COORD3D=0;
void LinesRenderer::bindAttribLocations(Program* program) {
//#define ATTRIB_LOC_COORD3D (1)
//	program->bindAttribLocation(ATTRIB_LOC_COORD3D, "coord3d");
	//program->bindAttribLocation(ATTRIB_LOC_COORD3D, "coord3d");
	ATTRIB_LOC_COORD3D=program->getAttribLocation("coord3d");
	INFO << "ATTRIB_LOC_CORRD3D="<<ATTRIB_LOC_COORD3D;
}

void LinesRenderer::renderLogOnce() {
	IMPL->onceLog=true;
}

static float redValue=0.5f;
static float redValueIncrement=0.01f;

void LinesRenderer::render(Program* program) {
	IMPL->renderCallCount++;
	if(IMPL->onceLog) {
		INFO << "in LinesRenderer::render, calls: "<<IMPL->renderCallCount;
		IMPL->onceLog=false;
	}

	//int width, height;
	//glfwGetFramebufferSize(window, &width, &height);
	//glViewport(0, 0, width, height);
	glClearColor(0.3f, 0.1f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	program->use();

	// set the uniform "mainColor" to some more or less random value
	GLint uLocation=program->uniformLocation(string("mainColor").c_str());
	if(redValue>=0.99f || redValue<=0.01f)
		redValueIncrement=-redValueIncrement;
	redValue+=redValueIncrement;
	glUniform4f(uLocation, redValue, 0.3, 0.2, 1.0);

	// see tut02 for how to use glBindBuffer() and glBufferData()
	// that makes it possible to not send the vertex data on every frame,
	// but only once.

	// send the vertices to openGL
	glEnableVertexAttribArray(ATTRIB_LOC_COORD3D);
	glVertexAttribPointer(ATTRIB_LOC_COORD3D, 3, GL_FLOAT, GL_FALSE, 0, IMPL->vertices);
	glDrawArrays(GL_TRIANGLES,  0, 3);
	glDisableVertexAttribArray(ATTRIB_LOC_COORD3D);
}
