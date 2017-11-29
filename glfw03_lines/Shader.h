/*
 * Shader.h
 *
 *  Created on: 29.11.2017
 *      Author: frank
 */

#ifndef SHADER_H_
#define SHADER_H_

#include <GLFW/glfw3.h>
#include <ostream>

/** Shader-Compiler interface.
 */
class Shader {
public:
	/** @param type */
	Shader(GLenum type);
	virtual ~Shader();

	/** Frontend to compile(source...)
	 * @param path to source of shader
	 */
	GLint compileFile(char* path);
	/** @param source lines of sourcecode
	 *  @param lineCount number of lines, each one null terminated
	 */
	GLint compile(char** sourceLines, int lineCount);
	void dumpCompileInfo(std::ostream out);

private:
	GLuint shader;
};

#endif /* SHADER_H_ */
