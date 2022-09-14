#pragma once

#include "../src/Headers.h"
#include "Entity.h"



namespace Components {

	struct TransformComponent {
		glm::vec3 translate = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec3 rotate = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);

		TransformComponent() = default;
		TransformComponent(std::vector<void*> v) : translate(*(glm::vec3*)v.at(0)) {};

	};

	//struct EntityTypeComponent {
	//	Entity::entityType type = Entity::entityType::NONE;
	//	EntityTypeComponent() = default;
	//};

	struct TextureComponent {
		unsigned int texture=-1;
		TextureComponent() = default;
		TextureComponent(std::vector<void*> v) : texture(*(unsigned int*)v.at(0)) {};

	};
	static std::vector<const type_info*> AllCompTypes = {&typeid(TransformComponent),&typeid(TextureComponent) };

};