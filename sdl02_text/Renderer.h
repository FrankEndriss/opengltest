/*
 * Renderer.h
 * Interface for components rendering into GLFW windows.
 */

#ifndef RENDERER_H_
#define RENDERER_H_

#include "gl_include.h"
#include "Program.h"

class Renderer {
protected:
	Renderer();
	virtual ~Renderer();

public:
	/** Renders some stuff into window */
	virtual void render(Program* program)=0;

	/** this will call program->link(...) for all used attributes in the renderer. */
	virtual GLint link(Program *program)=0;

	/** makes that the next call to render(window, program) will log something to INFO */
	virtual void renderLogOnce()=0;
};

#endif /* RENDERER_H_ */
