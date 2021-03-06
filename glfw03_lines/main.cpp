#include "gl_include.h"

#include <cstdlib>
#include <iostream>

#include "Shader.h"
#include "Program.h"
#include "LinesRenderer.h"
#include "logging.h"

using namespace std;

/** logs msg, does glfwTerminate() and exit(EXIT_FAILURE) */
static void fail(const char *msg) {
	FATAL<<"FATAL, will exit(EXIT_FAILURE): "<<msg;
	glfwTerminate();
	exit(EXIT_FAILURE);
}

/** GLFW error callback */
static void error_callback(int error, const char* description) {
	ERROR<< "Error: "<<error<<" "<<description;
}

/** Flag is set to denote that a switch to fullscreen or vice versa should happen.
 * To do such a switch the window must be re-created (up to glfw 3.1.2), which must
 * not be done from a handler, but from the main loop. (not reentrant).
 * In glfw 3.2.1 the switch can be done on one window, see:
 * http://www.glfw.org/docs/latest/window_guide.html#window_monitor
 **/
static bool switchFullscreen = false;

static void key_callback(GLFWwindow* window, int key, int scancode, int action,
		int mods) {
	INFO<<"key_callback, key="<< key << " scancode=" << scancode
			<< " action=" << action << " mods=" << mods;

	if (action != GLFW_PRESS)
		return;	// ignore Key-release

	if (key == GLFW_KEY_F && action == GLFW_PRESS) { // user pressed "f" key, switches screen modes
		switchFullscreen = true;
	} else if (key == GLFW_KEY_G && action == GLFW_PRESS) { // user pressed "g" key
		// ************* display current monitor values
		auto monitor = glfwGetWindowMonitor(window);
		if (monitor != NULL) {
			auto mode = glfwGetVideoMode(monitor);
			INFO << "Running in fullscreen, monitor values: ";
			INFO << "with: " << mode->width << " height:" << mode->height << " refreshRate:" << mode->refreshRate;
		} else {
			INFO << "cant determine WindowMonitor...most likely we run in windowed mode.";
			INFO << "querying monitors...";
			int mCount;
			auto monitors = glfwGetMonitors(&mCount);
			for (int i = 0; i < mCount; i++) {
				auto mode = glfwGetVideoMode(monitors[i]);
				INFO << "monitor[" << i << "] with: " << mode->width
						<< " height:" << mode->height << " refreshRate:"
						<< mode->refreshRate;
			}
		}
	} else if (key == GLFW_KEY_R && action == GLFW_PRESS) { // user pressed "r" key
		((Renderer*)glfwGetWindowUserPointer(window))->renderLogOnce();
	} else { /* exit application */
		INFO << "other key, will close window";
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
}

static void initWindow(GLFWwindow* window) {
	INFO << "in initWindow";
	/* Make the window's context current */
	glfwMakeContextCurrent(window);
	/* Make swaping on every hsync. (60fps). Swap-Interval 0 makes up to 4000fps. */
	glfwSwapInterval(1);

	glfwSetKeyCallback(window, key_callback);

	/* Set the viewport for opengl, use the whole window space. */
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	INFO << "setting viewport size, w=" << width << " h=" << height;
	glViewport(0, 0, width, height);
}

/** Closed window and opens a new one, returns the new one.
 * if window was in fullscreen mode, the new one is in
 * windowed mode.
 * if window is in windowed mode, the new one is in fullscreen mode.
 * The associated GL-Context is made current.
 */
GLFWwindow* switchFullscreenMode(GLFWwindow* window) {
	// see http://www.glfw.org/docs/latest/window_guide.html#window_monitor

	glfwDestroyWindow(window);

	auto fullscreen = glfwGetWindowMonitor(window) != NULL;
	if (!fullscreen) {
		auto monitor = glfwGetPrimaryMonitor();
		if (monitor == NULL) {
			fail("cant determine primary monitor, exit(EXIT_FAILURE)");
		}
		const GLFWvidmode* mode = glfwGetVideoMode(monitor);
		if (mode == NULL)
			fail("cant determine video mode of primary monitor");
		window = glfwCreateWindow(mode->width, mode->height, "Hello World",
				monitor, NULL);
	} else {
		window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
	}
	initWindow(window);
	return window;
}

int main(int argc, char** argv) {
	/* Initialize GLFW first. */
	if (!glfwInit())
		return -1;

	glfwSetErrorCallback(error_callback);

	/* set minimum required glfw version */
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);

	bool fullscreen = false;

	// very simple argument parsing
	if (argc > 1) {
		string fOption = string("-f");
		for (int i = 1; i < argc; i++)
			if (fOption == string(argv[i]))
				fullscreen = true;
	}

	GLFWwindow* window = NULL;
	if (fullscreen) { /* Create fullscreen window */
		auto monitor = glfwGetPrimaryMonitor();
		if (monitor == NULL)
			fail("cant determine primary monitor");
		auto mode = glfwGetVideoMode(monitor);
		if (mode == NULL)
			fail("cant determine mode of primary monitor");
		window = glfwCreateWindow(mode->width, mode->height, "Hello World",
				monitor, NULL);
	} else
		/* Create a windowed mode window and its OpenGL context */
		TRACE << "will create window";
		window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
		TRACE << "did create window";

	if (!window)
		fail("cant create window");

	initWindow(window);
	TRACE << "after initWindow";

	// create and compile the Shaders, then link the program
	TRACE << "will create new Program()";
	Program* program=new Program();
	TRACE << "did create new Program(), will create new LinesRenderer()";
	Renderer* renderer=new LinesRenderer();
	glfwSetWindowUserPointer(window, renderer);
	TRACE << "did create new LinesRenderer(), will loadAndCompileShaderSet()";

	// TODO move these statics to Renderer or separate class, or Program
	static string shaderName="triangle";
	static GLint attrLocs[]={
			0
	};
	static const char* attrNames[]={
			"coord3d"
	};

	if(program->loadAndCompileShaderSet(shaderName.c_str())) {
		TRACE << "loadAndCompile succeded";
		if(program->link(attrLocs, attrNames, 1)) {
			TRACE << "link succeded";
			program->use();
			TRACE << "use succeded";
		} else
			exit(EXIT_FAILURE);
	}

	// exit for DEBUG
	//exit(EXIT_SUCCESS);

	// TODO
	// switch to SDL2 instead of glfw
	// see example "second" and
	// https://www.libsdl.org/


	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window)) {
		/* if fullscreen mode switch should be done, then do it now. */
		if (switchFullscreen)
			window = switchFullscreenMode(window);
		switchFullscreen = false;

		//glClear(GL_COLOR_BUFFER_BIT);

		/* Render here */
		renderer->render(window, program);

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	glfwTerminate();
	WARN <<"finished, EXIT_SUCCESS";
	exit(EXIT_SUCCESS);
}
