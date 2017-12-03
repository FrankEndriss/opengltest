/*
 * Program.cpp
 */

#include "Program.h"
#include <cstring>
#include <malloc.h>
#include "Shader.h"
#include <iostream>
using namespace std;

Program::Program() {
	program=glCreateProgram();
	fShader=NULL;
	vShader=NULL;
	new Shader(GL_VERTEX_SHADER);
}

Program::~Program() {
	if(fShader)
		delete fShader;
	if(vShader)
		delete vShader;

	glDeleteProgram(program);
}

const static char* pathPostfixF=".f.glsl";
const static char* pathPostfixV=".v.glsl";
const int pathPostfixLen=strlen(pathPostfixF);

/** Helper function for loadAndCompileShaderSet(char*) */
static GLuint dumpAndCleanupShader(Shader** shader, char* path, GLuint result) {
		(*shader)->dumpCompileInfo();
		delete *shader;
		*shader=NULL;
		free(path);
		return result;
}

GLuint Program::loadAndCompileShaderSet(const char* pathPrefix) {
	const int pathPrefixLen=strlen(pathPrefix);
	char* path=(char*)malloc(pathPrefixLen+pathPostfixLen+1);

	cout<<"will create fragment Shader()"<<endl;
	// compile the fragment shader
	fShader=new Shader(GL_FRAGMENT_SHADER);
	cout<<"did create fragment Shader()"<<endl;
	path[0]='\0'; // empty string
	strncat(path, pathPrefix, pathPrefixLen);
	strncat(path, pathPostfixF, pathPostfixLen);
	cout<<"will Shader()->compileFile("<<path<<")"<<endl;
	GLuint res=fShader->compileFile(path);
	if(!res) { // compilation failed
		cout<<"Shader()->compileFile("<<path<<") failed"<<endl;
		return dumpAndCleanupShader(&fShader, path, res);
	}
	cout<<"Shader()->compileFile("<<path<<") succeeded"<<endl;

	// compile the vertex shader
	cout<<"will create vertex Shader()"<<endl;
	vShader=new Shader(GL_VERTEX_SHADER);
	cout<<"did create vertex Shader()"<<endl;
	path[0]='\0'; // empty string
	strncat(path, pathPrefix, pathPrefixLen);
	strncat(path, pathPostfixV, pathPostfixLen);
	cout<<"will Shader()->compileFile("<<path<<")"<<endl;
	res=vShader->compileFile(path);
	if(!res) { // compilation failed
		cout<<"Shader()->compileFile("<<path<<") failed"<<endl;
		return dumpAndCleanupShader(&vShader, path, res);
	}
	cout<<"Shader()->compileFile("<<path<<") succeeded"<<endl;

	free(path);
	return res;
}

GLint Program::link() {
	glAttachShader(program, vShader->shader);
	glAttachShader(program, fShader->shader);
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

void Program::use() {
	glUseProgram(program);
}

void Program::bindAttribLocation(int loc, const char* attrib) {
	glBindAttribLocation(program, loc, attrib);
}
