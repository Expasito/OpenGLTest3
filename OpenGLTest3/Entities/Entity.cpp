#include "Entity.h"

int Entity::idCounter = 0;

Entity::Entity() {
	init();
}

Entity::Entity(entityType type) {
	init();
}
Entity::~Entity() {
	for (ComposData c : this->compos) {
		delete c.ptr;
	}
}

void Entity::init() {
	this->id = Entity::idCounter;
	this->idCounter += 1;
}

int Entity::getId() {
	return this->id;
}