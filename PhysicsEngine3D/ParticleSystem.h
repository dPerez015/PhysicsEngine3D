#pragma once
#include <vector>
#include "ShaderCompilationUtils.h"
#include "Sphere.h"
#define MAXPARTICLES 1024*1024


class ParticleSystem
{
public:
	ParticleSystem();
	~ParticleSystem();

	static void Setup();
	static void Cleanup();
	void Draw(int count);
	void Init(GLuint vbo);

	int AddParticle(int rbIndex);

private: 
	GLuint particlesVao;
	GLuint particlesVbo;

	int numparticles;

	static GLuint program;

	std::vector<glm::vec3> positions;
	//std::vector<int> rigidBodyIndex;

};

