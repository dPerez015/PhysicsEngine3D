#pragma once
#include <vector>
#include <unordered_map>
#include "ShaderCompilationUtils.h"
#include "Shader.h"
#include "RigidBody.h"

extern glm::mat4 _MVP;
extern glm::mat4 _modelView;
extern glm::mat4 _projection;

union bitdepth32 {
	uint32_t theInt;
	GLbyte theBytes[4];
};

class CollisionEngine
{
public:
	static CollisionEngine& getInstance();
	CollisionEngine(CollisionEngine const&) = delete;
	void operator= (CollisionEngine const&) = delete;
	void generateParticles(float*vertex, int ammount, RigidBody* rb, int resolution,rbType type);
private:
	CollisionEngine();
	~CollisionEngine();

	GLuint fbo;
	GLuint texture;
	GLuint depthTexture;
	
	bitdepth32 theResult[1024];

	void setupFrameBuffer(int resolution);
	void clearFrameBuffer();
	void cleanupFrameBuffer();
	
	std::unordered_map<rbType, std::vector<glm::vec3>> particleBlueprints;

	Shader shader;
	std::vector<RigidBody*>rigidBodies;
};

