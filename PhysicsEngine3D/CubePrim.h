#pragma once
#include "ShaderCompilationUtils.h"
#include "RigidBody.h"

extern glm::mat4 _MVP;
extern glm::mat4 _modelView;
extern glm::mat4 _projection;


class CubePrim :public RigidBody
{
public:
	CubePrim();
	~CubePrim();

	static const char* vertShader_xform;
	static const char* fragShader_flatColor;

	static float cubeVerts[216];
	static float cubeVertex[108];
	static GLubyte cubeIdx[20];

	static void Setup();
	static void Cleanup();
	void Draw();

	void JustDraw();
	void setupParticles();

private:
	static bool shadersCreated;

	glm::mat4 modelMat;

	GLuint vao;
	GLuint cubeVbo;
	static GLuint cubeShaders[2];
	static GLuint cubeProgram;
};

