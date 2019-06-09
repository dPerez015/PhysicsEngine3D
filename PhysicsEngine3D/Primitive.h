#pragma once
#include "GL/glew.h"
#include <stdio.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

class Primitive {
public:
	virtual void Draw();
private:	
};