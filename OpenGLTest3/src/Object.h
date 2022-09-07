#pragma once

#include "Headers.h"

class Object
{
public:
	static float vertices[8 * 4];
	static unsigned int indices[6];
	glm::mat4 transform;
	glm::vec3 translate;
	glm::vec3 rotate;
	glm::vec3 scale;
	Object();
private:

};

