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

	particlesVarUpdate = ComputeShader("../Shaders/computeParticlePositioning.comp");

	particlesColision = ComputeShader("../Shaders/particleColision.comp");

	rbColisionReaction = ComputeShader("../Shaders/rigidbodyColisionReaction.comp");

	//creamos los buffers de las particulas para los compute shaders
	//posiciones finales
	glGenBuffers(1, &finalPosBuff);

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, finalPosBuff);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(particlesUpdatedPos), NULL, GL_STATIC_DRAW);

	//velocidades finales
	glGenBuffers(1, &finalVelBuff);

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, finalVelBuff);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(particlesUpdatedVel), NULL, GL_STATIC_DRAW);

	//posiciones relativas
	glGenBuffers(1, &relativePosBuff);

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, relativePosBuff);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(particlesRelativePos), NULL, GL_STATIC_DRAW);

	//fuerzas producidas
	glGenBuffers(1, &forceBuff);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, forceBuff);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(particlesForce), NULL, GL_STATIC_DRAW);
	

	//posiciones iniciales
	glGenBuffers(1, &initialPosBuff);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, initialPosBuff);
	glBufferData(GL_SHADER_STORAGE_BUFFER, 1024 * 1024 * sizeof(float)*4, NULL, GL_STATIC_DRAW);

	//posiciones de los rigidbodies
	glGenBuffers(1, &rigidBodyPosBuff);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, rigidBodyPosBuff);
	glBufferData(GL_SHADER_STORAGE_BUFFER, (1024 * 1024 * sizeof(float) * 4) / 64, NULL, GL_STATIC_DRAW);

	//velocidades de los rigidBodies
	glGenBuffers(1, &rigidBodyVelBuff);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, rigidBodyVelBuff);
	glBufferData(GL_SHADER_STORAGE_BUFFER, (1024 * 1024 * sizeof(float) * 4) / 64, NULL, GL_STATIC_DRAW);

	//velocidad angular de los rigidBodies
	glGenBuffers(1, &rigidBodyAngVelBuff);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, rigidBodyAngVelBuff);
	glBufferData(GL_SHADER_STORAGE_BUFFER, (1024 * 1024 * sizeof(float) * 4) / 64, NULL, GL_STATIC_DRAW);

	//quaternions de los rigidBodies
	glGenBuffers(1, &rigidBodyRotationBuff);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, rigidBodyRotationBuff);
	glBufferData(GL_SHADER_STORAGE_BUFFER, (1024 * 1024 * sizeof(float) * 4) / 64, NULL, GL_STATIC_DRAW);

	//indices de particulas de cada RigidBody
	glGenBuffers(1, &rigidBodyParticleIndexBuff);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, rigidBodyParticleIndexBuff);
	glBufferData(GL_SHADER_STORAGE_BUFFER, (1024 * 1024 * sizeof(GLint) *2) / 64, NULL, GL_STATIC_DRAW);

	//grid for particle colisions
	glGenBuffers(1, &gridBuffer);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, gridBuffer);
	glBufferData(GL_SHADER_STORAGE_BUFFER, 64*64*64*sizeof(glm::ivec4), NULL, GL_STATIC_DRAW);

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);


	for (int i = 0; i < 64 * 64 * 64 * 4; i++) {
		defaultGridIndexes[i] = -1;
	}


	ParticleSystem::Setup();
	particleSystem.Init(finalPosBuff);
}


CollisionEngine::~CollisionEngine()
{
	glDeleteFramebuffers(1, &fbo);
}

void CollisionEngine::draw() {
	//particleSystem.Draw(particlesInitialPos.size());
	for (auto it = rigidBodies.begin(); it != rigidBodies.end(); ++it) {
		(*it)->Draw();
	}
	
}
void CollisionEngine::bufferVector(GLuint buffer, std::vector<glm::vec4>* vector) {
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, buffer);
	glm::vec4* temp = reinterpret_cast<glm::vec4*>(glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, sizeof(glm::vec4)*vector->size(), GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT));
	for (auto it = vector->begin(); it != vector->end(); ++it) {
		*temp++ = (*it);
	}
	glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
}

void CollisionEngine::reciveBuffer(GLuint buffer, std::vector<glm::vec4>*vector) {
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, buffer);
	glm::vec4* temp = new glm::vec4[sizeof(glm::vec4)*vector->size()];
	int i = 0;
	temp = reinterpret_cast<glm::vec4*>(glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, sizeof(glm::vec4)*vector->size(), GL_MAP_READ_BIT));
	for (auto it = vector->begin(); it != vector->end(); ++it) {
		*it = temp[i]; 
		i++;
	}
	glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

void CollisionEngine::update(float dt) {
	//actualizamos los valores de los rigidbodies normal
	for (auto it = rigidBodies.begin(); it != rigidBodies.end(); ++it) {
		(*it)->UpdatePhysicalVar(dt);
	}

	//actualizamos los valores de las particulas de los rigidbodies
	//primero actualizamos los valores de las posiciones iniciales, por si ha aparecido algun nuevo valor.
	bufferVector(initialPosBuff, &particlesInitialPos);

	//despues enviamos los datos de los rigidbodies
	bufferVector(rigidBodyPosBuff, &RigidBody::position);

	bufferVector(rigidBodyVelBuff, &RigidBody::linearVelocity);

	bufferVector(rigidBodyAngVelBuff, &RigidBody::angularVelocity);

	//I do it this way to ensure apropiate size and layout
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, rigidBodyRotationBuff);
	float* tempRotation = reinterpret_cast<float*>(glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, sizeof(float)*RigidBody::rotation.size()*4, GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT));
	for (auto it = RigidBody::rotation.begin(); it != RigidBody::rotation.end(); ++it) {
		*tempRotation++ = it->x;
		*tempRotation++ = it->y;
		*tempRotation++ = it->z;
		*tempRotation++ = it->w;	
	}
	glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
	
	//adding the particle indexes information of each RB
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, rigidBodyParticleIndexBuff);
	GLint* tempIndexes = reinterpret_cast<GLint*>(glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, sizeof(GLint)*RigidBody::partIndexes.size(), GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT));
	for (auto it = RigidBody::partIndexes.begin(); it != RigidBody::partIndexes.end(); ++it) {
		*tempIndexes++ = *it;
	}
	glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, gridBuffer);
	glClearBufferData(GL_SHADER_STORAGE_BUFFER, GL_RGBA32I,GL_RED,GL_INT, &defaultGridIndexes);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
	
	

	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, initialPosBuff);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, finalPosBuff);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, finalVelBuff);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, relativePosBuff);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 4, rigidBodyPosBuff);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 5, rigidBodyVelBuff);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 6, rigidBodyAngVelBuff);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 7, rigidBodyRotationBuff);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 8, forceBuff);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 9, rigidBodyParticleIndexBuff);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 10, gridBuffer);
	
	
		
	particlesVarUpdate.use();
	particlesVarUpdate.setFloat("minX", -5);
	particlesVarUpdate.setFloat("minY", 0);
	particlesVarUpdate.setFloat("minZ", -5);
	particlesVarUpdate.setFloat("gridVoxelSize", 10.f/64.f);
	particlesVarUpdate.setInt("gridVoxelSize", 64);

	particlesVarUpdate.dispatch(glm::ceil((float)particlesInitialPos.size() / 128.f), 1.f, 1.f);
	
	particlesColision.use();
	particlesColision.setFloat("springCoef", 20.f);
	particlesColision.setFloat("dampingCoef", 4.8f);
	particlesColision.setFloat("shearCoef", 0.3f);
	particlesColision.setFloat("particleSize", 0.25f);
	particlesVarUpdate.setFloat("minX", -5);
	particlesVarUpdate.setFloat("minY", 0);
	particlesVarUpdate.setFloat("minZ", -5);
	particlesVarUpdate.setFloat("gridVoxelSize", 10.f / 64.f);
	particlesVarUpdate.setInt("gridVoxelSize", 64);
	particlesColision.dispatch(glm::ceil((float)particlesInitialPos.size() / 128.f), 1.f, 1.f);
	
	rbColisionReaction.use();
	rbColisionReaction.setFloat("dt", dt);
	rbColisionReaction.dispatch(glm::ceil((float)rigidBodies.size() / 128.f), 1.f, 1.f);
	
	/*glBindBuffer(GL_SHADER_STORAGE_BUFFER, gridBuffer);
	glm::ivec4* temp = new glm::ivec4[sizeof(defaultGridIndexes)];
	temp = reinterpret_cast<glm::ivec4*>(glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, sizeof(defaultGridIndexes), GL_MAP_READ_BIT));
	glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
	*/
	reciveBuffer(rigidBodyVelBuff, &RigidBody::linearVelocity);
	reciveBuffer(rigidBodyAngVelBuff, &RigidBody::angularVelocity);

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
	glClearDepth(1.f);
}
void CollisionEngine::cleanupFrameBuffer() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	glDisable(GL_COLOR_LOGIC_OP);
	//glLogicOp(GL_XOR);

	glDeleteTextures(1, &texture);
	glDeleteTextures(1, &depthTexture);
	
	glClearColor(0.2f, 0.2f, 0.2f, 1.f);
	glClearDepth(1.f);
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
		glm::mat4 orthoProjection = glm::ortho(minX, minX + maxDiference, minY, minY + maxDiference, minZ, minZ + maxDiference);


		//we render it to a texture
		setupFrameBuffer(resolution);
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_CULL_FACE);
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
						newParticlePos.x = minX + j * voxelOffset + halfVoxelOffset;
						newParticlePos.y =minY + i * voxelOffset + halfVoxelOffset;
						newParticlePos.z =minZ + depth * voxelOffset - halfVoxelOffset;
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
	//once we have the particles blueprint, we place them into an array and pass the indexes to the rigidbody
	std::vector<glm::vec3>* particlesFromRB = &particleBlueprints[type];

	//rb->firstPartID = particlesInitialPos.size();
	//rb->ammountOfParticles = particlesFromRB->size();
	rb->addParticleIndexes(particlesInitialPos.size(), particlesFromRB->size());
	for (auto it = particlesFromRB->begin(); it != particlesFromRB->end(); ++it) {
		//rb->addParticle(particlesInitialPos.size());
		particlesInitialPos.push_back(glm::vec4(*it,(float)rb->id));
	}

}
