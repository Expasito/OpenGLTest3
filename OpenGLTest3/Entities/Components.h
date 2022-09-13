#pragma once

#include "../src/Headers.h"
#include "Entity.h"



namespace Components {

	struct TransformComponent {
		glm::mat4 transform = glm::mat4(1.0f);
		glm::vec3 translate = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec3 rotate = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);

		TransformComponent() = default;
	};

	//struct EntityTypeComponent {
	//	Entity::entityType type = Entity::entityType::NONE;
	//	EntityTypeComponent() = default;
	//};

	struct TextureComponent {
		unsigned int texture;
		TextureComponent() = default;

	};

	template<typename... Component>
	struct ComponentGroup {};

	//using AllComponents = ComponentGroup<TransformComponent, EntityTypeComponent, TextureComponent>;
	static std::vector<const type_info*> AllCompTypes = {&typeid(TransformComponent),&typeid(TextureComponent) };

};