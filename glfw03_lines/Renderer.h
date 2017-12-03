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
	virtual void render(GLFWwindow* window, Program* program)=0;

	/** this will call program->bindAttribLocation(int, char*) for all used attributes of the renderer. */
	virtual void bindAttribLocations(Program *program)=0;
};

#endif /* RENDERER_H_ */
