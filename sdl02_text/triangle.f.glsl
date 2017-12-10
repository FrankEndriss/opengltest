#version 100

// example of a uniform
precision mediump float;
uniform vec4 mainColor;

varying vec3 varColor;

void main(void) {
	//gl_FragColor = mainColor;
	gl_FragColor = vec4(varColor, 1.0);
}

