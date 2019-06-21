#pragma once
#include <vector>
#include <unordered_map>
#include "ShaderCompilationUtils.h"
#include "Shader.h"
#include "ComputeShader.h"
#include "RigidBody.h"
#include "ParticleSystem.h"

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

	void update(float dt);
	void draw();
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
	
	//particle buffers
	GLuint finalPosBuff;
	GLuint initialPosBuff;
	GLuint relativePosBuff;
	GLuint finalVelBuff;
	GLuint forceBuff;

	//rigidbodies buffers
	GLuint rigidBodyPosBuff;
	GLuint rigidBodyVelBuff;
	GLuint rigidBodyAngVelBuff;
	GLuint rigidBodyRotationBuff;
	GLuint rigidBodyParticleIndexBuff;

	//grid 
	int resolution;
	float bounds[6];
	GLuint gridBuffer;

	Shader shader;
	ComputeShader particlesVarUpdate;
	ComputeShader particlesColision;
	ComputeShader rbColisionReaction;


	std::vector<RigidBody*>rigidBodies;
	std::vector<glm::vec4>particlesInitialPos;
	glm::vec4 particlesUpdatedPos[1024 * 1024];
	glm::vec4 particlesRelativePos[1024 * 1024];
	glm::vec4 particlesUpdatedVel[1024 * 1024];
	glm::vec4 particlesForce[1024 * 1024];

	int defaultGridIndexes[64 * 64 * 64 * 4];

	ParticleSystem particleSystem;
	
	void bufferVector(GLuint buffer, std::vector<glm::vec4>* vector);
	void reciveBuffer(GLuint buffer, std::vector<glm::vec4>* vector);
};

