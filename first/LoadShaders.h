#ifndef _LOADSHADERS_H
#define _LOADSHADERS_H

typedef struct {
    GLenum       type;
    const char*  filename;
    GLuint       shader;
} ShaderInfo;

GLuint LoadShaders(ShaderInfo*);

#endif
