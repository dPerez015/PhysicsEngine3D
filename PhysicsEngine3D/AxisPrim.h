#pragma once
#include "ShaderCompilationUtils.h"
extern glm::mat4 _MVP;
extern glm::mat4 _modelView;
extern glm::mat4 _projection;

class AxisPrim
{
public:
	AxisPrim();
	~AxisPrim();

	static float AxisVerts[24];
	static float AxisColors[24];
	static GLubyte AxisIdx[6];

	static const char* Axis_vertShader;
	static const char* Axis_fragShader;

	static void Setup();
	static void Cleanup();
	void Draw();
	void Init();

private: 
	static bool shadersCreated;
	
	GLuint AxisVao;
	GLuint AxisVbo[3];
	static GLuint AxisShader[2];
	static GLuint AxisProgram;
};

