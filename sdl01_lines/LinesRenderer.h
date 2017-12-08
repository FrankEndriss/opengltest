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

	/** this will call program->link(...) for all used attributes in the renderer. */
	GLint link(Program *program);

	void renderLogOnce();

	/** Renders some lines into window. */
	void render(Program* program);
private:
	class Impl;
	Impl* impl;
};

#endif /* LINESRENDERER_H_ */
