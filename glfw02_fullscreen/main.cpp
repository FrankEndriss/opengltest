#include <GL/gl.h>
#include <GLFW/glfw3.h>

#include <cstdlib>
#include <iostream>

using namespace std;

/** dumps msg to stdout, does glfwTerminate() and exit(FAILURE); */
static void fail(const char *msg) {
	cerr << msg << endl;
	glfwTerminate();
	exit(EXIT_FAILURE);
}

/** Function called to render the current screen.
 * Seems to be called 60 times per second.
 */
static void render(GLFWwindow *window) {
	// better dont render anything until we know what to do ;)
}

/** GLFW error callback */
static void error_callback(int error, const char* description) {
	cerr << "Error: " << error << " " << description << endl;
}

/** Flag is set to denote that a switch to fullscreen or vice versa should happen.
 * To do such a swith the window must be re-created (up to glfw 3.1.2), which must
 * not be done from a handler, but from the main loop. (not reentrant).
 * In glfw 3.2.1 the switch can be done on one window, see:
 * http://www.glfw.org/docs/latest/window_guide.html#window_monitor
 **/
static bool switchFullscreen = false;

static void key_callback(GLFWwindow* window, int key, int scancode, int action,
		int mods) {
	cout << "key_callback, key=" << key << " scancode=" << scancode
			<< " action=" << action << " mods=" << mods << endl;
	if (action != GLFW_PRESS)
		return;	// ignore Key-release

	if (key == GLFW_KEY_F && action == GLFW_PRESS) { // user pressed "f" key, switches screen modes
		switchFullscreen = true;
	} else if (key == GLFW_KEY_G && action == GLFW_PRESS) { // user pressed "g" key
		// ************* display current monitor values
		auto monitor = glfwGetWindowMonitor(window);
		if (monitor != NULL) {
			auto mode = glfwGetVideoMode(monitor);
			cout << "Running in fullscreen, monitor values: " << endl;
			cout << "with: " << mode->width << " height:" << mode->height
					<< " refreshRate:" << mode->refreshRate << endl;
		} else {
			cout
					<< "cant determine WindowMonitor...most likely we run in windowed mode."
					<< endl;
			cout << "querying monitors..." << endl;
			int mCount;
			auto monitors = glfwGetMonitors(&mCount);
			for (int i = 0; i < mCount; i++) {
				auto mode = glfwGetVideoMode(monitors[i]);
				cout << "monitor[" << i << "] with: " << mode->width
						<< " height:" << mode->height << " refreshRate:"
						<< mode->refreshRate << endl;
			}
		}
	} else { /* exit application */
		cout << "other key, will exit(EXIT_SUCCESS" << endl;
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
}

static void initWindow(GLFWwindow* window) {
	/* Make the window's context current */
	glfwMakeContextCurrent(window);
	/* Make swaping on every hsync. (60fps). Swap-Interval 0 makes up to 4000fps. */
	glfwSwapInterval(1);

	glfwSetKeyCallback(window, key_callback);

	/* Set the viewport for opengl, use the whole window space. */
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	cout << "setting viewport size, w=" << width << " h=" << height <<endl;
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
	/* Initialize the library */
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
			if (fOption == argv[i])
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
		window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);

	if (!window) {
		fail("cant create window, exit(FAILURE)");
	}

	initWindow(window);

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window)) {
		/* if fullscreen mode switch should be done, then do it now. */
		if (switchFullscreen)
			window = switchFullscreenMode(window);
		switchFullscreen = false;

		glClear(GL_COLOR_BUFFER_BIT);
		/* Render here */
		render(window);

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	glfwTerminate();
	exit(EXIT_SUCCESS);
}
