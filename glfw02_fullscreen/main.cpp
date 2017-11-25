
#include <GL/gl.h>
#include <GLFW/glfw3.h>

#include <iostream>

using namespace std;

/** Function called to render the current screen.
 * Seems to be called 60 times per second.
 */
static void render(GLFWwindow *window) {
	// better dont render anything until we know what to do ;)
}

static void error_callback(int error, const char* description)
{
    cerr << "Error: " << error << " " << description << endl;
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    cout << "key_callback, key="<<key<<" scancode="<<scancode<<" action="<<action<<" mods="<<mods<<endl;

    if(key==GLFW_KEY_F && action==GLFW_PRESS) { // user pressed "f" key, switches screen modes
	// TODO implement the switch as described in
	// http://www.glfw.org/docs/latest/window_guide.html#window_monitor
    } else /* exit application */
    	glfwSetWindowShouldClose(window, GL_TRUE);
}

int main(int argc, char** argv)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwSetErrorCallback(error_callback);

    /* set minimum required glfw version */
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwSetKeyCallback(window, key_callback);

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    /* Make swaping on every hsync. (60fps). Swap-Interval 0 makes up to 4000fps. */
    glfwSwapInterval(1);

    /* Set the viewport for opengl, use the whole window space. */
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)) {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);
	render(window);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
