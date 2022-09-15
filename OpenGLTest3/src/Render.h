
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
	static float* vertices;
	static size_t verticesSize;
	static Entity* skybox;
	static unsigned int VBO, VAO, EBO;
	static unsigned int fbo, colorTexture, depthTexture;
	static int windowWidth;
	static int windowHeight;
	static GLFWwindow* init();
	static void callBacks(GLFWwindow*, GLFWkeyfun, GLFWframebuffersizefun, GLFWmousebuttonfun, GLFWscrollfun, GLFWcursorposfun);
	static void initEntities();
	static void genCubeVert();
	static void draw(Entity*);
	static unsigned int getUniformLoc(std::string);
	static void activateSkybox();
	static void prepareFramebuffer();


};