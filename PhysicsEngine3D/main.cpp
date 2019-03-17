#include <windows.h>
#include <GL/glew.h>
#include <iostream>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>


int main() {
	std::cout << "Hola\n";
	if (!glfwInit()) {
		fprintf(stderr, "Couldn't initialize GLFW\n");
		return -1;
	}
	GLenum err = glewInit();
	if (GLEW_OK != err) {
		std::cout << "Error:	" << glewGetErrorString(err)<<std::endl;
	}
	std::cout << "GLEW: Version:	" << glewGetString(GLEW_VERSION);
	int i = 0;
}