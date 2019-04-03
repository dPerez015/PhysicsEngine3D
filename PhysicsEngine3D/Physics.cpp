//#define GLM_ENABLE_EXPERIMENTAL
#include "glm/common.hpp"
#include "glm/vec3.hpp"
#include "ComputeShader.h"

//#include "glm/gtx/rotate_vector.hpp"

namespace LilSpheres {
	extern const int maxParticles;
	extern float lifeTime;
	extern GLuint particlesVbo;
	extern void setupParticles(int numTotalParticles, float radius = 0.03f, float lifeT = 3.0f);
	extern void cleanupParticles();
	extern void updateParticles(int startIdx, int count, float* array_data);
	extern void drawParticles(int startIdx, int count);
}

struct Particle {
	glm::vec3 position;
	glm::vec3 speed;
	glm::vec3 acceleration;
	float timeAlive;
};
float ParticleLifeTime = 1.f;

Particle* particleArray;
float* onlyPositions;

void preparePositions() {
	for (int i = 0; i < LilSpheres::maxParticles; ++i) {
		onlyPositions[i * 3 + 0] = particleArray[i].position.x;
		onlyPositions[i * 3 + 1] = particleArray[i].position.y;
		onlyPositions[i * 3 + 2] = particleArray[i].position.z;
	}
}


ComputeShader computeShader;

void PhysicsInit() {
	/*
	particleArray = new Particle[LilSpheres::maxParticles];
	onlyPositions = new float[LilSpheres::maxParticles * 3];
	//set particles equidistant position
	//max particles set to 27000 so it should be 30 in each row
	float distance = 1.f / 30.f;

	for (int i = 0; i < LilSpheres::maxParticles;i++) {
		particleArray[i].position.x = (distance * (i % 30) * 10) - 5;
		particleArray[i].position.y = (distance * ((i / 30) % 30) * 10);
		particleArray[i].position.z = (distance * (i / 900) * 10) - 5;
	}

	preparePositions();

	LilSpheres::updateParticles(0, LilSpheres::maxParticles , onlyPositions);*/

	computeShader = ComputeShader("..\Shaders\compute.cs");
	computeShader.initiateBuffers();
	LilSpheres::particlesVbo = computeShader.dataBuffers[0];
	LilSpheres::setupParticles(LilSpheres::maxParticles);
}

void PhysicsUpdate(float dt) {

}

void PhysicsCleanup() {
	delete(particleArray);
}