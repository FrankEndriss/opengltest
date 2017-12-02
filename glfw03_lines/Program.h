/*
 * Program.h
 * Interface to openGL program, see
 * GLint glCreateProram(void)
 */

#ifndef PROGRAM_H_
#define PROGRAM_H_

#include "gl_include.h"
#include "Shader.h"

class Program {
public:
	Program();
	virtual ~Program();

	/** Loads a set of shader sourcefiles and compiles them.
	 * A set of shader sourcefiles is some files at path, and postfixed:
	 * <path>.v.glsl : Vertex shader
	 * <path>.f.glsl : Fragment shader
	 * <path>.t.glsl : Tesselation shader
	 * ... more to come.
	 * Note: IN GLES 2.0 there are only Vertex and Fragment shaders.
	 * However, this class will try to compile any file found
	 * fitting the name pattern.
	 * @return SUCCESS flag of compilation, ie 0 if at least one of the Shaders does not compile
	 **/
	GLuint loadAndCompileShaderSet(char* path);

	/** After compiling the shaders the program must be linked before using it.
	 * see glLinkProgram(GLuint)
	 */
	GLint link();

	/** After linking the program can be used, ie made the "current" program.
	 * see glUseProgram(GLuint)
	 */
	void use();

private:
	GLuint program;
	Shader* fShader;
	Shader* vShader;
};

#endif /* PROGRAM_H_ */
