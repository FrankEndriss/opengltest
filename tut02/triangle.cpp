/* Using standard C++ output libraries */
#include <cstdlib>
#include <iostream>
using namespace std;

/* Use glew.h instead of gl.h to get all the GL prototypes declared */
#include <GL/glew.h>
/* Using SDL2 for the base window and OpenGL context init */
#include <SDL.h>

#include "shader_utils.h"

// the program created from the shaders
GLuint program;

// the attribute location of 
GLint attribute_coord2d;

// VBO (VertexBufferObject) for vertex data (the triangles)
GLuint vbo_triangle;

bool init_resources(void) {
	GLint compile_ok = GL_FALSE, link_ok = GL_FALSE;

	GLuint vs, fs;
	if ((vs = create_shader("triangle.v.glsl", GL_VERTEX_SHADER))   == 0) return false;
	if ((fs = create_shader("triangle.f.glsl", GL_FRAGMENT_SHADER)) == 0) return false;

// now create the program from the shaders
	program = glCreateProgram();
	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	glGetProgramiv(program, GL_LINK_STATUS, &link_ok);
	if (!link_ok) {
		cerr << "Error in glLinkProgram" << endl;
		print_log(program);
		return false;
	}

	const char* attribute_name = "coord2d";
	attribute_coord2d = glGetAttribLocation(program, attribute_name);
	if (attribute_coord2d == -1) {
		cerr << "Could not bind attribute " << attribute_name << endl;
		return false;
	}

	// init the VBO holding the triangel vertices data
	GLfloat triangle_vertices[] = {
	    0.0,  0.8,
	   -0.8, -0.8,
	    0.8, -0.8,
	};
	glGenBuffers(1, &vbo_triangle);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_triangle);

	// copy the data to the buffer,and hence to the graphic device
	// If the data changes once per frame or more often, you could use GL_DYNAMIC_DRAW or GL_STREAM_DRAW.
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangle_vertices), triangle_vertices, GL_STATIC_DRAW);

	return true;
}

static int renderCounter=0;
static int increment=1;

void render(SDL_Window* window) {
	/* Clear the background as white-to-black fading. */
	if(renderCounter>100)
		increment=-1;
	else if(renderCounter<0)
		increment=1;
	renderCounter+=increment;

	float c=1.0-(1.0*renderCounter/100);
	glClearColor(c, c, c, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(program);

  	glBindBuffer(GL_ARRAY_BUFFER, vbo_triangle);
	glEnableVertexAttribArray(attribute_coord2d);
/*
	float m=renderCounter/100.0;
	GLfloat triangle_vertices[] = {
	    0.0,  0.8*m,
	   -0.8, -0.8,
	    0.8, -0.8,
	};
*/
	// TODO move the 0.8*m to the VBO

	/* Describe our vertices array to OpenGL (it can't guess its format automatically) */
	glVertexAttribPointer(
		attribute_coord2d, // attribute
		2,                 // number of elements per vertex, here (x,y)
		GL_FLOAT,          // the type of each element
		GL_FALSE,          // take our values as-is
		0,                 // no extra data between each position
		0		   // offset of first element
	);
	
	/* Push each element in buffer_vertices to the vertex shader */
	glDrawArrays(GL_TRIANGLES, 0, 3);
	
	glDisableVertexAttribArray(attribute_coord2d);

	/* Display the result */
	SDL_GL_SwapWindow(window);
}

void free_resources() {
	glDeleteProgram(program);
	glDeleteBuffers(1, &vbo_triangle);
}

void mainLoop(SDL_Window* window) {
	while (true) {
		SDL_Event ev;
		while (SDL_PollEvent(&ev)) {
			if (ev.type == SDL_QUIT)
				return;
		}
		render(window);
	}
}

int main(int argc, char* argv[]) {
	/* SDL-related initialising functions */
	SDL_Init(SDL_INIT_VIDEO);
	SDL_Window* window = SDL_CreateWindow("My First Triangle",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		640, 480,
		SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);
	SDL_GL_CreateContext(window);

	/* Extension wrangler initialising */
	GLenum glew_status = glewInit();
	if (glew_status != GLEW_OK) {
		cerr << "Error: glewInit: " << glewGetErrorString(glew_status) << endl;
		return EXIT_FAILURE;
	}

	/* When all init functions run without errors,
	   the program can initialise the resources */
	if (!init_resources())
		return EXIT_FAILURE;

	/* We can display something if everything goes OK */
	mainLoop(window);

	/* If the program exits in the usual way,
	   free resources and exit with a success */
	free_resources();
	return EXIT_SUCCESS;
}
