#version 430

in vec3 in_Position;
uniform mat4 mvpMat;

void main() {
	gl_Position = mvpMat * vec4(in_Position, 1.0);
}