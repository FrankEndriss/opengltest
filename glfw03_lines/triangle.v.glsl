#version 100

//uniform ivec3 mainColor;

attribute vec3 coord3d;

void main(void) {
	gl_Position = vec4(coord3d, 1.0);
}
