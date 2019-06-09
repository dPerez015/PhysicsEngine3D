#pragma once
#include "ShaderCompilationUtils.h"
#include "Sphere.h"

class ParticleSystem
{
public:
	ParticleSystem();
	ParticleSystem(int numParticles, float radius);
	~ParticleSystem();

	static void Setup();
	static void Cleanup();
	void Draw();
	void Init();

private: 
	GLuint particlesVao;
	GLuint particlesVbo;
	float radius;
	int numparticles;

};

