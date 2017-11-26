
#include <GL/gl.h>
#include <GLFW/glfw3.h>

#include <cstdlib>
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
    if(action!=GLFW_PRESS)
    	return;	// ignore Key-release

    if(key==GLFW_KEY_F && action==GLFW_PRESS) { // user pressed "f" key, switches screen modes
    	// see http://www.glfw.org/docs/latest/window_guide.html#window_monitor
    	const bool fullscreen= glfwGetWindowMonitor(window)!=NULL;
    	if(!fullscreen) {
    		GLFWmonitor *monitor=glfwGetPrimaryMonitor();
    		if(monitor==NULL) {
    			cerr<<"cant determine primary monitor, exit(EXIT_FAILURE)"<<endl;
    			exit(EXIT_FAILURE);
    		}
   			// const GLFWvidmode* mode = glfwGetVideoMode(monitor);
   			//glfwSetWindowMonitor(window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
   			cout<<"cannot switch to fullscreen mode on glfw 3.1.2 "<<endl;
    	} else {
    		// glfwSetWindowMonitor(window, NULL, 100, 100, 640, 480, 0);
   			cout<<"cannot switch to windowed mode on glfw 3.1.2"<<endl;
    	}
    } else if(key==GLFW_KEY_G && action==GLFW_PRESS) { // user pressed "g" key
    	// ************* display current monitor values
    	GLFWmonitor* monitor = glfwGetWindowMonitor(window);
    	if(monitor!=NULL) {
    		const GLFWvidmode* mode = glfwGetVideoMode(monitor);
    		cout << "Running in fullscreen, monitor values: " << endl;
    		cout << "with: "<<mode->width<<" height:"<<mode->height<<" refreshRate:"<<mode->refreshRate <<endl;
     	} else {
    		cout<<"cant determine WindowMonitor...most likely we run in windowed mode."<<endl;
    		cout<<"querying monitors..." <<endl;
    		int mCount;
    		GLFWmonitor** monitors=glfwGetMonitors(&mCount);
    		for(int i=0; i<mCount; i++) {
    			const GLFWvidmode* mode = glfwGetVideoMode(monitors[i]);
    			cout << "monitor["<<i<<"] with: "<<mode->width<<" height:"<<mode->height<<" refreshRate:"<<mode->refreshRate <<endl;
    		}
     	}
    } else /* exit application */
    	glfwSetWindowShouldClose(window, GL_TRUE);
}

/** dumps msg to stdout, does glfwTerminate() and exit(FAILURE); */
static void fail(const char *msg) {
	cerr<<msg<<endl;
    glfwTerminate();
	exit(EXIT_FAILURE);
}

int main(int argc, char** argv)
{

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwSetErrorCallback(error_callback);

    /* set minimum required glfw version */
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);

    bool fullscreen=false;

    // very simple argument parsing
    if(argc>1) {
    	string fOption=string("-f");
    	for(int i=1; i<argc; i++)
    		if(fOption==argv[1])
    			fullscreen=true;
    }

    GLFWwindow* window;
    if(fullscreen) { /* Create fullscreen window */
    	GLFWmonitor* monitor=glfwGetPrimaryMonitor();
    	if(monitor==NULL) fail("cant determine primary monitor");
		const GLFWvidmode* mode = glfwGetVideoMode(monitor);
    	if(mode==NULL) fail("cant determine mode of primary monitor");
    	window = glfwCreateWindow(mode->width, mode->height, "Hello World", monitor, NULL);
    } else /* Create a windowed mode window and its OpenGL context */
    	window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);

    if (!window) {
    	fail("cant create window, exit(FAILURE)");
    }

    glfwSetKeyCallback(window, key_callback);

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    /* Make swaping on every hsync. (60fps). Swap-Interval 0 makes up to 4000fps. */
    glfwSwapInterval(1);

    /* Set the viewport for opengl, use the whole window space. */
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    cout<<"setting viewport size, w="<<width<<" h="<<height;
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
    exit(EXIT_SUCCESS);
}
