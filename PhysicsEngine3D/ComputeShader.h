#pragma once
#include <GL/glew.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/gtc/random.hpp>
#include <glm//vec4.hpp>
namespace LilSpheres {
	extern const int maxParticles;
}

//https://www.geeks3d.com/20140704/tutorial-introduction-to-opengl-4-3-shader-storage-buffers-objects-ssbo-demo/
class ComputeShader
{
public:
	GLuint ID;
	//GLuint dataBuffers[2];
	ComputeShader();
	ComputeShader(const GLchar* filepath);
	//void initiateBuffers();
	void use();
	void dispatch(float,float,float);
	void setFloat(const std::string &name, float value) const;
	void setInt(const std::string &name, int value) const;
	~ComputeShader();
};

