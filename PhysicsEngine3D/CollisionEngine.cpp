#include "CollisionEngine.h"

CollisionEngine& CollisionEngine::getInstance(){
		static CollisionEngine instance;
		return instance;
}

CollisionEngine::CollisionEngine()
{
	//creamos un frame buffer para hacer ahi el renderizado de las primitivas
	glGenFramebuffers(1, &fbo);

	shader = Shader("../Shaders/VoxelGeneration.vs", "../Shaders/VoxelGeneration.fs");

}


CollisionEngine::~CollisionEngine()
{
	glDeleteFramebuffers(1, &fbo);
}

void CollisionEngine::setupFrameBuffer(int resolution) {
	
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	//we create a texture of the size of resolution and a bitdepth of 8 per component 24 total
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8UI, resolution, resolution, 0, GL_RGBA_INTEGER, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glBindTexture(GL_TEXTURE_2D, 0);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);

	//we create a depth and stencil texture to sample from it in the particle generation phase
	glGenTextures(1, &depthTexture);
	glBindTexture(GL_TEXTURE_2D, depthTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, resolution, resolution, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, depthTexture, 0);
	
	glBindTexture(GL_TEXTURE_2D, 0);

	if (!glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE) {
		fprintf(stderr, "The frameBuffer isn't complete\n");
	}

	clearFrameBuffer();

}
void CollisionEngine::clearFrameBuffer() {
	glClearColor(0, 0, 0, 0);
	glClearDepth(0);
}
void CollisionEngine::cleanupFrameBuffer() {
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	glDisable(GL_COLOR_LOGIC_OP);
	glLogicOp(GL_XOR);

	glDeleteTextures(1, &texture);
	glDeleteTextures(1, &depthTexture);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void CollisionEngine::generateParticles(float* vertex,int ammount,RigidBody* rb ,int resolution, rbType type) {
	rigidBodies.push_back(rb);
	rb->id = rigidBodies.size() - 1;

	//if the particles positions are not initilized we process the vertices and calculate their blueprint positions
	if (particleBlueprints.find(type) == particleBlueprints.end()) {
		//we check the bounds of the model
		float maxX, maxY, maxZ;
		float minX, minY, minZ;

		maxX = minX = vertex[0];
		maxY = minY = vertex[1];
		maxZ = minZ = vertex[2];

		for (int i = 1; i < ammount; i++) {
			float x = vertex[i * 3 + 0];
			float y = vertex[i * 3 + 1];
			float z = vertex[i * 3 + 2];

			//z
			if (maxX < x)
				maxX = x;
			else if (minX > x)
				minX = x;

			//y
			if (maxY < y)
				maxY = y;
			else if (minY > y)
				minY = y;

			//z
			if (maxZ < z)
				maxZ = z;
			else if (minZ > z)
				minZ = z;
		}

		float xDif = maxX - minX;
		float yDif = maxY - minY;
		float zDif = maxZ - minZ;

		float maxDiference = xDif;
		if (maxDiference < yDif)
			maxDiference = yDif;
		if (maxDiference < zDif)
			maxDiference = zDif;

		//we set the frustrum to fit the model
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_CULL_FACE);

		glm::mat4 orthoProjection = glm::ortho(minX, minX + maxDiference, minY, minY + maxDiference, minZ, minZ + maxDiference);


		//we render it to a texture
		setupFrameBuffer(resolution);
		glEnable(GL_COLOR_LOGIC_OP);
		glLogicOp(GL_XOR);

		shader.use();
		shader.setMatrix("mvpMat", orthoProjection);
		shader.setFloat("near", minZ);
		shader.setFloat("far", minZ + maxDiference);
		shader.setInt("resolution", resolution);

		int bitSize = 32;

		int bitmaskSize = bitSize / resolution;

		int bitMask = 0;
		for (int i = 0; i < bitmaskSize; i++) {
			bitMask = bitMask << 1;
			bitMask |= 1;
		}

		bitdepth32 theColor;
		theColor.theInt = 0;
		for (int i = 0; i < resolution; i++) {
			shader.setInt("lookUpTable[" + std::to_string((resolution - i) * 4 - 1) + "]", theColor.theBytes[3]);
			shader.setInt("lookUpTable[" + std::to_string((resolution - i) * 4 - 2) + "]", theColor.theBytes[2]);
			shader.setInt("lookUpTable[" + std::to_string((resolution - i) * 4 - 3) + "]", theColor.theBytes[1]);
			shader.setInt("lookUpTable[" + std::to_string((resolution - i) * 4 - 4) + "]", theColor.theBytes[0]);
			theColor.theInt = theColor.theInt << bitmaskSize;
			theColor.theInt = theColor.theInt | bitMask;
		}

		rb->JustDraw();

		glGetTextureImage(texture, 0, GL_RGBA_INTEGER, GL_UNSIGNED_BYTE, sizeof(theResult) / sizeof(theResult[0]), theResult);

		//checking the texture to generate the particles
		std::vector<glm::vec3>particlesPos;
		float voxelOffset = maxDiference / resolution;
		float halfVoxelOffset = voxelOffset / 2;

		for (int depth = 0; depth < resolution; depth++) {
			for (int i = 0; i < resolution; i++) {
				for (int j = 0; j < resolution; j++) {
					int masked = theResult[i*resolution + j].theInt & bitMask;
					if (masked == bitMask) {
						glm::vec3 newParticlePos;
						newParticlePos.x = j * voxelOffset + halfVoxelOffset;
						newParticlePos.y = i * voxelOffset + halfVoxelOffset;
						newParticlePos.z = depth * voxelOffset - halfVoxelOffset;
						particlesPos.push_back(newParticlePos);
					}
				}
			}
			bitMask = bitMask << bitmaskSize;
		}
		//we save the particles
		particleBlueprints.insert(std::pair<rbType,std::vector<glm::vec3>>(type, particlesPos));

		cleanupFrameBuffer();
	}


	
	
}
