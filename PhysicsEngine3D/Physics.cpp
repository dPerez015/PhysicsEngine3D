//#define GLM_ENABLE_EXPERIMENTAL
#include "glm/common.hpp"
#include "glm/vec3.hpp"
#include "ComputeShader.h"
#include "CubePrim.h"
#include "CollisionEngine.h"

CubePrim* cube;

std::vector<CubePrim*> cubes;

void AddCube() {
	CubePrim* cube;
	cube = new CubePrim();
	CollisionEngine::getInstance().generateParticles(cube->cubeVertex, 36, cube, 4, rbType::Cube);
	cube->setPosition(glm::vec3(0.f, 4.f, 0.f));
	cubes.push_back(cube);
}

void PhysicsInit() {
	CubePrim::Setup();
	//cube = new CubePrim();
	//CollisionEngine::getInstance().generateParticles(cube->cubeVertex,36,cube,4, rbType::Cube);
	//cube->setPosition(glm::vec3(0.f, 5.f, 0.f));
	AddCube();
}

float timebetweenCubes = 0.5f;
float timeSinceLastCube = 0.0f;
void PhysicsUpdate(float dt) {
	CollisionEngine::getInstance().update(dt);

	timeSinceLastCube += dt;
	if (timeSinceLastCube > timebetweenCubes) {
		timeSinceLastCube -= timebetweenCubes;
		AddCube();
	}
}

void PhysicsCleanup() {

}