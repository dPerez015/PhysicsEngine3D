#pragma once
#include <GL/glew.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/gtc/random.hpp>
namespace LilSpheres {
	extern const int maxParticles;
}

//https://www.geeks3d.com/20140704/tutorial-introduction-to-opengl-4-3-shader-storage-buffers-objects-ssbo-demo/
class ComputeShader
{
public:
	GLuint ID;
	GLuint dataBuffers[2];
	ComputeShader();
	ComputeShader(const GLchar* filepath);
	void initiateBuffers();
	void activate();
	~ComputeShader();
};

