#include "ComputeShader.h"



ComputeShader::ComputeShader(const GLchar* filepath)
{
	std::string computeCode;
	std::ifstream computeFile;
	computeFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try {
		computeFile.open(filepath);
		std::stringstream stream;
		stream << computeFile.rdbuf();
		computeFile.close();
		computeCode = stream.str();
	}
	catch (std::ifstream::failure e) {
		std::cerr << "Compute Shader Could not load file" << std::endl;
	}

	const char* computeShaderCode = computeCode.c_str();
	unsigned int shaderID;
	GLint res;
	shaderID = glCreateShader(GL_COMPUTE_SHADER);
	glShaderSource(shaderID, 1, &computeShaderCode, NULL);
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &res);
	if (res == GL_FALSE) {
		glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &res);
		char *buff = new char[res];
		glGetShaderInfoLog(shaderID, res, &res, buff);
		std::cerr << "Error compiling computeShader:	" << buff << std::endl;
		delete[] buff;
		glDeleteShader(shaderID);
		return;
	}

	ID = glCreateProgram();
	glAttachShader(ID, shaderID);
	glLinkProgram(ID);
	glGetProgramiv(ID, GL_LINK_STATUS, &res);
	
	if (res == GL_FALSE) {
		glGetProgramiv(ID, GL_INFO_LOG_LENGTH, &res);
		char *buff = new char[res];
		glGetProgramInfoLog(ID, res, &res, buff);
		std::cerr << "Error linking compute shader:	" << buff << std::endl;
		delete[] buff;
	}
}


ComputeShader::~ComputeShader()
{
}
