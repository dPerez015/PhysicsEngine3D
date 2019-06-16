#include "ParticleSystem.h"


void ParticleSystem::Setup() {
	SpherePrim::createSphereShaderAndProgram();
}

ParticleSystem::ParticleSystem()
{
	numparticles = 0;
}
ParticleSystem::ParticleSystem() {
	glGenVertexArrays(1, &particlesVao);
	glBindVertexArray(particlesVao);
	glGenBuffers(1, &particlesVbo);

	glBindBuffer(GL_ARRAY_BUFFER, particlesVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * MAXPARTICLES, 0, GL_DYNAMIC_DRAW);
	glVertexAttribPointer((GLuint)0, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

int ParticleSystem::AddParticle(int rbIndex) {
	positions.push_back(glm::vec3(0, 0, 0));
	rigidBodyIndex.push_back(rbIndex);
}

ParticleSystem::~ParticleSystem()
{
	glDeleteVertexArrays(1, &particlesVao);
	glDeleteBuffers(1, &particlesVbo);
}
