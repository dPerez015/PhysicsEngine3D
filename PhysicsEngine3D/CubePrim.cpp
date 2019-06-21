#include "CubePrim.h"

#pragma region Vertex
float CubePrim::cubeVerts[] = {
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,	

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,	 
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		 
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
};

float CubePrim::cubeVertex[] = {
		-0.5f, -0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f,  0.5f, -0.5f,

		-0.5f, -0.5f,  0.5f,
		 0.5f, -0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		-0.5f, -0.5f,  0.5f,

		-0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,

		 0.5f,  0.5f,  0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f,  0.5f,  0.5f,
		 0.5f, -0.5f,  0.5f,

		-0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f,  0.5f,
		 0.5f, -0.5f,  0.5f,
		-0.5f, -0.5f,  0.5f,
		-0.5f, -0.5f, -0.5f,

		-0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f,  0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f, -0.5f,
		-0.5f,  0.5f,  0.5f,
};

#pragma endregion

#pragma region Shaders
const char* CubePrim::vertShader_xform =
"#version 330\n\
in vec3 in_Position;\n\
in vec3 in_Normal;\n\
out vec3 Normal;\n\
uniform mat4 mvpMat;\n\
void main() {\n\
	gl_Position = mvpMat * vec4(in_Position, 1.0);\n\
	Normal=in_Normal;\n\
}";

const char* CubePrim::fragShader_flatColor =
"#version 330\n\
in vec3 Normal;\n\
out vec4 out_Color;\n\
uniform vec4 color;\n\
void main() {\n\
	vec3 norm= normalize(Normal);\n\
	vec3 lightDir = normalize(vec3(1,1,2));\n\
	float diff=max(dot(norm,lightDir),0.0);\n\
	vec4 ambient = 0.2*color;\n\
	out_Color = ambient+(color*diff);\n\
}";
#pragma endregion

GLuint CubePrim::cubeShaders[2] = { 0,0 };
GLuint CubePrim::cubeProgram = 0;
bool CubePrim::shadersCreated = false;

CubePrim::CubePrim()
{
	if (shadersCreated) {
		glGenVertexArrays(1,&vao);
		glGenBuffers(1, &cubeVbo);
		glBindVertexArray(vao);

		glBindBuffer(GL_ARRAY_BUFFER, cubeVbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVerts),cubeVerts,GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3*sizeof(float)));
		glEnableVertexAttribArray(1);

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	id = position.size();
	
	position.push_back(glm::vec4(1.f, 1.f, 0.f, 0.f));
	linearVelocity.push_back(glm::vec4(0.f, 0.f, 0.f, 0.f));
	angularVelocity.push_back(glm::vec4(0.f, 1.f, 0.f, 0.f));
	//angularVelocity.push_back(glm::vec4(0.f, 0.f, 0.f, 0.f));
	rotation.push_back(glm::quat(1.f, 0.f, 0.f, 0.f));
}

void CubePrim::setPosition(glm::vec3 p) {
	position[id] = glm::vec4(p,0.f);
}


CubePrim::~CubePrim()
{
	glDeleteBuffers(1, &cubeVbo);
	glDeleteVertexArrays(1, &vao);
}

void CubePrim::Setup() {
	cubeShaders[0] = ShaderCompilationUtils::compileShader(vertShader_xform, GL_VERTEX_SHADER, "cubeVert");
	cubeShaders[1] = ShaderCompilationUtils::compileShader(fragShader_flatColor, GL_FRAGMENT_SHADER, "cubeFrag");

	cubeProgram = glCreateProgram();
	glAttachShader(cubeProgram, cubeShaders[0]);
	glAttachShader(cubeProgram, cubeShaders[1]);
	glBindAttribLocation(cubeProgram, 0, "in_Position");
	glBindAttribLocation(cubeProgram, 1, "in_Normal");
	ShaderCompilationUtils::linkProgram(cubeProgram);

	shadersCreated = true;
}

void CubePrim::addParticleIndexes(int id, int count) {
	partIndexes.push_back(id);
	partIndexes.push_back(count);
}

void CubePrim::Cleanup() {
	glDeleteProgram(cubeProgram);
	glDeleteShader(cubeShaders[0]);
	glDeleteShader(cubeShaders[1]);
}

void CubePrim::UpdatePhysicalVar(float dt) {
	linearVelocity[id] += glm::vec4(0.0f, -1.0f, 0.0f,0.f)*9.8f*dt;
	if (glm::length(angularVelocity[id]) > 0.f) {
		float angle = glm::length(angularVelocity[id] * dt);
		glm::quat dq(glm::cos(angle / 2.0f), glm::normalize(angularVelocity[id])*glm::sin(angle / 2.0f));
		rotation[id] = dq * rotation[id];
	}
	position[id] += linearVelocity[id] * dt;
	

}

void CubePrim::Draw() {
	glBindVertexArray(vao);
	glUseProgram(cubeProgram);
	modelMat = glm::mat4(1.0f);
	modelMat = glm::translate(modelMat, glm::vec3(position[id].x, position[id].y, position[id].z));
	modelMat = modelMat * glm::toMat4(rotation[id]);
	modelMat = _MVP*modelMat;
	glUniformMatrix4fv(glGetUniformLocation(cubeProgram, "mvpMat"), 1, GL_FALSE, glm::value_ptr(modelMat));
	glUniform4f(glGetUniformLocation(cubeProgram, "color"), 0.0f, 0.6f, 0.6f, 1.f);

	glDrawArrays(GL_TRIANGLES, 0, 36);

	glUseProgram(0);
	glBindVertexArray(0);
}

void CubePrim::JustDraw() {
	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glUseProgram(0);
	glBindVertexArray(0);
}
