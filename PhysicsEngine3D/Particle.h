#pragma once
#include "glm/vec3.hpp"
class Particle
{
public:
	Particle();
	~Particle();

private:
	static glm::vec3* positions;
	static glm::vec3* speeds;
	static glm::vec3* accelerations;
	float* timesAlive;
};

