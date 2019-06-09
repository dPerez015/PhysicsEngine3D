#include "ParticleSystem.h"


void ParticleSystem::Setup() {
	SpherePrim::createSphereShaderAndProgram();
}

ParticleSystem::ParticleSystem()
{
	numparticles = 0;
}
ParticleSystem::ParticleSystem(int num, float rad) {
	numparticles = num;
	radius = rad;

	glGenVertexArrays(1, &particlesVao);
	glBindVertexArray(particlesVao);
	//glGenBuffers(1, &particlesVbo);

	glBindBuffer(GL_ARRAY_BUFFER, particlesVbo);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * numparticles, 0, GL_DYNAMIC_DRAW);
	glVertexAttribPointer((GLuint)0, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}


ParticleSystem::~ParticleSystem()
{
	glDeleteVertexArrays(1, &particlesVao);
	glDeleteBuffers(1, &particlesVbo);
}
