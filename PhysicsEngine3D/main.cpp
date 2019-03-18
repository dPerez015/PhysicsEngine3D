#include <windows.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <iostream>

extern void GLResize(int width, int height);

//Function called when there's a change in windows size, changes the size of the viewport
void GLFWwindowresize(GLFWwindow*, int w, int h) {
	GLResize(w, h);
}

//Function called when there's an error on glfw function
void error_callback(int error, const char* description) {
	std::cerr << "GLFW Error: " << description << std::endl;
}

int main() {
	//initialize GLFW library
	if (!glfwInit()) {
		fprintf(stderr, "Couldn't initialize GLFW\n");
		return -1;
	}
	//set callback function for glfw functions error
	glfwSetErrorCallback(error_callback);

	//I want opengl 3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	//Create window
	GLFWwindow* window = glfwCreateWindow(640, 480, "DP Engine", NULL, NULL);
	
	if (!window) {
		glfwTerminate();
		std::cout << "Window creation failed\n";
		return -1;
	}

	//set this main window as the context
	glfwMakeContextCurrent(window);
	glfwSetWindowSizeCallback(window,GLFWwindowresize);

	//glew init
	GLenum err = glewInit();
	if (GLEW_OK != err) {
		std::cout << "Error:	" << glewGetErrorString(err)<<std::endl;
	}
	std::cout << "GLEW: Version:	" << glewGetString(GLEW_VERSION);
	
	//main loop
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();

		
	}



	//Destroy the window
	glfwDestroyWindow(window);

	//terminate GLFW 
	glfwTerminate();
	return 0;
}