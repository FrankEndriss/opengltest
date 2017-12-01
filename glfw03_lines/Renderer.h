/*
 * Renderer.h
 * Interface for components rendering into GLFW windows.
 */

#ifndef RENDERER_H_
#define RENDERER_H_

#include "gl_include.h"

class Renderer {
protected:
	Renderer();
	virtual ~Renderer();

public:
	/** Renders some stuff into window */
	virtual void render(GLFWwindow* window)=0;
};

#endif /* RENDERER_H_ */
