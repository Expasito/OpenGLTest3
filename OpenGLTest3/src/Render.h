
#pragma once
#include "Headers.h"
#include "../Entities/Entity.h"

class Render {
public:
	//static std::vector<Entity> entities;
	static struct uniform {
		unsigned int loc;
		std::string name;
	};
	static std::vector<Render::uniform> uniforms;
	static std::vector<glm::vec3> translations;
	static std::vector<glm::vec3> rotations;
	static std::vector<glm::vec3> scalations;
	static std::vector<glm::vec3> color;
	static float* vertices;
	static size_t verticesSize;
	static Entity* skybox;
	static unsigned int VBO, VAO, EBO;
	static unsigned int fbo, colorTexture, depthTexture;
	static unsigned int instanceVBO, instanceVBO2, instanceVBO3, instanceVBO4;
	static int windowWidth;
	static int windowHeight;
	static GLFWwindow* init();
	static void callBacks(GLFWwindow*, GLFWkeyfun, GLFWframebuffersizefun, GLFWmousebuttonfun, GLFWscrollfun, GLFWcursorposfun);
	static void initEntities();
	static void genCubeVert();
	static void draw(Entity*);
	static void rend();
	static unsigned int getUniformLoc(std::string);
	static void activateSkybox();
	static void prepareFramebuffer();


};