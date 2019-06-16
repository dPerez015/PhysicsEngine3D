#pragma once
#include "GL/glew.h"
#include <vector>
enum rbType{Cube};

class RigidBody
{
public:
	int id;
	virtual void JustDraw()=0;

	virtual void setupParticles()=0;

	static std::vector<glm::vec3> position;
	static std::vector < glm::vec3> linearMomentum;
	static std::vector < glm::vec3> angularMomentum;
	static std::vector < glm::quat> rotation;
	static std::vector <glm::mat4> modelMatrix;

};

