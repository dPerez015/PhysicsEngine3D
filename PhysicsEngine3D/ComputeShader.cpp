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
		std::cerr << "Compute Shader Could not load file\n"<<e.what() << std::endl;
	}

	const char* computeShaderCode = computeCode.c_str();
	unsigned int shaderID;
	GLint res;
	shaderID = glCreateShader(GL_COMPUTE_SHADER);
	glShaderSource(shaderID, 1, &computeShaderCode, NULL);
	glCompileShader(shaderID);
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
/*
void ComputeShader::initiateBuffers() {
	
	glGenBuffers(2, dataBuffers);
	//POSITIONS
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, dataBuffers[0]);
	glBufferData(GL_SHADER_STORAGE_BUFFER, LilSpheres::maxParticles * sizeof(float) * 4, NULL, GL_STATIC_DRAW);
	
	float* pos = reinterpret_cast<float*>(glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, LilSpheres::maxParticles * sizeof(float) * 4, GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT));
	for (int i = 0; i < LilSpheres::maxParticles; ++i) {
		*pos++ = glm::linearRand<float>(-5, 5);
		*pos++ = glm::linearRand<float>(9, 10);
		*pos++ = glm::linearRand<float>(-5, 5);
		*pos++ = 1;
	}
	glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);

	//VELOCITIES
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, dataBuffers[1]);
	glBufferData(GL_SHADER_STORAGE_BUFFER, LilSpheres::maxParticles * sizeof(float) * 4, NULL, GL_STATIC_DRAW);
	float* vel = reinterpret_cast<float*>(glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, LilSpheres::maxParticles * sizeof(float) * 4, GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT));
	for (int i = 0; i < LilSpheres::maxParticles; ++i) {
		*vel++ = 0;
		*vel++ = glm::linearRand<float>(-1, 1);
		*vel++ = 0;
		*vel++ = 0;
	}
	glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);

}*/

void ComputeShader::use() {
	//glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, dataBuffers[0]);
	//glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, dataBuffers[1]);
	glUseProgram(ID);
	//glUniform1f(glGetUniformLocation(ID, "DT"), dt);
	//glUniform4fv(glGetUniformLocation(ID, "spherePos"),1 ,&sp.x);
	
}
void ComputeShader::dispatch(float x, float y, float z) {
	glDispatchCompute(x, y, z);
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
	glUseProgram(0);
}

void ComputeShader::setFloat(const std::string &name, float value) const
{
	GLint location = glGetUniformLocation(ID, name.c_str());
	glUniform1f(location, value);
}

void ComputeShader::setInt(const std::string &name, int value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

ComputeShader::ComputeShader()
{
}

ComputeShader::~ComputeShader()
{
}
