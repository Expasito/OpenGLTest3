#include "Object.h"

Object::Object() {
	this->transform = glm::mat4(1.0f);
	this->translate = glm::vec3(0, 0, 0);
	this->rotate = glm::vec3(0, 0, 0);
	this->scale = glm::vec3(1, 1, 1);
}
