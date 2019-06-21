#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "MouseEvent.h"
#include "Sphere.h"
#include "BoxPrim.h"
#include "CubePrim.h"
#include "AxisPrim.h"
#include "CollisionEngine.h"

const float FOV = glm::radians(65.f);
const float zNear = 1.f;
const float zFar = 50.f;

glm::mat4 _projection;
glm::mat4 _MVP;
glm::mat4 _modelView;
glm::mat4 _inv_modelview;
glm::vec4 _cameraPoint;

namespace Box {
	extern void setupCube();
	extern void cleanupCube();
	extern void drawCube();
}

struct prevMouse {
	float lastx, lasty;
	MouseEvent::Button button = MouseEvent::Button::None;
	bool waspressed = false;
} prevMouse;

float panv[3] = { 0.f, -5.f, -15.f };
float rota[2] = { 0.f, 0.f };



void GLmousecb(MouseEvent ev) {
	if (prevMouse.waspressed && prevMouse.button == ev.button) {
		float diffx = ev.posx - prevMouse.lastx;
		float diffy = ev.posy - prevMouse.lasty;
		switch (ev.button) {
		case MouseEvent::Button::Left: //ROTATE
			rota[0] += diffx * 0.005f;
			rota[1] += diffy * 0.005f;
			break;
		case MouseEvent::Button::Right: //MOVE XY
			panv[0] += diffx * 0.03f;
			panv[1] -= diffy * 0.03f;
			break;
		case MouseEvent::Button::Middle: //MOVE Z
			panv[2] += diffy * 0.05f;
			break;
		default: break;
		}
	}
	else {
		prevMouse.button = ev.button;
		prevMouse.waspressed = true;
	}
	prevMouse.lastx = ev.posx;
	prevMouse.lasty = ev.posy;
}

//resize window rendering after a resize from glfw (function called has callback)
void GLResize(int w, int h) {
	glViewport(0, 0, w, h);
	if (h != 0) _projection = glm::perspective(FOV, (float)w / (float)h, zNear, zFar);
	else _projection = glm::perspective(FOV, 0.f, zNear, zFar);
}

BoxPrim* box;
//extern CubePrim* cube;
AxisPrim* axis;
//SpherePrim* sphere;

//init rendering
void GLInit(int width, int height) {
	//set the viewport size in pixels
	glViewport(0, 0, width, height);
	//clear color value
	glClearColor(0.2f, 0.2f, 0.2f, 1.f);
	glClearDepth(1.f);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	_projection = glm::perspective(FOV, (float)width / (float)height, zNear, zFar);
	
	SpherePrim::Setup();
	BoxPrim::Setup();
	AxisPrim::Setup();
	CubePrim::Setup();

	box = new BoxPrim();
	axis = new AxisPrim();
	//sphere = new SpherePrim(0.1f, glm::vec3(0.0f,5.f,0.f));
}

//render
void GLRender() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	_modelView = glm::mat4(1.f);
	_modelView = glm::translate(_modelView, glm::vec3(panv[0], panv[1], panv[2]));
	_modelView = glm::rotate(_modelView, rota[1], glm::vec3(1.f, 0.f, 0.f));
	_modelView = glm::rotate(_modelView, rota[0], glm::vec3(0.f, 1.f, 0.f));

	_MVP = _projection * _modelView;

	//Box::drawCube();
	box->Draw();
	axis->Draw();
	//sphere->Draw();
	//cube->Draw();
	CollisionEngine::getInstance().draw();
}


//cleanup

void GLCleanup() {
	SpherePrim::Cleanup();
	BoxPrim::Cleanup();
	AxisPrim::Cleanup();
}