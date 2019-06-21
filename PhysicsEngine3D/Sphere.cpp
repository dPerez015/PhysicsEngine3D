#include "Sphere.h"
#pragma region Shaders
const char* SpherePrim::sphere_vertShader =
"#version 330\n\
in vec4 in_Position;\n\
uniform mat4 mv_Mat;\n\
void main() {\n\
	gl_Position = mv_Mat * vec4(in_Position);\n\
}";
const char* SpherePrim::sphere_geomShader =
"#version 330\n\
layout(points) in;\n\
layout(triangle_strip, max_vertices = 4) out;\n\
out vec4 eyePos;\n\
out vec4 centerEyePos;\n\
uniform mat4 projMat;\n\
uniform float radius;\n\
vec4 nu_verts[4];\n\
void main() {\n\
	vec3 n = normalize(-gl_in[0].gl_Position.xyz);\n\
	vec3 up = vec3(0.0, 1.0, 0.0);\n\
	vec3 u = normalize(cross(up, n));\n\
	vec3 v = normalize(cross(n, u));\n\
	nu_verts[0] = vec4(-radius*u - radius*v, 0.0); \n\
	nu_verts[1] = vec4( radius*u - radius*v, 0.0); \n\
	nu_verts[2] = vec4(-radius*u + radius*v, 0.0); \n\
	nu_verts[3] = vec4( radius*u + radius*v, 0.0); \n\
	centerEyePos = gl_in[0].gl_Position;\n\
	for (int i = 0; i < 4; ++i) {\n\
		eyePos = (gl_in[0].gl_Position + nu_verts[i]);\n\
		gl_Position = projMat * eyePos;\n\
		EmitVertex();\n\
	}\n\
	EndPrimitive();\n\
}";


const char* SpherePrim::sphere_fragShader_flatColor =
"#version 330\n\
in vec4 eyePos;\n\
in vec4 centerEyePos;\n\
out vec4 out_Color;\n\
uniform mat4 projMat;\n\
uniform mat4 mv_Mat;\n\
uniform vec4 color;\n\
uniform float radius;\n\
void main() {\n\
	vec4 diff = eyePos - centerEyePos;\n\
	float distSq2C = dot(diff, diff);\n\
	if (distSq2C > (radius*radius)) discard;\n\
	float h = sqrt(radius*radius - distSq2C);\n\
	vec4 nuEyePos = vec4(eyePos.xy, eyePos.z + h, 1.0);\n\
	vec4 nuPos = projMat * nuEyePos;\n\
	gl_FragDepth = ((nuPos.z / nuPos.w) + 1) * 0.5;\n\
	vec3 normal = normalize(nuEyePos - centerEyePos).xyz;\n\
	out_Color = vec4(color.xyz * dot(normal, (mv_Mat*vec4(0.0, 1.0, 0.0, 0.0)).xyz) + color.xyz * 0.3, 1.0 );\n\
}";
#pragma endregion


GLuint SpherePrim::sphereShaders[3] = { 0, 0, 0};
GLuint SpherePrim::sphereProgram = 0;
bool SpherePrim::shadersCreated = false;

SpherePrim::SpherePrim() {
	//Update(glm::vec3(0, 5, 0), 1);
}

SpherePrim::SpherePrim(float rad, glm::vec3 pos):radius(rad)
{
	glGenVertexArrays(1, &sphereVao);
	glBindVertexArray(sphereVao);
	glGenBuffers(1, &sphereVbo);

	glBindBuffer(GL_ARRAY_BUFFER, sphereVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 4, &pos, GL_DYNAMIC_DRAW);
	glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

SpherePrim::~SpherePrim()
{
	glDeleteBuffers(1, &sphereVbo);
	glDeleteVertexArrays(1, &sphereVao);
}

void SpherePrim::createSphereShaderAndProgram() {
	if (shadersCreated) return;

	sphereShaders[0] = ShaderCompilationUtils::compileShader(sphere_vertShader, GL_VERTEX_SHADER, "sphereVert");
	sphereShaders[1] = ShaderCompilationUtils::compileShader(sphere_geomShader, GL_GEOMETRY_SHADER, "sphereGeom");
	sphereShaders[2] = ShaderCompilationUtils::compileShader(sphere_fragShader_flatColor, GL_FRAGMENT_SHADER, "sphereFrag");

	sphereProgram = glCreateProgram();
	glAttachShader(sphereProgram, sphereShaders[0]);
	glAttachShader(sphereProgram, sphereShaders[1]);
	glAttachShader(sphereProgram, sphereShaders[2]);
	glBindAttribLocation(sphereProgram, 0, "in_Position");
	ShaderCompilationUtils::linkProgram(sphereProgram);

	shadersCreated = true;
}
void SpherePrim::cleanupSphereShaderAndProgram() {
	if (!shadersCreated) return;
	glDeleteProgram(sphereProgram);
	glDeleteShader(sphereShaders[0]);
	glDeleteShader(sphereShaders[1]);
	glDeleteShader(sphereShaders[2]);
	shadersCreated = false;
}
void SpherePrim::Setup() {
	createSphereShaderAndProgram();
}

void SpherePrim::Cleanup() {
	if (!shadersCreated) return;
	glDeleteProgram(sphereProgram);
	glDeleteShader(sphereShaders[0]);
	glDeleteShader(sphereShaders[1]);
	glDeleteShader(sphereShaders[2]);
	shadersCreated = false;
	shadersCreated = false;
}

void SpherePrim::Draw() {
	glBindVertexArray(sphereVao);
	glUseProgram(sphereProgram);
	glUniformMatrix4fv(glGetUniformLocation(sphereProgram, "mvpMat"), 1, GL_FALSE, glm::value_ptr(_MVP));
	glUniformMatrix4fv(glGetUniformLocation(sphereProgram, "mv_Mat"), 1, GL_FALSE, glm::value_ptr(_modelView));
	glUniformMatrix4fv(glGetUniformLocation(sphereProgram, "projMat"), 1, GL_FALSE, glm::value_ptr(_projection));
	glUniform4f(glGetUniformLocation(sphereProgram, "color"), 0.6f, 0.1f, 0.1f, 1.f);
	glUniform1f(glGetUniformLocation(sphereProgram, "radius"), SpherePrim::radius);
	glDrawArrays(GL_POINTS, 0, 1);

	glUseProgram(0);
	glBindVertexArray(0);
}

void SpherePrim::Update(glm::vec3 pos, float radius) {
	glBindBuffer(GL_ARRAY_BUFFER, sphereVbo);
	float* buff = (float*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
	buff[0] = pos.x;
	buff[1] = pos.y;
	buff[2] = pos.z;
	buff[3] = 1.f;
	glUnmapBuffer(GL_ARRAY_BUFFER);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	SpherePrim::radius = radius;
}

