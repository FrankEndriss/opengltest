/*
 * LinesRenderer.h
 * Renders some lines which are defined basically by a rectangle in 3D.
 */

#ifndef LINESRENDERER_H_
#define LINESRENDERER_H_

#include "Renderer.h"
#include "Program.h"

class LinesRenderer: public Renderer {
public:
	LinesRenderer();
	virtual ~LinesRenderer();

	void setLineStarts(float startOfFirstLineX, float startOfFirstLineY, float startOfLastLineX, float startOfLastLineY);
	void setLineEnds(float endOfFirstLineX, float endOfFirstLineY, float endOfLastLineX, float endOfLastLineY);
	void setNumLines(int number);

	/** this will call program->bindAttribLocation(int, char*) for all used attributes in the renderer. */
	void bindAttribLocations(Program *program);

	void renderLogOnce();

	/** Renders some lines into window. */
	void render(GLFWwindow* window, Program* program);
private:
	void* impl;
};

#endif /* LINESRENDERER_H_ */
