#pragma once
#include <GL/glew.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

//https://www.geeks3d.com/20140704/tutorial-introduction-to-opengl-4-3-shader-storage-buffers-objects-ssbo-demo/
class ComputeShader
{
public:
	unsigned int ID;
	unsigned int posBuffID;

	ComputeShader(const GLchar* filepath);
	void activate();
	~ComputeShader();
};

