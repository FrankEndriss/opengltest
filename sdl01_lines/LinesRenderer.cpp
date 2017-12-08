/*
 * LinesRenderer.cpp
 */

#include "LinesRenderer.h"
#include <malloc.h>
#include "logging.h"
#include <iostream>
using namespace std;

// private implementation
class LinesRenderer::Impl {
public:
	GLfloat *vertices;
// number of display lines
#define NUM_LINES 50
#define VERTICES_PER_LINE 2
#define FLOATS_PER_VERTEX 5
#define OFFSET1_X 0
#define OFFSET1_Y 1
#define OFFSET1_R 2
#define OFFSET1_G 3
#define OFFSET1_B 4
#define OFFSET2_X 5
#define OFFSET2_Y 6
#define OFFSET2_R 7
#define OFFSET2_G 8
#define OFFSET2_B 9

	Impl() {
		vertices=(GLfloat*)malloc(sizeof(GLfloat)*FLOATS_PER_VERTEX*NUM_LINES*VERTICES_PER_LINE);

		for(int i=0; i<NUM_LINES*FLOATS_PER_VERTEX*VERTICES_PER_LINE; i++)
			vertices[i]=0.0f;
	}

	~Impl() {
		free(vertices);
	}

	bool onceLog=false;
	uint renderCallCount=0;

	// moving vector of line starts
	float speed1_X=0.02;
	float speed1_Y=0.037;
	float speed2_X=0.017;
	float speed2_Y=0.013;
	int firstLineIdx=0;

	/** step moves the last line in the buffer
	 * to the location of the first line plus
	 * the moving vector.
	 * Then it sets the Index so that the last line is
	 * the new first line. This gives the impression of a moving line.
	 */
	void step() {
		int newFirstLineIdx=firstLineIdx+1;
		if(newFirstLineIdx>=NUM_LINES)
			newFirstLineIdx=0;

		// first dot of new first line
		int x1=vertices[newFirstLineIdx*FLOATS_PER_VERTEX*VERTICES_PER_LINE+OFFSET1_X]=vertices[firstLineIdx*FLOATS_PER_VERTEX*VERTICES_PER_LINE+OFFSET1_X]+speed1_X;
		int y1=vertices[newFirstLineIdx*FLOATS_PER_VERTEX*VERTICES_PER_LINE+OFFSET1_Y]=vertices[firstLineIdx*FLOATS_PER_VERTEX*VERTICES_PER_LINE+OFFSET1_Y]+speed1_Y;

		// second dot of new first line
		int x2=vertices[newFirstLineIdx*FLOATS_PER_VERTEX*VERTICES_PER_LINE+OFFSET2_X]=vertices[firstLineIdx*FLOATS_PER_VERTEX*VERTICES_PER_LINE+OFFSET2_X]+speed2_X;
		int y2=vertices[newFirstLineIdx*FLOATS_PER_VERTEX*VERTICES_PER_LINE+OFFSET2_Y]=vertices[firstLineIdx*FLOATS_PER_VERTEX*VERTICES_PER_LINE+OFFSET2_Y]+speed2_Y;

		firstLineIdx=newFirstLineIdx;

		// if collision with viewport border change direction
		if(x1>=1. || x1<=-1.)
			speed1_X*=-1;
		if(y1>=1. || y1<=-1.)
			speed1_Y*=-1;
		if(x2>=1. || x2<=-1.)
			speed2_X*=-1;
		if(y2>=1. || y2<=-1.)
			speed2_Y*=-1;
	}
};

LinesRenderer::LinesRenderer() : Renderer() {
	impl=new LinesRenderer::Impl();
}

LinesRenderer::~LinesRenderer() {
	delete impl;
}

#define ATTRIB_LOC_COORD2D	0
#define ATTRIB_LOC_COLOR3	1
GLint LinesRenderer::link(Program* program) {
	static GLuint attributesLocs[]={
			ATTRIB_LOC_COORD2D,
			ATTRIB_LOC_COLOR3
	};
	static string attributeNames[]={
			"coord2d",
			"color3"
	};
	// TODO send the colors, too
	return program->link(attributesLocs, attributeNames, 1);
}

void LinesRenderer::renderLogOnce() {
	impl->onceLog=true;
}

void LinesRenderer::render(Program* program) {
	impl->renderCallCount++;
	if(impl->onceLog) {
		INFO << "in LinesRenderer::render, calls: "<<impl->renderCallCount;
		impl->onceLog=false;
	}

	impl->step();

	//int width, height;
	//glfwGetFramebufferSize(window, &width, &height);
	//glViewport(0, 0, width, height);
	glClearColor(0.3f, 0.1f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	program->use();

	// set the uniform "mainColor" to some more or less random value
/*
	GLint uLocation=program->uniformLocation(string("mainColor").c_str());
	if(redValue>=0.99f || redValue<=0.01f)
		redValueIncrement=-redValueIncrement;
	redValue+=redValueIncrement;
	glUniform4f(uLocation, redValue, 0.3, 0.2, 1.0);
*/

	// see tut02 for how to use glBindBuffer() and glBufferData()
	// that makes it possible to not send the vertex data on every frame,
	// but only once.

	// send the vertices to openGL
	glEnableVertexAttribArray(ATTRIB_LOC_COORD2D);
	glVertexAttribPointer(ATTRIB_LOC_COORD2D, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*FLOATS_PER_VERTEX, impl->vertices);

	glDrawArrays(GL_LINES,  0, NUM_LINES*VERTICES_PER_LINE);
	glDisableVertexAttribArray(ATTRIB_LOC_COORD2D);
}
