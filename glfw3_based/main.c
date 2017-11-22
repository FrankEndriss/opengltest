
#include <GL/gl.h>
#include <GLFW/glfw3.h>

#include <stdio.h>


/** Function called to render the current screen.
 * Seems to be called 60 times per second.
 */
static void render(GLFWwindow *window) {
	static int count=0;
	double t=glfwGetTime();
	fprintf(stdout, "in render(window), count=%d time=%f\n", count++, t);
}

static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %d %s\n", error, description);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    fprintf(stdout, "key_callback, key=%d scancode=%d action=%d mods=%d\n", key, scancode, action, mods);

//    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
//        glfwSetWindowShouldClose(window, GLFW_TRUE);
}
int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwSetErrorCallback(error_callback);

    /* set minimum required opengl version */
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

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
