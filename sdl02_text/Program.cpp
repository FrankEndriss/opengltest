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
	fShader=NULL;
	vShader=NULL;
	uniforms=0;
}

Program::~Program() {
	if(fShader)
		delete fShader;
	if(vShader)
		delete vShader;

	glDeleteProgram(program);

	for(int i=0; i<uniformCount; i++)
		free(uniforms[i].name);
	free(uniforms);
}

const char* pathPostfixF=".f.glsl";
const char* pathPostfixV=".v.glsl";
//const size_t pathPostfixLen=strlen(pathPostfixF);

/** Helper function for loadAndCompileShaderSet(char*) */
static GLuint dumpAndCleanupShader(Shader** shader, string path, GLuint result) {
		(*shader)->dumpCompileInfo();
		delete *shader;
		*shader=NULL;
		return result;
}

GLuint Program::loadAndCompileShaderSet(string& pathPrefix) {
	//const int pathPrefixLen=strlen(pathPrefix);
	//char* path=(char*)malloc(pathPrefixLen+pathPostfixLen+1);

	cout<<"will create fragment Shader()"<<endl;
	// compile the fragment shader
	fShader=new Shader(GL_FRAGMENT_SHADER);

	//auto fShader2=std::make_shared<Shader>(GL_FRAGMENT_SHADER);
	//cout<< fShader2 << endl;

	cout<<"did create fragment Shader()"<<endl;
	//path[0]='\0'; // empty string
	//strncat(path, pathPrefix, pathPrefixLen);
	//strncat(path, pathPostfixF, pathPostfixLen);
	const string path=pathPrefix+pathPostfixF;
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
	//path[0]='\0'; // empty string
	//strncat(path, pathPrefix, pathPrefixLen);
	//strncat(path, pathPostfixV, pathPostfixLen);
	cout<<"will Shader()->compileFile("<<path<<")"<<endl;
	const string path2=pathPrefix+pathPostfixV;
	res=vShader->compileFile(path2);
	if(!res) { // compilation failed
		cout<<"Shader()->compileFile("<<path2<<") failed"<<endl;
		return dumpAndCleanupShader(&vShader, path2, res);
	}
	cout<<"Shader()->compileFile("<<path2<<") succeeded"<<endl;

	//free(path);

	glAttachShader(program, vShader->shader);
	glAttachShader(program, fShader->shader);

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
