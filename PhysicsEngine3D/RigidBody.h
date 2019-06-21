#pragma once
#define GLM_ENABLE_EXPERIMENTAL
#include "GL/glew.h"
#include "glm/gtx/quaternion.hpp"
#include <vector>

enum rbType{Cube};

class RigidBody
{
public:
	int id;
	virtual void JustDraw() = 0;
	virtual void Draw() = 0;
	virtual void UpdatePhysicalVar(float dt) = 0;

	virtual void addParticleIndexes(int id, int count)=0;

	static std::vector < glm::vec4> position;
	static std::vector < glm::vec4> linearVelocity; //all rigidbodies have a mass off 1 so  linear velocity = linearmomentum
	static std::vector < glm::vec4> angularVelocity; //we don't simulate the intertia tensor so angular velocity = angularMomentum
	static std::vector < glm::quat> rotation;
	static std::vector <GLint> partIndexes;
	//static std::vector < glm::mat4> modelMatrix;
	};

