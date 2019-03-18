#include <GLFW/glfw3.h>
#include <GL/glew.h>
//#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

const float FOV = glm::radians(65.f);
const float zNear = 1.f;
const float zFar = 50.f;

glm::mat4 _projection;

//resize window rendering after a resize from glfw (function called has callback)
void GLResize(int w, int h) {
	glViewport(0, 0, w, h);
	if (h != 0) _projection = glm::perspective(FOV, (float)w / (float)h, zNear, zFar);
	else _projection = glm::perspective(FOV, 0.f, zNear, zFar);
}

//init rendering


//render


//cleanup