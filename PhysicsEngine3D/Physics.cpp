//#define GLM_ENABLE_EXPERIMENTAL
#include "glm/common.hpp"
#include "glm/vec3.hpp"
#include "ComputeShader.h"
#include "CubePrim.h"
#include "CollisionEngine.h"

CubePrim* cube;

void PhysicsInit() {
	CubePrim::Setup();
	cube = new CubePrim();
	CollisionEngine::getInstance().generateParticles(cube->cubeVertex,36,cube,4, rbType::Cube);
	
}

void PhysicsUpdate(float dt) {

}

void PhysicsCleanup() {

}