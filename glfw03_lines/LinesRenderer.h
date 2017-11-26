/*
 * LinesRenderer.h
 *
 *  Created on: 26.11.2017
 *      Author: frank
 */

#ifndef LINESRENDERER_H_
#define LINESRENDERER_H_

#include "Renderer.h"

class LinesRenderer: public Renderer {
public:
	LinesRenderer();
	virtual ~LinesRenderer();

	void setLineStarts(float startOfFirstLineX, float startOfFirstLineY, float startOfLastLineX, float startOfLastLineY);
	void setLineEnds(float endOfFirstLineX, float endOfFirstLineY, float endOfLastLineX, float endOfLastLineY);
	void setNumLines(int number);

	/** Renders some lines into window. */
	void render(GLFWwindow* window);
};

#endif /* LINESRENDERER_H_ */
