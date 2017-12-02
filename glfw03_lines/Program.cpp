/*
 * Program.cpp
 */

#include "Program.h"
#include <cstring>
#include <malloc.h>
#include "Shader.h"

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

GLuint Program::loadAndCompileShaderSet(char* pathPrefix) {
	const int pathPrefixLen=strlen(pathPrefix);
	char* path=(char*)malloc(pathPrefixLen+pathPostfixLen+1);

	// compile the fragment shader
	fShader=new Shader(GL_FRAGMENT_SHADER);
	path[0]='\0'; // empty string
	strncat(path, pathPrefix, pathPrefixLen);
	strncat(path, pathPostfixF, pathPostfixLen);
	GLuint res=fShader->compileFile(path);
	if(!res) { // compilation failed
		return dumpAndCleanupShader(&fShader, path, res);
	}

	// compile the vertex shader
	vShader=new Shader(GL_VERTEX_SHADER);
	path[0]='\0'; // empty string
	strncat(path, pathPrefix, pathPrefixLen);
	strncat(path, pathPostfixV, pathPostfixLen);
	res=vShader->compileFile(path);
	if(!res) { // compilation failed
		return dumpAndCleanupShader(&vShader, path, res);
	}

	free(path);
	return res;
}

GLint Program::link() {
	glAttachShader(program, vShader->shader);
	glAttachShader(program, fShader->shader);
	glLinkProgram(program);
	GLint ret;
	glGetProgramiv(program, GL_LINK_STATUS, &ret);
	return ret;
}

void Program::use() {
	glUseProgram(program);
}
