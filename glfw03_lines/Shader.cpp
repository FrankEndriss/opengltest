/*
 * Shader.cpp
 *
 *  Created on: 29.11.2017
 *      Author: frank
 */

#include "gl_include.h"
#include "Shader.h"
#include <malloc.h>

#include <iostream>
#include <fstream>
#include <string>
#include <cstring>

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
	cout<<"size of file "<<path<<" is "<<size<< endl;
	char *source=(char*)malloc(size+1);
	memset(source, 0, size+1);

	ifs.seekg(0);
	ifs.read(source, size);
	cout << "compiling: "<<path<<endl;
	cout << "source:"<<endl;
	cout << source;
	GLint ret=compile(&source, 1);
	cout << "compile result: "<<ret<<endl;

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

//void Shader::dumpCompileInfo(std::ostream out) {
void Shader::dumpCompileInfo() {
	GLint len;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);
	if(len<=0) {
		std::cout<<"no compile info available" << endl;
		return;
	}
	GLchar* str=(GLchar*)malloc(sizeof(GLchar)*len);
	glGetShaderInfoLog(shader, len, &len, str);
	std::cout << str << std::endl;
	free(str);
}

