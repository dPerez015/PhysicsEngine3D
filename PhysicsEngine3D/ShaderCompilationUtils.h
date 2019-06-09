#pragma once
#include "GL/glew.h"
#include <stdio.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace ShaderCompilationUtils {
	GLuint compileShader(const char* shaderStr, GLenum shaderType, const char* name);
	void linkProgram(GLuint program);
}