#pragma once
#include "ShaderCompilationUtils.h"
extern glm::mat4 _MVP;
extern glm::mat4 _modelView;
extern glm::mat4 _projection;


class SpherePrim{
public:
	SpherePrim(float radius, glm::vec3 pos);
	SpherePrim();
	~SpherePrim();

	static const char* sphere_vertShader;
	static const char* sphere_geomShader;
	static const char* sphere_fragShader_flatColor;
	
	static void Setup();
	static void Cleanup();
	
	void Draw();
	void Update(glm::vec3 pos, float radius);

	static GLuint sphereShaders[3];
	static GLuint sphereProgram;
	static void createSphereShaderAndProgram();
	static void cleanupSphereShaderAndProgram();

private:
	static bool shadersCreated;  //default initialized to false


	GLuint sphereVao;
	GLuint sphereVbo;

	float radius;
};

