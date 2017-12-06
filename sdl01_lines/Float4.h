/*
 * Float4.h
 *
 *  Created on: 02.12.2017
 *      Author: frank
 */

#ifndef FLOAT4_H_
#define FLOAT4_H_

#include "gl_include.h"

// TODO make a template class instead of using GLfloat
// then derive all useful data types from it, even matrices
class Float4 {
public:
	Float4() : mX(0.0), mY(0.0), mZ(0.0), mW(0.0) { }
	Float4(GLfloat x) : mX(x), mY(0.0), mZ(0.0), mW(0.0) { }
	Float4(GLfloat x, GLfloat y) : mX(x), mY(y), mZ(0.0), mW(0.0) { }
	Float4(GLfloat x, GLfloat y, GLfloat z) : mX(x), mY(y), mZ(z), mW(0.0) { }
	Float4(GLfloat x, GLfloat y, GLfloat z, GLfloat w) : mX(x), mY(y), mZ(z), mW(w) { }
	virtual ~Float4();

	GLfloat x() { return mX; }
	GLfloat y() { return mY; }
	GLfloat z() { return mZ; }
	GLfloat w() { return mW; }

	void get4(GLfloat vec4[]) {
		get4(vec4, 0);
	}

	void get4(GLfloat vecN[], int offset) {
		vecN[offset+0]=mX; vecN[offset+1]=mY; vecN[offset+2]=mZ; vecN[offset+3]=mW;
	}

private:
	GLfloat mX, mY, mZ, mW;

};

#endif /* FLOAT4_H_ */
