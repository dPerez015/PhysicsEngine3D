#pragma once
#include "ShaderCompilationUtils.h"
#include "Shader.h"

extern glm::mat4 _MVP;
extern glm::mat4 _modelView;
extern glm::mat4 _projection;

class BoxPrim
{
public:
	BoxPrim();
	~BoxPrim();

	static const char* vertShader_xform;
	static const char* fragShader_flatColor;
	
	static float cubeVerts[24];
	static GLubyte cubeIdx[20];
	
	static void Setup();
	static void Cleanup();
	void Draw();

	void JustDraw();
	void Init();

private:
	static bool shadersCreated;

	GLuint cubeVao;
	GLuint cubeVbo[2];
	static GLuint cubeShaders[2];
	static GLuint cubeProgram;

};

