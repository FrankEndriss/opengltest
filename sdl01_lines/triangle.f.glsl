#version 100

// example of a uniform
precision mediump float;
uniform vec4 mainColor;

void main(void) {
	//gl_FragColor = mainColor;
	gl_FragColor = vec4(mainColor.r, 0.5, 0.7, 1.0);
}

