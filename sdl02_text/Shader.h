
#ifndef SHADER_H_
#define SHADER_H_

#include "gl_include.h"
#include <ostream>
#include <string>

/** Shader-Compiler interface.
 */
class Shader {
public:
	/** @param type GL_FRAGMENT_SHADER or GL_VERTEX_SHADER
	 */;
	Shader(GLenum type);
	virtual ~Shader();

	/** Frontend to compile(source...)
	 * @param path to source of shader
	 */
	GLint compileFile(const std::string& path);
	/** @param source lines of sourcecode
	 *  @param lineCount number of lines, each one null terminated
	 *  @return true for SUCCESS, else dumpCompileInfo will contain useful info
	 */
	GLint compile(char** sourceLines, int lineCount);
	void dumpCompileInfo();

	GLuint shader;
private:
	GLint compileInfoAvailable=0;
};

#endif /* SHADER_H_ */
