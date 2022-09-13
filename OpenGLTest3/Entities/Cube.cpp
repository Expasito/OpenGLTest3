//#include "Cube.h"
//
//float Cube::vertices[] = {
//	-1.0f, -1.0f, -1.0f,  0.0f, 0.0f,
//	 1.0f, -1.0f, -1.0f,  1.0f, 0.0f,
//	 1.0f,  1.0f, -1.0f,  1.0f, 1.0f,
//	 1.0f,  1.0f, -1.0f,  1.0f, 1.0f,
//	-1.0f,  1.0f, -1.0f,  0.0f, 1.0f,
//	-1.0f, -1.0f, -1.0f,  0.0f, 0.0f,
//
//	-1.0f, -1.0f,  1.0f,  0.0f, 0.0f,
//	 1.0f, -1.0f,  1.0f,  1.0f, 0.0f,
//	 1.0f,  1.0f,  1.0f,  1.0f, 1.0f,
//	 1.0f,  1.0f,  1.0f,  1.0f, 1.0f,
//	-1.0f,  1.0f,  1.0f,  0.0f, 1.0f,
//	-1.0f, -1.0f,  1.0f,  0.0f, 0.0f,
//
//	-1.0f,  1.0f,  1.0f,  1.0f, 0.0f,
//	-1.0f,  1.0f, -1.0f,  1.0f, 1.0f,
//	-1.0f, -1.0f, -1.0f,  0.0f, 1.0f,
//	-1.0f, -1.0f, -1.0f,  0.0f, 1.0f,
//	-1.0f, -1.0f,  1.0f,  0.0f, 0.0f,
//	-1.0f,  1.0f,  1.0f,  1.0f, 0.0f,
//
//	 1.0f,  1.0f,  1.0f,  1.0f, 0.0f,
//	 1.0f,  1.0f, -1.0f,  1.0f, 1.0f,
//	 1.0f, -1.0f, -1.0f,  0.0f, 1.0f,
//	 1.0f, -1.0f, -1.0f,  0.0f, 1.0f,
//	 1.0f, -1.0f,  1.0f,  0.0f, 0.0f,
//	 1.0f,  1.0f,  1.0f,  1.0f, 0.0f,
//
//	-1.0f, -1.0f, -1.0f,  0.0f, 1.0f,
//	 1.0f, -1.0f, -1.0f,  1.0f, 1.0f,
//	 1.0f, -1.0f,  1.0f,  1.0f, 0.0f,
//	 1.0f, -1.0f,  1.0f,  1.0f, 0.0f,
//	-1.0f, -1.0f,  1.0f,  0.0f, 0.0f,
//	-1.0f, -1.0f, -1.0f,  0.0f, 1.0f,
//
//	-1.0f,  1.0f, -1.0f,  0.0f, 1.0f,
//	 1.0f,  1.0f, -1.0f,  1.0f, 1.0f,
//	 1.0f,  1.0f,  1.0f,  1.0f, 0.0f,
//	 1.0f,  1.0f,  1.0f,  1.0f, 0.0f,
//	-1.0f,  1.0f,  1.0f,  0.0f, 0.0f,
//	-1.0f,  1.0f, -1.0f,  0.0f, 1.0f
//};
//
//Cube::Cube() {
//	Entity::Entity();
//	this->transform = glm::mat4(1.0f);
//	this->translate = glm::vec3(0, 0, 0);
//	this->rotate = glm::vec3(0, 0, 0);
//	this->scale = glm::vec3(1, 1, 1);
//
//}
//
//void Cube::adjustTextCords() {
//	const float textureDx = .01;
//	//modify texture cords by moving +- .01
//	for (int i = 3; i < 36 * 5; i += 5) {
//		if (Cube::vertices[i] == 1.0f)
//			Cube::vertices[i] -= textureDx;
//		if (Cube::vertices[i + 1] == 1.0f)
//			Cube::vertices[i + 1] -= textureDx;
//		if (Cube::vertices[i] == 0.0f)
//			Cube::vertices[i] += textureDx;
//		if (Cube::vertices[i + 1] == 0.0f)
//			Cube::vertices[i + 1] += textureDx;
//	}
//}