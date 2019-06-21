#include <windows.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <iostream>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw_gl3.h>
#include "MouseEvent.h"



extern void GLResize(int width, int height);
extern void GLInit(int width, int height);
extern void GLRender();
extern void GLCleanup();
extern void GLmousecb(MouseEvent ev);
extern void PhysicsInit();
extern void PhysicsUpdate(float);
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
		std::cerr << "Couldn't initialize GLFW\n";
		return -1;
	}
	//set callback function for glfw functions error
	glfwSetErrorCallback(error_callback);

	//I want opengl 3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	//Create window
	GLFWwindow* window = glfwCreateWindow(640, 480, "DP Engine", NULL, NULL);
	
	if (!window) {
		glfwTerminate();
		std::cerr << "Window creation failed\n";
		return -1;
	}

	//set this main window as the context
	glfwMakeContextCurrent(window);
	glfwSetWindowSizeCallback(window,GLFWwindowresize);

	//glew init
	GLenum err = glewInit();
	if (GLEW_OK != err) {
		std::cerr << "Error:	" << glewGetErrorString(err)<<std::endl;
	}
	std::cout << "GLEW: Version:	" << glewGetString(GLEW_VERSION)<<std::endl;
	


	//initialize Rendering
	int display_w, display_h;
	glfwGetFramebufferSize(window, &display_w, &display_h);

	//setup scene rendering
	GLInit(display_w, display_h);

	// Setup ImGui binding
	ImGui_ImplGlfwGL3_Init(window, true);

	//initialize Physics
	PhysicsInit();
	double previusTime = glfwGetTime();
	double currentTime = previusTime;
	float deltaTime;
	//main loop
	while (!glfwWindowShouldClose(window)) {
		currentTime = glfwGetTime();
		deltaTime = currentTime - previusTime;

		glfwPollEvents();
		ImGui_ImplGlfwGL3_NewFrame();
		ImGuiIO& io = ImGui::GetIO();
		if (!io.WantCaptureMouse) {
			MouseEvent ev = { io.MousePos.x, io.MousePos.y,
				(io.MouseDown[0] ? MouseEvent::Button::Left :
				(io.MouseDown[1] ? MouseEvent::Button::Right :
				(io.MouseDown[2] ? MouseEvent::Button::Middle :
				MouseEvent::Button::None))) };
			GLmousecb(ev);
		}

		PhysicsUpdate(0.016f);
		//PhysicsUpdate(deltaTime);
		GLRender();

		glfwSwapBuffers(window);
		previusTime = currentTime;
	}

	GLCleanup();
	//Destroy the window
	glfwDestroyWindow(window);

	//terminate GLFW 
	glfwTerminate();
	return 0;
}