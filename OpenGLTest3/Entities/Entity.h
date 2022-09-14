#pragma once
#include "../src/Headers.h"
#include "Components.h"
class Entity
{
	
public:
	static enum class entityType {
		NONE = -1, CUBE = 1, SPHERE = 2
	};
	static struct ComposData {
		void* ptr;
		size_t hash;
		ComposData(void* ptr, size_t hash) {
			this->ptr = ptr;
			this->hash = hash;
		}
	};
	std::vector<Entity::ComposData> compos;
	Entity(entityType);
	Entity();
	~Entity();
	
	int getId();
	template<typename T, typename...Args>
	T* addComponent(Args...args) {
		if (hasComponent<T>()) {
			//Must warn user that component was already created
			return getComponent<T>();
		}
		const size_t size = sizeof...(args);
		if constexpr(size >= 1) {
			auto values = {args... };
			helper<T>(values);
		}
		else {
			void* pt = new T();
			this->compos.push_back({ pt,typeid(T).hash_code() });
		}
		return getComponent<T>();
	};

	template<typename B, typename T>
	void helper(std::initializer_list<T> x) {
		const int size = x.size();
		std::vector<T> arr;
		arr.reserve(size);
		std::copy(x.begin(), x.end(), std::back_inserter(arr));
		std::vector<void*> arr2;
		for (int i = 0; i < arr.size(); i++) {
			void* ptr = (void*)(&arr.at(i));
			arr2.push_back(ptr);
		}
		void* pt = new B(arr2);
		this->compos.push_back({ pt,typeid(B).hash_code() });
	}

	template<typename T>
	T* getComponent() {
		for (ComposData c : this->compos) {
			if (typeid(T).hash_code() == c.hash) {
				return (T*)(c.ptr);
			}
		}
		return nullptr;
	}
	template<typename T>
	bool hasComponent() {
		for (ComposData c : this->compos) {
			if (typeid(T).hash_code() == c.hash) {
				return true;
			}
		}
		return false;
	}
	template<typename T>
	void removeComponent() {
		if (!hasComponent<T>()) {
			//warn user that component does not exist
			std::cout << "Does not have Component\n";
			return;
		}
		int index = 0;
		for (int i = 0; i < this->compos.size(); i++) {
			if (typeid(T).hash_code() == this->compos.at(i).hash) {
				index = i;
			}
		}
		delete this->compos.at(index).ptr;
		this->compos.erase(this->compos.begin()+index);
		return;
	}



private:
	static int idCounter;
	int id;
	void init();
	
};

