/*
 * Shader.cpp
 *
 *  Created on: 29.11.2017
 *      Author: frank
 */

#include <GLES2/gl2.h>
#include "Shader.h"
#include <malloc.h>

#include <iostream>
#include <fstream>
#include <string>

Shader::Shader(GLenum type) {
	this->shader=glCreateShader(type);
}

Shader::~Shader() {
	glDeleteShader(this->shader);
}

using namespace std;

GLint Shader::compileFile(char* path) {
	streampos begin, end;
	ifstream ifs(path, ios::binary);

	if(!ifs.is_open())
		return -1;

	begin = ifs.tellg();
	ifs.seekg(0, ios::end);
	end = ifs.tellg();
	int size=end-begin;
	char *source=(char*)malloc(size+1);

	ifs.seekg(0);
	ifs.read(source, size);
	source[size]='\0';
	GLint ret=compile(&source, 1);

	free(source);
	return ret;
}

GLint Shader::compile(char** source, int lineCount) {
	glShaderSource(shader, lineCount, source, NULL);
	glCompileShader(shader);
	GLint ret;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &ret);
	return ret;
}

void Shader::dumpCompileInfo(std::ostream out) {
	GLint len;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);
	GLchar* str=(GLchar*)malloc(sizeof(GLchar)*len);
	glGetShaderInfoLog(shader, len, &len, str);
	out << str << std::endl;
}

