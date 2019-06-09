#include "CubePrim.h"

#pragma region Vertex
float CubePrim::cubeVerts[] = {
	//-5,0,-5 -- 5, 10, 5
	-5.f,  0.f, -5.f,
	 5.f,  0.f, -5.f,
	 5.f,  0.f,  5.f,
	-5.f,  0.f,  5.f,
	-5.f, 10.f, -5.f,
	 5.f, 10.f, -5.f,
	 5.f, 10.f,  5.f,
	-5.f, 10.f,  5.f,
};
GLubyte CubePrim::cubeIdx[] = {
		1, 0, 2, 3, //Floor - TriangleStrip
		0, 1, 5, 4, //Wall - Lines
		1, 2, 6, 5, //Wall - Lines
		2, 3, 7, 6, //Wall - Lines
		3, 0, 4, 7  //Wall - Lines
};
#pragma endregion

#pragma region Shaders
const char* CubePrim::vertShader_xform =
"#version 330\n\
in vec3 in_Position;\n\
uniform mat4 mvpMat;\n\
void main() {\n\
	gl_Position = mvpMat * vec4(in_Position, 1.0);\n\
}";

const char* CubePrim::fragShader_flatColor = 
"#version 330\n\
out vec4 out_Color;\n\
uniform vec4 color;\n\
void main() {\n\
	out_Color = color;\n\
}";
#pragma endregion


GLuint CubePrim::cubeShaders[2] = { 0,0 };
GLuint CubePrim::cubeProgram = 0;
bool CubePrim::shadersCreated = false;

CubePrim::CubePrim()
{
	if (shadersCreated)
		Init();
}

void CubePrim::Init() {
	//we create the VAO and we bind it 
	glGenVertexArrays(1, &cubeVao);
	glBindVertexArray(cubeVao);
	glGenBuffers(2, cubeVbo);

	glBindBuffer(GL_ARRAY_BUFFER, cubeVbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 24, cubeVerts, GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeVbo[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLubyte) * 20, cubeIdx, GL_STATIC_DRAW);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}


CubePrim::~CubePrim()
{
	glDeleteBuffers(2, cubeVbo);
	glDeleteVertexArrays(1, &cubeVao);
}

void CubePrim::Setup() {
	cubeShaders[0] = ShaderCompilationUtils::compileShader(vertShader_xform, GL_VERTEX_SHADER, "cubeVert");
	cubeShaders[1] = ShaderCompilationUtils::compileShader(fragShader_flatColor, GL_FRAGMENT_SHADER, "cubeFrag");

	cubeProgram = glCreateProgram();
	glAttachShader(cubeProgram, cubeShaders[0]);
	glAttachShader(cubeProgram, cubeShaders[1]);
	glBindAttribLocation(cubeProgram, 0, "in_Position");
	ShaderCompilationUtils::linkProgram(cubeProgram);

	shadersCreated = true;
}


void CubePrim::Cleanup() {
	glDeleteProgram(cubeProgram);
	glDeleteShader(cubeShaders[0]);
	glDeleteShader(cubeShaders[1]);
}

void CubePrim::Draw() {
	glBindVertexArray(cubeVao);
	glUseProgram(cubeProgram);
	glUniformMatrix4fv(glGetUniformLocation(cubeProgram, "mvpMat"), 1, GL_FALSE, glm::value_ptr(_MVP));
	//FLOOR
	glUniform4f(glGetUniformLocation(cubeProgram, "color"), 0.6f, 0.6f, 0.6f, 1.f);
	glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_BYTE, 0);
	//WALLS
	glUniform4f(glGetUniformLocation(cubeProgram, "color"), 0.f, 0.f, 0.f, 1.f);
	glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_BYTE, (void*)(sizeof(GLubyte) * 4));
	glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_BYTE, (void*)(sizeof(GLubyte) * 8));
	glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_BYTE, (void*)(sizeof(GLubyte) * 12));
	glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_BYTE, (void*)(sizeof(GLubyte) * 16));

	glUseProgram(0);
	glBindVertexArray(0);
}