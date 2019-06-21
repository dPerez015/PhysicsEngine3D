#include "ParticleSystem.h"
GLuint ParticleSystem::program = 0;

void ParticleSystem::Setup() {
	SpherePrim::createSphereShaderAndProgram();
	program = SpherePrim::sphereProgram;
}

ParticleSystem::ParticleSystem() {
	
}

void ParticleSystem::Init(GLuint vbo) {
	particlesVbo = vbo;
	glGenVertexArrays(1, &particlesVao);
	glBindVertexArray(particlesVao);
	//glGenBuffers(1, &particlesVbo);
	glBindBuffer(GL_ARRAY_BUFFER, particlesVbo);
	//(GL_ARRAY_BUFFER, sizeof(float) * 3 * MAXPARTICLES, 0, GL_DYNAMIC_DRAW);
	glVertexAttribPointer((GLuint)0, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

int ParticleSystem::AddParticle(int rbIndex) {
	positions.push_back(glm::vec3(0, 0, 0));
	//rigidBodyIndex.push_back(rbIndex);
	return positions.size() - 1;
}

void ParticleSystem::Draw(int count) {
	glBindVertexArray(particlesVao);
	glUseProgram(program);
	glUniformMatrix4fv(glGetUniformLocation(program, "mvpMat"), 1, GL_FALSE, glm::value_ptr(_MVP));
	glUniformMatrix4fv(glGetUniformLocation(program, "mv_Mat"), 1, GL_FALSE, glm::value_ptr(_modelView));
	glUniformMatrix4fv(glGetUniformLocation(program, "projMat"), 1, GL_FALSE, glm::value_ptr(_projection));
	glUniform4f(glGetUniformLocation(program, "color"), 0.1f, 0.1f, 0.6f, 1.f);
	glUniform1f(glGetUniformLocation(program, "radius"), 0.1f);
	glDrawArrays(GL_POINTS, 0, count);

	glUseProgram(0);
	glBindVertexArray(0);
}


ParticleSystem::~ParticleSystem()
{
	glDeleteVertexArrays(1, &particlesVao);
	glDeleteBuffers(1, &particlesVbo);
}
