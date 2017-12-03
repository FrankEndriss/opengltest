#version 100

// example of a uniform
precision mediump float;
uniform vec4 mainColor;

void main(void) {
	gl_FragColor = mainColor;
}

