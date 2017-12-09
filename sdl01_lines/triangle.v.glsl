#version 100

precision mediump float;
uniform vec4 mainColor;

attribute vec2 coord2d;
attribute vec3 color3d;

varying vec3 varColor;

void main(void) {
	gl_Position = vec4(coord2d, 0.0, 1.0);
	varColor = color3d;
}
