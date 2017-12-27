/*
 * Program.cpp
 */

#include "Program.h"
#include <cstring>
#include <malloc.h>
#include "Shader.h"
#include <iostream>
using namespace std;
#include "logging.h"

Program::Program() {
	program=glCreateProgram();
	uniforms=0;
}

Program::~Program() {
	glDeleteProgram(program);

	for(int i=0; i<uniformCount; i++)
		free(uniforms[i].name);
	free(uniforms);
}

static GLuint compileShader(const string& path, Shader& shader) {
	cout<<"will Shader()->compileFile("<<path<<")"<<endl;
	const GLuint res=shader.compileFile(path);
	if(!res) { // compilation failed
		cout<<"Shader()->compileFile("<<path<<") failed"<<endl;
		shader.dumpCompileInfo();
	} else
		cout<<"Shader()->compileFile("<<path<<") succeeded"<<endl;

	return res;
}

/** Postfix names for Shader source files */
static string pathPostfixF=".f.glsl";
static string pathPostfixV=".v.glsl";

GLuint Program::loadAndCompileShaderSet(const string& pathPrefix) {
	Shader fShader(GL_FRAGMENT_SHADER);
	const GLuint res=compileShader(pathPrefix+pathPostfixF, fShader);
	if(!res) {
		return res;
	}

	Shader vShader(GL_VERTEX_SHADER);
	const GLuint res2=compileShader(pathPrefix+pathPostfixV, vShader);
	if(!res2) { // compilation failed
		return res2;
	}

	glAttachShader(program, vShader.shader);
	glAttachShader(program, fShader.shader);

	return res;
}

GLint Program::link(GLuint* attribLocations, string* attribNames, int count) {

	for(int i=0; i<count; i++) {
		glBindAttribLocation(program, attribLocations[i], attribNames[i].c_str());
		INFO << "bound attribute: "<< attribNames[i] << " to location "<< attribLocations[i];
	}

	glLinkProgram(program);
	GLint ret;
	glGetProgramiv(program, GL_LINK_STATUS, &ret);
	cout << "link status: "<<ret<<endl;
	if(!ret) { // dump linker info
		GLint infoLen=0;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLen);
		if(infoLen>1) {
			char* infoLog=(char*)malloc(sizeof(char)*infoLen);
			glGetProgramInfoLog(program, infoLen, NULL, infoLog);
			cout<<"Error linking program"<<endl<<infoLog<<endl;
		}
	}
	return ret;
}

GLint Program::uniformLocation(const char* name) {
	if(uniformCount==-1) { // need to initialize uniform table
		glGetProgramiv(program, GL_ACTIVE_UNIFORMS, &uniformCount);
		if(uniformCount<1)
			return -1;

		GLint maxNameLen;
		glGetProgramiv(program, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxNameLen);

		uniforms=(tag_uniform*)malloc(sizeof(tag_uniform)*uniformCount);
		for(int i=0; i<uniformCount; i++) {
			uniforms[i].name=(char*)malloc(maxNameLen);
			glGetActiveUniform(program, i, maxNameLen, NULL, &uniforms[i].size, &uniforms[i].type, uniforms[i].name);
			uniforms[i].location=glGetUniformLocation(program, uniforms[i].name);
			INFO << "uniform location for "<<uniforms[i].name<<"="<<uniforms[i].location;
		}
	}

	// query the uniform table
	for(int i=0; i<uniformCount; i++)
		if(string(name).compare(uniforms[i].name)==0)
			return uniforms[i].location;

	ERROR << "uniform location for "<<name<<" unknown!!!";
	return -1; // doesnt exist
}

void Program::use() {
	glUseProgram(program);
}
