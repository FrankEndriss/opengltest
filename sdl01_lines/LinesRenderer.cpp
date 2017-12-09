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
#define FLOATS_PER_LINE 10
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
#define IDXLINESTART(line) (line*FLOATS_PER_LINE)

	Impl() {
		vertices=(GLfloat*)malloc(sizeof(GLfloat)*NUM_LINES*FLOATS_PER_LINE);

		for(int i=0; i<NUM_LINES*FLOATS_PER_LINE; i++)
			vertices[i]=0.0f;

		// init colors of all vertices
		for(int line=0; line<NUM_LINES; line++) {
			vertices[IDXLINESTART(line) + OFFSET1_B]=1.0f;
			vertices[IDXLINESTART(line) + OFFSET2_R]=1.0f;
		}

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
	// moving vector of colors
	float speed1_R=0.0037;
	float speed1_G=0.0041;
	float speed1_B=0.0013;
	float speed2_R=0.00313;
	float speed2_G=0.00432;
	float speed2_B=0.00231;

	int firstLineIdx=0;

	// Color of all vertices for mode where all lines have one and
	// the same color.

	//GLfloat singleColor[3]={ 1.0f, 1.0f, 1.0f };

	/** if abs(value)>=1 delta is multiplied by -1 */
	GLfloat calcAndFlip(GLfloat value, GLfloat *delta, GLfloat min, GLfloat max) {
		float ret=value+(*delta);

		if(ret<min) {
			ret=min;
			(*delta)=-(*delta);
		} else if(ret>max) {
			ret=max;
			(*delta)=-(*delta);
		}
		return ret;
	}

	/** if abs(value)>=1 delta is multiplied by -1 */
	GLfloat calcAndFlipCol(float value, float *delta) {
		return calcAndFlip(value, delta, 0.0, 1.0);
	}

	GLfloat calcAndFlipCoo(float value, float *delta) {
		return calcAndFlip(value, delta, -1.0, 1.0);
	}




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
		vertices[IDXLINESTART(newFirstLineIdx)+OFFSET1_X] = calcAndFlipCoo(vertices[IDXLINESTART(firstLineIdx)+OFFSET1_X], &speed1_X);
		vertices[IDXLINESTART(newFirstLineIdx)+OFFSET1_Y] = calcAndFlipCoo(vertices[IDXLINESTART(firstLineIdx)+OFFSET1_Y], &speed1_Y);

		// second dot of new first line
		vertices[IDXLINESTART(newFirstLineIdx)+OFFSET2_X] = calcAndFlipCoo(vertices[IDXLINESTART(firstLineIdx)+OFFSET2_X], &speed2_X);
		vertices[IDXLINESTART(newFirstLineIdx)+OFFSET2_Y] = calcAndFlipCoo(vertices[IDXLINESTART(firstLineIdx)+OFFSET2_Y], &speed2_Y);

		// color of first dot
		vertices[IDXLINESTART(newFirstLineIdx)+OFFSET1_R] = calcAndFlipCol(vertices[IDXLINESTART(firstLineIdx)+OFFSET1_R], &speed1_R);
		vertices[IDXLINESTART(newFirstLineIdx)+OFFSET1_G] = calcAndFlipCol(vertices[IDXLINESTART(firstLineIdx)+OFFSET1_G], &speed1_G);
		vertices[IDXLINESTART(newFirstLineIdx)+OFFSET1_B] = calcAndFlipCol(vertices[IDXLINESTART(firstLineIdx)+OFFSET1_B], &speed1_B);

		// color of second dot
		vertices[IDXLINESTART(newFirstLineIdx)+OFFSET2_R] = calcAndFlipCol(vertices[IDXLINESTART(firstLineIdx)+OFFSET2_R], &speed2_R);
		vertices[IDXLINESTART(newFirstLineIdx)+OFFSET2_G] = calcAndFlipCol(vertices[IDXLINESTART(firstLineIdx)+OFFSET2_G], &speed2_G);
		vertices[IDXLINESTART(newFirstLineIdx)+OFFSET2_B] = calcAndFlipCol(vertices[IDXLINESTART(firstLineIdx)+OFFSET2_B], &speed2_B);

		firstLineIdx=newFirstLineIdx;
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
			"color3d"
	};
	return program->link(attributesLocs, attributeNames, 2);
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
	//glClearColor(0.3f, 0.1f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	//program->use();

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

	//glVertexAttrib3f(ATTRIB_LOC_COLOR3, impl->singleColor[0], impl->singleColor[1], impl->singleColor[2]);


	// single Color mode

	// send the vertices to openGL, coordinates
	glVertexAttribPointer(ATTRIB_LOC_COORD2D, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*FLOATS_PER_LINE/2, impl->vertices);

	// send the vertices to openGL, colors
	glVertexAttribPointer(ATTRIB_LOC_COLOR3, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*FLOATS_PER_LINE/2, ((impl->vertices)+OFFSET1_R));

	glEnableVertexAttribArray(ATTRIB_LOC_COORD2D);
	glEnableVertexAttribArray(ATTRIB_LOC_COLOR3);
	glDrawArrays(GL_LINES,  0, NUM_LINES*2);
	glDisableVertexAttribArray(ATTRIB_LOC_COORD2D);
	glDisableVertexAttribArray(ATTRIB_LOC_COLOR3);
}
