
#include <EGL/egl.h>

int main(int argc, char** argv) {

	EGLDisplay display=eglGetDisplay(EGL_DEFAULT_DISPLAY);
	if(display==EGL_NO_DISPlAY) {
		cout << "no display, exit(1)" << endl;
		exit(1);
	}

	return 0;
}
